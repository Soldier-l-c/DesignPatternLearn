#pragma once
class CDesignMgr
{
public:
	using DesiginList = std::vector<IDesignPtr>;

	void Init();
	void Test();

private:
	DesiginList m_desList;
};