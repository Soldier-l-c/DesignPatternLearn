#pragma once
class CDesignBase
{
public:
	virtual void Init() {};
	virtual void StartTest() = 0;

	virtual ~CDesignBase() {};
};
using IDesignPtr = std::shared_ptr<CDesignBase>;