#ifndef EXECUTER_H
#define EXECUTER_H
#include <tuple>
#include <functional>
#include <memory>


#include "Message.h"

template<typename TypeFunctor, typename Tuple, bool Enough, int CountArgs, int... N>
class callFunctor
{
public:
    auto static call(TypeFunctor f, Tuple&& t)
        {return callFunctor<TypeFunctor, Tuple, CountArgs == 1 + sizeof...(N), CountArgs, N..., sizeof...(N)>::call(f, std::forward<Tuple>(t));}
};

template<typename F, typename Tuple, int CountArgs, int... N>
class callFunctor<F, Tuple, true, CountArgs, N...>
{
public:
    auto static call(F f, Tuple&& t)
        {return f(std::get<N>(std::forward<Tuple>(t))...);}
};

template<typename F, typename Tuple>
auto call(F f, Tuple&& t)
{
    typedef typename std::decay<Tuple>::type type;
    return callFunctor<F, Tuple, 0 == std::tuple_size<type>::value, std::tuple_size<type>::value>::call(f, std::forward<Tuple>(t));
}

class AbstractExecuter
{
public:
    virtual void execute(AbstractMessagePtr message)=0;
};

template<typename... Types>
class Executer:public AbstractExecuter
{
private:
    std::function<void(Types...)> _functor;
public:
    Executer(std::function<void(Types...)> functor):_functor(functor){}
    ~Executer(void){}
    void execute(AbstractMessagePtr message) override
    {
        std::tuple<Types...> dataFromMessage = std::static_pointer_cast<const Message<Types...> >(message)->getData();
        call(_functor, dataFromMessage);
    }
};

#endif // EXECUTER_H
