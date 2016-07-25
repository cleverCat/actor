#include "TestStorage.h"
#include "Storage.h"
#include "helpers/LogForTest.h"
#include "Actor.h"
#include <memory>

TestStorage::TestStorage()
{

}

class MockActor:public Actor//, public std::enable_shared_from_this<Actor>
{
public:
    bool _flag;

    MockActor(AbstractActorPtr storage):Actor(storage),_flag(false){
        auto f=std::function<void(int)>([this] (int value) {
            std::cout<<"value = "<<value<<std::endl;
            assert(value==3);});
        this->addExecuterOnMessage(1, f);
    }

};

void TestStorage::testExecuteActor()
{
    AbstractActorPtr storage = std::shared_ptr<Storage>(new Storage());
    AbstractActorPtr a=std::shared_ptr<MockActor>(new MockActor(storage));
    std::shared_ptr<AbstractMessage> storageMessage = std::make_shared<Message<int> >(Message<int>(1, 3));
    a->addMessage(storageMessage);

    //auto flag=std::static_pointer_cast<std::shared_ptr<MockActor> >(a)->_flag;
}

void TestStorage::test()
{
    std::shared_ptr<Storage>(new Storage());
    testExecuteActor();
}



