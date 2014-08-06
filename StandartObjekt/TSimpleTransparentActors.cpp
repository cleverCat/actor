#include "actor/StandartObjekt/TSimpleTransparentActors.h"



void TSimpleTransparentActors::addActor(string name, IActorPtr actor)
{
    if (_map.end()==_map.find(name))
    {
        _map.insert(std::make_pair(name,actor));
        std::cout<<name;
        std::cout<<" insert in TSimpleTransparentActors"<<std::endl;
    }
    else  std::cout<<"for this name i get actor;"<<std::endl;
}

void TSimpleTransparentActors::addMessage(string name, TAbstractMessagePtr message)
{
    if (_map.end()==_map.find(name))
    {std::cout<<"for this name i dont have actor;"<<std::endl; return;}
    (*_map.find(name)).second->addMessage(message);
}

std::shared_ptr<TSimpleTransparentActors> TSimpleTransparentActors::instance()
{
    static auto instance = std::shared_ptr<TSimpleTransparentActors>(new TSimpleTransparentActors());
    return instance;
}
