#include "Runnable.h"





Runnable::Runnable():_stop(true),_thread(){}

Runnable::~Runnable()
{
    try{stop();}
    catch(std::exception ex){ifThrowExeptionOnStop(ex);}
    catch(...){ifThrowExeptionOnStop();}
}

void Runnable::stop()
{
    if (!_stop)
    {
        _stop = true;
        _thread.join();
    };
}

void Runnable::start() { _stop=false; _thread = std::thread(&Runnable::run, this);}

void Runnable::ifThrowExeptionOnStop(){throw std::exception();}

void Runnable::ifThrowExeptionOnStop(std::exception ex){throw ex;}
