#ifndef LOGFORTEST_H
#define LOGFORTEST_H

#include <list>
#include <algorithm>
#include <string>
#include <iostream>

#include "Singleton.h"

namespace Test {

class LogForTest
{
private:
    std::list<std::string> _blackList;
public:
    LogForTest():_blackList(){}

    void setBlackList(const std::list<std::string> & list) {_blackList=list;}

    void write(const std::string& nameMeasure, const std::string& message)const
    {
        if (std::find(_blackList.begin(), _blackList.end(), nameMeasure)!=_blackList.end()) return;
        std::cout<<message<<std::endl;
    }
};

}


typedef Singleton<Test::LogForTest> LogForTest;
static void logForTest(const std::string& nameMeasure, const std::string& message)
    { Singleton<Test::LogForTest>::instance().write(nameMeasure, message);}

#endif // LOGFORTEST_H
