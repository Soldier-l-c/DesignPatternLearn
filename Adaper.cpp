#pragma once
#include "stdafx.h"
#include "Adaper.h"

class Target
{
public:
	virtual void Run(const int& n) const
	{
		LOG << "RunStr int:[" << n << "]" << END;
	};
};

class Adaptee
{
public:
	//Adaptee中有Target 中Run方法没有办法实现的实现 	
	//适配器模式通过封装对象将复杂的转换过程隐藏于幕后。 被封装的对象甚至察觉不到适配器的存在。 
	void RunStr(const std::string& str) const
	{
		LOG << "RunStr str:[" << str << "]" << END;
	}
};

template<typename _Type>
class Adapter :public Target
{
	using TypePtr = std::shared_ptr<_Type>;
	TypePtr adpeePtr;

public:
	Adapter(const TypePtr& ptr) :adpeePtr(ptr) {};
	//适配的函数中存在将int类型转换为string的操作，以调用被适配的对象的方法
	virtual void Run(const int& n)const override
	{
		adpeePtr->RunStr(std::to_string(n));
	};
};

void Run(std::shared_ptr<Target> target)
{
	//调用方对适配器，以及适配器背后的数据转换没有感知
	target->Run(10);
}

void AdapterTest()
{
	std::shared_ptr<Target> dapter = std::make_shared<Adapter<Adaptee>>(std::make_shared<Adaptee>());
	auto target = std::make_shared<Target>();
	Run(dapter);
	Run(target);
}

void CAdaper::StartTest()
{
	LOG << "Run CAdaper test!" << END;

	AdapterTest();
}