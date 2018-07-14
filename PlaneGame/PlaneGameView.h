
// PlaneGameView.h: CPlaneGameView 类的接口
//
#include "windows.h"
#include "Mmsystem.h"
#pragma comment(lib,"winmm.lib")

#pragma once
#include "me.h"
#include "Welcome.h"
#include "Boss.h"
class CPlaneGameView : public CView
{	
	//枚举
	enum ObjType { enEnemy, enBomb, enBall, enExplosion, enBlood, enBuff, enboss };

protected: // 仅从序列化创建
	CPlaneGameView();
	DECLARE_DYNCREATE(CPlaneGameView)

// 特性
public:
	CPlaneGameDoc* GetDocument() const;
	CMe *myPlane;
	CBoss *boss;
	int score = 0;
	int isnoplane = 0;
// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CPlaneGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

protected:
	//内存DC
	CDC * pMemDC;
	//设备DC
	CClientDC* pDC;
	//内存位图
	CBitmap*    bmpMem;
	//当前窗口大小
	CRect* rect;
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void CPlaneGameView::UpdateFrame(CDC* pMemDC);
	void CPlaneGameView::RunGame();
	void CPlaneGameView::Welcome();
	
public:
	CObList listObj[7];
	int verDirection, horDirection;
	static int numOfEnemy;
	CWelcome* welcome;
	
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // PlaneGameView.cpp 中的调试版本
inline CPlaneGameDoc* CPlaneGameView::GetDocument() const
   { return reinterpret_cast<CPlaneGameDoc*>(m_pDocument); }
#endif

