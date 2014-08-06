#ifndef TTASK_H
#define TTASK_H

#include "actor/IActor.h"
#include <thread>
#include <utility>

//класс задачи для таймера
class TTask
{
    private:
        TAbstractMessagePtr _message;//сообщение
        IActorPtr _actor;//актор
        std::chrono::milliseconds _timeout;//таймаут
        const bool _isRepeat;//повторяеться ли таймер
public:
    TTask(TAbstractMessagePtr message, IActorPtr actor, std::chrono::milliseconds timeout, const bool isRepeat);
    virtual ~TTask();
    //узнаем повторяеться ли таймер
    bool isRepeat(void) const;
    //отправляем сообщение
    void sendMessage(void);
    //получаем заданный таймаут
    std::chrono::milliseconds getTimeout(void)const;
};

#endif // TTASK_H
