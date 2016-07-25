
#define HeaderForInlude <QCoreApplication>
#include "baseLib/importWithoutWarning.h"

#include <iostream>

#include "test/TestExecuter.h"
#include "test/TestMessage.h"
#include "test/TestActor.h"
#include "test/TestStorage.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout<<"start Test"<<std::endl;
  /*  TestExecuter().test();
    TestMessage().test();
    TestActor().test();*/
    TestStorage().test();
    std::cout<<"end Test"<<std::endl;

    return a.exec();
}
