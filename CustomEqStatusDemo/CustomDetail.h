/******************************************************************************
* @file    CustomDetail.h
* @brief   �y�J�X�^�}�C�Y�@�\�z�ݔ��󋵏ڍ׉�ʃN���X
* @auther  SCC
* @date    2024/08/26 �V�K�쐬
* @par
******************************************************************************/
#pragma once

#include "CustomDialogBase.h"
#include "afxcmn.h"
#include "CustomTreeListCtrl.h"

// CCustomDetail �_�C�A���O

class CCustomDetail : public CCustomDialogBase
{
	DECLARE_DYNAMIC(CCustomDetail)

public:
	CCustomDetail(CWnd* pParent = NULL, bool bRestore = false);   // �W���R���X�g���N�^�[
	virtual ~CCustomDetail();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_EQDETAIL };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	bool		mRestore;
	LOGFONT		mTreeLogFont;
	LOGFONT		mTreeLeafLogFont;
	HTREEITEM	mMenuItem;
	//CTreeNode*	mBackupNode;

	/// �c�[���`�b�v
	CString		mToolText;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	UpdateSortNo(HTREEITEM hItem);

protected:
	void	createTreeControl();
	void	createRoot();
	void	createMainNode(HTREEITEM parentitem, CTreeNode* parentnode);
	void	createSubNode(HTREEITEM parentitem, CTreeNode* parentnode);
	void	createLeaf(HTREEITEM parentitem, CTreeNode* parentnode);

	void	restoreRoot();
	void	restoreNode(CTreeNode* pnode, HTREEITEM ptree);

	void	setNodeWindowInfo(CTreeNode* pnode, UINT type, TCHAR* text, CTreeNode* parent);

	CString createLeafText(CString item, CString unit, CString control, bool bMon)
	{
		CString str;
#if _DEMO_PHASE < 999
		int nRand = rand();
		double dRand = (double)rand() * 360.0 / RAND_MAX;
		if(bMon == true)
			str.Format(_T("%s\t%0.3f\t%s\t%s"), (LPCTSTR)item, dRand, (LPCTSTR)unit, control.IsEmpty() == false ? mCOntrolSignString : _T(""));
		else
			str.Format(_T("%s\t\t%s\t%s"), (LPCTSTR)item, (LPCTSTR)unit, control.IsEmpty() == false ? mCOntrolSignString : _T(""));
#else
		str.Format(_T("%s\t\t%s\t%s"), (LPCTSTR)item, (LPCTSTR)unit, control.IsEmpty() == false ? mCOntrolSignString : _T(""));
#endif
		return str;
	}
	void	resizeFit();
	void	setTreeTitle(LPARAM lParam);
	void	updateMode();

	void	updateMenu(CMenu* pMenu);
	bool	updateMenuItem(MENUITEMINFO* pMenuItemInfo);

	void	saveHeaderWidth();

	CString	generateTreeText(CTreeNode* pnode);

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
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
	afx_msg void OnMenudetailSave();
	afx_msg void OnMenudetailSaveas();
	afx_msg void OnTvnItemexpandingTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult);
};
