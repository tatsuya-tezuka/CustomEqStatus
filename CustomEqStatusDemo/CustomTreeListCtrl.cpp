/******************************************************************************
* @file    CustomTreeListCtrl.cpp
* @brief   【カスタマイズ機能】設備状況詳細ツリークラス
* @auther  SCC
* @date    2024/08/26 新規作成
* @par
******************************************************************************/
#include "stdafx.h"
#include "CustomEqStatusDemo.h"

#include "CustomTreeListCtrl.h"


//=============================================================================
// CCustomTreeEdit
//=============================================================================
/*============================================================================*/
/*! ツリー内エディットボックス

-# コンストラクター

@param
@retval

*/
/*============================================================================*/
CCustomTreeEdit::CCustomTreeEdit(HTREEITEM iItem, int iSubItem, CString sInitText)
:msInitText(sInitText)
{
	miItem = iItem;
	miSubItem = iSubItem;
	mbESC = FALSE;
	mbKeyReturn = FALSE;
	mbKeyShift = FALSE;
	mbNotify = FALSE;
	mnNumberLimit = 100;
}

/*============================================================================*/
/*! ツリー内エディットボックス

-# デストラクター

@param
@retval

*/
/*============================================================================*/
CCustomTreeEdit::~CCustomTreeEdit()
{
}

/*============================================================================*/
/*! ツリー内エディットボックス

-# メッセージ マップの定義

@param
@retval

*/
/*============================================================================*/
BEGIN_MESSAGE_MAP(CCustomTreeEdit, CEdit)
	//{{AFX_MSG_MAP(CCustomTreeEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListEdit message handlers

/*============================================================================*/
/*! ツリー内エディットボックス

-# Windowメッセージをディスパッチ前に処理する

@param	pMsg	Windowメッセージ
@retval	BOOL

*/
/*============================================================================*/
BOOL CCustomTreeEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN){
		SHORT sKey = GetKeyState(VK_CONTROL);
		if (pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_DELETE
			|| pMsg->wParam == VK_ESCAPE
			|| sKey
			)
		{
			if (pMsg->wParam == VK_RETURN)
				mbKeyReturn = TRUE;
			if (GetKeyState(VK_SHIFT) & 0xff00)
				mbKeyShift = TRUE;
			else
				mbKeyShift = FALSE;

			::TranslateMessage(pMsg);
			if (!(GetStyle() & ES_MULTILINE) || pMsg->wParam != VK_ESCAPE){
				::DispatchMessage(pMsg);
			}
			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

/*============================================================================*/
/*! リストコントロール内エディットボックス

-# 数値の最大桁数を設定する

@param	len		桁数
@retval

*/
/*============================================================================*/
void CCustomTreeEdit::SetNumberLimit(UINT len)
{
	mnNumberLimit = len;
}

/*============================================================================*/
/*! ツリー内エディットボックス

-# 入力フォーカスを失う直前に呼び出される

@param pNewWnd	入力フォーカスを受け取るウィンドウへのポインター
@retval

*/
/*============================================================================*/
void CCustomTreeEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CString str;
	GetWindowText(str);

	if (mbNotify == TRUE)
	{
		return;
	}
	mbNotify = TRUE;

	// ラベル編集の終了を通知
	TV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = TVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.hItem = miItem;
	dispinfo.item.pszText = mbESC ? NULL : LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = mbESC ? 0 : str.GetLength();

	GetParent()->SetFocus();
	GetParent()->GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
}

/*============================================================================*/
/*! ツリー内エディットボックス

-# キーストロークが文字に変換されるとき呼び出される

@param nChar	キーの文字コード
@param nRepCnt	繰り返し回数
@param nFlags
@retval

*/
/*============================================================================*/
void CCustomTreeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 0x03 || nChar == 0x16 || nChar == 0x18 || nChar == 0x08)//Ctrl+C; Ctrl+V; Ctrl+X; BackSpace
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if (nChar == VK_ESCAPE)
			mbESC = TRUE;
		GetParent()->SetFocus();
		return;
	}
	else
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

/*============================================================================*/
/*! ツリー内エディットボックス

-# ウィンドウが作成されるとき呼び出される

@param lpCreateStruct	ウィンドウを作成するための情報
@retval	int

*/
/*============================================================================*/
int CCustomTreeEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont* font = GetParent()->GetFont();
	SetFont(font);

	SetWindowText(msInitText);
	SetFocus();
	//	CalculateSize();
	SetSel(0, -1);

	CString	str;
	GetWindowText(str);
	int length = mnNumberLimit;
	SetLimitText(length);
	return 0;
}


//=============================================================================
// ◆CCustomTreeListCtrl
//=============================================================================
CCustomTreeListCtrl::CCustomTreeListCtrl()
{
	myOffset = 0;

	mpEdit = NULL;
	mbInplace = FALSE;

	mbDragDragging = false;
	mhDragItemDrag = NULL;
	mhDragItemDrop = NULL;
	mpDragImagelist = NULL;
	mDragNode = NULL;

	// 制御用フォントの作成
	mControlFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (mControlFont.GetLogFont(&lf)){
		lf.lfHeight = mTreeFontHeight;
		lf.lfWeight = FW_BOLD;
		lf.lfUnderline = 1;
		mControlFont.DeleteObject();
		mControlFont.CreateFontIndirect(&lf);
	}

	mDefaultFont.CreateStockObject(DEFAULT_GUI_FONT);
	if (mDefaultFont.GetLogFont(&lf)){
		lf.lfHeight = -mTreeFontHeight;
		lf.lfWeight = FW_NORMAL;
		mDefaultFont.DeleteObject();
		mDefaultFont.CreateFontIndirect(&lf);
	}

	mNodeTitleFont.CreateStockObject(DEFAULT_GUI_FONT);
	if (mNodeTitleFont.GetLogFont(&lf)) {
		lf.lfHeight = -mTreeFontHeight;
		lf.lfWeight = FW_NORMAL;
		mNodeTitleFont.DeleteObject();
		mNodeTitleFont.CreateFontIndirect(&lf);
	}
	mNodeMainFont.CreateStockObject(DEFAULT_GUI_FONT);
	if (mNodeMainFont.GetLogFont(&lf)) {
		lf.lfHeight = -mTreeFontHeight;
		lf.lfWeight = FW_NORMAL;
		mNodeMainFont.DeleteObject();
		mNodeMainFont.CreateFontIndirect(&lf);
	}
	mNodeSubFont.CreateStockObject(DEFAULT_GUI_FONT);
	if (mNodeSubFont.GetLogFont(&lf)) {
		lf.lfHeight = -mTreeFontHeight;
		lf.lfWeight = FW_NORMAL;
		mNodeSubFont.DeleteObject();
		mNodeSubFont.CreateFontIndirect(&lf);
	}
	mNodeLeafFont.CreateStockObject(DEFAULT_GUI_FONT);
	if (mNodeLeafFont.GetLogFont(&lf)) {
		lf.lfHeight = -mTreeFontHeight;
		lf.lfWeight = FW_NORMAL;
		mNodeLeafFont.DeleteObject();
		mNodeLeafFont.CreateFontIndirect(&lf);
	}

	mLastSelectItem = NULL;
}


CCustomTreeListCtrl::~CCustomTreeListCtrl()
{
	if (mpEdit != NULL)
		delete mpEdit;

	mControlFont.DeleteObject();

	mDefaultFont.DeleteObject();
	mNodeTitleFont.DeleteObject();
	mNodeMainFont.DeleteObject();
	mNodeSubFont.DeleteObject();
	mNodeLeafFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CCustomTreeListCtrl, CTreeCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CCustomTreeListCtrl::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CCustomTreeListCtrl::OnTvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, &CCustomTreeListCtrl::OnTvnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CCustomTreeListCtrl::OnTvnEndlabeledit)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CCustomTreeListCtrl::OnNMDblclk)
END_MESSAGE_MAP()


/*============================================================================*/
/*! ツリーリストコントロール

-# コントロールの作成

@param	parent	親ウィンドウ

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::Create(CWnd* parent)
{
	mTreeParent = parent;

	mHeaderCtrl.Create(WS_CHILD | WS_VISIBLE | HDS_FULLDRAG, CRect(), mTreeParent, eHeaderID);

	CFont temp;
	temp.CreateStockObject(DEFAULT_GUI_FONT);
	mHeaderCtrl.SetFont(&temp);
	temp.DeleteObject();

	// ヘッダーコントロールの高さを求める
	CDC* pDC = GetDC();
	pDC->SelectObject(&mDefaultFont);
	CSize szExt = pDC->GetTextExtent(_T("W"));
	mcyHeader = szExt.cy + 7;// +(IsVersion6 ? 10 : 7);

	ReleaseDC(pDC);

	SetImageList(NULL, TVSIL_NORMAL);

	// カラムの開始位置から文字描画位置のオフセットを求める
	mxOffset = 6;
	mxPos = 0;
	mSelectItems.clear();

	// ヘッダーアイテムの作成
	for (int i = 0; i < sizeof(mTreeItemHeader) / sizeof(mTreeItemHeader[0]); i++) {
		if (i == 0)
			SetHeaderItem(i, mTreeItemHeader[i], mTreeHeaderItemSize);
		else
			SetHeaderItem(i, mTreeItemHeader[i], mTreeHeaderSize);
	}

	// アイテムインデントの設定
	SetIndent(5);

	// カラム情報の更新
	UpdateColumns();
}
/*============================================================================*/
/*! ツリーリストコントロール

-# ウィンドウをサブクラス化する前にサブクラス化を許可するには、フレームワークによって呼び出されます

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();
}
/*============================================================================*/
/*! ツリーリストコントロール

-# 事前メッセージ処理

@param

@retval
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::PreTranslateMessage(MSG* pMsg)
{
	return CTreeCtrl::PreTranslateMessage(pMsg);
}
/*============================================================================*/
/*! ツリーリストコントロール

-# メッセージ処理

@param

@retval
*/
/*============================================================================*/
LRESULT CCustomTreeListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}
/*============================================================================*/
/*! ツリーリストコントロール

-# 描画イベント

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	CBitmap memBitmap;

	// 作業用ビットマップの作成（ちらつき防止）
	memDC.CreateCompatibleDC(&dc);
	if (memBitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height())) {
		CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

		// メモリビットマップの描画
		CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

		// DC領域のコピー
		dc.BitBlt(0, 0, rcClient.right, rcClient.bottom, &memDC, 0, 0, SRCCOPY);

		// 後始末
		memDC.SelectObject(pOldBitmap);
		memBitmap.DeleteObject();
	}
	// 後始末
	memDC.DeleteDC();
}
/*============================================================================*/
/*! ツリーリストコントロール

-# 背景描画

@param

@retval
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::OnEraseBkgnd(CDC* pDC)
{
	//CRect	rClient;
	//GetClientRect(&rClient);
	//pDC->FillSolidRect(rClient, RGB(255,0,255));
	return TRUE;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# 垂直スクロールバーイベント

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this) {
		SetFocus();
	}
	if (mbInplace) {
		Invalidate(FALSE);
		mbInplace = FALSE;
	}

	CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

/*============================================================================*/
/*! ツリーリストコントロール

-# マウス左ボタンダウンイベント

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	{
		// 値セルを押下された場合は選択状態にして何もしない
		HTREEITEM hItem;
		UINT col = 0;
		hItem = HitTestEx(point, col);
		if (hItem == NULL) {
			SetFocus();
			return;
		}
		if (col == eTreeItemSubType_Value) {
			SelectItem(hItem);
			SetFocus();
			return;
		}
	}

	// Controlキーを押下している場合は、複数選択
	// Controlキーを押下していない場合は、単数選択あるいは名称変更
	if (!(MK_CONTROL & nFlags)/* && !(MK_SHIFT & nFlags)*/) {
		// Controlキーは押下状態ではない

		CTreeCtrl::OnLButtonDown(nFlags, point);

		HTREEITEM hItem;
		UINT col = 0;
		hItem = HitTestEx(point, col);
		if (hItem == NULL) {
			SetFocus();
			return;
		}
		if (col == eTreeItemSubType_Value) {
			SetFocus();
			return;
		}

		// 先頭カラム以外のラベル編集
		if (hItem != NULL && col != 0) {
			//CString text = GetSubItemText(hItem, col);
			if (cellClick(hItem, col, point) == TRUE) {
				// 編集モードへ切り替え
				SelectItem(hItem);
				SwitchEditMode(hItem, col, point);
				return;
			}
		}

		clearSelection();
		SelectItem(hItem);
		SetFocus();
	}
	else {
		SetFocus();
		do
		{
			HTREEITEM hItem;
			UINT col = 0;
			hItem = HitTestEx(point, col);
			if (hItem == NULL){
				break;
			}
			if (ItemHasChildren(hItem)) {
				break;
			}
			if (GetParentItem(hItem) == NULL){
				break;
			}
			unsigned short shKeyState = GetKeyState(VK_CONTROL);
			shKeyState >>= 15;
			if (shKeyState == 1){
				procControlKeyPress(hItem);
				HTREEITEM hSelectedItem = GetSelectedItem();
				if (ItemHasChildren(hSelectedItem)) {
					SelectItem(hItem);
				}
				break;
			}
			else{
				if (mSelectItems.size() == 0){
					SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
					mSelectItems.push_back(hItem);
					break;
				}
			}
			mLastSelectItem = hItem;
			if (mSelectItems.size() == 1)
			{
				clearSelection();
				SetItemState(mLastSelectItem, TVIS_SELECTED, TVIS_SELECTED);
				mSelectItems.push_back(mLastSelectItem);
			}
		} while (false);
	}
}
/*============================================================================*/
/*! ツリーコントロール拡張機能

-# 先頭セル以外のセルクリック時の処理

@param		hItem		ツリーアイテム
@param		nSubItem	ツリーサブアイテム
@param		point		マウスカーソル位置
@retval

*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::cellClick(HTREEITEM hItem, UINT nSubItem, CPoint point)
{
#ifdef _NOPROC
	//return FALSE;
#endif
	CCustomDetail* p = (CCustomDetail*)mTreeParent;
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(p);

	if (pnode->GetWindowInfo().mode != eTreeItemMode_Edit) {
		return FALSE;
	}


	UINT mask = 1 << eTreeItemSubType_Item | 1 << eTreeItemSubType_Unit;
	if ((1 << nSubItem) & mask)
		return TRUE;

	// 制御セルが押下されたかチェック
	bool bControl = IsControl(point);
	if (bControl == false) {
		return FALSE;
	}

	CString strText = GetSubItemText(hItem, eTreeItemSubType_Control);
	// 制御文字列の場合は制御コマンド実行
	if (strText.IsEmpty() == false && strText == CString(mCOntrolSignString)) {
		// 制御コマンドを送信
		pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(p, hItem);
		CString strCntl = pnode->GetMonCtrl().cname;
		p->MessageBox(_T("（仮）制御コマンドを送信します\n") + strCntl);
		return FALSE;
	}
	return TRUE;
}
/*============================================================================*/
/*! ツリーコントロール拡張機能

-# マウス左ボタンダブルクリックイベント

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}
/*============================================================================*/
/*! ツリーリストコントロール

-# マウス移動イベント

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	HTREEITEM	hItem = 0;
	UINT        flags = 0;
	CPoint		pt = point;
	ClientToScreen(&pt);

#ifdef _NOPROC
#endif
	if (mbDragDragging){
		// ドラッグ中
		// ドラッグ アンド ドロップ操作中にドラッグされているイメージを移動
		mpDragImagelist->DragMove(pt);

		// ドロップウィンドウの確認
		CWnd* pDropWnd = WindowFromPoint(pt);
		BOOL bTarget = FALSE;

		if (pDropWnd == this) {
			// 自分から自分
			TRACE("Drag From me to me\n");
		}
		else {
			// 自分から他人
			TRACE("Drag From me to you\n");
		}

		// ドロップ先が有効かを確認する
		TRACE("=== Drag Target Display : %s\n", CStringA(mDragNode->GetMonCtrl().display));
		vector<CTreeNode*>::iterator itr;
		for (itr = theApp.GetCustomControl().GetDataManager().GetTreeNode().begin(); itr != theApp.GetCustomControl().GetDataManager().GetTreeNode().end(); itr++) {
			if ((*itr)->GetWindowInfo().tree == pDropWnd) {
				if ((*itr)->GetWindowInfo().wnd == NULL)
					break;
				bTarget = ((CCustomDetail*)((*itr)->GetWindowInfo().wnd))->DragDrop_SetSelectTarget(eFromType_Custom, (LPARAM)mDragNode);
				break;
			}
		}

		// ドロップ先の有効無効によってマウスカーソルを変更する
		if (bTarget == TRUE) {
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		else {
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
		}

	}
	else{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		UINT col = 0;
		hItem = hitControl(point);
		if (hItem != NULL){
			if (ptInRectPointCell(point) == TRUE){
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
			}
		}
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}
/*============================================================================*/
/*! ツリーリストコントロール

-# マウス左ボタンアップイベント

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
#ifdef _NOPROC
#endif

	if (!(MK_CONTROL & nFlags)/* && !(MK_SHIFT & nFlags)*/){
		if (mSelectItems.size() > 1){
			clearSelection();
			if (mLastSelectItem != NULL) {
				SetItemState(mLastSelectItem, TVIS_SELECTED, TVIS_SELECTED);
				mSelectItems.push_back(mLastSelectItem);
			}
		}
	}

	if (mbDragDragging){
		// ドラッグ中

		// ウィンドウのロックを解除し、ウィンドウを更新できるようにドラッグイメージを非表示
		mpDragImagelist->DragLeave(NULL);
		// ドラッグ操作を終了
		mpDragImagelist->EndDrag();
		// イメージリストの削除
		delete mpDragImagelist;
		mpDragImagelist = NULL;

		// ドロップ先の確認
		CPoint pt = point;
		ClientToScreen(&pt);
		// ドロップウィンドウの確認
		CWnd* pDropWnd = WindowFromPoint(pt);

		bool bMove = false;
		if (pDropWnd == this) {
			// 自分から自分なので移動する
			bMove = true;
		}
		else {
			// 自分から他人なのでコピーする
			bMove = false;
		}

		BOOL bTarget = FALSE;
		vector<CTreeNode*>::iterator itr;
		for (itr = theApp.GetCustomControl().GetDataManager().GetTreeNode().begin(); itr != theApp.GetCustomControl().GetDataManager().GetTreeNode().end(); itr++) {
			if ((*itr)->GetWindowInfo().tree == pDropWnd) {
				if ((*itr)->GetWindowInfo().wnd == NULL)
					break;
				bTarget = ((CCustomDetail*)((*itr)->GetWindowInfo().wnd))->DragDrop_SetSelectTarget(eFromType_Custom, (LPARAM)mDragNode);
				break;
			}
		}
		
		if (bTarget == TRUE) {
			// ドロップ先はドロップ可能なのでドロップ処理を行う
			vector<CTreeNode*>::iterator itr;
			for (itr = theApp.GetCustomControl().GetDataManager().GetTreeNode().begin(); itr != theApp.GetCustomControl().GetDataManager().GetTreeNode().end(); itr++) {
				if ((*itr)->GetWindowInfo().tree == pDropWnd) {
					if ((*itr)->GetWindowInfo().wnd == NULL)
						break;
					((CCustomDetail*)((*itr)->GetWindowInfo().wnd))->DragDrop_SetDropTarget(eFromType_Custom, (LPARAM)mDragNode, bMove);
					break;
				}
			}
		}
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		ReleaseCapture();
		mbDragDragging = false;
		SelectDropTarget(NULL);
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

/*============================================================================*/
/*! ツリーリストコントロール

-# カスタム描画

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMCUSTOMDRAW* pNMCustomDraw = (NMCUSTOMDRAW*)pNMHDR;
	NMTVCUSTOMDRAW* pNMTVCustomDraw = (NMTVCUSTOMDRAW*)pNMHDR;

	switch (pNMCustomDraw->dwDrawStage)
	{
	case CDDS_PREPAINT: // 描画前
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT: // 項目の描画前
	{
		//*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
		HTREEITEM hItem = (HTREEITEM)pNMCustomDraw->dwItemSpec;
		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hItem);
		if (pnode == NULL) {
			// デフォルト描画
			*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
			break;
		}
		switch (pnode->GetWindowInfo().type) {
		case	eTreeItemType_Window:
		case	eTreeItemType_Title:
			SelectObject(pNMCustomDraw->hdc, mNodeTitleFont);
			*pResult = CDRF_NEWFONT;
			break;
		case	eTreeItemType_Main:
			SelectObject(pNMCustomDraw->hdc, mNodeMainFont);
			*pResult = CDRF_NEWFONT;
			break;
		case	eTreeItemType_Sub:
			SelectObject(pNMCustomDraw->hdc, mNodeSubFont);
			*pResult = CDRF_NEWFONT;
			break;
		default:
			SelectObject(pNMCustomDraw->hdc, mNodeLeafFont);
			*pResult = CDRF_NEWFONT;
			break;
		}
		*pResult = *pResult | CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;

		break;
	}

	case CDDS_ITEMPOSTPAINT:    // 項目の描画後
	{
		HTREEITEM hItem = (HTREEITEM)pNMCustomDraw->dwItemSpec;
		CRect rcItem = pNMCustomDraw->rc;

		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hItem);
		if (pnode == NULL){
			// デフォルト描画
			*pResult = CDRF_DODEFAULT;
			break;
		}

		//UINT type = pnode->getWindowInfo().type;

		if (rcItem.IsRectEmpty()){
			// デフォルト描画
			*pResult = CDRF_DODEFAULT;
			break;
		}

		CDC dc;
		dc.Attach(pNMCustomDraw->hdc);

		switch (pnode->GetWindowInfo().type) {
		case	eTreeItemType_Window:
		case	eTreeItemType_Title:
			dc.SelectObject(mNodeTitleFont);
			break;
		case	eTreeItemType_Main:
			dc.SelectObject(mNodeMainFont);
			break;
		case	eTreeItemType_Sub:
			dc.SelectObject(mNodeSubFont);
			break;
		default:
			dc.SelectObject(mNodeLeafFont);
			break;
		}

		CRect rcLabel;
		GetItemRect(hItem, &rcLabel, TRUE);

		// ラベル項目の塗りつぶし（コントロールの背景色で塗りつぶす）
		dc.FillSolidRect(&rcItem, GetBkColor());

		int nColsCnt = mHeaderCtrl.GetItemCount();

		//// 横線の描画
		int xOffset = 0;
		//for (int i = 0; i<nColsCnt; i++){
		//	xOffset += mColWidths[i];
		//	rcItem.right = xOffset - 1;
		//	dc.DrawEdge(&rcItem, BDR_SUNKENINNER, BF_RIGHT);
		//}
		//// 縦線描画
		//dc.DrawEdge(&rcItem, BDR_SUNKENINNER, BF_BOTTOM);

		CString strText = GetItemText(hItem);
		CString strSub;
		AfxExtractSubString(strSub, strText, eDetailItem, '\t');

		// ラベル項目のサイズ取得
		CRect rcText(0, 0, 0, 0);
		dc.DrawText(strSub, &rcText, DT_NOPREFIX | DT_CALCRECT);
		rcLabel.right = min(rcLabel.left + rcText.right + 4, mColWidths[0] - 4);

		COLORREF backcolor = pnode->GetColor().textback;
		if (pNMCustomDraw->uItemState & CDIS_SELECTED || GetDropHilightItem() == hItem){
			// 選択時の背景色をハイライトに設定
			dc.FillSolidRect(&rcItem, GetSysColor(COLOR_HIGHLIGHT));
			backcolor = GetSysColor(COLOR_HIGHLIGHT);
		}
		else{
			// 各ノード毎の背景色に設定
			//dc.FillSolidRect(&rcItem, pnode->getColor().textback);
			rcText = rcItem;
			rcText.left = 0;
			rcText.right = mColWidths[0];
			dc.FillSolidRect(&rcText, backcolor);
		}

		// フォーカス時の描画
		//if (pNMCustomDraw->uItemState & CDIS_FOCUS){
		//	dc.DrawFocusRect(&rcLabel);
		//}

		// ラベルの描画
		dc.SetBkMode(TRANSPARENT);
		rcText = rcLabel;
		rcText.DeflateRect(2, 1);
		dc.SetTextColor(pnode->GetColor().text);
		dc.DrawText(strSub, &rcText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

		xOffset = mColWidths[0];

		// その他のカラム文字の描画
		for (int i = 1; i<nColsCnt; i++){
			switch (i){
			case	eTreeItemSubType_Value:
				dc.SetTextColor(pnode->GetColor().value);
				break;
			case	eTreeItemSubType_Unit:
				dc.SetTextColor(pnode->GetColor().unit);
				break;
			default:
				dc.SetTextColor(pnode->GetColor().text);
				break;
			}
			rcText = rcLabel;
			rcText.left = xOffset;
			rcText.right = xOffset + mColWidths[i];
			dc.FillSolidRect(&rcText, backcolor);
			if (AfxExtractSubString(strSub, strText, i, '\t')){
				rcText.DeflateRect(mxOffset, 1, 2, 1);
				if (strSub == CString(mCOntrolSignString)){
					CFont* p = (CFont*)dc.SelectObject(&mControlFont);
					//dc.SetTextColor(RGB(0, 0, 255));
					dc.SetTextColor(calcContrastColor(backcolor));
					dc.DrawText(mCOntrolSignStringDisplay, &rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);
					dc.SelectObject(p);
				}
				else{
					dc.DrawText(strSub, &rcText, (i == 1) ? DT_SINGLELINE | DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS : DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);
				}
			}
			xOffset += mColWidths[i];
		}

		// アイテム領域を塗りつぶしているので最後に罫線を描画する（罫線なしの場合は以下を削除する）
		const bool bGrid = true;
		if (bGrid == true){
			// 横線の描画
			rcItem = pNMCustomDraw->rc;
			xOffset = 0;
			for (int i = 0; i<nColsCnt; i++){
				xOffset += mColWidths[i];
				rcItem.right = xOffset - 1;
				dc.DrawEdge(&rcItem, BDR_SUNKENINNER, BF_RIGHT);
			}
			// 縦線描画
			dc.DrawEdge(&rcItem, BDR_SUNKENINNER, BF_BOTTOM);
		}

		dc.Detach();

		*pResult = CDRF_DODEFAULT;
		break;
	}

	default:
		*pResult = CDRF_DODEFAULT;
	}
	//*pResult = 0;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# ラベル編集開始

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
#ifdef _NOPROC
	*pResult = 1;
	return;
#endif
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, pTVDispInfo->item.hItem);
	if (pnode != NULL){
		UINT type = pnode->GetWindowInfo().type;
		if (type == eTreeItemType_Item){
			// マウス位置からセル場所を取得する
			HTREEITEM hItem;
			UINT col = 0;
			CPoint point;
			GetCursorPos(&point);
			ScreenToClient(&point);
			hItem = HitTestEx(point, col);
			// 先頭セルの場合は編集処理を行う
			if (hItem != NULL){
				if (col != 0) {
					*pResult = 0;
					return;
				}
				// 先頭セルの場合はラベル編集を行う
				SwitchEditMode(hItem, col, point);
				*pResult = 1;
				return;
			}
		}
	}
	*pResult = 0;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# ラベル編集終了

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, pTVDispInfo->item.hItem);
	if (pnode == NULL){
		*pResult = 0;
		return;
	}

	UINT type = pnode->GetWindowInfo().type;

	if (pTVDispInfo->item.mask & TVIF_TEXT){
		// 先頭アイテムの場合
		if (type == eTreeItemType_Item)
		{
			if (pTVDispInfo->item.pszText != NULL){
				CString str = pTVDispInfo->item.pszText;
				SetSubItemText(pTVDispInfo->item.hItem, ((CCustomTreeEdit*)mpEdit)->GetSubItem(), str);
				switch (((CCustomTreeEdit*)mpEdit)->GetSubItem()){
				case	eDetailItem:
					swprintf_s(pnode->GetMonCtrl().display, mNameSize, _T("%s"), (LPCTSTR)GetSubItemText(pTVDispInfo->item.hItem, eDetailItem));
					break;
				case	eDetailUnit:
					swprintf_s(pnode->GetMonCtrl().unit, mUnitSize, _T("%s"), (LPCTSTR)GetSubItemText(pTVDispInfo->item.hItem, eDetailUnit));
					break;
				}
			}
			if (mpEdit != NULL){
				delete mpEdit;
				mpEdit = NULL;
			}
			SelectItem(pTVDispInfo->item.hItem);
			SetFocus();
			*pResult = 0;
			return;
		}

		// 先頭アイテム以外の場合
		swprintf_s(pnode->GetMonCtrl().display, mNameSize, _T("%s"), pTVDispInfo->item.pszText);
		SetItemText(pTVDispInfo->item.hItem, pTVDispInfo->item.pszText);
		Invalidate(FALSE);
		if (type == eTreeItemType_Title)
		{
			if (mTreeParent != NULL){
				mTreeParent->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)pTVDispInfo->item.hItem);
				//pnode = theApp.GetDataManager().SearchWndNode(mTreeParent);
				//pnode->getWindowInfo().manager->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)mTreeParent);
			}
		}
	}
	*pResult = 0;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# ドラッグ処理の開始

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

#ifdef _NOPROC
#endif

	CPoint      ptAction;
	UINT        nFlags;

	// ドラッグアイテムの取得
	GetCursorPos(&ptAction);
	ScreenToClient(&ptAction);
	mhDragItemDrag = HitTest(ptAction, &nFlags);
	mhDragItemDrop = NULL;

	// ドラッグ有効チェック
	if (enableDragItem(mhDragItemDrag) == false) {
		return;
	}
	mbDragDragging = true;

	CPoint point;
	// ドラッグイメージの作成
	mpDragImagelist = createDragImageEx(mhDragItemDrag);
	//mpImagelist = CreateDragImage(mhItemDrag);
	// ウィンドウをロックせずに、ドラッグ操作中にドラッグイメージを表示
	mpDragImagelist->DragShowNolock(TRUE);
	// 新しいドラッグ イメージを作成
	mpDragImagelist->SetDragCursorImage(0, CPoint(0, 0));
	// イメージのドラッグを開始
	mpDragImagelist->BeginDrag(0, CPoint(0, 0));
	// ドラッグ アンド ドロップ操作中にドラッグされているイメージを移動
	//mpImagelist->DragMove(ptAction);
	// ドラッグ操作中に更新をロックし、指定した位置にドラッグ イメージを表示
	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen(&pt);
	mpDragImagelist->DragEnter(NULL, pt);
	// ドラッグされたノードを取得
	mDragNode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, mhDragItemDrag);

	SetCapture();
}

/*============================================================================*/
/*! ツリーリストコントロール

-# ドラッグの有効性確認

@param		hItem		ツリーアイテム
@retval

*/
/*============================================================================*/
bool CCustomTreeListCtrl::enableDragItem(HTREEITEM hItem)
{
	CTreeNode* pnode;
	BOOL bDropExecute = FALSE;

	pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hItem);
	if (pnode != NULL && pnode->GetWindowInfo().type == eTreeItemType_Item)
		return TRUE;

#ifdef _NOPROC
//	return FALSE;
#endif
	if (pnode != NULL && pnode->GetWindowInfo().type == eTreeItemType_Main)
		return TRUE;
	if (pnode != NULL && pnode->GetWindowInfo().type == eTreeItemType_Sub)
		return TRUE;

	return FALSE;
}

/*============================================================================*/
/*! ツリーリストコントロール

-# ツリー項目の開閉時のイベント

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// イベントを無効にする
	*pResult = 1; // ツリーの開閉を有効にするのには０を設定する
}

/*============================================================================*/
/*! ツリーリストコントロール

-# サブアイテムテキストの取得

@param	hItem	アイテムのハンドル
@param	col		サブアイテム

@retval	int	アイテム
*/
/*============================================================================*/
CString CCustomTreeListCtrl::GetSubItemText(HTREEITEM hItem, int col)
{
	CString str;
	str = GetItemText(hItem);
	vector<CString>	list;
	vector<CString>::iterator itr;

	extractSubString(str, list);

	str = _T("");
	if (list.size() > (UINT)col) {
		str = list[col];
	}
	return str;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# サブアイテムテキストの設定

@param	hItem	アイテムのハンドル
@param	col		サブアイテム

@retval	int	アイテム
*/
/*============================================================================*/
void CCustomTreeListCtrl::SetSubItemText(HTREEITEM hItem, int col, CString strText)
{
	CString str;
	str = GetItemText(hItem);
	vector<CString>	list;
	vector<CString>::iterator itr;

	extractSubString(str, list);

	if (list.size() <= (UINT)col)
		return;

	list[col] = strText;

	str.Empty();
	for (itr = list.begin(); itr != list.end(); itr++){
		if (str.IsEmpty() == false){
			str += '\t';
		}
		str += (*itr);
	}

	SetItemText(hItem, str);
}
/*============================================================================*/
/*! ツリーリストコントロール

-# マウス位置からアイテム、サブイテム位置を求める

@param	point	マウス位置
@param	col		サブアイテム

@retval	int	アイテム
*/
/*============================================================================*/
HTREEITEM CCustomTreeListCtrl::HitTestEx(CPoint &point, UINT& col)
{
	int colnum = 0;
	HTREEITEM hItem = HitTest(point, NULL);
	col = 0;

	// ヘッダーカラム数の取得
	int nColumnCount = mHeaderCtrl.GetItemCount();

	CRect rectClient, rect;
	GetClientRect(rectClient);
	GetItemRect(hItem, &rect, FALSE);
	rect.right = rectClient.right;
	if (rect.PtInRect(point) == FALSE) {
		return NULL;
	}

	// マウス位置のカラム位置を取得する
	for (colnum = 0; colnum < nColumnCount; colnum++) {
		int colwidth = mColWidths[colnum];
		if (point.x >= rect.left && point.x <= (rect.left + colwidth)) {
			col = colnum;
			return hItem;
		}
		rect.left += colwidth;
	}
	return NULL;
}

/*============================================================================*/
/*! ツリーリストコントロール

-# マウスポインタが制御文字列にあるかのチェック

@param  point   マウス位置

@retval 正常終了時にツリーアイテムを返す
*/
/*============================================================================*/
bool CCustomTreeListCtrl::IsControl(CPoint point)
{
	HTREEITEM hItem = hitControl(point);
	if (hItem != NULL){
		if (ptInRectPointCell(point) == TRUE){
			return true;
		}
	}
	return false;
}

/*============================================================================*/
/*! ツリーリストコントロール

-# コントロールのリサイズ

@param	parent	親ウィンドウ

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::ResizeControl(int dx, int dy)
{
	CRect rect;

	mHeaderCtrl.GetWindowRect(rect);
	mTreeParent->ScreenToClient(rect);
	rect.right += dx;
	mHeaderCtrl.MoveWindow(rect);
	mHeaderCtrl.Invalidate();

	GetWindowRect(rect);
	mTreeParent->ScreenToClient(rect);
	rect.right += dx;
	rect.bottom += dy;
	MoveWindow(rect);
	Invalidate();

	UpdateScroller();
	RepositionControls();
}
/*============================================================================*/
/*! ツリーリストコントロール

-# ヘッダーアイテム間のマウスダブルクリック

@param	item	アイテム番号

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::DividerDblClick(int item)
{
	AdjustColumnWidth(item, FALSE);
	Invalidate();
	UpdateWindow();
}

/*============================================================================*/
/*! ツリーリストコントロール

-# ヘッダー情報の登録

@param	parent	親ウィンドウ

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::SetHeaderItem(int item, CString title, int width)
{
	HDITEM hditem;
	hditem.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
	hditem.fmt = HDF_CENTER | HDF_STRING/* | HDF_OWNERDRAW*/;
	hditem.cxy = width;
	hditem.pszText = (LPWSTR)(LPCTSTR)title;
	mHeaderCtrl.InsertItem(item, &hditem);
}
/*============================================================================*/
/*! ツリーリストコントロール

-# カラム情報の更新

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomTreeListCtrl::UpdateColumns()
{
	mcxTotal = 0;

	HDITEM hditem;
	hditem.mask = HDI_WIDTH;
	int nCnt = mHeaderCtrl.GetItemCount();

	// ヘッダーカラム幅を格納する領域の確保
	mColWidths.clear();
	mColWidths.resize(nCnt);

	// ヘッダーコントロールのカラム幅の取得
	for (int i = 0; i<nCnt; i++) {
		if (mHeaderCtrl.GetItem(i, &hditem)) {
			mColWidths[i] = hditem.cxy;
			mcxTotal += hditem.cxy;
			if (i == 0)
				mcxFirstCol = hditem.cxy;
		}
	}

	UpdateScroller();
	RepositionControls();
}
/*============================================================================*/
/*! ツリーリストコントロール
-# 各コントロールの再配置

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomTreeListCtrl::RepositionControls()
{
	// 各子コントロールの再配置を行う
	if (m_hWnd) {
		CRect rcClient;
		mTreeParent->GetClientRect(&rcClient);
		int cx = rcClient.Width();
		int cy = rcClient.Height();

		// 横スクロール位置からの配置
		int x = 0;
		if (cx < mcxTotal) {
			x = mTreeParent->GetScrollPos(SB_HORZ);
			cx += x;
		}
		mHeaderCtrl.MoveWindow(-x, 0 + myOffset, cx, mcyHeader);
		MoveWindow(-x, mcyHeader + myOffset, cx, cy - (mcyHeader + myOffset));
	}
}

/*============================================================================*/
/*! ツリーリストコントロール

-# カラム幅の調整

@param	nColumn				調整を行うカラム番号
@param	bIgnoreCollapsed	閉じているアイテムを無視するかのフラグ

@retval void
*/
/*============================================================================*/
void CCustomTreeListCtrl::AdjustColumnWidth(int nColumn, BOOL bIgnoreCollapsed)
{
	int nMaxWidth = getMaxColumnWidth(GetRootItem(), nColumn, 0, bIgnoreCollapsed);

	HDITEM hditem;
	hditem.mask = HDI_WIDTH;
	mHeaderCtrl.GetItem(nColumn, &hditem);
	hditem.cxy = nMaxWidth + 25;
	mHeaderCtrl.SetItem(nColumn, &hditem);
}
/*============================================================================*/
/*! ツリーリストコントロール

-# ヘッダー情報の水平サイズ取得

@param	col		ヘッダー位置（-1の場合全て）

@retval	int		サイズ
*/
/*============================================================================*/
int CCustomTreeListCtrl::GetHeaderWidth(int col/*=-1*/)
{
	HDITEM hditem;
	int size = 0;
	// ヘッダーコントロールのカラム幅の取得
	hditem.mask = HDI_WIDTH;
	int nCnt = mHeaderCtrl.GetItemCount();
	for (int i = 0; i<nCnt; i++) {
		if (mHeaderCtrl.GetItem(i, &hditem)) {
			mColWidths[i] = hditem.cxy;
			if (col == i || col < 0){
				size += hditem.cxy;
			}
		}
	}

	return size;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# ツリーのアイテムを編集モードへ切り替える

@param	hItem		ツリーアイテムハンドル
@param	col			カラム番号
@param	place		クリック場所

@retval	BOOL		TRUE：編集モード可、FALSE：編集モード否
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::SwitchEditMode(HTREEITEM hItem, UINT col, CPoint point)
{
#ifdef _NOPROC
		SetFocus();
		return FALSE;
#endif
	if (hItem == NULL)
		return FALSE;

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hItem);
	UINT type = pnode->GetWindowInfo().type;
	if (type != eTreeItemType_Item) {
		SetFocus();
		return FALSE;
	}

	CEdit* pedit = editSubLabel(hItem, col);
	if (pedit == NULL){
		SetFocus();
	}
	//CString text;
	//pedit->GetWindowText(text);

	return TRUE;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# スクロールバーの更新

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomTreeListCtrl::UpdateScroller()
{
	CRect rcClient;
	mTreeParent->GetClientRect(&rcClient);
	int cx = rcClient.Width();

	int lx = mxPos;

	if (mxPos > mcxTotal - cx)
		mxPos = mcxTotal - cx;
	if (mxPos < 0)
		mxPos = 0;

	SCROLLINFO scrinfo;
	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	scrinfo.nPage = cx;
	scrinfo.nMin = 0;
	scrinfo.nMax = mcxTotal;
	scrinfo.nPos = mxPos;
	mTreeParent->SetScrollInfo(SB_HORZ, &scrinfo);
}

/*============================================================================*/
/*! ツリーコントロール拡張機能

-# Ctrlキー押下時の選択処理

@param	hItem	ツリー選択アイテム

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::procControlKeyPress(HTREEITEM hCurItem)
{
	if (mSelectItems.size() > 0)
	{
		if (!isSameLevel(hCurItem)){
			SelectItem(hCurItem);
			clearSelection();
			return;
		}
	}

	int nState = (TVIS_SELECTED == GetItemState(hCurItem, TVIS_SELECTED)) ? 0 : TVIS_SELECTED;
	SetItemState(hCurItem, nState, TVIS_SELECTED);
	if (0 == nState)
	{
		removeFromSelectList(hCurItem);
	}
	else
	{
		mSelectItems.push_back(hCurItem);
	}
}

/*============================================================================*/
/*! ツリーコントロール拡張機能

-# Shiftキー押下時の選択処理

@param	hItem	ツリー選択アイテム

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::procShiftKeyPress(HTREEITEM hCurItem)
{
	if (mSelectItems.size() > 0)
	{
		if (!isSameLevel(hCurItem)){
			return;
		}
	}
	HTREEITEM hItemFrom = mSelectItems[0];
	SetItemState(hCurItem, TVIS_SELECTED, TVIS_SELECTED);
	//SelectItems(hItemFrom, hCurItem);
}

/*============================================================================*/
/*! ツリーコントロール拡張機能

-# Ctrlキー押下時の選択処理

@param	hItem	ツリー選択アイテム

@retval
*/
/*============================================================================*/
bool CCustomTreeListCtrl::isSameLevel(HTREEITEM hItem)
{
	bool bSameLevel = true;
	vector<HTREEITEM>::iterator itr;
	for (itr = mSelectItems.begin(); itr != mSelectItems.end(); ++itr)
	{
		if (GetParentItem(hItem) != GetParentItem(*itr))
		{
			bSameLevel = false;
		}
	}
	return bSameLevel;
}

/*============================================================================*/
/*! ツリーコントロール拡張機能

-# Ctrlキー押下時の選択処理

@param	hItem	ツリー選択アイテム

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::removeFromSelectList(HTREEITEM hItem)
{
	vector<HTREEITEM>::iterator itr;
	for (itr = mSelectItems.begin(); mSelectItems.end() != itr; ++itr)
	{
		if ((*itr) == hItem){
			mSelectItems.erase(itr);
			//mSelectItems.clear();
			break;
		}
	}
}

/*============================================================================*/
/*! ツリーコントロール拡張機能

-# 選択リストのクリア

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::clearSelection()
{
	int nSelItemCount = (int)mSelectItems.size();
	for (int nIdx = 0; nIdx < nSelItemCount; ++nIdx)
	{
		SetItemState(mSelectItems[nIdx], 0, TVIS_SELECTED);
	}
	mSelectItems.clear();
}

/*============================================================================*/
/*! ツリーリストコントロール

-# リーフの更新

@param	hTargetItem			ドロップアイテム
@param	pDataObject			ドラッグ情報

@retval BOOL
*/
/*============================================================================*/
CString CCustomTreeListCtrl::createDragString(HTREEITEM hDragItem)
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hDragItem);
	if (pnode == NULL)
		return _T("");

	CString str;
	str.Format(_T("%s\t%s\t%s\t%s"), pnode->GetMonCtrl().display, pnode->GetMonCtrl().unit, pnode->GetMonCtrl().mname, pnode->GetMonCtrl().cname);

	return str;
}

/*============================================================================*/
/*! ツリーリストコントロール

-# 複数選択

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::selectMultiItem(HTREEITEM hClickedItem, UINT nFlags)
{
	if(nFlags & MK_CONTROL){
		// 現在の選択アイテムを取得
		HTREEITEM hSelectedItem = GetSelectedItem();

		// 選択アイテム、クリックアイテムの状態取得
		BOOL bIsClickedItemSelected = GetItemState(hClickedItem, TVIS_SELECTED) & TVIS_SELECTED;
		BOOL bIsSelectedItemSelected = GetItemState(hSelectedItem, TVIS_SELECTED) & TVIS_SELECTED;

		// クリックアイテムを選択する
		SelectItem(hClickedItem);

		// 選択されているアイテムの状態を復元する
		if (bIsSelectedItemSelected)
			SetItemState(hSelectedItem, TVIS_SELECTED, TVIS_SELECTED);

		// クリックアイテムも状態で選択、非選択を設定する
		if (bIsClickedItemSelected)
			SetItemState(hClickedItem, 0, TVIS_SELECTED);
		else
			SetItemState(hClickedItem, TVIS_SELECTED, TVIS_SELECTED);
	}
}
/*============================================================================*/
/*! ツリーリストコントロール

-# サブアイテム位置にCMFCMaskedEditを表示する

@param	hItem		アイテムのハンドル
@param	col			サブアイテム
@param	val			値
@param	type		識別子

@retval	CComboBox
*/
/*============================================================================*/
CEdit* CCustomTreeListCtrl::editSubLabel(HTREEITEM hItem, int col)
{
	CString text = GetSubItemText(hItem, col);

	//if (text.IsEmpty() == true)
	//	return NULL;

	// ヘッダーカラム数の取得
	int nColumnCount = mHeaderCtrl.GetItemCount();
	if (col >= nColumnCount)
		return NULL;

	int offset = 0;
	for (int i = 0; i < col; i++)
		offset += mColWidths[i];

	CRect rect;
	GetItemRect(hItem, &rect, FALSE);

	CRect rcClient;
	GetClientRect(&rcClient);
	if (offset + rect.left < 0 || offset + rect.left > rcClient.right){
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		rect.left -= size.cx;
	}

	DWORD dwStyle;
	dwStyle = ES_LEFT;

	rect.left += offset + 4;
	rect.right = rect.left + mColWidths[col] - 3;
	if (rect.right > rcClient.right)
		rect.right = rcClient.right;

	dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL/*|ES_RIGHT*/;
	if (mpEdit != NULL){
		mpEdit->SendMessage(WM_KILLFOCUS);
		delete mpEdit;
		mpEdit = NULL;
	}

#define IDC_EDITCTRL 0x1234
	mpEdit = new CCustomTreeEdit(hItem, col, text);
	mpEdit->Create(dwStyle, rect, this, IDC_EDITCTRL);
	SelectItem(NULL); // これをしないとKillFocusで編集が終了する

	return mpEdit;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# ドラッグイメージの作成

@param

@retval
*/
/*============================================================================*/
CImageList* CCustomTreeListCtrl::createDragImageEx(HTREEITEM hItem)
{
	if (GetImageList(TVSIL_NORMAL) != NULL)
		return CreateDragImage(hItem);

	CRect rect, rc;
	GetItemRect(hItem, rect, TRUE);
	rc = rect;
	rect.top = rect.left = 0;
	rect.right = GetHeaderWidth();// rc.Width();
	rect.bottom = rc.Height();

	// ビットマップの作成
	CClientDC	dc(this);
	CDC 		memDC;

	if (!memDC.CreateCompatibleDC(&dc))
		return NULL;

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	CFont* pOldFont = memDC.SelectObject(GetFont());

	memDC.FillSolidRect(&rect, mDragBackColor);
	//memDC.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	//memDC.TextOut(rect.left, rect.top, GetItemText(hItem));
	memDC.SetTextColor(mDragTextColor);
	CString strSub;
	AfxExtractSubString(strSub, GetItemText(hItem), eDetailItem, '\t');
	memDC.DrawText(strSub, rect, DT_LEFT | DT_TOP);

	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldMemDCBitmap);

	// イメージリストの作成
	CImageList* pImageList = new CImageList;
	pImageList->Create(rect.Width(), rect.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pImageList->Add(&bitmap, RGB(0, 255, 0));

	return pImageList;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# マウスポインタが制御文字列にあるかのチェック

@param  point   マウス位置

@retval 正常終了時にツリーアイテムを返す
*/
/*============================================================================*/
HTREEITEM CCustomTreeListCtrl::hitControl(CPoint point)
{
	int colnum = 0;
	UINT col = 0;
	HTREEITEM hItem = HitTestEx(point, col);
	if (hItem == NULL)
		return NULL;

	CString strText = GetItemText(hItem);
	int nColsCnt = mHeaderCtrl.GetItemCount();
	for (int i = 1; i < nColsCnt; i++){
		CString strSub;
		if (AfxExtractSubString(strSub, strText, i, '\t')){
			if (strSub == CString(mCOntrolSignString) && col == i){
				return hItem;
			}
		}
	}
	return NULL;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# マウスポインタがセル内の文字列上にあるかのチェック

@param  point   マウス位置

@retval bool
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::ptInRectPointCell(CPoint point)
{
	int colnum = 0;
	UINT col = 0;
	HTREEITEM hItem = HitTestEx(point, col);
	if (hItem == NULL)
		return FALSE;

	CString strText = GetItemText(hItem);
	int nColsCnt = mHeaderCtrl.GetItemCount();
	CString cellText = _T("");
	for (int i = 1; i < nColsCnt; i++){
		CString strSub;
		if (AfxExtractSubString(strSub, strText, i, '\t')){
			if (i == col){
				cellText = strSub;
				if (strSub == CString(mCOntrolSignString)){
					cellText = mCOntrolSignStringDisplay;
				}
				break;
			}
		}
	}
	if (cellText.IsEmpty())
		return FALSE;

	CDC* pDC = GetDC();
	CFont* pOldFont = pDC->SelectObject(GetFont());
	CSize size = pDC->GetTextExtent(cellText);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	CRect rect;
	if (getColumnsRect(hItem, col, rect) == false)
		return FALSE;

	HDITEM hditem = { 0 };
	hditem.mask = HDI_FORMAT;
	mHeaderCtrl.GetItem(col, &hditem);

	if (hditem.fmt & HDF_CENTER)
		rect.DeflateRect((rect.Width() - size.cx) / 2, 0);
	else if (hditem.fmt & HDF_RIGHT)
		rect.left = rect.right - size.cx;
	else
		rect.right = rect.left + size.cx;

	return rect.PtInRect(point);
}
/*============================================================================*/
/*! ツリーリストコントロール

-# 指定カラム矩形の取得

@param	hItem	アイテム
@param	col		サブアイテム
@param	rect	サブアイテム矩形

@retval	bool
*/
/*============================================================================*/
bool CCustomTreeListCtrl::getColumnsRect(HTREEITEM hItem, UINT col, CRect& rect)
{
	if (hItem == NULL){
		return false;
	}

	// ヘッダーカラム数の取得
	int nColumnCount = mHeaderCtrl.GetItemCount();

	CRect rectClient;
	GetClientRect(rectClient);
	GetItemRect(hItem, &rect, FALSE);
	rect.right = rectClient.right;
	for (int colnum = 0; colnum < nColumnCount; colnum++) {
		int colwidth = mColWidths[colnum];
		if (colnum == col) {
			rect.right = rect.left + colwidth;
			return true;
		}
		rect.left += colwidth;
	}
	return false;
}

/*============================================================================*/
/*! ツリーリストコントロール

-# カラム幅のの取得

@param	hItem				ツリーアイテム
@param	nColumn				調整を行うカラム番号
@param	nDepth				ツリーレベルの深さ
@param	bIgnoreCollapsed	閉じているアイテムを無視するかのフラグ

@retval void
*/
/*============================================================================*/
int CCustomTreeListCtrl::getMaxColumnWidth(HTREEITEM hItem, int nColumn, int nDepth, BOOL bIgnoreCollapsed)
{
	int nMaxWidth = 0;

	CString strText = GetItemText(hItem);
	CString strSub;
	if (AfxExtractSubString(strSub, strText, nColumn, '\t')){
		CDC dc;
		dc.CreateCompatibleDC(NULL);
		CFont* pOldFont = dc.SelectObject(GetFont());
		// 文字列幅をフォント、文字数から計算
		nMaxWidth = dc.GetTextExtent(strSub, strSub.GetLength()).cx;
		dc.SelectObject(pOldFont);
		dc.DeleteDC();
	}

	// インデントを考慮する
	if (nColumn == 0){
		int nIndent = nDepth;
		if (GetWindowLong(m_hWnd, GWL_STYLE) & TVS_LINESATROOT)
			nIndent++;

		int nImage, nSelImage;
		GetItemImage(hItem, nImage, nSelImage);
		if (nImage >= 0)
			nIndent++;

		nMaxWidth += nIndent * GetIndent();
	}

	if (!bIgnoreCollapsed || (GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED)){
		// 子レベル幅を加算
		HTREEITEM hSubItem = GetChildItem(hItem);
		while (hSubItem){
			int nSubWidth = getMaxColumnWidth(hSubItem, nColumn, nDepth + 1, bIgnoreCollapsed);
			if (nSubWidth > nMaxWidth)
				nMaxWidth = nSubWidth;
			hSubItem = GetNextSiblingItem(hSubItem);
		}
	}

	return nMaxWidth;
}

#ifndef _NOPROC
/*============================================================================*/
/*! 設備詳細

-# コールバック関数

@param
@retval

*/
/*============================================================================*/
DROPEFFECT CALLBACK CCustomTreeListCtrl::Callback_DragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_COPY;
}
/*============================================================================*/
/*! 設備詳細

-# コールバック関数

@param
@retval

*/
/*============================================================================*/
DROPEFFECT CALLBACK CCustomTreeListCtrl::Callback_DragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CCustomTreeListCtrl* ptree = (CCustomTreeListCtrl*)pWnd;

	CPoint pt = CPoint(point);
	// ドロップ位置情報から対象のアイテムを取得する
	HTREEITEM hItem = ptree->HitTest(pt);
	if (hItem == NULL){
		ptree->SelectDropTarget(NULL);
	}
	else{
		CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(ptree->GetParent(), hItem);
		if (pnode == NULL){
			ptree->SelectDropTarget(NULL);
		}
		else{
			ptree->Expand(hItem, TVE_EXPAND);
			UINT type = pnode->getWindowInfo().type;
			switch (type){
			case	eTreeItemType_Sub:
				ptree->SelectDropTarget(hItem);
				break;
			case	eTreeItemType_Item:
				ptree->SelectDropTarget(hItem);
				break;
			default:
				ptree->SelectDropTarget(NULL);
			}
		}
	}

	return DROPEFFECT_COPY;
}
/*============================================================================*/
/*! 設備詳細

-# コールバック関数

@param
@retval

*/
/*============================================================================*/
void CALLBACK CCustomTreeListCtrl::Callback_DragLeave(CWnd* pWnd)
{
	CCustomTreeListCtrl* ptree = (CCustomTreeListCtrl*)pWnd;
	ptree->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE, 0);
}
/*============================================================================*/
/*! 設備詳細

-# コールバック関数（ドロップイベント）

@param
@retval

*/
/*============================================================================*/
BOOL CALLBACK CCustomTreeListCtrl::Callback_DragDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	CCustomTreeListCtrl* ptree = (CCustomTreeListCtrl*)pWnd;

	CPoint pt = CPoint(point);
	HTREEITEM hItem = ptree->HitTest(pt);
	if (hItem == NULL){
		return FALSE;
	}
	CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(ptree->GetParent(), hItem);
	if (pnode == NULL){
		return FALSE;
	}

	// ドロップ先によって処理を分ける
	BOOL ret = FALSE;
	UINT type = pnode->getWindowInfo().type;
	if (type == eTreeItemType_Sub){
		ret = ptree->AddLeaf(hItem, pnode, point, pDataObject);
	}
	if (type == eTreeItemType_Item){
		ret = ptree->UpdateLeaf(hItem, pnode, point, pDataObject);
	}
	ptree->SelectDropTarget(NULL);

	return ret;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# リーフの登録

@param	hTargetItem			ドロップアイテム
@param	pDataObject			ドラッグ情報

@retval BOOL
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::AddLeaf(HTREEITEM hTargetItem, CTreeNode* pnode, CPoint point, COleDataObject* pDataObject)
{
	stDragData data;
	BOOL ret = CreateDragData(hTargetItem, pnode, point, pDataObject, data);
	if (ret == FALSE)
		return FALSE;

	((CCustomDetail*)mTreeParent)->DropAddLeaf(hTargetItem, pnode, &data);

	return TRUE;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# リーフの更新

@param	hTargetItem			ドロップアイテム
@param	pDataObject			ドラッグ情報

@retval BOOL
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::UpdateLeaf(HTREEITEM hTargetItem, CTreeNode* pnode, CPoint point, COleDataObject* pDataObject)
{
	stDragData data;
	BOOL ret = CreateDragData(hTargetItem, pnode, point, pDataObject, data);
	if (ret == FALSE)
		return FALSE;

	if (data.indexes.size() != 1)
		return FALSE;

	((CCustomDetail*)mTreeParent)->DropUpdateLeaf(hTargetItem, pnode, &data);

	return TRUE;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# リーフの更新

@param	hTargetItem			ドロップアイテム
@param	pDataObject			ドラッグ情報

@retval BOOL
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::CreateDragData(HTREEITEM hTargetItem, CTreeNode* pnode, CPoint point, COleDataObject* pDataObject, stDragData& data)
{
	data.point.x = point.x;
	data.point.y = point.y;

	std::vector<CString>::iterator itr;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CF_DSPTEXT);
	CString dragText = static_cast<LPCTSTR>(::GlobalLock(hGlobal));

	CString str;
	int index = 0;
	// ドラッグ種別取得
	AfxExtractSubString(str, dragText, index++, '\t');
	BYTE type = *((BYTE*)str.GetBuffer());
	data.type = (UINT)type;

	if (type != eFromType_Mon && type != eFromType_Cntl)
		return FALSE;

	vector<CString>& list = (type == eFromType_Mon) ? theApp.GetDataManager().GetDataMonitor().GetEqList() : theApp.GetDataManager().GetDataControl().GetEqList();

	while (AfxExtractSubString(str, dragText, index++, '\t')) {
		if (str.IsEmpty())
			continue;
		itr = std::find(list.begin(), list.end(), str);
		int item = (int)std::distance(list.begin(), itr);
		data.indexes.push_back(item);
	}

	return TRUE;
}
#endif

/*============================================================================*/
/*! ツリーリストコントロール

-# フォントの取得

@param	type	ノード種別

@retval CFont
*/
/*============================================================================*/
CFont& CCustomTreeListCtrl::GetFontEx(UINT type)
{
	switch (type) {
	case	eTreeItemType_Title:
		return mNodeTitleFont;
		break;
	case	eTreeItemType_Main:
		return mNodeMainFont;
		break;
	case	eTreeItemType_Sub:
		return mNodeSubFont;
		break;
	case	eTreeItemType_Item:
		return mNodeLeafFont;
		break;
	}
	return mDefaultFont;
}
/*============================================================================*/
/*! ツリーリストコントロール

-# フォントの設定

@param	type	ノード種別
@param	lf		論理フォント

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::SetFontEx(UINT type, LOGFONT& lf)
{
	switch (type) {
	case	eTreeItemType_Window:
		mDefaultFont.DeleteObject();
		mDefaultFont.CreateFontIndirect(&lf);
		SetFont(&mDefaultFont);
		break;
	case	eTreeItemType_Title:
		mNodeTitleFont.DeleteObject();
		mNodeTitleFont.CreateFontIndirect(&lf);
		break;
	case	eTreeItemType_Main:
		mNodeMainFont.DeleteObject();
		mNodeMainFont.CreateFontIndirect(&lf);
		break;
	case	eTreeItemType_Sub:
		mNodeSubFont.DeleteObject();
		mNodeSubFont.CreateFontIndirect(&lf);
		break;
	case	eTreeItemType_Item:
		mNodeLeafFont.DeleteObject();
		mNodeLeafFont.CreateFontIndirect(&lf);
		break;
	}
}
/*============================================================================*/
/*! ツリーリストコントロール

	-# ツリー ビュー アイテムの全開

	@param	hItem		ツリーアイテムハンドル

	@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::ExpandAll(HTREEITEM hItem/*=NULL*/)
{
	if (hItem == NULL){
		hItem = GetRootItem();
	}
	Expand(hItem, TVE_EXPAND);
	HTREEITEM hNextItem = GetChildItem(hItem);

	while (hNextItem != NULL) {
		Expand(hNextItem, TVE_EXPAND);
		ExpandAll(hNextItem);
		hNextItem = GetNextItem(hNextItem, TVGN_NEXT);
	}
}
