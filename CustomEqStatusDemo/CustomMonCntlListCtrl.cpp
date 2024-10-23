#include "stdafx.h"
#include "CustomMonCntlListCtrl.h"

CCustomMonCntlListCtrl::CCustomMonCntlListCtrl()
{
#if _DEMO_PHASE >= 100
	mDragData.indexes.clear();
	mpDragImage = NULL;
	mcDragBackColor = GetSysColor(COLOR_WINDOW);
	mcDragTextColor = GetSysColor(COLOR_WINDOWTEXT);
	mDragDropCallback = NULL;
	mDragEnabled = false;

	mListTarget = eFromType_None;
#endif
}

CCustomMonCntlListCtrl::~CCustomMonCntlListCtrl()
{
}

BEGIN_MESSAGE_MAP(CCustomMonCntlListCtrl, CListCtrl)
#if _DEMO_PHASE >= 100
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CCustomMonCntlListCtrl::OnLvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
#endif
END_MESSAGE_MAP()


#if _DEMO_PHASE >= 100
/*============================================================================*/
/*! リストコントロール

-# ドラッグの開始

@param		pNMHDR	NMHDR 構造体
@param		pResult	処理結果

@retval
*/
/*============================================================================*/
void CCustomMonCntlListCtrl::OnLvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV == NULL) {
		*pResult = 0;
		return;
	}

	mDragData.indexes.clear();
	if (mDragDropCallback != NULL) {
		mDragDropCallback(eUserMessage_Drag_GetIndex, (LPARAM)mListTarget, (LPARAM)&mDragData.indexes, 0);
	}
	mDragData.type = mListTarget;

	if (mDragData.indexes.size() > 0) {
		if (mpDragImage != NULL)
			delete mpDragImage;
		CPoint ptDragItem;
		mpDragImage = CreateDragImageEx(&ptDragItem);
		if (mpDragImage) {
			mpDragImage->BeginDrag(0, ptDragItem);
			mpDragImage->DragEnter(CWnd::GetDesktopWindow(), pNMLV->ptAction);
			SetCapture();
		}
	}

	*pResult = 0;
}

/*============================================================================*/
/*! リストコントロール

-# マウス移動イベント

@param

@retval
*/
/*============================================================================*/
void CCustomMonCntlListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mpDragImage) {
		CPoint ptDragImage(point);
		ClientToScreen(&ptDragImage);
		mpDragImage->DragMove(ptDragImage);

		// ドロップウィンドウの確認
		CWnd* pDropWnd = WindowFromPoint(ptDragImage);
		mDragData.point.x = ptDragImage.x;
		mDragData.point.y = ptDragImage.y;

		BOOL bTarget = FALSE;
		if (mDragDropCallback != NULL) {
			bTarget = mDragDropCallback(eUserMessage_Drag_Select, (LPARAM)pDropWnd, (LPARAM)&mDragData, 0);
		}
		if (bTarget == TRUE) {
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		else {
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
		}
	}

	CListCtrl::OnMouseMove(nFlags, point);
}


/*============================================================================*/
/*! リストコントロール

-# マウス左ボタンアップイベント

@param

@retval
*/
/*============================================================================*/
void CCustomMonCntlListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (mpDragImage) {
		::ReleaseCapture();
		mpDragImage->DragLeave(CWnd::GetDesktopWindow());
		mpDragImage->EndDrag();

		delete mpDragImage;
		mpDragImage = NULL;
		DropItem(point, (nFlags & MK_CONTROL) ? true : false);
	}

	CListCtrl::OnLButtonUp(nFlags, point);
}

/*============================================================================*/
/*! グループリスト

-# ドロップ対象項目を対象ツリーコントロールに通知する

@param

@retval
*/
/*============================================================================*/
void CCustomMonCntlListCtrl::DropItem(CPoint point, bool bCtrl)
{
	CPoint pt(point);
	ClientToScreen(&pt);
	CWnd* pDropWnd = WindowFromPoint(pt);
	if (pDropWnd == NULL)
		return;

	mDragData.point.x = pt.x;
	mDragData.point.y = pt.y;

	if (mDragDropCallback != NULL) {
		mDragDropCallback(eUserMessage_Drag_DropTarget, (LPARAM)pDropWnd, (LPARAM)&mDragData, 0);
	}
}

/*============================================================================*/
/*! グループリスト

-# ドラッグイメージの作成

@param		lpPoint	位置情報

@retval
*/
/*============================================================================*/
CImageList* CCustomMonCntlListCtrl::CreateDragImageEx(LPPOINT lpPoint)
{
	CRect rectSingle;
	CRect rectComplete(0, 0, 0, 0);
	int	nIndex = -1;
	BOOL bFirst = TRUE;

	// ここで選択されているアイテムの矩形と文字列を取得する

	// ビットマップを作成するための矩形を取得する
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos) {
		nIndex = GetNextSelectedItem(pos);
		GetItemRect(nIndex, rectSingle, LVIR_BOUNDS);
		if (rectSingle.bottom < 0)
			continue;
		if (bFirst) {
			GetItemRect(nIndex, rectComplete, LVIR_BOUNDS);
			bFirst = FALSE;
		}
		rectComplete.UnionRect(rectComplete, rectSingle);
	}

	CClientDC dcClient(this);
	CDC dcMem;
	CBitmap Bitmap;
	// リストコントロールのフォントを取得
	CFont* oldfont, * pfont = GetFont();

	// メモリDCとビットマップの作成
	if (!dcMem.CreateCompatibleDC(&dcClient)) {
		return NULL;
	}

	if (!Bitmap.CreateCompatibleBitmap(&dcClient, rectComplete.Width(), rectComplete.Height())) {
		return NULL;
	}

	CBitmap* pOldMemDCBitmap = dcMem.SelectObject(&Bitmap);
	// 背景塗りつぶし
	dcMem.FillSolidRect(0, 0, rectComplete.Width(), rectComplete.Height(), mDragImageMaskColor);

	oldfont = dcMem.SelectObject(pfont);
	COLORREF oldcolor = dcMem.SetTextColor(mcDragTextColor);

	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	CString str;
	CRect rectItem, rectBase;

	// 先頭の選択アイテム矩形を取得（基準矩形）
	rectBase = CRect(0, 0, 0, 0);
	pos = GetFirstSelectedItemPosition();
	while (pos) {
		nIndex = GetNextSelectedItem(pos);
		if (nIndex >= 0) {
			GetItemRect(nIndex, rectBase, LVIR_BOUNDS);
			if (rectBase.bottom < 0)
				continue;
			break;
		}
	}

	// 選択されているアイテムのテキストを取得してメモリDCに描画する
	pos = GetFirstSelectedItemPosition();
	while (pos) {
		nIndex = GetNextSelectedItem(pos);
		GetItemRect(nIndex, rectItem, LVIR_BOUNDS);
		if (rectItem.bottom < 0)
			continue;
		//TRACE("Drag Item(%d (%d,%d,%d,%d))\n", nIndex, rectItem.left, rectItem.top, rectItem.right, rectItem.bottom);
		CRect rectDraw = CRect(0, 0, 0, 0);
		for (int col = 0; col < nColumnCount; col++) {
			int colwidth = GetColumnWidth(col);
			str = GetItemText(nIndex, col);
			rectDraw.left = rectDraw.right;
			rectDraw.right = rectDraw.left + colwidth;
			rectDraw.top = rectItem.top - rectBase.top;
			rectDraw.bottom = rectDraw.top + rectItem.Height();
			dcMem.FillSolidRect(rectDraw, mcDragBackColor);
			dcMem.DrawText(str, rectDraw, DT_LEFT | DT_TOP);
			break;
		}
	}

	dcMem.SetTextColor(oldcolor);
	dcMem.SelectObject(oldfont);
	dcMem.SelectObject(pOldMemDCBitmap);

	// ドラッグイメージの作成
	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(rectComplete.Width(), rectComplete.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pCompleteImageList->Add(&Bitmap, mDragImageMaskColor);

	Bitmap.DeleteObject();
	if (lpPoint) {
		CPoint ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);
		lpPoint->x = ptCursor.x - rectComplete.left;
		lpPoint->y = ptCursor.y - rectComplete.top;
	}

	return pCompleteImageList;
}


BOOL CCustomMonCntlListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->wParam == VK_ESCAPE)
	{
		if (mpDragImage) {
			::ReleaseCapture();
			mpDragImage->DragLeave(CWnd::GetDesktopWindow());
			mpDragImage->EndDrag();

			delete mpDragImage;
			mpDragImage = NULL;
		}
		return TRUE;
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
#endif
