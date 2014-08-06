#include "actor/StandartObjekt/TPool.h"
using namespace NPool;

NPool::TPool::TPool():_map(),_messages()
{
	setFunctorOnMessage(Command::GetFreeAtActor, TFunctorOnMessage(getFreeAtActor));
	setFunctorOnMessage(Command::AddActorInPool, TFunctorOnMessage(addActorInPool));
	setFunctorOnMessage(Command::RemoveActorInPool, TFunctorOnMessage(removeActorInPool));
	setFunctorOnMessage(Command::UncnowCommand, TFunctorOnMessage(sendMessageInPool));
}

NPool::TPool::~TPool(){}

void TPool::addActor(IActor::IActorPtr actor)
{
	if (_map.find(actor)==_map.end())
	{
		_map.insert(std::make_pair(actor,false));
		actor->setFunctorOnMessage(Command::SetFreeActorInPool,sendFreeAtPool);
	}
}

void TPool::removeActor(IActor::IActorPtr actor)
{
	if (_map.find(actor)==_map.end())
		_map.erase(actor);
}

void TPool::setFreeActor(IActor::IActorPtr actor)
{
	if (_map.find(actor)==_map.end())
		_map.find(actor)->second=false;
}

void TPool::sendMessageOnPool(TAbstractMessagePtr message)
{
	_messages.push_back(message);
	for (auto actor:_map)
	{
		if (_messages.size()==0) return;
		if (!actor.second)
		{
            actor.first->addMessage(_messages.front());
            _messages.pop_front();
			actor.first->addMessage(NMessageFunction::normalMessageTheSender(Command::SetFreeActorInPool,this->getThis()));
		}
	}
}

