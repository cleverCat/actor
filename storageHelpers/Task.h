#ifndef TASK_H
#define TASK_H

#include "Message.h"
#include "AbstractActor.h"
#include <thread>
#include <utility>

//класс задачи для таймера
class Task
{
    private:
        AbstractMessagePtr _message;//сообщение
        AbstractActorPtr _actor;//актор
        std::chrono::milliseconds _timeout;//таймаут
        const bool _isRepeat;//повторяеться ли таймер
public:
    //конструктор задачи
    Task(AbstractMessagePtr message, AbstractActorPtr actor, std::chrono::milliseconds timeout, const bool IsRepeat);
    virtual ~Task();
    //узнаем повторяеться ли таймер
    bool isRepeat(void) const;
    //отправляем сообщение
    void sendMessage(void);
    //получаем заданный таймаут
    std::chrono::milliseconds getTimeout(void)const;
};


#endif // TASK_H
