#ifndef T_SIMPLE_TRANSPARENT_ROUTER_H
#define T_SIMPLE_TRANSPARENT_ROUTER_H


#include <map>
#include <set>
#include <string>

#include "actor/IActor.h"
#include "actor/Messages.h"



//класс для хранения глобальных акторов
class TSimpleTransparentActors
{
private:
    std::map<std::string, IActorPtr> _map;

    explicit TSimpleTransparentActors():_map(){}


public:
    virtual ~TSimpleTransparentActors(){}

    //добавляем актор для обращении к нему по имени
    void addActor(std::string name,IActorPtr actor);
    //посылаем комманду интересующему нас актору ))
    void addMessage(std::string name, TAbstractMessagePtr message);
    //получаем ссылку на экземпляр
    static std::shared_ptr<TSimpleTransparentActors> instance();

};

#endif
