#include "actor/TStorageActors.h"
#include <memory>



TStorageActors::TStorageActors():isRun(false),_thread(),_isUpdateTimer(0),_lockTimer(),_timeAndTask(),cv()
{
	setFunctorOnMessage(Command::ActorDead, TFunctorOnMessage(NStrongActorsFunctionOnMessage::stopStorageThread));
	setFunctorOnMessage(Command::RunStorageThread, TFunctorOnMessage(NStrongActorsFunctionOnMessage::runStorageThread));
	setFunctorOnMessage(Command::ExecuteActor, TFunctorOnMessage(NStrongActorsFunctionOnMessage::executeActor));
	setFunctorOnMessage(Command::AddTask, TFunctorOnMessage(NStrongActorsFunctionOnMessage::addTask));

}

TStorageActors::~TStorageActors()
{
	if (isRun)
	{
		this->addMessage(NMessageFunction::normalMessage(Command::ActorDead));
		_thread.join();
	}
}

void TStorageActors::addMessage(TAbstractMessagePtr message)
{
	if (!isRun)// если тред для обработки сообщений и выполнения комманд не запущен
	{
		this->isRun=true; //выставим флажек выполнения
		this->addMessage(NMessageFunction::normalMessage(Command::RunStorageThread)); //посылаем сами себе комманду запуска
		this->execute(0); //выполним обработку комманды
	}

	if ((*message).getType()==TypeMessage::Extra)
		_queueMessage.push_front(message);
	else{  _queueMessage.push_back(message);}
	cv.notify_all();
}

void TStorageActors::execute(const uint TimeOut)//выбираем сообщение из очереди и вызываем соответствующий функтор
{

	(void)TimeOut;
	TAbstractMessagePtr message;
	std::string nameType(typeid(*this).name());
	try{
		try
		{
			message=_queueMessage.wait_and_pop();
		}
		catch(ClosedException){return; } //мы не дождались данных и очередь закрылась ничего страшного просто выходим ))
		if (message->getMessage()==Command::ActorDead){isRun=false; return;}
		if (_mapFunctorsOnMessage.end()==_mapFunctorsOnMessage.find(message->getMessage())){}//неизвестная комманда пришла в хранилище
		else{_mapFunctorsOnMessage.find(message->getMessage())->second(message , this->getThis());}
	}catch(std::exception& ex)
	{
		std::cout<<"error message"<<message->getMessage();
		std::cout<<"error from"+nameType;
	}
}

void TStorageActors::initThread()
{ _thread=std::thread(TStorageActors::run, std::ref(*this));}

void TStorageActors::executeTask()
{
	auto timer =std::chrono::system_clock::now()+std::chrono::milliseconds(1); //раз в секунду при отсутствии заданий таймер сам тикнет не думаю что это плохо
	while(isRun)
	{
		try
		{
			std::unique_lock<std::mutex> lk(_lockTimer);
			if(cv.wait_until(lk, timer, [this](){return _queueMessage.unstableSize()>0;})) //часты ложные срабатывания
			{
				///*flag*/ вытащить из очереди сообщений и исполнить сообщение самого таймера
				if (_queueMessage.unstableSize()>0)
				{
					execute();
				}
			}
			/*timeout*/
			for (auto& i:_timeAndTask)
			{
				 auto now = std::chrono::system_clock::now();
				if (i.second.first<=now)
				{
					i.second.second->sendMessage();
					i.second.first=now+std::chrono::milliseconds(i.second.second->getTimeout());
					if (!i.second.second->isRepeat()) _timeAndTask.erase(i.first);


				 }
				 if (timer>i.second.first) timer=i.second.first;
			 }
			 if (_timeAndTask.size()==0)		timer=std::chrono::system_clock::now()+std::chrono::milliseconds(1000); //раз в секунду при отсутствии заданий таймер сам тикнет не думаю что это плохо
		 }
		 catch (std::exception& e)
		 {
			 std::cout<<e.what();
			 assert(false);
		 }
		 catch(...){/*проблемы в хранилище акторов*/ qDebug()<<"problem from storage execute";}
	 }
}

void TStorageActors::run(TStorageActors &threadActor)
{
	threadActor.executeTask();
}

unsigned long TStorageActors::addTask(std::shared_ptr<TTask> task)
{
	auto timer =std::chrono::system_clock::now()+std::chrono::milliseconds(task->getTimeout());
	if (_timeAndTask.find( (unsigned long)(&(*task)))==_timeAndTask.end())
		_timeAndTask.insert(std::make_pair( (unsigned long)(&(*task)) ,std::make_pair(timer,task)));
	return (unsigned long)(&(*task));
}

void TStorageActors::killTask(unsigned long task)
{
	if (_timeAndTask.find(task)!=_timeAndTask.end())
		_timeAndTask.erase(task);
}

void TStorageActors::stop()
{
	isRun=false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  TStorageActors::test
///
///

class TTestTaskclass:public IActor
{
	TAbstractMessagePtr _message;
	unsigned int _countAdd;

private:TestPrimitive test(void){std::list<TestPrimitive> listTests; return TestPrimitive("not have test", listTests);}///убрал метод чтобы просто не мешался
public:
	TTestTaskclass():_message(nullptr),_countAdd(0) {}
	~TTestTaskclass(){}
	void addMessage(TAbstractMessagePtr message){_countAdd++; _message=message;}
	void execute(const unsigned int TimeOut=0){(void)TimeOut;}

	TAbstractMessagePtr getMessage(void)const {return _message;}
	unsigned int getCountAddMessage(){return _countAdd;}
};





TtestResultAndComment testCreate(void)
{

	try{
		for (int i=0 ;i<600;i++)
		{
			std::shared_ptr<TStorageActors> storage(new TStorageActors());
			storage->addMessage(NMessageFunction::normalMessage(Command::Test));
		}

	} catch(...){return  std::make_pair(false,"problem from create or destroy storage");}
   return std::make_pair(true,"");
}

TtestResultAndComment testAddTimer(void)
{

	try{
		const bool isRepeat=true;
		std::chrono::milliseconds timeout=std::chrono::milliseconds(100);
		IActorPtr actor(new TTestTaskclass());
		TAbstractMessagePtr message=NMessageFunction::normalMessage(Command::Test);
		std::shared_ptr<TTask> task(new TTask(message, actor, timeout, isRepeat));
		{
		std::shared_ptr<TStorageActors> timer(new TStorageActors());
		timer->addMessage(NMessageFunction::normalMessageWithDate(Command::AddTask,task));
		std::this_thread::sleep_for(std::chrono::milliseconds(231));
		}
		  if (std::static_pointer_cast<TTestTaskclass >(actor)->getMessage()!=message) return  std::make_pair(false,"problem from send message on actor");
		  if (std::static_pointer_cast<TTestTaskclass >(actor)->getCountAddMessage()!=2) return  std::make_pair(false,"problem from send message on actor not send try count send message");

	} catch(...){return  std::make_pair(false,"problem from add Task");}
   return std::make_pair(true,"");
}



TestPrimitive TStorageActors::test()
{
	std::list<TestPrimitive> listTests;
	listTests.push_back(TestPrimitive("test_Create",testCreate));
	listTests.push_back(TestPrimitive("test_AddTimer",testAddTimer));
	return TestPrimitive("testStorage", listTests);
}
