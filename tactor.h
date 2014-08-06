#ifndef TACTOR_H
#define TACTOR_H

#include "actor/Messages.h"

#include "actor/TStorageActors.h"
#include "actor/TExeption.h"


///базовый класс для всех акторов уже имеет свой поток или же его можно задать в конструкторе позволяет собрать актор с нужными обработчиками на ходу
class TActor:public  NExeptionRouter::TExeptionRouter
{
public:
private:

	//TUnboundedQueueForNonThrowMovable<TAbstractMessagePtr > _queueMessage;
	TStorageActorsPtr _storage;
public:
	explicit TActor(TStorageActorsPtr storage);
	explicit TActor(void);
    virtual ~TActor();
public:
    ///добавляем сообщение в очередь сообщений
    void addMessage(TAbstractMessagePtr message);

    ///выбираем сообщение из очереди и вызываем соответствующий функтор
    virtual void execute(const uint TimeOut=0);

    TestPrimitive test(void)override{return TestPrimitive("IActor Test",std::list<TestPrimitive>());}

};

namespace NStandartFunctionsOnMessage//стандартные функторы на обработку сообщений
{
     void noAction(TAbstractMessagePtr message, IActorPtr currentActor);
}


#endif // TACTOR_H
