#include "stdafx.h"
#include "DesignBase.h"
#include "Observer.h"

//观察者基类
class IObserver
{
public:
	virtual void OnNotify() = 0; 
};
using IObserverPtr = std::shared_ptr<IObserver>;

//主要执行任务的基类
class DoWorkBase
{
public:
	virtual void AddObserver(const IObserverPtr& ptr) = 0;
	virtual void RemoveObserver(const IObserverPtr& ptr) = 0;
	
	virtual void StartDoWork() = 0;

	virtual ~DoWorkBase() {};

protected:
	virtual void NotifyObservers() = 0;
};
using IDoWorkPtr = std::shared_ptr<DoWorkBase>;

//主要执行任务的类，已经被实现了的类
class DoWork : public DoWorkBase
{
public:
	virtual void AddObserver(const IObserverPtr& ptr) override
	{
		const auto iter = std::find_if(m_observerList.begin(), m_observerList.end(), [ptr](const IObserverPtr& p) {return ptr == p; });
		if (iter != m_observerList.end())return;

		m_observerList.emplace_back(ptr);
	};

	virtual void RemoveObserver(const IObserverPtr& ptr)override
	{
		const auto iter = std::find_if(m_observerList.begin(), m_observerList.end(), [ptr](const IObserverPtr& p) {return ptr == p; });
		if (iter != m_observerList.end())
		{
			m_observerList.erase(iter);
		}
	};

	virtual void StartDoWork()override
	{
		//do 
		//...
		NotifyObservers();
		
	};

protected:
	virtual void NotifyObservers()override
	{
		std::thread([this] {
			std::for_each(m_observerList.begin(), m_observerList.end(), [](const IObserverPtr& p) {if (p)p->OnNotify(); });
			}).detach();
	};

protected:
	std::vector<IObserverPtr> m_observerList;
};

//用户实现的类，需要接收主要执行任务对象的通知。通过重写OnNotify()实现接收通知
class CObserverNotify : public IObserver , public std::enable_shared_from_this<CObserverNotify>
{
public:

	virtual void OnNotify() override
	{
		LOG(INFO) << "OnNotify has been triggered!";
		//do
	};

	void DoSomething()
	{
		//保存一下，防止被提前析构
		m_pwork = std::make_shared<DoWork>();
		m_pwork->AddObserver(shared_from_this());
		m_pwork->StartDoWork();
	};

private:
	IDoWorkPtr m_pwork{ nullptr };
};

void CObserver::StartTest()
{
	LOG(INFO) << "StartTest CObserver！";
	auto ptr = std::make_shared<CObserverNotify>();
	ptr->DoSomething();

	std::this_thread::sleep_for(MILL_SECOND(100));
}
