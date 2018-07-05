
// PlaneGameView.cpp: CPlaneGameView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "PlaneGame.h"
#endif

#include "PlaneGameDoc.h"
#include "PlaneGameView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPlaneGameView

IMPLEMENT_DYNCREATE(CPlaneGameView, CView)

BEGIN_MESSAGE_MAP(CPlaneGameView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_CREATE()
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
	CBitmap bitmap;
	bitmap.LoadBitmapW(IDB_enemy);

	CImageList imgList;
	imgList.Create(35, 35, ILC_COLOR8 | ILC_MASK, 2, 4);
	imgList.Add(&bitmap, RGB(0, 0, 0));
	imgList.Draw(pDC, 0,CPoint(50, 50), ILD_TRANSPARENT);
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
	short key = GetKeyState(VK_LEFT);
	if ((key & 0x8000) != 0)
	{
		horDirection = -1;
	}
	key = GetKeyState(VK_RIGHT);
	if ((key & 0x8000) != 0)
	{
		horDirection = 1;
	}
	key = GetKeyState(VK_UP);
	if ((key & 0x8000) != 0)
	{
		verDirection = -1;
	}
	key = GetKeyState(VK_DOWN);
	if ((key & 0x8000) != 0)
	{
		verDirection = 1;
	}
	myPt.x = myPt.x + 20 * horDirection;
	myPt.y = myPt.y + 20 * verDirection;
	horDirection = 0;
	verDirection = 0;

	CDC *pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	CBrush brush(RGB(146, 146, 146));
	pDC->SelectObject(&brush);
	pDC->Rectangle(&rect);
	CBitmap bitmapMyPlane;
	bitmapMyPlane.LoadBitmap(316);
	CImageList imglistMyPlane;
	imglistMyPlane.Create(50, 50, ILC_COLOR8 | ILC_MASK, 2, 4);
	imglistMyPlane.Add(&bitmapMyPlane, RGB(0, 0, 0));
	imglistMyPlane.Draw(pDC, 0, myPt, ILD_TRANSPARENT);
	CView::OnTimer(nIDEvent);
}


int CPlaneGameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	horDirection = 0, verDirection = 0;
	SetTimer(0, 100, 0);
	//SetTimer(2, 500, 0);
	//SetTimer(3, 1000, 0);
	myPt.x = 0;
	myPt.y = 0;
	return 0;
}
