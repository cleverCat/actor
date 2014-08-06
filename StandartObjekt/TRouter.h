#ifndef TROUTER_H
#define TROUTER_H

#include "actor/tactor.h"
#include <utility>
#include <iostream>

namespace NStandartActors
{
	
	class ARouter:public TActor
	{
	private:
        std::map<Command, IActorPtr> _mapLink;
        std::map<Command, std::list<TAbstractMessagePtr> > _lostMessages;
	public:
        ARouter();
		ARouter(TStorageActorsPtr storage);
        virtual ~ARouter();

	///добавляем правило пересылки 
    void addLink(Command command, IActorPtr resiver);
    ///добавляем правила пересылки
    void addLinks(std::list<Command> commands, IActorPtr resiver);

	///добавляем сообщение 
    void addMessageOnList(TAbstractMessagePtr message);
		
	};

    /*
 ███████╗██╗  ██╗███████╗ ██████╗██╗   ██╗████████╗███████╗██████╗ ███████╗    ███╗   ███╗███████╗███████╗███████╗ █████╗  ██████╗ ███████╗
 ██╔════╝╚██╗██╔╝██╔════╝██╔════╝██║   ██║╚══██╔══╝██╔════╝██╔══██╗██╔════╝    ████╗ ████║██╔════╝██╔════╝██╔════╝██╔══██╗██╔════╝ ██╔════╝
 █████╗   ╚███╔╝ █████╗  ██║     ██║   ██║   ██║   █████╗  ██████╔╝███████╗    ██╔████╔██║█████╗  ███████╗███████╗███████║██║  ███╗█████╗
 ██╔══╝   ██╔██╗ ██╔══╝  ██║     ██║   ██║   ██║   ██╔══╝  ██╔══██╗╚════██║    ██║╚██╔╝██║██╔══╝  ╚════██║╚════██║██╔══██║██║   ██║██╔══╝
 ███████╗██╔╝ ██╗███████╗╚██████╗╚██████╔╝   ██║   ███████╗██║  ██║███████║    ██║ ╚═╝ ██║███████╗███████║███████║██║  ██║╚██████╔╝███████╗
 ╚══════╝╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═════╝    ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝    ╚═╝     ╚═╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝
     */


     NOT_USED static void command(TAbstractMessagePtr message, IActorPtr currentActor)
     {
         castActor<ARouter>(currentActor)->addMessageOnList(message);
     }

     NOT_USED static void addLink(TAbstractMessagePtr message, IActorPtr currentActor)
     {
         qDebug()<<"addLink";
         auto CommandAndRessiver=boost::any_cast<std::pair<Command, IActorPtr> >(message->getData());
         castActor<ARouter>(currentActor)->addLink(CommandAndRessiver.first,CommandAndRessiver.second);
     }

     NOT_USED static void addLinks(TAbstractMessagePtr message, IActorPtr currentActor)
     {
         auto CommandsAndRessiver=boost::any_cast<std::pair<std::list<Command>, IActorPtr> >(message->getData());
         castActor<ARouter>(currentActor)->addLinks(CommandsAndRessiver.first,CommandsAndRessiver.second);
     }
}

#endif
