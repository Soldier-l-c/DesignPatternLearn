#pragma once
/*
* 原型模式
*/
#include "DesignBase.h"
class CProtoType :public CDesignBase
{
public:
	virtual void Init()override;
	virtual void StartTest() override;

};