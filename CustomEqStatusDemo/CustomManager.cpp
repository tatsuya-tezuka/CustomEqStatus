/******************************************************************************
* @file    CustomManager.cpp
* @brief   【カスタマイズ機能】設備状況管理クラス
* @auther  SCC
* @date    2024/08/26 新規作成
* @par
******************************************************************************/
// CustomManager.cpp : 実装ファイル
//
#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomManager.h"
#include "afxdialogex.h"


// CCustomManager ダイアログ

IMPLEMENT_DYNAMIC(CCustomManager, CCustomDialogBase)

CCustomManager::CCustomManager(CWnd* pParent /*=NULL*/)
: CCustomDialogBase(CCustomManager::IDD, pParent)
, mSelectType(0)
{

}

CCustomManager::~CCustomManager()
{
}

void CCustomManager::DoDataExchange(CDataExchange* pDX)
{
	CCustomDialogBase::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_USER, mSelectType);
	DDX_Control(pDX, IDC_LIST_MANAGER, mManagerList);
}


BEGIN_MESSAGE_MAP(CCustomManager, CCustomDialogBase)
	ON_BN_CLICKED(IDC_RADIO_USER, &CCustomManager::OnBnClickedRadioUser)
	ON_BN_CLICKED(IDC_RADIO_MASTER, &CCustomManager::OnBnClickedRadioMaster)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MANAGER, &CCustomManager::OnNMRClickListManager)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MANAGER, &CCustomManager::OnNMDblclkListManager)
END_MESSAGE_MAP()


void CCustomManager::_CreateDemo(int nSelect)
{
	mManagerList.SetRedraw(FALSE);

	mManagerList.RemoveAllGroups();
	mManagerList.DeleteAllItems();

	int item = 0;
	if (nSelect == eSelectUser){
		mManagerList.AddItem(item, 0, _T("アンテナ"), 0);
		mManagerList.AddItem(item, 1, _T("Demo"));
		mManagerList.AddItem(item, 2, _T("グループなし"));
		item++;
		mManagerList.AddItem(item, 0, _T("S帯送信"), 0);
		mManagerList.AddItem(item, 1, _T("Demo"));
		mManagerList.AddItem(item, 2, _T("グループ１"));
		item++;
		mManagerList.AddItem(item, 0, _T("S帯測距"), 0);
		mManagerList.AddItem(item, 1, _T("Demo"));
		mManagerList.AddItem(item, 2, _T("グループ１"));
		item++;
		mManagerList.AddItem(item, 0, _T("X-TX"), 0);
		mManagerList.AddItem(item, 1, _T("Demo"));
		mManagerList.AddItem(item, 2, _T("グループなし"));
	}
	else{
		mManagerList.AddItem(item, 0, _T("#アンテナ"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("グループなし"));
		item++;
		mManagerList.AddItem(item, 0, _T("#S帯送信"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("グループ１"));
		item++;
		mManagerList.AddItem(item, 0, _T("#S帯測距"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("グループ１"));
		item++;
		mManagerList.AddItem(item, 0, _T("#X-TX"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("グループなし"));
	}

	mManagerList.SetRedraw(TRUE);

	mManagerList.GroupByColumn(eManagerGroup, (nSelect == eSelectUser)?TRUE:FALSE);
}

// CCustomManager メッセージ ハンドラー

/*============================================================================*/
/*! 設備詳細管理

-# 初期化

@param

@retval
*/
/*============================================================================*/
BOOL CCustomManager::OnInitDialog()
{
	CCustomDialogBase::OnInitDialog();

	mManagerList.CreateGroupControl();

#ifdef _DEMO
	_CreateDemo((int)eSelectUser);
#endif


	SetControlInfo(IDC_LIST_MANAGER, ANCHORE_LEFTTOP | RESIZE_BOTH);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
/*============================================================================*/
/*! 設備詳細管理

-# 「ユーザ」ボタン押下イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnBnClickedRadioUser()
{

#ifdef _DEMO
	_CreateDemo((int)eSelectUser);
#endif
}
/*============================================================================*/
/*! 設備詳細管理

-# 「マスタ」ボタン押下イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnBnClickedRadioMaster()
{

#ifdef _DEMO
	_CreateDemo((int)eSelectMaster);
#endif
}
/*============================================================================*/
/*! 設備詳細管理

-# リストコントロールでのマウス右クリックイベント（※メニュー表示）

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnNMRClickListManager(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
}
/*============================================================================*/
/*! 設備詳細管理

-# リストコントロールでのマウスダブルクリック（※対象ウィンドウのアクティブ化）

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnNMDblclkListManager(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
}
