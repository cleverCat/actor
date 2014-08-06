
#include <set>
#include <typeindex>
#include <exception>

#include "actor/IActor.h"

#include "actor/TExeption.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  TExeptionRouter::TExeptionFilter::TExeptionFilter
NExeptionRouter::TExeptionFilter::TExeptionFilter(std::set<type_index> exeptions, std::set<Command> commands, std::set<IActorPtr> executers):_listExecuters(executers),_terms(std::make_pair(exeptions,commands)){}

NExeptionRouter::TExeptionFilter::~TExeptionFilter(){}

bool NExeptionRouter::TExeptionFilter::inExeption(std::type_index errorType)const///проверяем есть ли это исключение в списке исключений
{
    if (_terms.first.empty()) return true;
    if (_terms.first.end()!=_terms.first.find(errorType)) return true;
    return false;
}


bool NExeptionRouter::TExeptionFilter::inCommand(Command errorCommand)const///проверяем есть ли это исключение в списке комманд
{
    if (_terms.second.empty()) return true;
    if (_terms.second.end()!=_terms.second.find(errorCommand)) return true;
    return false;
}

bool NExeptionRouter::TExeptionFilter::executeExeption(const std::exception & error,const Command errorCommand, IActorPtr actorDropExeption)const///рассылаем исключение по списку рассылки
{
    if (inExeption(typeid(error)) && inCommand(errorCommand))
    {
        for (auto actor:_listExecuters)
        {
            TAbstractMessagePtr ko (new TMessage<std::pair<std::exception,Command> >(Command::SendException,TypeMessage::Extra,std::pair<std::exception,Command >(error,errorCommand),{actorDropExeption}));
             actor->addMessage(ko);
        }
        return true;
    }else return false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// NExeptionRouter::TExeptionRouter::TExeptionRouter
NExeptionRouter::TExeptionRouter::TExeptionRouter():_filters(){}

NExeptionRouter::TExeptionRouter::~TExeptionRouter(){}

void NExeptionRouter::TExeptionRouter::executeFilters(const exception &ex, const Command command)
{
    for (auto i = _filters.rend(); i != _filters.rbegin(); ++i)
    {
        if (i->executeExeption(ex, command, this->getThis())) return;//все хорошо обработчик найден теперь это проблемы тех чуваков которым мы отправили сообщение
    }
    ///о еей мы не нашли обработчик исключения похоже пора записать это в лог или смолчать я еще не решил что лучше
    /// ну или кинуть это исключение дальше мы ведь не смогли его обработать в конце концов
    throw ex;
}

void NExeptionRouter::TExeptionRouter::addFilter(const NExeptionRouter::TExeptionFilter filter)
{
    _filters.push_back(filter);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

