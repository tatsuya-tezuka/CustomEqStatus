/******************************************************************************
* @file    TreeListCtrl.h
* @brief   �y�J�X�^�}�C�Y�@�\�z�ݔ��󋵏ڍ׃c���[�N���X
* @auther  SCC
* @date    2024/08/26 �V�K�쐬
* @par
******************************************************************************/
#pragma once

#include "afxcmn.h"

//=============================================================================
// ��CCustomTreeEdit
//=============================================================================
//!< ���X�g���Ŏg�p����G�f�B�b�g�R���g���[��
class CCustomTreeEdit : public CEdit
{
	// Construction
public:
	CCustomTreeEdit(HTREEITEM iItem, int iSubItem, CString sInitText);
	virtual ~CCustomTreeEdit();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum eTreeEditType
	{
		eTreeEditType_Other,		// ���͕������`�F�b�N���Ȃ�
		eTreeEditType_Char,
		eTreeEditType_Float,
		eTreeEditType_Int,
	};

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	BOOL		mbKeyReturn;
	BOOL		mbKeyShift;
	UINT		mnNumberLimit;
protected:
	HTREEITEM	miItem;
	int			miSubItem;
	CString		msInitText;
	BOOL		mbESC;         // To indicate whether ESC key was pressed
	BOOL		mbNotify;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void		SetNumberLimit(UINT len);
	HTREEITEM	GetItem() { return miItem; }
	int			GetSubItem() { return miSubItem; }

protected:

	/* ------------------------------------------------------------------------------------ */

	//{{AFX_VIRTUAL(CTreeEdit)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CTreeEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#define abs(x)		((x) < 0 ? (-(x)) : (x))

//=============================================================================
// ��CCustomTreeListCtrl
//=============================================================================
class CCustomDetail;
class CCustomTreeListCtrl : public CTreeCtrl
{
public:
	CCustomTreeListCtrl();
	~CCustomTreeListCtrl();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum eTreeIDs { eHeaderID = 1, eTreeID = 2 };
	enum eStatus { eEnable = 1, eSelect = 2, eDrop = 3 };
	enum { eItem, eValue, eUnit, eControl, eMax };
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/// �c���[���
	CWnd*				mTreeParent;
	/// �w�b�_�[���
	CHeaderCtrl			mHeaderCtrl;
	int					mcyHeader;
	int					mcxTotal;
	int					mxPos;
	int					mxOffset;
	int					myOffset;
	vector<int>			mColWidths;
	int					mcxFirstCol;

	CImageList			mDragImageList;

	///���x���ҏW
	CEdit*				mpEdit;
	BOOL				mbInplace;

	/// �h���b�O���h���b�v
	bool				mbDragDragging;
	HTREEITEM			mhDragItemDrag;
	HTREEITEM			mhDragItemDrop;
	CImageList*			mpDragImagelist;
	//CCustomDropTarget	mCustomDropTarget;
	CTreeNode*			mDragNode;

	/// �m�[�h�p�t�H���g
	CFont				mDefaultFont;
	CFont				mNodeTitleFont;
	CFont				mNodeMainFont;
	CFont				mNodeSubFont;
	CFont				mNodeLeafFont;

	/// ����p�t�H���g
	CFont				mControlFont;

	/// �c�[���`�b�v
	CToolTipCtrl		mToolTip;

	/// �I���A�C�e�����X�g
	vector<HTREEITEM>	mSelectItems;
	HTREEITEM			mLastSelectItem;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CWnd*	GetParent() { return mTreeParent; }
	void	Create(CWnd* parent);
	void	ResizeControl(int dx, int dy);
	void	DividerDblClick(int item);

	int		GetPosX() { return mxPos; }
	void	SetPosX(int val) { mxPos = val; }
	int		GetTotalCX() { return mcxTotal; }

	/// �t�H���g
	CFont& GetFontEx(UINT type);
	void SetFontEx(UINT type, LOGFONT& lf);

	/// �w�b�_�[���
	int		GetHeaderHeight() { return mcyHeader; }
	void	SetHeaderItem(int item, CString title, int width);
	void	AdjustColumnWidth(int nColumn, BOOL bIgnoreCollapsed);
	int		GetHeaderWidth(int col = -1);

	/// ���x���ҏW
	CString	GetSubItemText(HTREEITEM hItem, int col);
	void	SetSubItemText(HTREEITEM hItem, int col, CString strText);
	bool	IsControl(CPoint point);
	BOOL	SwitchEditMode(HTREEITEM hItem, UINT col, CPoint point);

	/// �h���b�O���h���b�v

	void	UpdateColumns();
	void	UpdateScroller();
	void	RepositionControls();
	HTREEITEM	HitTestEx(CPoint &point, UINT& col);

	void	GetHeaderWidth(UINT *width, int max)
	{
		int nColsCnt = mHeaderCtrl.GetItemCount();
		for (int i = 0; i < nColsCnt; i++){
			if (i >= max)
				break;
			width[i] = mColWidths[i];
		}
	}

	void	SetHeaderWidth(UINT *width, int max)
	{
		HDITEM hditem;
		hditem.mask = HDI_WIDTH;
		int nColsCnt = mHeaderCtrl.GetItemCount();
		for (int i = 0; i < nColsCnt; i++){
			if (i >= max)
				break;
			hditem.cxy = width[i];
			mHeaderCtrl.SetItem(i, &hditem);
		}
	}

	vector<HTREEITEM>& GetSelectedItems() { return mSelectItems; }

	void	ExpandAll(HTREEITEM hItem = NULL);

protected:
	int			getMaxColumnWidth(HTREEITEM hItem, int nColumn, int nDepth, BOOL bIgnoreCollapsed);
	HTREEITEM	hitControl(CPoint point);

	/// ���x���ҏW
	CEdit*		editSubLabel(HTREEITEM hItem, int col);

	/// �h���b�O���h���b�v
	bool		enableDragItem(HTREEITEM hItem);
	CImageList*	createDragImageEx(HTREEITEM hItem);

	BOOL		ptInRectPointCell(CPoint point);
	bool		getColumnsRect(HTREEITEM hItem, UINT col, CRect& rect);

	void		selectMultiItem(HTREEITEM hClickedItem, UINT nFlags);

	inline void extractSubString(CString strText, vector<CString>& list)
	{
		list.clear();
		CString temp;
		int index = 0;
		while (AfxExtractSubString(temp, strText, index++, '\t')) {
			list.push_back(temp);
		}
	}

	inline COLORREF calcContrastColor(COLORREF color)
	{
		int a = (int)abs((int)color);

		if (abs((int)((color)& 0xFF) - 0x80) <= (UINT)0x40 &&
			abs((int)((color >> 8) & 0xFF) - 0x80) <= (UINT)0x40 &&
			abs((int)((color >> 16) & 0xFF) - 0x80) <= (UINT)0x40 )
			return (COLORREF)((0x7F7F7F + color) & 0xFFFFFF);
		else
			return (COLORREF)(color ^ 0xFFFFFF);
	}

	void	procControlKeyPress(HTREEITEM hCurItem);
	void	procShiftKeyPress(HTREEITEM hCurItem);
	bool	isSameLevel(HTREEITEM hItem);
	void	removeFromSelectList(HTREEITEM hItem);
	void	clearSelection();

	CString	createDragString(HTREEITEM hDragItem);

	BOOL	cellClick(HTREEITEM hItem, UINT nSubItem, CPoint point);

	/* ------------------------------------------------------------------------------------ */


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};

