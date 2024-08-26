/******************************************************************************
* @file    CustomManager.h
* @brief   【カスタマイズ機能】設備状況管理クラス
* @auther  SCC
* @date    2024/08/26 新規作成
* @par
******************************************************************************/
#pragma once

#include "CustomDialogBase.h"
#include "afxcmn.h"

// CCustomManager ダイアログ

class CCustomManager : public CCustomDialogBase
{
	DECLARE_DYNAMIC(CCustomManager)

public:
	CCustomManager(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CCustomManager();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_MANAGER };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum { eSelectUser = 1, eSelectMaster };

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int mSelectType;
	CListCtrl mManagerList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioUser();
	afx_msg void OnBnClickedRadioMaster();
	afx_msg void OnNMRClickListManager(NMHDR *pNMHDR, LRESULT *pResult);
};
