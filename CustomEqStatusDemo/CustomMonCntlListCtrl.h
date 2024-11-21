#pragma once
#include <afxcmn.h>
#include "CustomDataManager.h"

class CCustomMonCntlListCtrl : public CListCtrl
{
public:
	CCustomMonCntlListCtrl();
	~CCustomMonCntlListCtrl();

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

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

#if _DEMO_PHASE >= 999
	BOOL	GroupByColumn(int nCol);
	void	SetListTarget(UINT val) { mListTarget = val; }
#endif
	/// カラムの自動サイズ調整
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
	/// リスト項目の登録
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

protected:

	CImageList* CreateDragImageEx(LPPOINT lpPoint);




	///
	/// ドラッグ＆ドロップ関連
	///
	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
	const COLORREF	mDragImageMaskColor = RGB(255, 0, 255);
	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/// ドラッグ＆ドロップ関連
	UINT					mDragFormat;
	UINT					mDropFormat;

	stDragData				mDragData;
	CImageList*				mpDragImage;
	COLORREF				mcDragBackColor;
	COLORREF				mcDragTextColor;
	bool					mDragEnabled;
	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	SetDragFormat(UINT format) { mDragFormat = format; }
	UINT	GetDragFormat() { return mDragFormat; }
	void	SetDropFormat(UINT format) { mDropFormat = format; }
	UINT	GetDropFormat() { return mDropFormat; }
protected:
#if _DEMO_PHASE >= 50
	void	ClearDropTarget();
	void	SetDropTarget(CPoint point);
	BOOL	DataObjectToList(CCustomDropObject* pDataObject);
	BOOL	PrepareItemBuff(POINT point, CStringArray& list);

	static DROPEFFECT CALLBACK Callback_MonCntl_DragOver(CWnd* pWnd, void* pDataObject, UINT dwKeyState, CPoint point);
	static BOOL CALLBACK Callback_MonCntl_DragDrop(CWnd* pWnd, void* pDataObject, UINT dwKeyState, CPoint point);
	static void CALLBACK Callback_MonCntl_DragLeave(CWnd* pWnd);
#endif

	/* ------------------------------------------------------------------------------------ */

	DECLARE_MESSAGE_MAP()

#if _DEMO_PHASE >= 50
	afx_msg void OnLvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
#endif
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

