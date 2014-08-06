#include "TProfilerQueue.h"



TProfilerQueue::TProfilerQueue():_totalCount(0),_maxCount(0),_countInMeasurments(){}

TProfilerQueue::~TProfilerQueue(){}

void TProfilerQueue::addValue(const unsigned int countQueueValue)
{
    _totalCount+=countQueueValue;
    if (_maxCount<countQueueValue) _maxCount=countQueueValue;
    _countInMeasurments.push_back(countQueueValue);
    if (_countInMeasurments.size()>100) _countInMeasurments.pop_front();
}

std::string TProfilerQueue::inJson() const
{
    std::string inJsonString="{";
    inJsonString+="\'totalCount\':"+std::to_string(_totalCount)+",";
    inJsonString+="\'maxCount\':"+std::to_string(_maxCount)+",";
    inJsonString+="\'countInMeasurments\':[";
    for (auto i = _countInMeasurments.begin(); i != _countInMeasurments.end(); ++i)
    {inJsonString+=std::to_string(*i); if (i!=_countInMeasurments.end()) inJsonString+=",";};
    inJsonString+="]}";
    return inJsonString;
}
