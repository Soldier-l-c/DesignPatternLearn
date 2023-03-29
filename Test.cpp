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
	};
	class C
	{
	public:
		virtual void fun() { LOG(INFO) << "CI"; };
	};
	class B : public A, public C
	{

	};

	//B中会有两个虚表指针，一个是从A继承的，一个是从B继承的。
	LOG(INFO) << "TestSizeOf,sizeof(A):[" << sizeof(A) << "] sizeof(B):[" << sizeof(B) << "] ";

	C* a = new B();

	//此处输出'CI' 此时指针类型是C*,当调用fun时，会从B继承的Cvptr指向的虚函数表寻找函数fun的地址，从而实现调用。
	//同理，如果a的类型是A*，输出就是'AI'
	a->fun();

	B* b = new B();
	//报错，不明确调用那个虚函数吧内的fun
	//b->fun();
}
