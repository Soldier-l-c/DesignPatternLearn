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

	//B�л����������ָ�룬һ���Ǵ�A�̳еģ�һ���Ǵ�B�̳еġ�
	LOG(INFO) << "TestSizeOf,sizeof(A):[" << sizeof(A) << "] sizeof(B):[" << sizeof(B) << "] ";

	C* a = new B();

	//�˴����'CI' ��ʱָ��������C*,������funʱ�����B�̳е�Cvptrָ����麯����Ѱ�Һ���fun�ĵ�ַ���Ӷ�ʵ�ֵ��á�
	//ͬ�����a��������A*���������'AI'
	a->fun();

	B* b = new B();
	//��������ȷ�����Ǹ��麯�����ڵ�fun
	//b->fun();
}
