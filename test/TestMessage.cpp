#include <memory>
#include <assert.h>
#include <iostream>

#include "TestMessage.h"
#include "Message.h"



TestMessage::TestMessage()
{
}

void TestMessage::test(){
    std::shared_ptr<AbstractMessage> m= std::make_shared<Message<int, double>>(Message<int, double>(1, 3, 2.45));
    std::cout<<m->getID();
}
