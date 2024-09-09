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
#include "CustomGroupListCtrl.h"
#include "CustomSynchroWindow.h"

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
	enum { eSelectUser = 0, eSelectMaster };

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	CCustomSynchroWindow		mSyncWindow;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CCustomSynchroWindow& GetCustomSyncWindow() { return mSyncWindow; }

protected:
	void	createItem(UINT nSelect);
	void	createEquipment();
	void	updateGroup();
	void	createEqDetail(CTreeNode* node = NULL);
	void	updateMenuItemStatus(CMenu* pMenu);

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int mSelectType;
	CCustomGroupListCtrl mManagerList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioUser();
	afx_msg void OnBnClickedRadioMaster();
	afx_msg void OnNMRClickListManager(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListManager(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnManagerNew();
	afx_msg void OnManagerDelete();
	afx_msg void OnManagerMonitor();
	afx_msg void OnManagerEdit();
	afx_msg void OnManagerShow();
	afx_msg void OnManagerHide();
	afx_msg void OnManagerCreate();
	afx_msg void OnManagerCancel();
	afx_msg void OnManagerLoad();
	afx_msg void OnManagerSave();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
