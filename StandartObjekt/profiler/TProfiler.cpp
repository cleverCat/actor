#include "actor/StandartObjekt/profiler/TProfiler.h"
#include "actor/StandartObjekt/profiler/TAddProfilerValue.h"
#include "actor/IActor.h"
#include <sys/time.h>

long long mtime()
{
  struct timeval t;

  gettimeofday(&t, NULL);
  long long mt = (long long)t.tv_sec * 1000 + t.tv_usec / 1000;
  return mt;
}


TAddProfilerTimeValue<ON>::TAddProfilerTimeValue(std::string typeNameObj, Command message):_startTime(mtime()),_typeNameObj(typeNameObj),_message(message)
{
	addProfiler();
}



TAddProfilerTimeValue<ON>::~TAddProfilerTimeValue()
{
    auto lastTime=mtime();
	auto time_span=lastTime-_startTime;
    auto message=NMessageFunction::normalMessageWithDate(Command::Profiler,std::make_tuple(_typeNameObj,_message,static_cast<double>(time_span)));
	TSimpleTransparentActors::instance()->addMessage(std::string("profiler"),message);
}


TAddProfilerQueueValue<ON>::TAddProfilerQueueValue(std::string typeNameObj, unsigned int countValue)
{
	addProfiler();
	auto message=NMessageFunction::normalMessageWithDate(Command::ProfilerQueue,std::make_tuple(typeNameObj,countValue));
	TSimpleTransparentActors::instance()->addMessage(std::string("profiler"),message);
}








TProfiler::TProfiler():_mapTimeMeasurment(),_mapQueueValue()
{
	setFunctorOnMessage(Command::Profiler, TFunctorOnMessage(NProfiler::addProfilerTimeValue));
	setFunctorOnMessage(Command::ProfilerQueue, TFunctorOnMessage(NProfiler::addProfilerQueueValue));
	setFunctorOnMessage(Command::GetProfilerInJson, TFunctorOnMessage(NProfiler::getProfilerInJson));

}

TProfiler::~TProfiler(){}

void TProfiler::addTimeValue(string typeNameObj, Command message, double timeValue)
{
    if (_mapTimeMeasurment.end()==_mapTimeMeasurment.find(typeNameObj)) _mapTimeMeasurment.insert(std::make_pair(typeNameObj , std::map<Command , TProfilerValue>()));
    if (_mapTimeMeasurment.find(typeNameObj)->second.end()==_mapTimeMeasurment.find(typeNameObj)->second.find(message)) _mapTimeMeasurment.find(typeNameObj)->second.insert(std::make_pair(message,TProfilerValue()));
    _mapTimeMeasurment.find(typeNameObj)->second.find(message)->second.addValue(timeValue);
}

void TProfiler::addQueueValue(string typeNameObj, const unsigned int queueCountValue)
{
    if (_mapQueueValue.end()==_mapQueueValue.find(typeNameObj)) _mapQueueValue.insert(std::make_pair(typeNameObj , TProfilerQueue()));
    _mapQueueValue.find(typeNameObj)->second.addValue(queueCountValue);
}

string TProfiler::inJson() const
{
    std::string inJsonString="{\'actorProfile\':[";
    for(auto i =_mapTimeMeasurment.begin();i!=_mapTimeMeasurment.end();++i)
    {
        inJsonString+="\'"+i->first+"\':[";
        for(auto j =i->second.begin();j!=i->second.end();++j)
        { inJsonString+="\'"+std::to_string(j->first)+"\':"+j->second.inJson(); if(j!=i->second.end()) inJsonString+=",";  };
        inJsonString+="]";
        if(i!=_mapTimeMeasurment.end()) inJsonString+=",";
    }
    inJsonString+="]";
    inJsonString+=", \'actorProfileQueue\':[";
    for (auto i =_mapQueueValue.begin();i!=_mapQueueValue.end();++i)
    {
        inJsonString+="\'"+i->first+"\':"+i->second.inJson();
        if(i!=_mapQueueValue.end()) inJsonString+=",";
    }
    inJsonString+="]}";
    return inJsonString;
}

void TProfiler::execute(const uint TimeOut)//выбираем сообщение из очереди и вызываем соответствующий функтор
{
    //std::cout<<"EXECUTE";
    (void)TimeOut;
    TAbstractMessagePtr message;
    try{
        //	try
        {message=_queueMessage.pop();}//TimeOut);}
        //	catch(TUnboundedQueueTimeoutException){return;} //мы не дождались данных по таймауту ничего страшного подождем во время вызова следующего execute
        if (_mapFunctorsOnMessage.end()==_mapFunctorsOnMessage.find(message->getMessage()))
        {_mapFunctorsOnMessage[UncnowCommand](message, this->getThis() );}
        else
        {
            //std::cout<<"_mapFunctorsOnMessage";
            //std::cout<<(*message).getMessage();
            _mapFunctorsOnMessage.find(message->getMessage())->second(message , this->getThis());
        }
    }catch(std::exception& ex)
    {
        qDebug()<<"error message"<<message->getMessage();
        std::string nameType(typeid(this).name());
        qDebug()<<"error from"+QString::fromStdString(nameType);
        this->executeFilters(ex,message->getMessage());

    }
}

NOT_USED void addProfiler(void)
{
    static bool isProfilerStart=false;
    static boost::mutex lock;
    if (!isProfilerStart)
    {
        lock.lock();
        IActorPtr profiler(new TProfiler());
        TSimpleTransparentActors::instance()->addActor(std::string("profiler"),profiler);
        isProfilerStart=true;
        lock.unlock();
    }
}
