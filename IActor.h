#ifndef IACTOR_H
#define IACTOR_H

#include <memory>
#include <functional>
#include <map>

#include <boost/cast.hpp>

#include "actor/TQueue.h"

#include "actor/Messages.h"

#include "actor/test/testBase/TTestPrimitive.h"



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++" //ребята писавшие enable_shared_from_this забыли виртуальный деструктор поэтому приходиться выключать этот варнинг
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"




#ifdef __GNUC__
#define NOT_USED __attribute__ ((unused))
#else
#define NOT_USED
#endif

typedef std::shared_ptr<const TAbstractMessage> TAbstractMessagePtr;

/*!
  интерфейс акторов
 */
class IActor:public  std::enable_shared_from_this<IActor>
{
public :
	typedef std::shared_ptr<IActor> IActorPtr;
	typedef std::function< void(TAbstractMessagePtr, IActorPtr ) > TFunctorOnMessage;
protected:
	TQueue<TAbstractMessagePtr> _queueMessage; //чередь сообщений
	std::map<Command, TFunctorOnMessage > _mapFunctorsOnMessage; // мап функторов срабатывающих при приходе сообщений с определенным id
public :
    explicit IActor();
    virtual ~IActor();
public:
    std::shared_ptr<IActor> getThis();
    ///добавляем функтор на сообщение
    void setFunctorOnMessage(Command command , TFunctorOnMessage functor);
	///отправляет сообщение этому актору
	virtual void addMessage(TAbstractMessagePtr message)=0;
    ///обрабатываем сообщения посланные актору
	virtual void execute(const unsigned int TimeOut=0)=0;
    ///пиши тесты черт возьми на свои акторы
    virtual TestPrimitive test(void)=0;//{return TestPrimitive("IActor Test",std::list<TestPrimitive>());}

};

typedef IActor::IActorPtr IActorPtr;
typedef IActor::TFunctorOnMessage TFunctorOnMessage;

///преобразовываем IActorPtr в указатель на конкретный тип актора
template<class TActor_ptr>
NOT_USED static std::shared_ptr<TActor_ptr> castActor(IActorPtr currentActor)
{
    (void)::boost::polymorphic_downcast<TActor_ptr*>(currentActor.get());
    return std::static_pointer_cast<TActor_ptr>(currentActor);
}

#pragma GCC diagnostic pop
#endif // IACTOR_H
