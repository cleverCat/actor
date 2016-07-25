#include "Task.h"


Task::Task(AbstractMessagePtr message, AbstractActorPtr actor, std::chrono::milliseconds timeout, const bool IsRepeat):_message(message), _actor(actor), _timeout(timeout), _isRepeat(IsRepeat){}

Task::~Task(){}

bool Task::isRepeat(void) const {return _isRepeat;}

void Task::sendMessage() {_actor->addMessage(_message);}

chrono::milliseconds Task::getTimeout() const{return _timeout;}


