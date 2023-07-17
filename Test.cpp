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

	//B�л����������ָ�룬һ���Ǵ�A�̳еģ�һ���Ǵ�B�̳еġ�
	LOG(INFO) << "TestSizeOf,sizeof(A):[" << sizeof(A) << "] sizeof(B):[" << sizeof(B) << "] ";

	C* c = new B();

	//�˴����'CI' ��ʱָ��������C*,������funʱ�����B�̳е�Cvptrָ����麯����Ѱ�Һ���fun�ĵ�ַ���Ӷ�ʵ�ֵ��á�
	//ͬ�����a��������A*���������'AI'
	c->FF();

	A* a = (A*)c;
	a->fun();

	A* aa = new B();
	aa->fun();

	B* b = new B();

}
