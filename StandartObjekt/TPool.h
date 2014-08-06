#ifndef TPOOL_H
#define TPOOL_H
#include "actor/tactor.h"
#include "actor/IActor.h"

namespace NPool
{

class TPool:public TActor
{
private:
	std::map<IActorPtr, bool> _map;
	std::list<TAbstractMessagePtr> _messages;
public:
	TPool();
	virtual ~TPool();
	//добавляем актор в пул
	void addActor(IActorPtr actor);
	//убираем актор из пула
	void removeActor(IActorPtr actor);
	//выставляем флажек свободы у актора
	void setFreeActor(IActorPtr actor);
	//отправляем сообщение в пул
	void sendMessageOnPool(TAbstractMessagePtr message);


};

NOT_USED static void addActorInPool(TAbstractMessagePtr message, IActorPtr currentActor)
{
	castActor<TPool>(currentActor)->addActor(boost::any_cast<IActorPtr>(message->getData()));
}

NOT_USED static void removeActorInPool(TAbstractMessagePtr message, IActorPtr currentActor)
{
	castActor<TPool>(currentActor)->removeActor(boost::any_cast<IActorPtr>(message->getData()));
}

NOT_USED static void sendMessageInPool(TAbstractMessagePtr message, IActorPtr currentActor)
{
	castActor<TPool>(currentActor)->sendMessageOnPool(message);
}

NOT_USED static void sendFreeAtPool(TAbstractMessagePtr message, IActorPtr currentActor)
{
	for (auto& sender:message->getSenders())
		sender->addMessage(NMessageFunction::normalMessageTheSender(Command::GetFreeAtActor,currentActor));
}

NOT_USED static void getFreeAtActor(TAbstractMessagePtr message, IActorPtr currentActor)
{
	for (auto& sender:message->getSenders())
		castActor<TPool>(currentActor)->setFreeActor(sender);
}

}


#endif
