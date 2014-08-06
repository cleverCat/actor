#ifndef MESSAGES_H
#define MESSAGES_H
#include <memory>
#include <assert.h>
#include <list>
#include <type_traits>
#include <boost/any.hpp>

#ifdef __GNUC__
#define NOT_USED __attribute__ ((unused))
#else
#define NOT_USED
#endif

///комманды которые будет различать система акторов
enum Command
{
	//base actors message
UncnowCommand,
DontCreateActorOfThisID,
Satisfiability,
RemoveActor,
AddActor,
ActorDead,
AddInCounterOnActor,
RemoveInCounterOnActor,
SendException,
	//Profiler
Profiler,
ProfilerQueue,
GetProfilerInJson,
	//storage actors message
AddActorOnSorage,
RunStorageThread,
RemoveActorFromStorage,
ExecuteActor,
AddTask,
	//base
CreateTable,
GetTableOnParam,
PushDataFromTable,
AddMeasurement,
GetDataOnTable,
	//repeatOnResponseFromUrl
GetDataOnURLParams,
CreateActorOnRequest,
FindFromDisk,
SendResponse,
SetStorage,
	//settings
SetSettings,
GetSetting,
	//Log
AddToLog,
SetCurrentTypeMessageLog,
	//ManagerCNCManager
GetAddresCNCs,
AnswerManagerCNCs,
SetAddresCNCs,
GetStateCNC,
GetListParamCNC,
SetUnsetParamsOnCNC,
GetHotOscillData,
GetCommand,
AddLink,
	//Protocol
SendBytes,
SendCommand,
SendCommandOnCNC,
	//ListParam
SetParam,
SetState,
StartGetsParam,
GetParam,
GetStartGroupParam,
GetStopGroupParam,
StopGetsParam,
GetState,
UnsetParam,
SetActiveParams,
GetListParams,
	//ManagerData
SendData,
	//TRouter
AddLinks,
	//Port
InitComPort,
	//TGetListParams
SetListParams,
	//TCleverCash
SendSerialize,
GetSerialize,
PushData,
	//Test
Test,
	//Pool
SetFreeActorInPool,
GetFreeAtActor,
AddActorInPool,
RemoveActorInPool
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
