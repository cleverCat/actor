
#include <memory>
#include <functional>
#include <map>

#include <boost/cast.hpp>

#include "actor/TQueue.h"

#include "actor/Messages.h"

#include "actor/IActor.h"

IActor::IActor():_queueMessage(),_mapFunctorsOnMessage(){}

IActor::~IActor(){ _queueMessage.close();} //не забываем закрыть очередь акторов чтобы поток вытягивающий из очереди не завис

std::shared_ptr<IActor> IActor::getThis(){return shared_from_this();}

void IActor::setFunctorOnMessage(Command command, IActor::TFunctorOnMessage functor)
{
    std::pair<Command , TFunctorOnMessage >  messageAndFunctor=std::pair<Command , TFunctorOnMessage >(command, functor);
    if (_mapFunctorsOnMessage.end()==_mapFunctorsOnMessage.find(command))
        _mapFunctorsOnMessage.insert(messageAndFunctor);
    else
        _mapFunctorsOnMessage.find(command)->second=functor;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

