#include "baseLib/importWithoutWarning.h"
#include "TestExecuter.h"
#include <iostream>
#include <Executer.h>


TestExecuter::TestExecuter()
{
}

static void foo(int i, double d)
{
    std::cout << "foo: " << i << " " << d << std::endl;
}

void TestExecuter::test()
{
    std::tuple<int, double> t1(1, 2.3);
    call(foo, t1);
    std::shared_ptr<AbstractMessage> m= std::make_shared<Message<int, double> >(Message<int, double>(1, 3, 2.45));
    std::function<void(int, double)> f1111= foo;
    std::shared_ptr<AbstractExecuter> f1 = std::make_shared<Executer<int, double> >(Executer<int, double>(f1111));
    f1->execute(m);
    qDebug()<<"test";
}
