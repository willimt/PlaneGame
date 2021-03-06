#pragma once


// CEnterName 对话框

class CEnterName : public CDialogEx
{
	DECLARE_DYNAMIC(CEnterName)

public:
	CEnterName(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEnterName();
	CString name;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENTERNAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
