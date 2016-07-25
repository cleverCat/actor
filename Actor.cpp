#include "Actor.h"
#include <iostream>
#include "Storage.h"

Actor::Actor(AbstractActorPtr storage):AbstractActor(),_storage(storage){}

void Actor::execute(void)
{
    auto message = _queueMessage.pop();
    if (_executers.find(message->getID())==_executers.end()){throw NotGetExecutersInActor();};
    _executers.find(message->getID())->second->execute(message);
 //   std::cout<<"Actor::execute"<<std::endl;
}

AbstractActorPtr Actor::getThis(){return shared_from_this();}

void Actor::addMessage(AbstractMessagePtr message)
{
    _queueMessage.push_back(message);
    std::shared_ptr<AbstractMessage> storageMessage = std::make_shared<Message<AbstractActorPtr> >(Message<AbstractActorPtr>(StorageMessge::EXECUTE_ACTOR, this->getThis()));
    _storage->addMessage(storageMessage);
}
