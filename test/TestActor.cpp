#include <memory>
#include <iostream>

#define HeaderForInlude <QDebug>
#include "baseLib/importWithoutWarning.h"

#include "TestActor.h"
#include "Actor.h"
#include "Storage.h"
#include "helpers/LogForTest.h"

TestActor::TestActor()
{
}

class MockActor:public Actor
{
public:
    MockActor(AbstractActorPtr storage):Actor(storage)
    {
        auto f=std::function<void(int)>([this] (int value) {
            std::cout<<"value = "<<value<<std::endl;
            assert(value==3);});
        this->addExecuterOnMessage(1, f);
    }
};

class MockStorage:public AbstractActor
{
private:
    std::shared_ptr<MockActor> _equalActor;
public:
    void setEqualActor(std::shared_ptr<MockActor> actor){this->_equalActor=actor;}

    void addMessage(AbstractMessagePtr message)
    {
        auto id=message->getID();
        assert(id==StorageMessge::EXECUTE_ACTOR);
        std::cout<<"type message ="<<id<<std::endl;
        auto t=std::static_pointer_cast<const Message<AbstractActorPtr> >(message)->getData();
        auto a = std::get<0>(t);
        std::cout<<"equal Actors"<<std::endl;
        assert(a==this->_equalActor);
    }

    void execute(){}

};

static std::shared_ptr<MockActor> getActor(void){
    AbstractActorPtr storage = std::shared_ptr<MockStorage>(new MockStorage());
    auto a=std::shared_ptr<MockActor>(new MockActor(storage));
    std::static_pointer_cast<MockStorage>(storage)->setEqualActor(a);
    return a;
}

void TestActor::test(){
    //std::shared_ptr<Storage>(new Storage());
    logForTest("TestActor", "start actor test");
    auto a = getActor();
    logForTest("TestActor", "create actor");
    std::shared_ptr<AbstractMessage> m = std::make_shared<Message<int> >(Message<int>(1, 3));
    logForTest("TestActor", "create message");
    a->addMessage(m);
    logForTest("TestActor", "add message");
    a->execute();
    logForTest("TestActor", "execute");
    logForTest("TestActor", "test Actor complited");
}
