
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
#include "HttpClient.h"
#include "EnterName.h"

#include "Bomb.h"
#include "Enemy_small.h"
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
		UpdateFrame(pMemDC, 0);
		Welcome();
		break;
	}
	case 1:
	{
		//刷新图像
		UpdateFrame(pMemDC, 1);
		RunGame();
		break;
	}
	case 2:
	{
		listObj[enBuff].AddTail(new CBuff(2));
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

	//初始化游戏
	if (myPlane == NULL)
	{
		KillTimer(1);
		KillTimer(2);
		KillTimer(3);
		myPlane = new CMe;
	}
	for (int i = 0;i < 6; i++)
	{
		if(listObj[i].GetCount() != 0)
			listObj[i].RemoveAll();
	}
	heart_x = 0;
	for (int i = 0; i < 5; i++)
	{
		listObj[enBlood].AddTail(new CBlood(heart_x));
		heart_x += 30;
	}
	score = 0;

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
				listObj[enBomb].AddTail(new CBomb(pt.x + 10, pt.y + 25, 0, 0));
				listObj[enBomb].AddTail(new CBomb(pt.x + 50, pt.y + 25, 0, 0));
			}
			if (myPlane->GetBullet() == 1)
			{
				listObj[enBomb].AddTail(new CBomb(pt.x, pt.y + 15, -2, 1));
				listObj[enBomb].AddTail(new CBomb(pt.x + 10, pt.y + 10, -1, 1));
				listObj[enBomb].AddTail(new CBomb(pt.x + 20, pt.y + 10, 0, 1));
				listObj[enBomb].AddTail(new CBomb(pt.x + 30, pt.y + 10, 1, 1));
				listObj[enBomb].AddTail(new CBomb(pt.x + 40, pt.y + 15, 2, 1));
			}
			if (myPlane->GetBullet() == 2)
			{
				listObj[enBomb].AddTail(new CBomb(pt.x + 20, pt.y + 10, -3, 2));
				listObj[enBomb].AddTail(new CBomb(pt.x + 40, pt.y + 10, 3, 2));
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
	if ((key & 0x8000) != 0)
	{
		KillTimer(1);
		KillTimer(2);
		KillTimer(3);
		if (AfxMessageBox(_T("是否继续游戏？"), MB_YESNO) == 6)
		{
			SetTimer(1, 45, NULL);
			SetTimer(2, 5000, NULL);
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
		listObj[enEnemy].AddTail(new CEnemy_small);
	}
	nCreator--;
	if (myPlane == NULL)
	{
		nCreator = 1;
	}

	//boss 出现
	if (score == 20 && isnoplane == 0)
	{
		isnoplane = 1;
		listObj[enEnemy].AddTail(new CBoss(50));
	}
	if (isnoplane == 1)
		nCreator = 1;

	
	//敌人发射子弹
	POSITION xPos1 = NULL, xPos2 = NULL;
	for (xPos1 = listObj[enEnemy].GetHeadPosition(); (xPos2 = xPos1) != NULL;)
	{
		if (myPlane == NULL)break;
		CEnemy* pEnemy = (CEnemy*)listObj[enEnemy].GetNext(xPos1);
		pEnemy->AddBomb(listObj,myPlane);

	}


	//战机导弹炸掉敌人
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

				//删除导弹
				listObj[enBomb].RemoveAt(mPos2);


				//删除敌机
				if (pEnemy->Attacked(score))
				{
					listObj[enEnemy].RemoveAt(ePos2);
					delete pEnemy;
					delete pBomb;
					break;
				}

			}
		}

	}

	//敌机子弹打中战机
	POSITION pos1 = NULL, pos2 = NULL;
	for (pos1 = listObj[enBall].GetHeadPosition();(pos2 = pos1) != NULL;)
	{
		CBomb_enemy* pEnemy = (CBomb_enemy *)listObj[enBall].GetNext(pos1);
		CRect pRect = pEnemy->GetRect();
		CRect tmpRect;
		if (myPlane == NULL)break;
		if (tmpRect.IntersectRect(&pRect, myPlane->GetRect()) )
		{
			//减血
			if (!myPlane->IsNoEnemy() && listObj[enBlood].GetCount() > 0)
			{
				listObj[enBlood].RemoveTail();
			}
			heart_x -= 30;
			//删除子弹
			listObj[enBall].RemoveAt(pos2);

		}
	}

	//战机与敌机相撞
	pos1 = NULL, pos2 = NULL;
	for (pos1 = listObj[enEnemy].GetHeadPosition();(pos2 = pos1) != NULL;)
	{
		CEnemy_small* pEnemy = (CEnemy_small *)listObj[enEnemy].GetNext(pos1);
		CRect pRect = pEnemy->GetRect();
		CRect tmpRect;
		if (myPlane == NULL)break;
		if (tmpRect.IntersectRect(&pRect, myPlane->GetRect()) )
		{	
			//加分
			score++;
			//减血
			if (!myPlane->IsNoEnemy())
			{
				listObj[enBlood].RemoveTail();
			}
			
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


		//创建对话框变量
		CEnterName Dlg;
		if (Dlg.DoModal() != IDOK)
			return;
		CString name = Dlg.name;

		//将score转化为CString类型
		CString score_s;
		score_s.Format(_T("%d"), score);

		//上传数据
		CHttpClient http;
		CString url = _T("http://106.14.206.81/doAdd.php?name=") + name + _T("&score=") + score_s;
		CString res;
		http.HttpGet(url, res);
		MessageBox(res);

		//重启游戏
		welcome = new CWelcome;
		SetTimer(0, 100, NULL);


	}

	//吃到Buff
	pos1 = NULL, pos2 = NULL;
	for (pos1 = listObj[enBuff].GetHeadPosition();(pos2 = pos1) != NULL;)
	{
		CBuff* pBuff = (CBuff *)listObj[enBuff].GetNext(pos1);
		CRect pRect = pBuff->GetRect();
		CRect tmpRect;
		if (myPlane == NULL)break;
		if (tmpRect.IntersectRect(&pRect, myPlane->GetRect()) )
		{
			pBuff->GetBuff(listObj, myPlane);
			//删除Buff
			listObj[enBuff].RemoveAt(pos2);
			delete pBuff;
			break;
		}
	}
}


void CPlaneGameView::UpdateFrame(CDC* pMemDC, int scene)
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

	//绘图
	Draw(pDC, pMemDC, scene);

	//复制内存DC到设备DC
	pDC->BitBlt(0, 0, GAME_WIDTH, GAME_HEIGHT, pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(pBmpOld);
	//删除
	pBmpOld->DeleteObject();
	bmpMem->DeleteObject();
	pMemDC->DeleteDC();
	pDC->DeleteDC();
}
void CPlaneGameView::Draw(CDC* pDC, CDC* pMemDC, int scene)
{
	switch (scene)
	{
	case 0:
	{
		//菜单选择项
		if (welcome->Draw(pMemDC, FALSE) == 1)
		{
			SetTimer(1, 45, NULL);
			SetTimer(2, 5000, NULL);
			SetTimer(3, 5000, NULL);
			delete welcome;
			KillTimer(0);
		}
		else if (welcome->Draw(pMemDC, FALSE) == 2)
		{

			CHttpClient http;
			CString url = _T("http://106.14.206.81/getRank.php");
			CString str;
			http.HttpGet(url, str);
			if (str == "") str = _T("网络连接错误！");
			else str = _T("昵称\t分数\t时间\n") + str;
			KillTimer(0);
			if (AfxMessageBox(str))
			{
				welcome->No();
				SetTimer(0, 100, NULL);
			}
		}
		else if(welcome->Draw(pMemDC, FALSE) == 3)
		{
			exit(1);
		}
		break;

	}
	case 1:
	{
		//背景绘制
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
		for (int i = 0; i < 7; i++)
		{
			POSITION pos1, pos2;
			for (pos1 = listObj[i].GetHeadPosition(); (pos2 = pos1) != NULL;)
			{
				CGameObject* pObj = (CGameObject*)listObj[i].GetNext(pos1);

				if (!pObj->Draw(pMemDC, FALSE) && pObj != NULL)
				{
					listObj[i].RemoveAt(pos2);
					delete pObj;
				}
			}
		}

		//if (boss != NULL)
		//{
		//	boss->Draw(pMemDC, false);
		//	CString str;
		//	CPoint p = boss->GetPoint();
		//	str.Format(_T("HP:  %d / %d"), boss->GetLife(), 50);
		//	pMemDC->SetBkMode(TRANSPARENT);
		//	pMemDC->SetTextAlign(TA_TOP);
		//	pMemDC->SetTextColor(RGB(0, 0, 0));
		//	pMemDC->TextOut(p.x, p.y + 90, str);
		//}
		break;
	}
	}
}
int CPlaneGameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	PlaySound(_T("res\\game_music.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	myPlane = new CMe;
	//myPlane->WhosYourDaddy(TRUE);
	welcome = new CWelcome;
	//初始5颗心
	for (int i = 0; i < 5; i++)
	{
		listObj[enBlood].AddTail(new CBlood(heart_x));
		heart_x += 30;
	}
	
	SetTimer(0, 100, NULL);

	//加载图片
	CMe::LoadImage();
	CBomb::LoadImage();
	CEnemy_small::LoadImage();
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
