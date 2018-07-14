
// PlaneGameView.cpp: CPlaneGameView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "PlaneGame.h"
#endif

#include "windows.h"
#include "Mmsystem.h"
#pragma comment(lib,"winmm.lib")

#include "PlaneGameDoc.h"
#include "PlaneGameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "resource.h"

#include "Bomb.h"
#include "Enemy.h"
#include "Enemy_middle.h"
#include "Background.h"
#include "Blood.h"
#include "Explosion.h"
#include "Buff.h"
#include "Bomb_enemy.h"
#include "Me.h"
#include "Explosion_me.h"
#include "Welcome.h"
#include "Boss.h"
int CPlaneGameView::numOfEnemy = 0; //敌机数量
int heart_x = 0; //心的x轴位置
// CPlaneGameView
//bool isstop = false;
IMPLEMENT_DYNCREATE(CPlaneGameView, CView)

BEGIN_MESSAGE_MAP(CPlaneGameView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CPlaneGameView 构造/析构
 
CPlaneGameView::CPlaneGameView()
{
	// TODO: 在此处添加构造代码

}

CPlaneGameView::~CPlaneGameView()
{
}

BOOL CPlaneGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPlaneGameView 绘图

void CPlaneGameView::OnDraw(CDC* pDC)
{
	CPlaneGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

}


// CPlaneGameView 打印

BOOL CPlaneGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPlaneGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPlaneGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CPlaneGameView 诊断

#ifdef _DEBUG
void CPlaneGameView::AssertValid() const
{
	CView::AssertValid();
}

void CPlaneGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlaneGameDoc* CPlaneGameView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlaneGameDoc)));
	return (CPlaneGameDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlaneGameView 消息处理程序


void CPlaneGameView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	switch (nIDEvent)
	{
	case 0:
	{
		Welcome();
		break;
	}
	case 1:
	{
		//刷新图像
		UpdateFrame(pMemDC);
		RunGame();
		break;
	}
	case 2:
	{
		listObj[enBuff].AddTail(new CBuff());
		break;
	}
	case 3:
	{
		listObj[enEnemy].AddTail(new CEnemy_middle(myPlane));
		break;
	}

	}


	CView::OnTimer(nIDEvent);
}
void CPlaneGameView::Welcome()
{


	//监控键盘消息

	short key = GetKeyState(VK_SPACE);
	if ((key & 0x8000) != 0)
	{
		welcome->OK();
	}
	key = GetKeyState(VK_UP);
	if ((key & 0x8000) != 0)
	{
		welcome->SelectMode(0);
	}
	key = GetKeyState(VK_DOWN);
	if ((key & 0x8000) != 0)
	{
		welcome->SelectMode(1);
	}

	//PlaySound(_T("res\\hello.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//建立设备DC
	pDC = new CClientDC(this);
	//CRect rcClient;
	//GetClientRect(&rcClient);

	//创建内存DC
	pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);

	//建立内存位图
	bmpMem = new CBitmap;
	bmpMem->CreateCompatibleBitmap(pDC, GAME_WIDTH, GAME_HEIGHT);

	//将位图选入内存DC
	CBitmap *pBmpOld = pMemDC->SelectObject(bmpMem);

	//菜单选择项
	if (welcome->Draw(pMemDC, FALSE) == 1)
	{
		SetTimer(1, 45, NULL);
		SetTimer(2, 50000, NULL);
		SetTimer(3, 5000, NULL);
		PlaySound(_T("res\\game_music.wav"), NULL, SND_ASYNC | SND_LOOP);
		delete welcome;
		KillTimer(0);;
	}
	else if (welcome->Draw(pMemDC, FALSE) == 3)
	{

		DestroyWindow();
	}

	//复制内存DC到设备DC
	pDC->BitBlt(0, 0, GAME_WIDTH, GAME_HEIGHT, pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(pBmpOld);
	//删除

	pBmpOld->DeleteObject();
	bmpMem->DeleteObject();
	pMemDC->DeleteDC();
	pDC->DeleteDC();
	
}
void CPlaneGameView::RunGame()
{
	//监控键盘消息
	short key = GetKeyState(VK_SPACE);
	if ((key & 0x8000) != 0 && myPlane != NULL)
	{
		if (myPlane->Fired())
		{
			CPoint pt = myPlane->GetPoint();
			if (myPlane->GetBullet() == 0)
			{
				listObj[enBomb].AddTail(new CBomb(pt.x + 10, pt.y + 25,0));
				listObj[enBomb].AddTail(new CBomb(pt.x + 50, pt.y + 25,0));
			}
			if (myPlane->GetBullet() == 1)
			{
				listObj[enBomb].AddTail(new CBomb(pt.x, pt.y + 15, -2));
				listObj[enBomb].AddTail(new CBomb(pt.x + 10, pt.y + 10, -1));
				listObj[enBomb].AddTail(new CBomb(pt.x + 20, pt.y + 10, 0));
				listObj[enBomb].AddTail(new CBomb(pt.x + 30, pt.y + 10, 1));
				listObj[enBomb].AddTail(new CBomb(pt.x + 40, pt.y + 15, 2));
			}
			if (myPlane->GetBullet() == 2)
			{
				listObj[enBomb].AddTail(new CBomb(pt.x + 20, pt.y + 10, -3));
				listObj[enBomb].AddTail(new CBomb(pt.x + 40, pt.y + 10, 3));
			}

		}
	}
	key = GetKeyState(VK_LEFT);
	if ((key & 0x8000) != 0 && myPlane != NULL)
	{
		myPlane->SetHorMotion(-1);
	}
	key = GetKeyState(VK_RIGHT);
	if ((key & 0x8000) != 0 && myPlane != NULL)
	{
		myPlane->SetHorMotion(1);
	}
	key = GetKeyState(VK_UP);
	if ((key & 0x8000) != 0 && myPlane != NULL)
	{
		myPlane->SetVerMotion(-1);
	}
	key = GetKeyState(VK_DOWN); 
	if ((key & 0x8000) != 0 && myPlane != NULL)
	{
		myPlane->SetVerMotion(1);
	}

	//暂停
	key = GetKeyState(VK_ESCAPE);
	if ((key & 0x8000) != 0 )
	{
		KillTimer(1);
		KillTimer(2);
		KillTimer(3);
		if (AfxMessageBox(_T("是否继续游戏？"), MB_YESNO) == 6)
		{
			SetTimer(1, 45, NULL);
			SetTimer(2, 50000, NULL);
			SetTimer(3, 5000, NULL);
		}
		else
			exit(1);
	}
	//随机产生敌机
	static int nCreator = rand() % 5 + 2;
	if (nCreator <= 0)
	{
		nCreator = rand() % 5 + 2;
		listObj[enEnemy].AddTail(new CEnemy);
	}
	nCreator--;
	if (myPlane == NULL)
	{
		nCreator=1;
	}
	//boss 出现
	if (score == 20)
	{
		isnoplane = 1;
		boss = new CBoss;
		boss->SetLife();
	}
	if (isnoplane == 1)
		nCreator = 1;
	// 子弹击中boss
	POSITION mmPos1 = NULL, mmPos2 = NULL;
	for (mmPos1 = listObj[enBomb].GetHeadPosition(); (mmPos2 = mmPos1) != NULL;)
	{
		CBomb* pBomb = (CBomb*)listObj[enBomb].GetNext(mmPos1);
		CRect mRect = pBomb->GetRect();
		CRect tRect;
		if (boss == NULL)break;
		if (tRect.IntersectRect(&mRect, boss->GetRect()))
		{
			//减生命值
			boss->Change();
			//添加爆炸效果
			listObj[enExplosion].AddTail(new CExplosion(mRect.left + 50, mRect.top - 50));
			//删除导弹
			listObj[enBomb].RemoveAt(mmPos2);
			delete pBomb;
			//删除BOSS

			if (boss->GetLife() == 0)
			{
				isnoplane = 0;
				//加分
				score += 1000;
				//添加爆炸效果
				listObj[enExplosion].AddTail(new CExplosion(boss->GetRect().left, boss->GetRect().top));
				delete boss;
				boss = NULL;
				break;
			}
		}
	}
	//BOSS发射子弹
	if (boss!= NULL)
	{
		CPoint bossPt = boss->GetPoint();
		if (boss->Fired())
		{
			listObj[enBall].AddTail(new CBomb_enemy(bossPt.x - 100, bossPt.y + 140, 1));
			listObj[enBall].AddTail(new CBomb_enemy(bossPt.x - 50, bossPt.y + 120, 1));
			listObj[enBall].AddTail(new CBomb_enemy(bossPt.x + 0, bossPt.y + 100, 1));
			listObj[enBall].AddTail(new CBomb_enemy(bossPt.x + 50, bossPt.y + 120, 1));
			listObj[enBall].AddTail(new CBomb_enemy(bossPt.x + 100, bossPt.y + 140, 1));
		}
	}
	//敌机子弹
	POSITION xPos1 = NULL, xPos2 = NULL;
	for (xPos1 = listObj[enEnemy].GetHeadPosition(); (xPos2 = xPos1) != NULL;)
	{
		if (myPlane == NULL)break;
		CEnemy* penemy = (CEnemy*)listObj[enEnemy].GetNext(xPos1);
		CPoint pt = penemy->GetPoint();
		CPoint mpt = myPlane->GetPoint();
		if (!penemy->Fired())
			continue;
		BOOL by = FALSE;
		if (penemy->GetMontion() == 1 && pt.y < mpt.y)
			by = TRUE;
		if (by &&pt.x >= mpt.x - 50 && pt.x < mpt.x + 50)
		{
			listObj[enBall].AddTail(new CBomb_enemy(pt.x + 14, pt.y + 18, penemy->GetMontion()));
		}
	}


	//战机导弹炸掉敌机
	POSITION mPos1 = NULL, mPos2 = NULL;
	for (mPos1 = listObj[enBomb].GetHeadPosition();(mPos2 = mPos1) != NULL;)
	{
		CBomb* pBomb = (CBomb*)listObj[enBomb].GetNext(mPos1);
		CRect bRect = pBomb->GetRect();

		POSITION ePos1 = NULL, ePos2 = NULL;
		for (ePos1 = listObj[enEnemy].GetHeadPosition();(ePos2 = ePos1) != NULL;)
		{
			CEnemy* pEnemy = (CEnemy*)listObj[enEnemy].GetNext(ePos1);
			CRect mRect = pEnemy->GetRect();
			CRect tmpRect;
			if (tmpRect.IntersectRect(&bRect, mRect))
			{
				//添加爆炸效果
				listObj[enExplosion].AddTail(new CExplosion(mRect.left, mRect.top));
				score++;
				//删除导弹
				listObj[enBomb].RemoveAt(mPos2);
				delete pBomb;

				//删除敌机
				listObj[enEnemy].RemoveAt(ePos2);
				delete pEnemy;
				break;
			}
		}

	}
	//敌机子弹炸掉战机
	POSITION pos1 = NULL, pos2 = NULL;
	for (pos1 = listObj[enBall].GetHeadPosition();(pos2 = pos1) != NULL;)
	{
		CBomb_enemy* pEnemy = (CBomb_enemy *)listObj[enBall].GetNext(pos1);
		CRect pRect = pEnemy->GetRect();
		CRect tmpRect;
		if (myPlane == NULL)break;
		if (tmpRect.IntersectRect(&pRect, myPlane->GetRect()))
		{
			//减血
			listObj[enBlood].RemoveTail();
			heart_x -= 30;
			//删除子弹
			listObj[enBall].RemoveAt(pos2);

		}
	}

	//战机与敌机相撞
	pos1 = NULL, pos2 = NULL;
	for (pos1 = listObj[enEnemy].GetHeadPosition();(pos2 = pos1) != NULL;)
	{
		CEnemy* pEnemy = (CEnemy *)listObj[enEnemy].GetNext(pos1);
		CRect pRect = pEnemy->GetRect();
		CRect tmpRect;
		if (myPlane == NULL)break;
		if (tmpRect.IntersectRect(&pRect, myPlane->GetRect()))
		{
			score++;
			//减血
			listObj[enBlood].RemoveTail();
			heart_x -= 30;
			//删除敌机
			listObj[enEnemy].RemoveAt(pos2);
			//添加爆炸效果
			listObj[enExplosion].AddTail(new CExplosion(pRect.left, pRect.top));
			delete pEnemy;
			break;
		}
	}

	//没血停止游戏
	if (listObj[enBlood].GetCount() == 0)
	{
		if (myPlane == NULL)return;
		listObj[enExplosion].AddTail(new CExplosion_me(myPlane->GetRect().left, myPlane->GetRect().top));
		delete myPlane;
		myPlane = NULL;
		PlaySound(_T("res\\hello.wav"), NULL, SND_ASYNC | SND_LOOP);
	}

	//吃到心
	pos1 = NULL, pos2 = NULL;
	for (pos1 = listObj[enBuff].GetHeadPosition();(pos2 = pos1) != NULL;)
	{
		CBuff* pBuff = (CBuff *)listObj[enBuff].GetNext(pos1);
		CRect pRect = pBuff->GetRect();
		CRect tmpRect;
		if (myPlane == NULL)break;
		if (tmpRect.IntersectRect(&pRect, myPlane->GetRect()) && listObj[enBlood].GetCount() <= 4)
		{
			//加血
			listObj[enBlood].AddTail(new CBlood(heart_x));
			heart_x += 30;
			//删除Buff
			listObj[enBuff].RemoveAt(pos2);
			delete pBuff;
			break;
		}
	}
}

void CPlaneGameView::UpdateFrame(CDC* pMemDC)
{
	
	//建立设备DC
	pDC = new CClientDC(this);
	//CRect rcClient;
	//GetClientRect(&rcClient);

	//创建内存DC
	pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);

	//建立内存位图
	bmpMem = new CBitmap;
	bmpMem->CreateCompatibleBitmap(pDC, GAME_WIDTH, GAME_HEIGHT);

	//将位图选入内存DC
	CBitmap *pBmpOld = pMemDC->SelectObject(bmpMem);

	//背景绘制
	//pMemDC->FillSolidRect(0, 0, GAME_WIDTH, GAME_HEIGHT, RGB(84, 142, 239));

	CBackground backgroud;
	backgroud.Draw(pMemDC, true);

	//战机绘制
	if (myPlane != NULL)
		myPlane->Draw(pMemDC, false);

	//积分绘制
	CString s;
	s.Format(_T("score : %d"), score);
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->TextOutW(10, 40, s);

	//导弹、爆炸、敌机、子弹 绘制&删除
	for (int i = 0; i < 6; i++)
	{
			POSITION pos1, pos2;
			for (pos1 = listObj[i].GetHeadPosition(); (pos2 = pos1) != NULL;)
			{
				CGameObject* pObj = (CGameObject*)listObj[i].GetNext(pos1);
					if (!pObj->Draw(pMemDC, FALSE))
					{
						listObj[i].RemoveAt(pos2);
						delete pObj;
					}
			}
	}
	if (boss != NULL)
	{
		boss->Draw(pMemDC, false);
		CString str;
		CPoint p = boss->GetPoint();
		str.Format(_T("HP:  %d / %d"), boss->GetLife(), 50);
		pMemDC->SetBkMode(TRANSPARENT);
		pMemDC->SetTextAlign(TA_TOP);
		pMemDC->SetTextColor(RGB(0, 0, 0));
		pMemDC->TextOut(p.x, p.y + 90, str);
	}
	
	//复制内存DC到设备DC
	pDC->BitBlt(0, 0, GAME_WIDTH, GAME_HEIGHT, pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(pBmpOld);
	//删除
	pBmpOld->DeleteObject();
	bmpMem->DeleteObject();
	pMemDC->DeleteDC();
	pDC->DeleteDC();
}

int CPlaneGameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	PlaySound(_T("res\\hello.wav"), NULL,  SND_ASYNC | SND_LOOP);
	//PlaySound(_T("res\\game_music.wav"), NULL, SND_ASYNC | SND_LOOP);
	myPlane = new CMe;
	welcome = new CWelcome;
	boss = NULL;
	//初始5颗心
	for (int i = 0; i < 5; i++)
	{
		listObj[enBlood].AddTail(new CBlood(heart_x));
		heart_x += 30;
		//listObj[enBuff].AddTail(new CBuff);
	}
	
	SetTimer(0, 100, NULL);
	CMe::LoadImage();
	CBomb::LoadImage();
	CEnemy::LoadImage();
	CEnemy_middle::LoadImage();
	CBackground::LoadImage();
	CBlood::LoadImage();
	CExplosion::LoadImage();
	CBuff::LoadImage();
	CBomb_enemy::LoadImage();
	CExplosion_me::LoadImage();
	CWelcome::LoadImage();
	CBoss::LoadImage();
	return 0;
}


void CPlaneGameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
