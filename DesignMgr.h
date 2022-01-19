#pragma once
#include "DesignBase.h"
class CDesignMgr
{
public:
	using DesiginList = std::vector<DesignPtr>;

	void Init();
	void Test();

private:
	DesiginList m_desList;
};