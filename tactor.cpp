#include "actor/tactor.h"
#include <iostream>
#include <deque>
#include <memory>
#include <exception>
#include <chrono>

#include "actor/Messages.h"
#include <boost/any.hpp>
#define HeaderForInlude "QDebug"
#include "actor/baseLib/importWithoutWarning.h"
#include "actor/IActor.h"
#include "actor/TStorageActors.h"
#include "actor/TExeption.h"

#include "actor/StandartObjekt/profiler/TAddProfilerValue.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TActor::TActor(TStorageActorsPtr storage):_storage(storage)
{
	setFunctorOnMessage(Command::UncnowCommand, TFunctorOnMessage(NStandartFunctionsOnMessage::noAction));
	setFunctorOnMessage(Command::ActorDead, TFunctorOnMessage(NStandartFunctionsOnMessage::noAction));
}

TActor::TActor(void):_storage(TStorageActorsPtr(new TStorageActors()))
{
    setFunctorOnMessage(Command::UncnowCommand, TFunctorOnMessage(NStandartFunctionsOnMessage::noAction));
    setFunctorOnMessage(Command::ActorDead, TFunctorOnMessage(NStandartFunctionsOnMessage::noAction));
}

TActor::~TActor()
{
    //TMessage<int> messageDead(Command::ActorDead,TypeMessage::Extra, this->getId());
    //_mapFunctorsOnMessage[Command::ActorDead](std::make_shared<TMessage<int> >(messageDead),this);

}

void TActor::addMessage(TAbstractMessagePtr message)//добавляем сообщение в очередь
{
    if ((*message).getType()==TypeMessage::Extra)
        _queueMessage.push_front(message);
    else{  _queueMessage.push_back(message);}
    _storage->addMessage(NMessageFunction::normalMessageWithDate<IActorPtr>(Command::ExecuteActor,this->getThis()) );
}

void TActor::execute(const uint TimeOut)//выбираем сообщение из очереди и вызываем соответствующий функтор
{

    (void)TimeOut;
    TAbstractMessagePtr message;
    std::string nameType(typeid(*this).name());
    try{
        //try
        {
            addProfilerQueueValue(nameType,static_cast<unsigned int >(_queueMessage.unstableSize()));
            message=_queueMessage.pop();
        }//TimeOut);}
        //catch(TUnboundedQueueTimeoutException){return;} //мы не дождались данных по таймауту ничего страшного подождем во время вызова следующего execute

        auto addProfilerMeasurment=addProfilerTimeValue(nameType,(*message).getMessage());

        if (_mapFunctorsOnMessage.end()==_mapFunctorsOnMessage.find(message->getMessage()))
        {_mapFunctorsOnMessage[UncnowCommand](message, this->getThis() );}
        else
        {
            _mapFunctorsOnMessage.find(message->getMessage())->second(message , this->getThis());
        }
    }catch(std::exception& ex)
    {
        this->executeFilters(ex,(*message).getMessage());
        qDebug()<<"error message"<<message->getMessage();
        qDebug()<<"error from"+QString::fromStdString(nameType);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void NStandartFunctionsOnMessage::noAction(TAbstractMessagePtr message, IActorPtr currentActor)
{qDebug()<<"не известная комманда"; (void)currentActor;(void)message; qDebug()<<"uncnow command";	}
