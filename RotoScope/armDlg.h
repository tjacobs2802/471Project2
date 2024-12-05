#pragma once
#include "afxdialogex.h"


// armDlg dialog

class armDlg : public CDialogEx
{
	DECLARE_DYNAMIC(armDlg)

public:
	armDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~armDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_armDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_x;
	int m_y;
	double m_angle;
	afx_msg void OnEnChangemangle();
};
