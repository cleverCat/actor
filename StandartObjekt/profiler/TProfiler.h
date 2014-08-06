#ifndef T_PROFILER_ACTORS_H
#define T_PROFILER_ACTORS_H


#include <map>
#include <set>
#include <string>
#include <ratio>
#include <chrono>
#include <list>
#include <utility>
#include <ctime>
#include <tuple>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "actor/tactor.h"
#include "actor/Messages.h"
#include "actor/StandartObjekt/TSimpleTransparentActors.h"

#include "actor/StandartObjekt/profiler/TProfilerQueue.h"
#include "actor/StandartObjekt/profiler/TProfilerValue.h"





///актор хранящий результатаы профилирования
class TProfiler:public TActor
{
private:
	std::map<std::string, std::map<Command, TProfilerValue> > _mapTimeMeasurment;
	std::map<std::string, TProfilerQueue> _mapQueueValue;
public:
	TProfiler();
    virtual ~TProfiler();

public:
    //добавить временные показатели
    void addTimeValue(std::string typeNameObj,Command message ,double timeValue);
    //добавить показания загрузки очереди
    void addQueueValue(std::string typeNameObj,const unsigned int queueCountValue);
    //представить показания в виде json обьекта для отображения в файл или по сети
    std::string inJson(void)const;

    void execute(const uint TimeOut=0);

};

namespace NProfiler
{
	NOT_USED static void addProfilerTimeValue(TAbstractMessagePtr message, IActorPtr currentActor)
	{
		//std::cout<<"NProfiler::addProfileValue ";
		auto data=boost::any_cast<std::tuple<std::string,Command,double> >(message->getData());
		castActor<TProfiler>(currentActor)->addTimeValue(std::get<0>(data),std::get<1>(data),std::get<2>(data));

	}

	NOT_USED static void addProfilerQueueValue(TAbstractMessagePtr message, IActorPtr currentActor)
	{
		//std::cout<<"NProfiler::addProfilerQueueValue ";
		auto data=boost::any_cast<std::tuple<std::string,unsigned int> >(message->getData());
		castActor<TProfiler>(currentActor)->addQueueValue(std::get<0>(data),std::get<1>(data));

	}

	NOT_USED static void getProfilerInJson(TAbstractMessagePtr message, IActorPtr currentActor)
	{
		std::cout<<"getProfilerInJson";
		auto profilerInJson=castActor<TProfiler>(currentActor)->inJson();
		for(auto i:message->getSenders()) i->addMessage(NMessageFunction::normalMessageWithDate<std::string>(Command::GetProfilerInJson,profilerInJson));
		std::cout<<"getProfilerInJson";
	}
}

NOT_USED void addProfiler(void);







#endif
