#pragma once
#include "afxdialogex.h"

typedef struct {
	UINT		id;
	UINT		type;
	UINT		color;
} stColorConfig;
static const stColorConfig mColorConfig[] = {
	{ IDC_BACKCOLOR, eTreeItemType_Window, eColorType_Window },
	{ IDC_TITLE_BACKCOLOR, eTreeItemType_Title, eColorType_TitleBack },
	{ IDC_TITLE_TEXTCOLOR, eTreeItemType_Title, eColorType_TitleText },
	{ IDC_MAIN_BACKCOLOR, eTreeItemType_Main, eColorType_MainBack },
	{ IDC_MAIN_TEXTCOLOR, eTreeItemType_Main, eColorType_MainText },
	{ IDC_SUB_BACKCOLOR, eTreeItemType_Sub, eColorType_SubBack },
	{ IDC_SUB_TEXTCOLOR, eTreeItemType_Sub, eColorType_SubText },
	{ IDC_LEAF_BACKCOLOR, eTreeItemType_Item, eColorType_ItemBack },
	{ IDC_LEAF_TEXTCOLOR, eTreeItemType_Item, eColorType_ItemText },
	{ IDC_LEAF_VALCOLOR, eTreeItemType_Item, eColorType_ValueText },
	{ IDC_LEAF_UNITCOLOR, eTreeItemType_Item, eColorType_UnitText },
};

// CCustomDetailConfig ダイアログ

class CCustomDetailConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomDetailConfig)

public:
	CCustomDetailConfig(CWnd* pParent = nullptr, UINT flag=0);   // 標準コンストラクター
	virtual ~CCustomDetailConfig();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DETAIL_CONFIG };
#endif
	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	COLORREF	mColor[eColorType_Max];
	LOGFONT		mFont[eTreeItemType_Item+1];

protected:
	UINT		mType;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	void	setFont(UINT type);

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedTitleFont();
	afx_msg void OnBnClickedMainFont();
	afx_msg void OnBnClickedSubFont();
	afx_msg void OnBnClickedLeafFont();
	afx_msg void OnBnClickedOk();
};
