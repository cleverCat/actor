#include "TProfilerValue.h"

TProfilerValue::TProfilerValue():_allTime(0),_times(){}

TProfilerValue::~TProfilerValue(){}

void TProfilerValue::addValue(double timeValue)
{
    _allTime+=timeValue;
    _times.push_back(timeValue);
    if (_times.size()>100) _times.pop_front();
}

std::string TProfilerValue::inJson() const
{
    std::string inJsonString="{";
    inJsonString+="\'allTime\':"+std::to_string(_allTime)+",";
    inJsonString+="\'times\':[";
    for (auto i = _times.begin(); i != _times.end(); ++i)
    {inJsonString+=std::to_string(*i); if (i!=_times.end()) inJsonString+=",";};
    inJsonString+="]}";
    return inJsonString;
}

TtestResultAndComment TProfilerValue::test()
{
    TProfilerValue pv;
    for (int i = 0; i < 110; ++i)
    {
        pv.addValue(1);
    }
    assert(static_cast<int>(pv._allTime)==110);
    assert(pv._times.size()<100);
    assert(pv._times.size()>10);
    if (!(static_cast<int>(pv._allTime)==110)){return TtestResultAndComment(false, "problem in TProfilerValue");}
    if (!(pv._times.size()<100)){return TtestResultAndComment(false,"problem in TProfilerValue");}
    if (!(pv._times.size()>10)){ return TtestResultAndComment(false,"problem in TProfilerValue");}
    return TtestResultAndComment(true,"");
}
