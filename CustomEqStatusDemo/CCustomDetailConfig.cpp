// CCustomDetailConfig.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "afxdialogex.h"
#include "CCustomDetailConfig.h"


// CCustomDetailConfig ダイアログ

IMPLEMENT_DYNAMIC(CCustomDetailConfig, CDialogEx)

CCustomDetailConfig::CCustomDetailConfig(CWnd* pParent /*=nullptr*/, UINT flag/*=0*/)
	: CDialogEx(IDD_DIALOG_DETAIL_CONFIG, pParent)
{
	mType = flag;
}

CCustomDetailConfig::~CCustomDetailConfig()
{
}

void CCustomDetailConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCustomDetailConfig, CDialogEx)
	ON_BN_CLICKED(IDC_TITLE_FONT, &CCustomDetailConfig::OnClickedTitleFont)
	ON_BN_CLICKED(IDC_MAIN_FONT, &CCustomDetailConfig::OnBnClickedMainFont)
	ON_BN_CLICKED(IDC_SUB_FONT, &CCustomDetailConfig::OnBnClickedSubFont)
	ON_BN_CLICKED(IDC_LEAF_FONT, &CCustomDetailConfig::OnBnClickedLeafFont)
	ON_BN_CLICKED(IDOK, &CCustomDetailConfig::OnBnClickedOk)
END_MESSAGE_MAP()


// CCustomDetailConfig メッセージ ハンドラー

/*============================================================================*/
/*! 設備詳細画面設定クラス

-# 初期化

@param

@retval
*/
/*============================================================================*/
BOOL CCustomDetailConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i < sizeof(mColorConfig) / sizeof(mColorConfig[0]); i++) {
		((CMFCColorButton*)GetDlgItem(mColorConfig[i].id))->SetColor(mColor[mColorConfig[i].color]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! 設備詳細画面設定クラス

-# タイトルフォント設定

@param

@retval
*/
/*============================================================================*/
void CCustomDetailConfig::OnClickedTitleFont()
{
	setFont(eTreeItemType_Title);
}

/*============================================================================*/
/*! 設備詳細画面設定クラス

-# メインノードフォント設定

@param

@retval
*/
/*============================================================================*/
void CCustomDetailConfig::OnBnClickedMainFont()
{
	setFont(eTreeItemType_Main);
}

/*============================================================================*/
/*! 設備詳細画面設定クラス

-# サブノードフォント設定

@param

@retval
*/
/*============================================================================*/
void CCustomDetailConfig::OnBnClickedSubFont()
{
	setFont(eTreeItemType_Sub);
}

/*============================================================================*/
/*! 設備詳細画面設定クラス

-# リーフフォント設定

@param

@retval
*/
/*============================================================================*/
void CCustomDetailConfig::OnBnClickedLeafFont()
{
	setFont(eTreeItemType_Item);
}

/*============================================================================*/
/*! 設備詳細画面設定クラス

-# フォント設定

@param

@retval
*/
/*============================================================================*/
void CCustomDetailConfig::setFont(UINT type)
{
	CClientDC dc(NULL);
	LOGFONT lf = mFont[type];
	lf.lfHeight = -::MulDiv(-lf.lfHeight, dc.GetDeviceCaps(LOGPIXELSY), 72);
	CFontDialog dlg(&lf, CF_NOSCRIPTSEL);
	if (dlg.DoModal() == IDOK)
	{
		lf.lfHeight = -::MulDiv(-lf.lfHeight, 72, dc.GetDeviceCaps(LOGPIXELSY));
		mFont[type] = lf;
	}
}

/*============================================================================*/
/*! 設備詳細画面設定クラス

-# OKボタン押下

@param

@retval
*/
/*============================================================================*/
void CCustomDetailConfig::OnBnClickedOk()
{
	for (int i = 0; i < sizeof(mColorConfig) / sizeof(mColorConfig[0]); i++) {
		mColor[i] = ((CMFCColorButton*)GetDlgItem(mColorConfig[i].id))->GetColor();
	}

	CDialogEx::OnOK();
}
