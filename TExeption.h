#ifndef TEXEPTION_ROUTER
#define TEXEPTION_ROUTER
#include <exception>
#include <typeindex>
#include <set>

#include "actor/IActor.h"


///штука которая форвордит исключения по конечным адресам
namespace NExeptionRouter
{
	///класс описывающий исключение
	class TExeptionFilter
	{
	private:
        std::set<IActorPtr> _listExecuters;
		///условия при которых исключение сработает
        std::pair<std::set<std::type_index>,std::set<Command> > _terms;
    public:
        TExeptionFilter(std::set<std::type_index> exeptions, std::set<Command> commands,std::set<IActorPtr> executers);
        virtual ~TExeptionFilter();
    private:

		///проверяем есть ли это исключение в списке исключений если список исключений пуст сщитаем что оно там есть
        bool inExeption(std::type_index errorType)const;
		///проверяем есть ли это исключение в списке комманд во время которых происходит исключение если список комманд пуст сщитаем что оно там есть
        bool inCommand(Command errorCommand)const;

    public:

        ///если исключение подходит под наш фильтр рассылаем тем кто на него подписан сообщение о том что произошло исключение и возвращаем true
		/// в обратном случае возвращаем false
        bool executeExeption(const std::exception & error,const Command errorCommand, IActorPtr actorDropExeption)const;
		
	};

///штука которая форвордит исключения по конечным адресам
class TExeptionRouter:public IActor
{
private:
	///список фильтров в которых мы будем искать наше исключенение
	std::list<TExeptionFilter> _filters;
public:
    TExeptionRouter();
    virtual ~TExeptionRouter();

	///находим обработчик исключения и отправляем ему сообщение об ошибке
    void executeFilters(const std::exception & ex,const Command command);

	///добавляем фильтр 
    void addFilter(const TExeptionFilter filter);

};
}

#endif
