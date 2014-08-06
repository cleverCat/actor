#ifndef TPROFILERVALUE_H
#define TPROFILERVALUE_H

#include <list>
#include <iostream>
#include "assert.h"
#include "actor/test/testBase/TTestPrimitive.h"

///класс для хранения данных об таймингах обработки сообщений
class TProfilerValue
{
private:
	double _allTime;
	std::list<double > _times;
public:
	TProfilerValue();
    virtual ~TProfilerValue();
	///добавляем значение
    void addValue(double timeValue);

	///получаем значение измерения в виде jsona
    std::string inJson(void)const;

    TtestResultAndComment test(void);

};
#endif // TPROFILERVALUE_H
