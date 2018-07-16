#include "StdAfx.h"
#include "Bomb.h"
#include "resource.h"

CImageList CBomb::m_Images;

CBomb::~CBomb(void)
{
}

BOOL CBomb::Draw(CDC* pDC, BOOL bPause)
{
	if(change==0)
	m_ptPos.y = m_ptPos.y - 12;
	if (change == 1)//��ƫ
	{
		m_ptPos.x = m_ptPos.x + 6;
		m_ptPos.y = m_ptPos.y - 12;
	}
	if (change == 2)
	{
		m_ptPos.x = m_ptPos.x + 9;
		m_ptPos.y = m_ptPos.y - 9;
	}
	if (change == -1)//��ƫ
	{
		m_ptPos.x = m_ptPos.x - 6;
		m_ptPos.y = m_ptPos.y - 12;
	}
	if (change == -2)
	{
		m_ptPos.x = m_ptPos.x - 9;
		m_ptPos.y = m_ptPos.y - 9;
	}
	if (change == 3)
	{
		if (m_ptPos.x - px < 50)
		{
			m_ptPos.x = m_ptPos.x + 4;
			m_ptPos.y = m_ptPos.y - 2;
		}
		else
		{
			m_ptPos.y = m_ptPos.y - 12;
		}
	}
	if (change == -3)
	{
		if (m_ptPos.x - px > -50)
		{
			m_ptPos.x = m_ptPos.x - 4;
			m_ptPos.y = m_ptPos.y - 2;
		}
		else
		{
			m_ptPos.y = m_ptPos.y - 12;
		}
	}
	return m_Images.Draw(pDC, m_BulletType, m_ptPos, ILD_TRANSPARENT);
}

BOOL CBomb::LoadImage()
{
	return CGameObject::LoadImageW(m_Images, IDB_Bomb, RGB(255, 255, 255), 4, 28, 1);
}
