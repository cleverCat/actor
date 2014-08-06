#include "/actor/TRouter.h"

class TTestRouter:public ITest
{
public:
    TTestRouter(){}
    virtual ~TTestRouter(){}
public:

	bool test(void)
	{
		ARouter router();
        (void)router;
		return true;
	}
};
