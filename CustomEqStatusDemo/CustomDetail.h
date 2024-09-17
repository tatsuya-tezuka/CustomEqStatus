/******************************************************************************
* @file    CustomDetail.h
* @brief   【カスタマイズ機能】設備状況詳細画面クラス
* @auther  SCC
* @date    2024/08/26 新規作成
* @par
******************************************************************************/
#pragma once

#include "CustomDialogBase.h"
#include "afxcmn.h"
#include "CustomTreeListCtrl.h"

// CCustomDetail ダイアログ

class CCustomDetail : public CCustomDialogBase
{
	DECLARE_DYNAMIC(CCustomDetail)

public:
	CCustomDetail(CWnd* pParent = NULL, bool bRestore = false);   // 標準コンストラクター
	virtual ~CCustomDetail();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_EQDETAIL };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	UINT		mMode;

protected:
	bool		mRestore;
	LOGFONT		mTreeLogFont;
	HTREEITEM	mMenuItem;
	CTreeNode*	mBackupNode;

	/// ツールチップ
	CString		mToolText;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	void	createTreeControl();
	static BOOL CALLBACK messageClick(CWnd* pwnd, HTREEITEM hItem, UINT nSubItem, CPoint point);
	static BOOL CALLBACK messageDrag(CWnd* pwnd, UINT status, HTREEITEM hItem, LPARAM lParam1, LPARAM lParam2, LPARAM lParam3);
	void	createRoot();
	void	createMainNode(HTREEITEM parentitem, CTreeNode* parentnode);
	void	createSubNode(HTREEITEM parentitem, CTreeNode* parentnode);
	void	createLeaf(HTREEITEM parentitem, CTreeNode* parentnode);

	void	restoreRoot();
	void	restoreNode(CTreeNode* pnode, HTREEITEM ptree);

	void	setNodeWindowInfo(CTreeNode* pnode, UINT type, TCHAR* text, CTreeNode* parent);

	CString createLeafText(CString item, CString unit, CString control)
	{
		CString str;
		str.Format(_T("%s\t\t%s\t%s"), item, unit, control.IsEmpty() == false ? mCOntrolSignString : _T(""));
		return str;
	}
	void	resizeFit();
	void	setTreeTitle(LPARAM lParam);
	void	updateMode();

	void	updateMenu(CMenu* pMenu);
	bool	updateMenuItem(MENUITEMINFO* pMenuItemInfo);

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnProcSize(CWnd* pWnd, int dx, int dy);

	DECLARE_MESSAGE_MAP()
public:
	CCustomTreeListCtrl mTreeCtrl;
	afx_msg void OnNMRClickTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	afx_msg void OnHeaderItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHeaderDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDetailAdd();
	afx_msg void OnDetailDelete();
	afx_msg void OnDetailRename();
	afx_msg void OnDetailMonctrl();
	afx_msg void OnDetailConfig();
	afx_msg void OnTvnGetInfoTipTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnMenudetailClose();
	afx_msg void OnMenudetailEdit();
	afx_msg void OnMenudetailMonitor();
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
};
