#include "UnitSprite.h"

float CUnitSprite::getDist(float fPosX, float fPosY)
{
	int iDx = m_pSprite->getPositionX() - fPosX;
	int iDy = m_pSprite->getPositionY() - fPosY;
	// 得到两个物件之间的长度（也是距离）
	float dist = sqrt(float(iDx*iDx + iDy*iDy));
	return dist;
}

float CUnitSprite::getAngle(float fPosX, float fPosY)
{
	float fDx = fPosX - m_pSprite->getPositionX();
	float fDy = fPosY - m_pSprite->getPositionY();
	float fAngle;
	if(fDy == 0)
	{
		fAngle = 0;
	}
	else
	{
		fAngle = (float)CC_RADIANS_TO_DEGREES(atanf(fDx/fDy));
		
		if(fDy < 0)
		{
			if(fDx < 0)
				fAngle = 180 + fabs(fAngle);
			else
				fAngle = 180 - fabs(fAngle);
		}
	}
	return fAngle;
}