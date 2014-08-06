#ifndef __TEST_PRIMITIVE__
#define __TEST_PRIMITIVE__

#include <list>
#include <functional>

typedef std::pair<bool,std::string> TtestResultAndComment;

///класс для тестирования
/// может содержать 1 тест в виде функтора или список тестов
class TestPrimitive
{
private:
    const std::string _name; //имя теста
    const std::list<TestPrimitive> _tests; //список вложенных тестов
    const std::function<TtestResultAndComment(void) > _test;//тест
public:
     TestPrimitive(std::string name, std::list<TestPrimitive> listTest):_name(name),_tests(listTest),_test(){}//коструктор для контейнера тестов
     TestPrimitive(std::string name, std::function<TtestResultAndComment(void) > testFunctor ):_name(name),_tests(std::list<TestPrimitive>()),_test(testFunctor){}//конструктор для конечного теста
    virtual ~TestPrimitive(){}

    ///получаем результаты тестирования в виде json строки //возможно в будущем возвращать обьект а затем его серилизовывать
    std::string test()const
    {
        if (_tests.size()==0)
        {
            auto resultTest=_test();
            return "{"+_name+":"+(resultTest.first?"chek":"unchek")+(resultTest.second!=""?", comment:"+resultTest.second:"")+"}"; //немного магии оператора (условие?если да:если нет)
        }
        else
        {
            std::string result="{";
            for (auto it=_tests.begin();it!=_tests.end();it++)
                {result+=(it!=_tests.begin()?",":"")+it->test();}
            result+="}";
            return result;
        }
    }



};
#endif

