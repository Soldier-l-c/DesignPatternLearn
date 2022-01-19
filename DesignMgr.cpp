#include "stdafx.h"
#include "DesignMgr.h"

void CDesignMgr::Init()
{

}

void CDesignMgr::Test()
{
	std::for_each(m_desList.begin(), m_desList.end(), [](DesignPtr& ptr) {if (ptr)ptr->StartTest(); });
}
