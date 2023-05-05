#pragma once
class CTest :public CDesignBase
{
public:
	virtual ~CTest();
	CTest() = default;
	virtual void StartTest()override;

private:
	void TestSizeOf();
};