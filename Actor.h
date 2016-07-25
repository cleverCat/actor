#ifndef ACTOR_H
#define ACTOR_H

#include <memory>
#include <map>
#include <functional>

#include "Executer.h"
#include "Message.h"
#include "TQueue.h"
#include "AbstractActor.h"

class Actor:public AbstractActor, public std::enable_shared_from_this<Actor>
{
private:
    AbstractActorPtr _storage;
public:
    Actor(AbstractActorPtr storage);

    virtual void execute(void);
    AbstractActorPtr getThis(void);

    void addMessage(AbstractMessagePtr message);
};

#endif // ACTOR_H
