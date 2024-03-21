#include "stdafx.h"
#include "DesignMgr.h"

#include "ProTotype.h"
#include "Adaper.h"
#include "TemplateMethod.h"
#include "Observer.h"
#include "Test.h"
#include "LeetCodeTest.h"
#include "TreeTest.h"
#include "DijkstraTest.h"
#include "SortTest.h"

#define MAKE_PTR(_class)std::make_shared<_class>()

void CDesignMgr::Init()
{
	m_desList =
	{
		MAKE_PTR(CProtoType),
		MAKE_PTR(CAdaper),
		MAKE_PTR(CTemplateMethod),
		MAKE_PTR(CObserver),
		MAKE_PTR(CTest),
		MAKE_PTR(LeetCodeTest),
		MAKE_PTR(CTreeTest),
		MAKE_PTR(DijkstraTest),
		MAKE_PTR(SortTest)
	};
}

void CDesignMgr::Test()
{
	std::for_each(m_desList.begin(), m_desList.end(), [](IDesignPtr& ptr) {if (ptr)ptr->StartTest(); });
}
