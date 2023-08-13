#include "stdafx.h"
#include "DesignMgr.h"

int main()
{
	auto desmgr = std::make_shared<CDesignMgr>();
	desmgr->Init();
	desmgr->Test();
	system("pause");
	std::map<int, int>().lower_bound(1);
}