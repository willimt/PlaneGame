#include "stdafx.h"
#include "Me.h"
#include "Resource.h"

CMe::CMe(void)
{
	m_ptPos.x = 512;
	m_ptPos.y = 400;
	m_nHorMotion = 0;
	m_nVerMotion = 0;
}

CMe::~CMe(void)
{
}

CImageList CMe::m_Images;

BOOL CMe::Draw(CDC* pDC, BOOL bPause)
{
	m_ptPos.x = m_ptPos.x + m_nHorMotion * 5;

	return m_Images.Draw(pDC, 0, m_ptPos, ILD_TRANSPARENT);
}

BOOL CMe::LoadImage()
{
	return CGameObject::LoadImageW(m_Images, IDB_ME, RGB(0, 0, 0), 50, 60, 1);
}

BOOL CMe::Fired()
{
	return true;
}