#ifndef T_ADD_PROFILER_ACTORS_H
#define T_ADD_PROFILER_ACTORS_H
#include "actor/Messages.h"
#include <chrono>

// Характеристика включена
struct ON{};
// Характеристика выключена
struct OFF{};

//typedef State ON;

///обертка для добавления в профайлер информации о таймингах
template<typename T>
class TAddProfilerTimeValue
{};




template<>
class TAddProfilerTimeValue<ON>
{
    private:
        long long _startTime;
		std::string _typeNameObj;
		Command _message;

public:
	explicit TAddProfilerTimeValue(std::string typeNameObj,Command message );
	~TAddProfilerTimeValue();
    
};


template<>
class TAddProfilerTimeValue<OFF>
{
public:
	explicit TAddProfilerTimeValue(std::string typeNameObj,Command message )
	{(void)typeNameObj;(void)message;}
	~TAddProfilerTimeValue(){}
    
};

///обертка для добавления в профайлер информации об сосотоянии очереди
template<typename T>
class TAddProfilerQueueValue
{};




template<>
class TAddProfilerQueueValue<ON>
{
private:
public:
	explicit TAddProfilerQueueValue(std::string typeNameObj, unsigned int countValue );
	~TAddProfilerQueueValue(){}

};


template<>
class TAddProfilerQueueValue<OFF>
{
public:
	explicit TAddProfilerQueueValue(std::string typeNameObj, unsigned int countValue )
	{(void)typeNameObj;(void)countValue;}
	~TAddProfilerQueueValue(){}

};

/////////////////////////CONFIG////////////////////////////////////////////////
typedef TAddProfilerTimeValue<ON>	addProfilerTimeValue;
typedef TAddProfilerQueueValue<ON>	addProfilerQueueValue;
//////////////////////////////////////////////////////////////////////////////

#endif



