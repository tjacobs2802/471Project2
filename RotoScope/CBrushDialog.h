#pragma once
#include "afxdialogex.h"

// CBrushDialog dialog
class CBrushDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CBrushDialog)

public:
    CBrushDialog(CWnd* pParent = nullptr);
    virtual ~CBrushDialog();

    // Dialog Data
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_PAINTDLG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_MESSAGE_MAP()

public:
    int m_brushSize; // Brush size
    int m_brushR;    // Red component
    int m_brushG;    // Green component
    int m_brushB;    // Blue component

    afx_msg void OnEnChangeBrushSettings();
};