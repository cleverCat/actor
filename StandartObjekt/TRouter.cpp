#include "actor/StandartObjekt/TRouter.h"

///класс роутера
NStandartActors::ARouter::ARouter():_mapLink(),_lostMessages()
{    
    setFunctorOnMessage(Command::AddLink, TFunctorOnMessage(NStandartActors::addLink));
    setFunctorOnMessage(Command::AddLinks, TFunctorOnMessage(NStandartActors::addLinks));
    setFunctorOnMessage(Command::UncnowCommand, TFunctorOnMessage(NStandartActors::command));
}

///добавим связь между коммандой и актором которому будем пересылать
NStandartActors::ARouter::ARouter(TStorageActorsPtr storage):TActor(storage),_mapLink(),_lostMessages()
{
	setFunctorOnMessage(Command::AddLink, TFunctorOnMessage(NStandartActors::addLink));
	setFunctorOnMessage(Command::AddLinks, TFunctorOnMessage(NStandartActors::addLinks));
	setFunctorOnMessage(Command::UncnowCommand, TFunctorOnMessage(NStandartActors::command));
}

NStandartActors::ARouter::~ARouter(){}

void NStandartActors::ARouter::addLink(Command command, IActor::IActorPtr resiver)
{
	//qDebug()<<"ARouter::addLink";
	//qDebug()<<command;
	if (_mapLink.end()==_mapLink.find(command))
	{
		_mapLink.insert(std::make_pair(command,resiver));
		if (_lostMessages.end()!=_lostMessages.find(command))
		{
			for (auto message:_lostMessages.find(command)->second)
                resiver->addMessage(message);
            _lostMessages.erase(_lostMessages.find(command));
        }
    }
}
 ///добавляем правила пересылки
void NStandartActors::ARouter::addLinks(std::list<Command> commands, IActor::IActorPtr resiver){ for(auto command:commands){addLink(command,resiver);};}

///добавим сообщение для пересылки
void NStandartActors::ARouter::addMessageOnList(TAbstractMessagePtr message)
{
    Command command=message->getMessage();
    if (_mapLink.end()==_mapLink.find(command))
    {
        qDebug()<<"uncnowCommand";
        qDebug()<<command;
        if (_lostMessages.end()==_lostMessages.find(command))
        {
            auto l=std::list<TAbstractMessagePtr>();
            l.push_back(message);
            _lostMessages.insert(std::make_pair(command,l));
        }
        else
            _lostMessages.find(command)->second.push_back(message);
    }
    else
        _mapLink.find(command)->second->addMessage(message);
}

