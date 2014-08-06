#include "actor/TTask.h"

//конструктор задач для таймера
TTask::TTask(TAbstractMessagePtr message, IActorPtr actor, std::chrono::milliseconds timeout, const bool isRepeat):_message(message), _actor(actor), _timeout(timeout), _isRepeat(isRepeat){}

TTask::~TTask(){}

bool TTask::isRepeat(void) const {return _isRepeat;}

void TTask::sendMessage() {_actor->addMessage(_message);}

chrono::milliseconds TTask::getTimeout() const{return _timeout;}


