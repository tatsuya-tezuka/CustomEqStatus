#pragma once
#include <afxcmn.h>
#include "CustomDataManager.h"

class CCustomMonCntlListCtrl : public CListCtrl
{
public:
	CCustomMonCntlListCtrl();
	~CCustomMonCntlListCtrl();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
#if _DEMO_PHASE >= 100
	const COLORREF	mDragImageMaskColor = RGB(255, 0, 255);
#endif
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
#if _DEMO_PHASE >= 100
	/// �h���b�O���h���b�v�֘A
	stDragData				mDragData;
	CImageList* mpDragImage;
	COLORREF				mcDragBackColor;
	COLORREF				mcDragTextColor;
	bool					mDragEnabled;
	DRAGDROPCALLBACK		mDragDropCallback;
	CBaseDropSource			mDragSource;

	UINT					mListTarget;
#endif

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
#if _DEMO_PHASE >= 100
	BOOL	GroupByColumn(int nCol);
	void	SetListTarget(UINT val) { mListTarget = val; }
#endif
	/// �J�����̎����T�C�Y����
	void AutoSizeColumns(int col = -1)
	{
		SetRedraw(FALSE);
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int nColumnCount = pHeader->GetItemCount();

		for (int i = 0; i < nColumnCount; i++)
		{
			SetColumnWidth(i, LVSCW_AUTOSIZE);
			int nColumnWidth = GetColumnWidth(i);
			SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
			int nHeaderWidth = GetColumnWidth(i);
			SetColumnWidth(i, max(nColumnWidth, nHeaderWidth));
		}
		SetRedraw(TRUE);
	}
	/// ���X�g���ڂ̓o�^
	int AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam = 0, int nImage = -1)
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

#if _DEMO_PHASE >= 100
	/// �h���b�O���h���b�v�֘A
	void SetDragDropEnable(bool bEnable, DRAGDROPCALLBACK callback)
	{
		mDragEnabled = bEnable;
		mDragDropCallback = callback;
	}
#endif

protected:

	CImageList* CreateDragImageEx(LPPOINT lpPoint);
	void		DropItem(CPoint point, bool bCtrl);

	/* ------------------------------------------------------------------------------------ */

	DECLARE_MESSAGE_MAP()

#if _DEMO_PHASE >= 100
	afx_msg void OnLvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
#endif
};

