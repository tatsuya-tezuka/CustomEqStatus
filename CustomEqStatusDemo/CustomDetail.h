#pragma once

#include "CustomDialogBase.h"
#include "afxcmn.h"
#include "TreeListCtrl.h"

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

protected:
	bool		mRestore;
	CFont		mTreeFont;

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
	void	setTreeTitle(LPARAM lParam);

	CString createLeafText(CString item, CString unit, CString control)
	{
		CString str;
		str.Format(_T("%s\t\t%s\t%s"), item, unit, control.IsEmpty() == false ? mCOntrolSignString : _T(""));
		return str;
	}

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnProcSize(CWnd* pWnd, int dx, int dy);

	DECLARE_MESSAGE_MAP()
public:
	CTreeListCtrl mTreeCtrl;
	afx_msg void OnNMRClickTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnGetdispinfoTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
};
