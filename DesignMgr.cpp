#include "stdafx.h"
#include "DesignMgr.h"

#include "ProTotype.h"
#include "Adaper.h"
#include "TemplateMethod.h"
#include "Observer.h"

#define MAKE_PTR(_class)std::make_shared<_class>()

void CDesignMgr::Init()
{
	m_desList = 
	{
		MAKE_PTR(CProtoType),
		MAKE_PTR(CAdaper),
		MAKE_PTR(CTemplateMethod),
		MAKE_PTR(CObserver)
	};
}

void CDesignMgr::Test()
{
	std::for_each(m_desList.begin(), m_desList.end(), [](DesignPtr& ptr) {if (ptr)ptr->StartTest(); });
}
