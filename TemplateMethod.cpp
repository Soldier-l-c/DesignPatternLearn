#pragma once
#include "stdafx.h"
#include "TemplateMethod.h"
/*
模板方法，采用晚绑定模式：使用已经完成的模块时，只需要重写其部分函数不需要知道其内部具体流程逻辑
流程已经在完成的模块中实现，重写需要变化部分流程(方法)。
比如使用第三方界面库QT时，不需要关注其内部的处理逻辑，只需要重写需要变化的方法（ChangeEvent, Print）即可，
*/
class CTBase
{
public:
	void Run()//不变方法，流程已经在完成的模块中实现，使用这个模块需要实现部分流程
	{
		__Step1();
		__Step2();
		__Step3();
		__Step4();
	}
	virtual ~CTBase() {};
protected:
	void __Step1() {};//不变方法
	virtual void __Step2() = 0;//需要变更的方法
	virtual void __Step3() = 0;//需要变更的方法
	void __Step4() {};//不变方法
};

class CTUse :public CTBase
{
protected:
	virtual void __Step2() {};//需要变更的方法
	virtual void __Step3() {};//需要变更的方法
};


void CTemplateMethod::StartTest()
{
	LOG(INFO) << "Run CTemplateMethod test!";
	auto ptr = std::make_shared<CTUse>();
	ptr->Run();
}
