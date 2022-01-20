#include "stdafx.h"
#include "DesignBase.h"
#include "Observer.h"

//观察者基类
class IObserver
{
public:
	virtual void OnNotify() = 0; 
};
using ObserverPtr = std::shared_ptr<IObserver>;

//主要执行任务的基类
class DoWorkBase
{
public:
	virtual void AddObserver(const ObserverPtr& ptr) = 0;
	virtual void RemoveObserver(const ObserverPtr& ptr) = 0;
	virtual void NotifyObservers() = 0;
	virtual void StartDoWork() = 0;

	virtual ~DoWorkBase() {};
protected:
	std::vector<ObserverPtr> m_observerList;
};

//主要执行任务的类，已经被实现了的类
class DoWork : public DoWorkBase
{
public:
	virtual void AddObserver(const ObserverPtr& ptr) override
	{
		m_observerList.emplace_back(ptr);
	};

	virtual void RemoveObserver(const ObserverPtr& ptr)override
	{
		const auto iter = std::find_if(m_observerList.begin(), m_observerList.end(), [ptr](const ObserverPtr& p) {return ptr == p; });
		if (iter != m_observerList.end())
		{
			m_observerList.erase(iter);
		}
	};

	virtual void NotifyObservers()override
	{
		std::for_each(m_observerList.begin(), m_observerList.end(), [](const ObserverPtr& p) {if(p)p->OnNotify(); });
	};

	virtual void StartDoWork()override
	{
		NotifyObservers();
		//do 
	};
};

//用户实现的类，需要接收主要执行任务对象的通知。通过重写OnNotify()实现接收通知
class CObserverNotify : public IObserver , public std::enable_shared_from_this<CObserverNotify>
{
public:

	virtual void OnNotify() override
	{
		LOG << "OnNotify has been triggered!" << END;
		//do
	};

	void DoSomething()
	{
		std::shared_ptr<DoWorkBase> ptr = std::make_shared<DoWork>();
		ptr->AddObserver(shared_from_this());
		ptr->StartDoWork();
	};
};


void CObserver::StartTest()
{
	LOG << "StartTest CObserver！" << END;
	auto ptr = std::make_shared<CObserverNotify>();
	ptr->DoSomething();
}
