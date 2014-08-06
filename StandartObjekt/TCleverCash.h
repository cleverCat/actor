#ifndef __TCLEVER_CASH__
#define __TCLEVER_CASH__

#include <iostream>
#include <list>
#include <string>
#include <numeric>
#include <functional>
#include <assert.h> 
#include <actor/tactor.h>
#include <actor/IActor.h>



std::string testJson(std::string param, std::list<std::string>& _datas)
{
	std::list<std::string> result;
	for (auto i = _datas.rbegin(); i!=_datas.rend(); ++i)
	{
        if (*i==param)
			{break;}
        result.push_back(*i);
	}
	
	
	if (result.size()>0)
	{
		return (std::string("[")+std::accumulate(
												result.rbegin(), result.rend(), std::string(),
												[&](const std::string& first, const std::string& second){
																										  if (first=="")
																												return second;
																										  else
																												return first+std::string(",")+second;
																										}
													)+ std::string("]"));
	}else
	{
		return(std::string("[")+ std::string("]"));
	}

}

///кеш забывающий старые элементы и позволяющий самому пройтись по элементам
template<typename T, typename TypeParamsFunctor>
class TCleverCash:TActor
{
private:
    std::list<T> _listData; //данные
    const unsigned int _Size; //длинна запоминаемых данных
	std::function< std::string(TypeParamsFunctor, std::list<T>&)> _functor;
public:
    TCleverCash(std::function< std::string(TypeParamsFunctor, std::list<T>&)> functor, const unsigned int size);
	~TCleverCash(){}

	void push(T data)
	{
		_listData.push_back(data);
		if (_Size<=_listData.size()) _listData.pop_front();
	}


	std::string getInJson(TypeParamsFunctor param)
	{
		return _functor(param, _listData);
	}

    static bool test1()
	{
        std::function< std::string(std::string, std::list<std::string>&)> f=testJson;
		TCleverCash<std::string,std::string> cleverCat(f,10);
		cleverCat.push("0");
		cleverCat.push("1");
		cleverCat.push("2");
		cleverCat.push("3");
		cleverCat.push("4");
		cleverCat.push("5");
		cleverCat.push("6");
		assert(std::string("[4,5,6]")==cleverCat.getInJson("3"));
		return true;
	}
	
};

namespace NCleverCash
{
    template<typename T, typename TypeParamsFunctor>
    NOT_USED static void pushDataFromCleverCash(TAbstractMessagePtr message, IActorPtr currentActor)
        {(castActor<TCleverCash<T,TypeParamsFunctor> >(currentActor))->push(boost::any_cast<T>(message->getData()));}

    template<typename T, typename TypeParamsFunctor>
    NOT_USED static void getSerialize(TAbstractMessagePtr message, IActorPtr currentActor)
    {
        std::string json=(castActor<TCleverCash<T,TypeParamsFunctor> >(currentActor)->getInJson(boost::any_cast<TypeParamsFunctor>(message->getData())));
        for (auto & actor:message->getSenders())
            {actor->addMessage(NMessageFunction::normalMessageWithDate(Command::SendSerialize,json));}

    }

}



#endif
