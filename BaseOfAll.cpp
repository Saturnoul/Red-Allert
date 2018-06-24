#include "BaseOfAll.h"

 Basement::Basement()
 {
	SetVisible(false);
	BloodProgress->setType(kCCProgressTimerTypeBar);
	BloodProgress->setMidpoint(ccp(0, 0));
	BloodProgress->setBarChangeRate(ccp(1, 0));            //构造函数并设置血条不可见
	BloodProgress->setPercentage(100.0f);
}
void Basement::SetVisible(bool judge)
{
	BloodBackground->setVisible(judge);                  //设置血条是否可见
	BloodProgress->setVisible(judge);
}
bool Basement::minusBloodAmount(float attack)
{
	if (BloodProgress->getPercentage() - attack <= 0)
	{
		BloodProgress->setPercentage(0);              //受到攻击血量减少
		return false;
	}
	else
	{
		BloodProgress->setPercentage(BloodProgress->getPercentage() - attack);
		return true;
	}
}
