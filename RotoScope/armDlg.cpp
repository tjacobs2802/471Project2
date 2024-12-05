// armDlg.cpp : implementation file
//

#include "pch.h"
#include "RotoScope.h"
#include "afxdialogex.h"
#include "armDlg.h"


// armDlg dialog

IMPLEMENT_DYNAMIC(armDlg, CDialogEx)

armDlg::armDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_armDlg, pParent)
	, m_x(0)
	, m_y(0)
	, m_angle(0)
{

}

armDlg::~armDlg()
{
}

void armDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_m_x, m_x);
	DDX_Text(pDX, IDC_m_y, m_y);
	DDX_Text(pDX, IDC_m_angle, m_angle);
}


BEGIN_MESSAGE_MAP(armDlg, CDialogEx)
	ON_EN_CHANGE(IDC_m_angle, &armDlg::OnEnChangemangle)
END_MESSAGE_MAP()


// armDlg message handlers


void armDlg::OnEnChangemangle()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
