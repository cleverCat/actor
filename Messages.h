#ifndef MESSAGES_H
#define MESSAGES_H
#include <memory>
#include <assert.h>
#include <list>
#include <type_traits>
#include <boost/any.hpp>
#include <iostream>

#ifdef __GNUC__
#define NOT_USED __attribute__ ((unused))
#else
#define NOT_USED
#endif

///комманды которые будет различать система акторов
enum Command
{
	//base actors message
UncnowCommand=0,
DontCreateActorOfThisID=1,
Satisfiability=2,
RemoveActor=3,
AddActor=4,
ActorDead=5,
AddInCounterOnActor=6,
RemoveInCounterOnActor=7,
SendException=8,
	//Profiler
Profiler=9,
ProfilerQueue=10,
GetProfilerInJson=11,
	//storage actors message
AddActorOnSorage=12,
RunStorageThread=13,
RemoveActorFromStorage=14,
ExecuteActor=15,
AddTask=16,
	//base
CreateTable=17,
GetTableOnParam=18,
PushDataFromTable=19,
AddMeasurement=20,
GetDataOnTable=21,
	//repeatOnResponseFromUrl
GetDataOnURLParams=22,
CreateActorOnRequest=23,
FindFromDisk=24,
SendResponse=25,
SetStorage=26,
	//settings
SetSettings=27,
GetSetting=28,
	//Log
AddToLog=29,
SetCurrentTypeMessageLog=30,
	//ManagerCNCManager
GetAddresCNCs=31,
AnswerManagerCNCs=32,
SetAddresCNCs=33,
GetStateCNC=34,
GetListParamCNC=35,
SetUnsetParamsOnCNC=36,
GetHotOscillData=37,
GetCommand=38,
AddLink=39,
	//Protocol
SendBytes=40,
SendCommand=41,
SendCommandOnCNC=42,
	//ListParam
SetParam=43,
SetState=44,
StartGetsParam=45,
GetParam=46,
GetStartGroupParam=47,
GetStopGroupParam=48,
StopGetsParam=49,
GetState=50,
UnsetParam=51,
SetActiveParams=52,
GetListParams=53,
	//ManagerData
SendData=54,
	//TRouter
AddLinks=55,
	//Port
InitComPort=56,
	//TGetListParams
SetListParams=57,
	//TCleverCash
SendSerialize=58,
GetSerialize=59,
PushData=60,
	//Test
Test=61,
	//Pool
SetFreeActorInPool=62,
GetFreeAtActor=63,
AddActorInPool=64,
RemoveActorInPool=65
};

///тип сообщения (в зависимости от типа меняеться приоритет выполнения
enum TypeMessage
{
 Normal=0,
 Extra=1,
 WithTheExpectation=2
};
///класс для сообщения без типа
class NotType{
public: explicit NotType();
	virtual ~NotType();
};

///делаем предобьявление класса интерфейса актора чтобы иметь возможность запихнуть ссылку на него в отправителей
class IActor;

///интерфейс сообщений
class TAbstractMessage
{
public:
	explicit TAbstractMessage(void);
	virtual ~TAbstractMessage();
	///получить комманду сообщения
	virtual Command getMessage(void) const=0;
   ///получить тип сообщения
	virtual TypeMessage getType(void)const=0;
	/// плучаем данные упакованные в boost_any
	virtual boost::any getData(void)const=0;
	/// получаем список отправителей если они конечно сподобились запихнуть себя в него
	virtual std::list<std::shared_ptr<IActor> > getSenders(void)const=0;///просто удобный юзкейс
};



////////////запрещаем не указатели на не константные обьекты(защищаем слабую психику нашего программиста)////////////////////////
template<class T>
struct isCorrectTMessageData: std::integral_constant<bool, true>{};
template<class T>
struct isCorrectTMessageData<std::shared_ptr<T> >: std::integral_constant<bool, false>{};
template<class T>
struct isCorrectTMessageData<const std::shared_ptr<T> >: std::integral_constant<bool, false>{};
template<class T>
struct isCorrectTMessageData<volatile const std::shared_ptr<T> >: std::integral_constant<bool, false>{};
template<class T>
struct isCorrectTMessageData<T*>: std::integral_constant<bool, false>{};
template<class T>
struct isCorrectTMessageData<T* const>: std::integral_constant<bool, false>{};
template<class T>
struct isCorrectTMessageData<T* const volatile>: std::integral_constant<bool, false>{};
template<class T>
struct isCorrectTMessageData<std::shared_ptr<const T> >: std::integral_constant<bool, true>{};
template<class T>
struct isCorrectTMessageData<const std::shared_ptr<const T> >: std::integral_constant<bool, true>{};
template<class T>
struct isCorrectTMessageData<const T*>: std::integral_constant<bool, true>{};
template<class T>
struct isCorrectTMessageData<const T* const>: std::integral_constant<bool, true>{};

////////////////////////////////////////////////////////////////////////////////////

///класс сообщения
template<typename Data=NotType>
class TMessage:public TAbstractMessage
{
private:
	Command _message;
	TypeMessage _typeMessage;
	const Data _data;
	std::list<std::shared_ptr<IActor> > _senders;
public:
	///сообщение с данными
   explicit TMessage(Command message, TypeMessage typeMessage, Data data, std::list<std::shared_ptr<IActor> > senders ):_message(message),_typeMessage(typeMessage),_data(data),_senders(senders){}
	///сообщение без данных
   explicit TMessage(Command message, TypeMessage typeMessage, std::list<std::shared_ptr<IActor> > senders ):_message(message),_typeMessage(typeMessage),_data(NotType()), _senders(senders){}
	virtual ~TMessage(){}
	///получить данные
	boost::any getData(void)const override {boost::any data=_data; return data;}
	///получить комманду
	Command getMessage(void)const override{return _message;}
	///получить тип сообщения
	TypeMessage getType(void)const override {return _typeMessage;}
	 /// получаем список отправителей
	std::list<std::shared_ptr<IActor> > getSenders(void)const override	{assert(_senders.size()!=0);return _senders;
	}

};


namespace NMessageFunction
{
	typedef std::shared_ptr<const TAbstractMessage> TAbstractMessagePtr;

	NOT_USED static TAbstractMessagePtr normalMessage(Command command)
	{
       // std::cout<<command<<"||"<<std::endl;
		TAbstractMessagePtr resultNormalMessage(new TMessage<NotType>(command,TypeMessage::Normal,std::list< std::shared_ptr<IActor> >()));
		return resultNormalMessage;
	}

	template<typename TData>
	NOT_USED static TAbstractMessagePtr normalMessageWithDate(Command command, TData data)
	{TAbstractMessagePtr  resultNormalMessageWithDate(new TMessage<TData >(command,TypeMessage::Normal,data,std::list< std::shared_ptr<IActor> >() )); return resultNormalMessageWithDate;}

	NOT_USED static TAbstractMessagePtr normalMessageTheSender(Command command,std::shared_ptr<IActor> sender)
	{
		std::list< std::shared_ptr<IActor> > listSender; listSender.push_back(sender);
		TAbstractMessagePtr resultNormalMessage(new TMessage<NotType>(command,TypeMessage::Normal,listSender));
		return resultNormalMessage;
	}

	template<typename TData>
	NOT_USED static TAbstractMessagePtr normalMessageWithDateTheSender(Command command, TData data,std::shared_ptr<IActor> sender)
	{
		std::list< std::shared_ptr<IActor> > listSender;
		listSender.push_back(sender);
		TAbstractMessagePtr  resultNormalMessageWithDate(new TMessage<TData >(command,TypeMessage::Normal,data,listSender));
		return resultNormalMessageWithDate;
	}

	NOT_USED static TAbstractMessagePtr normalMessageTheSender(Command command,std::list< std::shared_ptr<IActor> > senders)
	{
		TAbstractMessagePtr resultNormalMessage(new TMessage<NotType>(command,TypeMessage::Normal,senders));
		return resultNormalMessage;
	}

	template<typename TData>
	NOT_USED static TAbstractMessagePtr normalMessageWithDateTheSender(Command command, TData data,std::list< std::shared_ptr<IActor> > senders)
	{
		TAbstractMessagePtr  resultNormalMessageWithDate(new TMessage<TData >(command,TypeMessage::Normal,data,senders));
		return resultNormalMessageWithDate;
	}

}


#endif // MESSAGES_H
