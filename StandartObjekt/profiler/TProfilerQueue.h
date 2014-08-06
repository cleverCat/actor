#ifndef TPROFILERQUEUE_H
#define TPROFILERQUEUE_H

#include <list>
#include <iostream>

///класс для хранения данных о загруженности очереди
class TProfilerQueue
{
	unsigned int _totalCount;
	unsigned int _maxCount;
	std::list<unsigned int> _countInMeasurments;
public:
    TProfilerQueue();
    virtual ~TProfilerQueue();
	///добавляем значение
    void addValue(const unsigned int countQueueValue);
	///получаем значение измерения в виде jsona
    std::string inJson(void)const;
};

#endif // TPROFILERQUEUE_H
