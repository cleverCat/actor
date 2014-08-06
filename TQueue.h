#ifndef __IACTOR_QUEUE__
#define __IACTOR_QUEUE__

#include <list>
#include <queue>
#include <memory>
#include <type_traits>
#include <chrono>
#include <assert.h>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
///!!! WARNING к сожелению std::mutex тек памятью со страшной силой, пришлость его заменить на бустовый аналог

#include <atomic>
///класс ошибки которая возникает при закрытие очереди(когда мы в ней больше не нуждаемся)
///чтобы не повесить процесс ждущий из очереди когда он все еще ждет а уже пора закрываться
class ClosedException: std::exception{
    // exception interface
public:
    const char *what() const noexcept;
};

typedef boost::unique_lock<boost::mutex> TLock;

using namespace std;

///заготовка для элементов очереди, хвоста и головы ))
template<typename T>
class TAbstractQueueElement
{
public:

	std::shared_ptr<TAbstractQueueElement<T> > _prev;//ссылка на следующий элемент
    boost::mutex _lockElem; //мьютекс для блокировки обращений к элементу

public:
    TAbstractQueueElement():_prev(nullptr),_lockElem(){}//после создания элемент станет головой так что у него нет предыдущего элемента
    virtual ~TAbstractQueueElement(){}
///получаем блокировку текущего элемента
    TLock getLock(void) {return  TLock(_lockElem);}
///получаем данные лежащие внутри
	virtual T getData(std::shared_ptr<TAbstractQueueElement<T> > tail)=0;

	virtual void setData(std::shared_ptr<TAbstractQueueElement<T> > prev)=0;
    virtual void setData(T data)=0;
};
///элемент очереди
template <typename T>
class TQueueElement: public TAbstractQueueElement<T>
{
private:
    T _data;
public:
    TQueueElement(T data):_data(data) {}
    ~TQueueElement(){}

    T getData(std::shared_ptr<TAbstractQueueElement<T> > tail)
    {
        (void)tail;
        return this->_data;

    }

    void setData(std::shared_ptr<TAbstractQueueElement<T> > prev)
    {
        auto loc=this->getLock();
        if (this->_prev!=nullptr){this->_prev->setData(prev);}
        else{this->_prev=prev;}
    }

    void setData(T data){(void)data;}
};


///хвост
template <typename T>
class TTailQueueElement: public TAbstractQueueElement<T>
{
private:
    boost::condition_variable _pushToQueue;
    bool _isClosed;

public:
    TTailQueueElement():_pushToQueue(),_isClosed(false) {}
    virtual ~TTailQueueElement(){}

///закрываем очередь
    void close(void)
    {
        auto loc=this->getLock();
        _isClosed=true;
        _pushToQueue.notify_all();
    }


///устанавливаем голову
    void setData(std::shared_ptr<TAbstractQueueElement<T> > prev)
    {
       // std::cout<<"setData"<<std::endl;
        auto loc=this->getLock();
        this->_prev=prev;
        _pushToQueue.notify_all(); //сообщаем о том что пора продолжить вытягивание элементов из очереди
    }
///устанавливаем тело бывшее тело становиться головой
    void setData(T data)
    {
         std::shared_ptr<TAbstractQueueElement<T> > newPrevTail(new TQueueElement<T>(data));
        auto loc=this->getLock();
        newPrevTail->setData(this->_prev);
        this->_prev=newPrevTail;
         _pushToQueue.notify_all();
    }

    T getData(std::shared_ptr<TAbstractQueueElement<T> > tail)
    {
        //std::cout<<"getData"<<std::endl;
        auto loc=this->getLock();
        while (this->_prev==nullptr)//while защита от ложных срабатываний
            {
            _pushToQueue.wait(loc,[this](){return (this->_prev!=nullptr);}); //ожидаем появление элементов с реальными данными в очереди
            if (_isClosed) throw ClosedException();
            };
        auto prev=this->_prev;
        auto locHead=this->_prev->getLock();
        auto data=this->_prev->getData(tail);
        this->_prev=this->_prev->_prev;
        if (this->_prev==nullptr) {prev->_prev=tail; };//устанавливаем глове ссылку на хвост тк мы выдернули все данные ))
        //std::cout<<data;
        return data;
    }

};


///очередь для межпоточного взаимодействия
template<typename T>
class TQueue
{
private:
	std::shared_ptr<TAbstractQueueElement<T> > _tail;
	std::shared_ptr<TAbstractQueueElement<T> > _head;

    boost::mutex _lockHead;
    std::atomic<size_t> _size;
public:
    ///получаем размер очереди
    size_t unstableSize(void)const{return _size;}//!!!возможны проблемы при обращении из нескольких потоков использовать только для справочной информации
    TQueue():_tail(std::shared_ptr<TAbstractQueueElement<T> >(new TTailQueueElement<T>())),_head(_tail),_lockHead(),_size(0){}
    virtual ~TQueue(){}



///получаем элемент из очереди
    T pop()
    {
        _size--;
        return _tail->getData(_tail);
    }
///пихаем элемент в очередь
    void push(T data)
    {

        std::shared_ptr<TAbstractQueueElement<T> > head(new TQueueElement<T>(data));
        TLock lock(_lockHead);//блокируем голову от возможных попыток записи из других потоков
        _head->setData(head);
        _head=head;
        _size++;

    }
///закрываем очередь
    void close(void)
    {
        _head=nullptr;
        _tail=nullptr;
    }

   ///пихаем элемент в конец очереди
    void push_front(T data)
    {
        _size++;
        _tail->setData(data);
    }
    void push_back(T data){push(data);}


   T wait_and_pop(const unsigned int TimeOut=0){(void)TimeOut;return pop();}

   T wait_and_pop(std::chrono::duration<int, std::micro> timeout){(void)timeout;return pop();}
   T waitTimeoutPop(const unsigned int TimeOut=0){(void)TimeOut;return pop();}

};

#endif
