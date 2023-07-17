#include "stdafx.h"
#include "Test.h"

CTest::~CTest()
{
}

void CTest::StartTest()
{
	TestSizeOf();
}

void CTest::TestSizeOf()
{
	class A
	{
	public:
		virtual void fun() { LOG(INFO) << "AI"; };
	private:
		std::string name_;
	};
	class C
	{
	public:
		virtual void FF() { LOG(INFO) << "CI"; };
	};
	class B : public A, public C
	{
	public:
		//virtual void fun() { LOG(INFO) << "BI"; };

	};

	//B中会有两个虚表指针，一个是从A继承的，一个是从B继承的。
	LOG(INFO) << "TestSizeOf,sizeof(A):[" << sizeof(A) << "] sizeof(B):[" << sizeof(B) << "] ";

	C* c = new B();

	//此处输出'CI' 此时指针类型是C*,当调用fun时，会从B继承的Cvptr指向的虚函数表寻找函数fun的地址，从而实现调用。
	//同理，如果a的类型是A*，输出就是'AI'
	c->FF();

	A* a = (A*)c;
	a->fun();

	A* aa = new B();
	aa->fun();

	B* b = new B();

}
