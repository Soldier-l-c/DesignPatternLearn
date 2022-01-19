#include "stdafx.h"
#include "DesignMgr.h"

int main()
{
	auto desmgr = std::make_shared<CDesignMgr>();
	desmgr->Init();
	desmgr->Test();
}