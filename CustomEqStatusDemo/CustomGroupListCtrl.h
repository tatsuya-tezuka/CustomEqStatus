/******************************************************************************
* @file    CustomGroupListCtrl.h
* @brief   �y�J�X�^�}�C�Y�@�\�z�ݔ��󋵊Ǘ���ʗp�O���[�v���X�g�R���g���[��
* @auther  SCC
* @date    2024/08/26 �V�K�쐬
* @par
******************************************************************************/
#pragma once
#include "afxcmn.h"

//=============================================================================
// ��CCustomGroupEdit
//=============================================================================
//!< ���X�g���Ŏg�p����G�f�B�b�g�R���g���[��
class CCustomGroupEdit : public CEdit
{
	// Construction
public:
	CCustomGroupEdit(int iItem, int iSubItem, CString sInitText);

	// Attributes
public:
	BOOL	m_bKeyReturn;
	BOOL    m_bKeyShift;
	UINT	m_nNumberLimit;

	// Operations
public:
	void		SetNumberLimit(UINT len);
	int			GetItem() { return m_iItem; }
	int			GetSubItem() { return m_iSubItem; }

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CCustomGroupEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int			m_iItem;
	int			m_iSubItem;
	CString		m_sInitText;
	BOOL		m_bESC;         // To indicate whether ESC key was pressed
	BOOL		m_bNotify;
};

class CCustomGroupHeaderCtrl : public CHeaderCtrl
{
public:
	CCustomGroupHeaderCtrl();
	virtual ~CCustomGroupHeaderCtrl();

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

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */

	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

class CCustomGroupListCtrl : public CListCtrl
{
public:
	CCustomGroupListCtrl();
	virtual ~CCustomGroupListCtrl();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	///  �O���[�v���X�g�̃h���b�O�f�[�^
	typedef struct {
		UINT		group;		// �O���[�v�ԍ�
		POINT		point;		// �}�E�X�ʒu���
		vector<int>	indexes;	// �h���b�O�A�C�e��
	} stGroupDragData;
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CWnd*	mParent;
	CFont	mListFont;
	CEdit	*mpEdit;

	/// �h���b�O���h���b�v�֘A
	/// �h���b�O���h���b�v�֘A
	stGroupDragData		mDragData;
	CImageList*			mpDragImage;
	COLORREF			mcDragBackColor;
	COLORREF			mcDragTextColor;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	CreateGroupControl(CWnd* parent);
	BOOL	GroupByColumn(int nCol, BOOL bEnableGroup=TRUE);

	void	SortItem();
	void	SortGroup();

	/// �J�����̎����T�C�Y����
	void AutoSizeColumns(int col = -1)
	{
		SetRedraw(FALSE);
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int mincol = col < 0 ? 0 : col;
		int maxcol = col < 0 ? pHeader->GetItemCount() : col;

		for (col = mincol; col <= maxcol; col++){
			SetColumnWidth(col, LVSCW_AUTOSIZE);
			int nColumnWidth = GetColumnWidth(col);
			SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
			int nHeaderWidth = GetColumnWidth(col);
			SetColumnWidth(col, max(nColumnWidth, nHeaderWidth));
		}
		SetRedraw(TRUE);
	}
	/// ���X�g���ڂ̓o�^
	int AddItem(int nItem, int nSubItem, CString strItem, LPARAM dwParam = 0, int nImage = -1)
	{
		LV_ITEM lv;
		memset(&lv, 0, sizeof(lv));
		lv.mask = LVIF_TEXT;
		lv.iItem = nItem;
		lv.iSubItem = nSubItem;
		if (nSubItem == 0) {
			lv.mask |= LVIF_PARAM;
			lv.lParam = dwParam;
		}
		if (nImage >= 0) {
			lv.mask |= LVIF_IMAGE;
			lv.iImage = nImage;
		}
		lv.pszText = (LPTSTR)(LPCTSTR)strItem;
		lv.cchTextMax = strItem.GetLength();

		if (nSubItem == 0)
			return InsertItem(&lv);
		return SetItem(&lv);
	}

	int		HitTestEx(CPoint &point, int *col);
	bool	EditExecute(int item, int colnum);

	int		GroupHitTest(const CPoint& point);
	int		GetRowGroupId(int nRow) { return getRowGroupId(nRow); }
	CString	GetGroupHeader(int nRow) { return getGroupHeader(nRow); }
	BOOL	SetGroupHeader(int nGroupID, const CString& header);
	UINT	GetGroupInner(UINT nGroup, UINT& min);
	int		GetHeaderGroupId(CString& strHeader) { return getHeaderGroupId(strHeader); }

protected:
	LRESULT	insertGroupHeader(int nIndex, int nGroupId, const CString& strHeader, DWORD dwState = LVGS_NORMAL, DWORD dwAlign = LVGA_HEADER_LEFT);
	BOOL	setGroupTask(int nGroupID, const CString& task);
	BOOL	setGroupSubtitle(int nGroupID, const CString& subtitle);
	BOOL	setGroupFooter(int nGroupID, const CString& footer, DWORD dwAlign = LVGA_FOOTER_LEFT);
	BOOL	setRowGroupId(int nRow, int nGroupId);

	int		getRowGroupId(int nRow);
	CString	getGroupHeader(int nGroupId);
	int		getHeaderGroupId(CString& strHeader);

	CEdit*	editSubLabel(int item, int colnum);

	CImageList* createDragImageEx(LPPOINT lpPoint);
	void	dropItem(CPoint point);

	/* ------------------------------------------------------------------------------------ */

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();
};

