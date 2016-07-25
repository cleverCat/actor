#ifndef MESSAGE_H
#define MESSAGE_H

#include <tuple>
#include <memory>
#include <iostream>

//////////////////абстрактный класс сообщений нужен чтобы можно было положить сообщения разных типов в одну коллекцию///////////////////////////
class AbstractMessage
{
private:
    const int _id;
public:
    AbstractMessage(const int id):_id(id){}
    //получаем id сообщения
    int getID(void)const{return _id;}
    virtual ~AbstractMessage(void){}
};

///класс для определения пустого значения
class NoneType{};

///////////////шаблонный класс сообщений///////////////
template<typename... Types>
class Message: public AbstractMessage
{
private:

    const std::tuple<Types...> _data;
public:
    Message(int id, const Types&... args):AbstractMessage(id),_data(std::make_tuple(args...)){}
    ~Message(void){}

    //получаем данные из сообщения
    std::tuple<Types...> getData(void)const{/*std::cout<<"return Data"<<std::endl;*/ return _data;}

};

typedef std::shared_ptr<const AbstractMessage> AbstractMessagePtr;

template<typename ...Types>
AbstractMessagePtr createMessage(const int id, Types ...args)
{
    const std::shared_ptr<AbstractMessage> message = std::make_shared<Message<Types...> >(Message<Types...>(id, args...));
    return message;
}

#endif // MESSAGE_H
