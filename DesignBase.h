class CDesignBase
{
public:
	virtual void Init() {};
	virtual void StartTest() {};

};
using DesignPtr = std::shared_ptr<CDesignBase>;