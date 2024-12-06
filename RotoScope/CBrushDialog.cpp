#include "pch.h"
#include "resource.h"
#include "RotoScope.h"
#include "afxdialogex.h"
#include "CBrushDialog.h"

// CBrushDialog dialog

IMPLEMENT_DYNAMIC(CBrushDialog, CDialogEx)

CBrushDialog::CBrushDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_PAINTDLG, pParent), m_brushSize(5), m_brushR(255), m_brushG(0), m_brushB(0)
{
}

CBrushDialog::~CBrushDialog()
{
}

void CBrushDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_PAINTSIZE, m_brushSize);   // Brush size control
    DDV_MinMaxInt(pDX, m_brushSize, 1, 100);     // Validate size range (1-100)

    DDX_Text(pDX, IDC_PaintR, m_brushR);         // Red color control
    DDV_MinMaxInt(pDX, m_brushR, 0, 255);        // Validate red range (0-255)

    DDX_Text(pDX, IDC_PaintG, m_brushG);         // Green color control
    DDV_MinMaxInt(pDX, m_brushG, 0, 255);        // Validate green range (0-255)

    DDX_Text(pDX, IDC_PaintB, m_brushB);         // Blue color control
    DDV_MinMaxInt(pDX, m_brushB, 0, 255);        // Validate blue range (0-255)
}

BEGIN_MESSAGE_MAP(CBrushDialog, CDialogEx)
    ON_EN_CHANGE(IDC_PAINTSIZE, &CBrushDialog::OnEnChangeBrushSettings)
    ON_EN_CHANGE(IDC_PaintR, &CBrushDialog::OnEnChangeBrushSettings)
    ON_EN_CHANGE(IDC_PaintG, &CBrushDialog::OnEnChangeBrushSettings)
    ON_EN_CHANGE(IDC_PaintB, &CBrushDialog::OnEnChangeBrushSettings)
END_MESSAGE_MAP()

// CBrushDialog message handlers

void CBrushDialog::OnEnChangeBrushSettings()
{
    // TODO: Add notification handling logic if necessary
}