#ifndef RUNNABLE_H
#define RUNNABLE_H

#include <atomic>
#include <thread>

class Runnable
{
public:
    Runnable(void);
    virtual ~Runnable();

    Runnable(Runnable const&) = delete;
    Runnable& operator =(Runnable const&) = delete;

    ///останавливаем поток
    void stop();
    ///запускаем поток
    void start();

protected:
    ///обработчик потока
    virtual void run()=0;

    ///что то пошло не так и мы не можем остановить поток ((
    virtual void ifThrowExeptionOnStop(void);
    virtual void ifThrowExeptionOnStop(std::exception ex);
    std::atomic<bool> _stop;

private:
    std::thread _thread;
};

///EXAMPLE
//class myThread : public Runnable
//{
//protected:
//    void run() { while (!_stop) { /* do something... */ }; }
//};

#endif // RUNNABLE_H
