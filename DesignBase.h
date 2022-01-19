#pragma once
class CDesignBase
{
public:
	virtual void Init() {};
	virtual void StartTest() {};

	virtual ~CDesignBase() {};
};
using DesignPtr = std::shared_ptr<CDesignBase>;