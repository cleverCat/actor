#include "actor/StandartObjekt/TCleverCash.h"


template<typename T, typename TypeParamsFunctor>
TCleverCash<T,TypeParamsFunctor>::TCleverCash(std::function<string (TypeParamsFunctor, std::list<T> &)> functor, const unsigned int size):_functor(functor),_Size(size)
{
    setFunctorOnMessage(Command::PushData, TFunctorOnMessage(NCleverCash::pushDataFromCleverCash<T,TypeParamsFunctor>));
    setFunctorOnMessage(Command::GetSerialize, TFunctorOnMessage(NCleverCash::getSerialize<T,TypeParamsFunctor>));
}
