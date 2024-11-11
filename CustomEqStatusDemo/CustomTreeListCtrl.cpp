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

	// ドラッグ＆ドロップ
	DragDrop_Initialize(mTreeParent);
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
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
		if (mpDragImage) {
			::ReleaseCapture();
			mpDragImage->DragLeave(NULL);
			mpDragImage->EndDrag();
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			delete mpDragImage;
			mpDragImage = NULL;
			theApp.GetCustomControl().GetCustomDragTarget().OnDragLeave(this);
		}
		return TRUE;

	}

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
#if _DEMO_PHASE < 50
	return FALSE;
#endif
	CCustomDetail* p = (CCustomDetail*)mTreeParent;
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(p);

	UINT mask = 1 << eTreeItemSubType_Item | 1 << eTreeItemSubType_Unit;
	if ((1 << nSubItem) & mask)
		return TRUE;

	// 項目、単位以外はここにくる

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

	TRACE("*** OnLButtonDblClk\n");
	if (!(MK_CONTROL & nFlags)/* && !(MK_SHIFT & nFlags)*/) {
		// Controlキーは押下状態ではない

		HTREEITEM hItem;
		UINT col = 0;
		hItem = HitTestEx(point, col);
		if (hItem == NULL) {
			SetFocus();
			return;
		}
		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(mTreeParent);
		if (pnode != NULL && pnode->GetWindowInfo().mode == eTreeItemMode_Edit) {
			UINT mask = 1 << eTreeItemSubType_Item | 1 << eTreeItemSubType_Unit;
			if ((1 << col) & mask) {
				if (hItem != NULL /*&& col != 0*/) {
					//CString text = GetSubItemText(hItem, col);
					//if (cellClick(hItem, col, point) == TRUE)
					{
						// 編集モードへ切り替え
						SelectItem(hItem);
						SwitchEditMode(hItem, col, point);
						return;
					}
				}
			}
		}
		SelectItem(hItem);
		SetFocus();
	}
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
#if _DEMO_PHASE < 50
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
		if (mpEdit != NULL) {
			delete mpEdit;
			mpEdit = NULL;
		}
		*pResult = 0;
		return;
	}

	UINT type = pnode->GetWindowInfo().type;

	if (!(pTVDispInfo->item.mask & TVIF_TEXT)) {
		if (mpEdit != NULL) {
			delete mpEdit;
			mpEdit = NULL;
		}
		*pResult = 0;
		return;
	}
	if (pTVDispInfo->item.pszText == NULL) {
		if (mpEdit != NULL) {
			delete mpEdit;
			mpEdit = NULL;
		}
		SelectItem(pTVDispInfo->item.hItem);
		SetFocus();
		*pResult = 0;
		return;
	}

	// 先頭アイテムの場合
	if (type == eTreeItemType_Item)
	{
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
		if (mTreeParent != NULL) {
			mTreeParent->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)pTVDispInfo->item.hItem);
		}
	}
	if (mpEdit != NULL) {
		delete mpEdit;
		mpEdit = NULL;
	}
	SelectItem(pTVDispInfo->item.hItem);
	SetFocus();
	*pResult = 0;
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
	TRACE("*** OnNMDblclk\n");
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
#if _DEMO_PHASE < 50
	SetFocus();
	return FALSE;
#endif
	if (hItem == NULL)
		return FALSE;

	CEdit* pedit = editSubLabel(hItem, col);
	if (pedit == NULL){
		SetFocus();
		return FALSE;
	}

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

/*============================================================================*/
/*! ツリーリストコントロール

-# 全アイテムを展開する

@param

@retval int
*/
/*============================================================================*/
void CCustomTreeListCtrl::ExpandAllItems(HTREEITEM item)
{
	if (ItemHasChildren(item)) {
		Expand(item, TVE_EXPAND);
		item = GetChildItem(item);
		if (item) {
			do {
				ExpandAllItems(item);
			} while ((item = GetNextSiblingItem(item)) != NULL);
		}
	}
}

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




/*============================================================================*/
///
/// ★ドラッグ＆ドロップ関連
///
/*============================================================================*/

/*============================================================================*/
/*! ツリーリストコントロール（ドラッグ＆ドロップ関連）

-# コントロールの作成

@param	parent	親ウィンドウ

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::DragDrop_Initialize(CWnd* parent)
{
	CCustomDetail* p = (CCustomDetail*)parent;
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(p);
	if (pnode != NULL && pnode->GetWindowInfo().kind == eTreeItemKind_Master) {
		mDragFormat = CCustomDropObject::DF_MASTER;
		mDropFormat = CCustomDropObject::DF_NONE;
	}
	else {
		mDragFormat = CCustomDropObject::DF_USER;
		mDropFormat = CCustomDropObject::DF_USER | CCustomDropObject::DF_MASTER | CCustomDropObject::DF_MONITOR | CCustomDropObject::DF_CONTROL;
	}

	theApp.GetCustomControl().GetCustomDragTarget().Register(this, mDragFormat);
	theApp.GetCustomControl().GetCustomDragTarget().SetCallbackDragOver(this, Callback_Detail_DragOver);
	theApp.GetCustomControl().GetCustomDragTarget().SetCallbackDragDrop(this, Callback_Detail_DragDrop);
	theApp.GetCustomControl().GetCustomDragTarget().SetCallbackDragLeave(this, Callback_Detail_DragLeave);

	mcDragBackColor = GetSysColor(COLOR_WINDOW);
	mcDragTextColor = GetSysColor(COLOR_WINDOWTEXT);

}

/*============================================================================*/
/*! ツリーリストコントロール（ドラッグ＆ドロップ関連）

-# コールバック関数：ドラッグオーバー処理

@param	pWnd		ドロップウィンドウ
@param	pDataObject	ドラッグデータ情報
@param	dwKeyState	キーボード情報
@param	point		ドロップ位置

@retval
*/
/*============================================================================*/
DROPEFFECT CALLBACK CCustomTreeListCtrl::Callback_Detail_DragOver(CWnd* pWnd, void* pDataObject, UINT dwKeyState, CPoint point)
{
	CCustomTreeListCtrl* ptree = (CCustomTreeListCtrl*)pWnd;
	CCustomDropObject* pdata = (CCustomDropObject*)pDataObject;

	if (!(pdata->mFormat & ptree->GetDropFormat())) {
		return DROPEFFECT_NONE;
	}

	//TRACE("#DragOver\n");

	CPoint pt = CPoint(point);
	ptree->ScreenToClient(&pt);
	// ドロップ位置情報から対象のアイテムを取得する
	HTREEITEM hItem = ptree->HitTest(pt);
	if (hItem == NULL) {
		return DROPEFFECT_NONE;
	}
	DWORD dw = (DWORD)ptree->TYPEDATA((DWORD)ptree->GetItemData(hItem));
	//TRACE("#DragOver(%d->%d)\n", pdata->mKind, dw);

	// ドラッグとドロップ先の関係を調べる
	if (ptree->IsDropTarget(hItem, pdata) == FALSE) {
		return DROPEFFECT_NONE;
	}

	ptree->SelectDropTarget(hItem);

	DROPEFFECT de = DROPEFFECT_NONE;
	if (dwKeyState & MK_SHIFT) {
		de = DROPEFFECT_MOVE;
	}
	else {
		de = DROPEFFECT_COPY;
	}

	return de;
}

/*============================================================================*/
/*! ツリーリストコントロール（ドラッグ＆ドロップ関連）

-# コールバック関数：ドラッグドロップ処理

@param	pWnd		ドロップウィンドウ
@param	pDataObject	ドラッグデータ情報
@param	dropEffect	ドロップ方法
@param	point		ドロップ位置

@retval
*/
/*============================================================================*/
BOOL CALLBACK CCustomTreeListCtrl::Callback_Detail_DragDrop(CWnd* pWnd, void* pDataObject, UINT dropEffect, CPoint point)
{
	CCustomTreeListCtrl* ptree = (CCustomTreeListCtrl*)pWnd;
	CCustomDropObject* pdata = (CCustomDropObject*)pDataObject;

	if (!(pdata->mFormat & ptree->GetDropFormat())) {
		ptree->ClearDropTarget();
		return DROPEFFECT_NONE;
	}

	//TRACE("#DragDrop\n");

	CPoint pt = CPoint(point);
	ptree->ScreenToClient(&pt);
	// ドロップ位置情報から対象のアイテムを取得する
	HTREEITEM hItem = ptree->HitTest(pt);
	if (hItem == NULL) {
		//ptree->SelectDropTarget(NULL);
		ptree->ClearDropTarget();
		return DROPEFFECT_NONE;
	}
	DWORD dw = (DWORD)ptree->TYPEDATA((DWORD)ptree->GetItemData(hItem));
	// ドラッグとドロップ先の関係を調べる
	if (ptree->IsDropTarget(hItem, pdata) == FALSE) {
		//ptree->SelectDropTarget(NULL);
		ptree->ClearDropTarget();
		return DROPEFFECT_NONE;
	}

	BOOL bRet = ptree->DataObjectToList(hItem, pdata);
	if (dropEffect == CCustomDropObject::DE_MOVE) {
		// 移動時の削除はドラッグ側で行う
	}
	//ptree->SelectDropTarget(NULL);
	ptree->ClearDropTarget();
	return dropEffect;
}

/*============================================================================*/
/*! ツリーリストコントロール（ドラッグ＆ドロップ関連）

-# コールバック関数：ドラッグリーブ処理

@param	pWnd		ドロップウィンドウ

@retval
*/
/*============================================================================*/
void CALLBACK CCustomTreeListCtrl::Callback_Detail_DragLeave(CWnd* pWnd)
{
	CCustomTreeListCtrl* ptree = (CCustomTreeListCtrl*)pWnd;
	TRACE("#DragLeave\n");
	ptree->ClearDropTarget();
}

/*============================================================================*/
/*! ツリーリストコントロール（ドラッグ＆ドロップ関連）

-# ドラッグ処理の開始

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnTvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

#if _DEMO_PHASE >= 90

	if (pNMTreeView == NULL) {
		*pResult = 0;
		return;
	}
	if (IsDragEnable() == false) {
		*pResult = 0;
		return;
	}

	CPoint      ptAction;
	UINT        nFlags;
	// ドラッグアイテムの取得
	GetCursorPos(&ptAction);
	ScreenToClient(&ptAction);
	HTREEITEM item = HitTest(ptAction, &nFlags);
	if (item == NULL) {
		*pResult = 0;
		return;
	}
	DWORD dw = (DWORD)TYPEDATA((DWORD)GetItemData(item));
	if (dw == CCustomDropObject::DK_TITLE) {
		*pResult = 0;
		return;
	}

	CPoint ptDragItem;
	mpDragImage = CreateDragImageMulti(item, &ptDragItem);
	if (mpDragImage != NULL) {
		CStringArray list;
		CNode* root = new CNode();
		if (PrepareItemBuff(root) == FALSE) {
			delete root;
			ASSERT(0);
			*pResult = 0;
			return;
		}
		SetFocus();
		//printRec(_T(""), root);
		theApp.GetCustomControl().GetCustomDragTarget().OnDragBegin(root, mDragFormat, CCustomDropObject::DT_NODE);
		delete root;

		theApp.GetCustomControl().GetCustomDragTarget().SetDragKind(this, dw);
		// ウィンドウをロックせずに、ドラッグ操作中にドラッグイメージを表示
		mpDragImage->DragShowNolock(TRUE);
		// 新しいドラッグ イメージを作成
		//mpDragImage->SetDragCursorImage(0, CPoint(0, 0));
		// イメージのドラッグを開始
		mpDragImage->BeginDrag(0, ptDragItem);
		// ドラッグ操作中に更新をロックし、指定した位置にドラッグ イメージを表示
		POINT pt = pNMTreeView->ptDrag;
		ClientToScreen(&pt);
		mpDragImage->DragEnter(NULL, pt);
		SetCapture();
	}
#endif

	*pResult = 0;
}

/*============================================================================*/
/*! ツリーリストコントロール（ドラッグ＆ドロップ関連）

-# ドラッグ可能かをチェックする

@param

@retval
*/
/*============================================================================*/
bool CCustomTreeListCtrl::IsDragEnable()
{
	if (GetSelectedCount() == 1)
		return true;

	bool bNode = false, bLeaf = false;
	for (HTREEITEM item = GetFirstSelectedItem(); item != 0; item = GetNextSelectedItem(item)) {
		if (ItemHasChildren(item))
			bNode = true;
		else
			bLeaf = true;
	}
	if (bNode && bLeaf)
		return false;
	return true;
}


/*============================================================================*/
/*! ツリーリストコントロール（ドラッグ＆ドロップ関連）

-# マウス左ボタンダウンイベント

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
#if _DEMO_PHASE >= 90
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

	TRACE("OnLButtonDown : Select Items : \n");

	CTreeCtrl::OnLButtonDown(nFlags, point);

	// 先頭カラム以外のラベル編集
	// 制御イベントかをチェック
	if (cellClick(hItem, col, point) == TRUE) {
	}
	SelectItem(hItem);
	SetFocus();
#else
	CTreeCtrl::OnLButtonDown(nFlags, point);
#endif
}

/*============================================================================*/
/*! ツリーリストコントロール（ドラッグ＆ドロップ関連）

-# マウス移動イベント

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
#if _DEMO_PHASE >= 90
	if (mpDragImage != NULL) {
		// ドラッグ中
		// ドラッグ アンド ドロップ操作中にドラッグされているイメージを移動
		CPoint ptDragImage(point);
		ClientToScreen(&ptDragImage);
		mpDragImage->DragMove(ptDragImage);

		BOOL bTarget = FALSE;
		UINT ret = theApp.GetCustomControl().GetCustomDragTarget().OnDragMove(this, nFlags, ptDragImage);
		if (ret == CCustomDropObject::DE_COPY) {
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DRAGCOPY));
		}
		else if (ret == CCustomDropObject::DE_MOVE) {
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DRAGMOVE));
		}
		else {
			SetCursor(theApp.LoadCursor(IDC_CURSOR_DRAGERROR));
		}
	}
#endif

	CTreeCtrl::OnMouseMove(nFlags, point);
}
/*============================================================================*/
/*! ツリーリストコントロール（ドラッグ＆ドロップ関連）

-# マウス左ボタンアップイベント

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
#if _DEMO_PHASE >= 90
	if (mpDragImage) {
		::ReleaseCapture();
		mpDragImage->DragLeave(NULL/*CWnd::GetDesktopWindow()*/);
		mpDragImage->EndDrag();
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

		delete mpDragImage;
		mpDragImage = NULL;

		CPoint ptDragImage(point);
		ClientToScreen(&ptDragImage);
		UINT ret = theApp.GetCustomControl().GetCustomDragTarget().OnDragDrop(this, (nFlags & MK_SHIFT) ? CCustomDropObject::DE_MOVE : CCustomDropObject::DE_COPY, ptDragImage);
		//TRACE("#Drop Result(%d)\n", ret);
		if (ret == CCustomDropObject::DE_COPY) {
			// コピー（＋CTRL）の場合;
			TRACE("#DROP_COPY\n");
		}
		else if (ret == CCustomDropObject::DE_MOVE) {
			// 移動の場合
			TRACE("#DROP_MOVE\n");
			DeleteSelectedItems();
		}
		else {
			//TRACE("#DROPEFFECT_NONE\n");
		}
		UpdateWindow();
	}
	TRACE("OnLButtonUp : Select Items : \n");
#endif
	CTreeCtrl::OnLButtonUp(nFlags, point);
}
/*============================================================================*/
/*! ツリーリストコントロール（ドラッグ＆ドロップ関連）

-# ドラッグアイテムの削除

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::DeleteSelectedItems()
{
	for (HTREEITEM item = GetFirstSelectedItem(); item != 0; item = GetNextSelectedItem(item)) {
		CTreeNode* pparent_node = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, GetParentItem(item));
		pparent_node->DeleteTreeNode(item);
		DeleteItem(item);
	}
}

/*============================================================================*/
/*! ツリーリストコントロール（ドラッグ＆ドロップ関連）

-# ドラッグイメージの作成

@param

@retval
*/
/*============================================================================*/
CImageList* CCustomTreeListCtrl::CreateDragImageMulti(HTREEITEM hItem, LPPOINT lpPoint)
{
	if (GetImageList(TVSIL_NORMAL) != NULL)
		return CreateDragImage(hItem);

	CRect rectComplete(0, 0, 0, 0);
	CRect rectSingle;
	BOOL bFirst = TRUE;

	for (HTREEITEM item = GetFirstSelectedItem(); item != 0; item = GetNextSelectedItem(item)) {
		GetItemRect(item, rectSingle, FALSE);
		if (rectSingle.bottom < 0)
			continue;
		if (bFirst) {
			GetItemRect(item, rectComplete, FALSE);
			bFirst = FALSE;
		}
		rectComplete.UnionRect(rectComplete, rectSingle);
	}

	// ビットマップの作成
	CClientDC	dc(this);
	CDC 		memDC;

	if (!memDC.CreateCompatibleDC(&dc))
		return NULL;

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dc, rectComplete.Width(), rectComplete.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	CFont* pOldFont = memDC.SelectObject(GetFont());

	memDC.FillSolidRect(0, 0, rectComplete.Width(), rectComplete.Height(), mDragImageMaskColor);

	memDC.SetTextColor(mcDragTextColor);

	CRect rectItem, rectBase;
	for (HTREEITEM item = GetFirstSelectedItem(); item != 0; item = GetNextSelectedItem(item)) {
		GetItemRect(item, rectBase, FALSE);
		if (rectBase.bottom < 0)
			continue;
		break;
	}

	CString strSub;
	for (HTREEITEM item = GetFirstSelectedItem(); item != 0; item = GetNextSelectedItem(item)) {
		GetItemRect(item, rectItem, FALSE);
		if (rectItem.bottom < 0)
			continue;
		CRect rectDraw = CRect(0, 0, 0, 0);
		rectDraw.left = rectDraw.right;
		rectDraw.right = rectDraw.left + rectComplete.Width();
		rectDraw.top = rectItem.top - rectBase.top;
		rectDraw.bottom = rectDraw.top + rectItem.Height();
		AfxExtractSubString(strSub, GetItemText(item), 0, '\t');
		memDC.FillSolidRect(rectDraw, mcDragBackColor);
		memDC.DrawText(strSub, rectDraw, DT_LEFT | DT_TOP);
		TRACE("(%d, %d) - (%d, %d)\n", rectDraw.left, rectDraw.top, rectDraw.right, rectDraw.bottom);
	}

	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldMemDCBitmap);

	// イメージリストの作成
	CImageList* pImageList = new CImageList;
	pImageList->Create(rectComplete.Width(), rectComplete.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pImageList->Add(&bitmap, mDragImageMaskColor);

	bitmap.DeleteObject();

	if (lpPoint) {
		CPoint ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);
		lpPoint->x = ptCursor.x - rectComplete.left;
		lpPoint->y = ptCursor.y - rectComplete.top;
	}

	return pImageList;
}

/*============================================================================*/
/*! ツリーコントロール（ドラッグ＆ドロップ関連）

-# ドラッグアイテムの作成

@param

@retval
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::PrepareItemBuff(CNode* root)
{
	CString str;
	for (HTREEITEM item = GetFirstSelectedItem(); item != 0; item = GetNextSelectedItem(item)) {
		str = GetItemText(item);
		CNode* cur = root->createChildIfNotExist(str);
		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, item);
		cur->setNodeData(pnode);
		cur->setWnd(this);
		PrepareChildItem(item, cur);
	}

	return TRUE;
}

/*============================================================================*/
/*! ツリーコントロール（ドラッグ＆ドロップ関連）

-# ドラッグ子アイテムの作成

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::PrepareChildItem(HTREEITEM hItem, CNode* root)
{
	CString str;
	if (ItemHasChildren(hItem)) {
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = GetChildItem(hItem);
		while (hChildItem != NULL) {
			hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
			str = GetItemText(hChildItem);
			CNode* cur = root->createChildIfNotExist(str);
			CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hChildItem);
			cur->setNodeData(pnode);
			cur->setWnd(this);
			PrepareChildItem(hChildItem, cur);
			hChildItem = hNextItem;
		}
	}
}
/*============================================================================*/
/*! ツリーコントロール（ドラッグ＆ドロップ関連）

-# ドロップされたデータをリストへ登録

@param

@retval
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::IsDropTarget(HTREEITEM hItem, CCustomDropObject* pDataObject)
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(mTreeParent);
	if (pnode != NULL && pnode->GetWindowInfo().mode != eTreeItemMode_Edit) {
		return FALSE;
	}

	DWORD dw = (DWORD)TYPEDATA((DWORD)GetItemData(hItem));
	switch (pDataObject->mFormat) {
	case	CCustomDropObject::DF_USER:
	case	CCustomDropObject::DF_MASTER:
		switch (pDataObject->mKind) {
		case	CCustomDropObject::DK_MAINNODE:
			if (dw != CCustomDropObject::DK_TITLE)
				return FALSE;
			break;
		case	CCustomDropObject::DK_SUBNODE:
			if (dw != CCustomDropObject::DK_MAINNODE)
				return FALSE;
			break;
		case	CCustomDropObject::DK_LEAF:
			if (dw != CCustomDropObject::DK_SUBNODE && dw != CCustomDropObject::DK_LEAF)
				return FALSE;
			break;
		}
		break;
	case	CCustomDropObject::DF_MONITOR:
	case	CCustomDropObject::DF_CONTROL:
		switch (dw) {
		case	CCustomDropObject::DK_TITLE:
		case	CCustomDropObject::DK_MAINNODE:
			return FALSE;
		}
		break;
	}

	return TRUE;
}
/*============================================================================*/
/*! ツリーコントロール（ドラッグ＆ドロップ関連）

-# コールバック関数：ドラッグリーブ処理

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::ClearDropTarget(HTREEITEM hRoot/*=NULL*/)
{
	SelectDropTarget(NULL);
	return;

	// ドラッグ アンド ドロップ ターゲットとして強調表示クリア
	if (hRoot == NULL) {
		hRoot = GetRootItem();
	}
	HTREEITEM hChildItem = GetChildItem(hRoot);
	while (hChildItem != NULL)
	{
		if (GetItemState(hChildItem, TVIS_SELECTED) & TVIS_SELECTED) {
			TRACE("#SELECTED OFF\n");
			SetItemState(hChildItem, 0, TVIS_SELECTED);
		}
		if (GetItemState(hChildItem, TVIS_DROPHILITED) & TVIS_DROPHILITED) {
			TRACE("#DROPHILITED OFF\n");
			SetItemState(hChildItem, 0, TVIS_DROPHILITED);
		}
		if (GetDropHilightItem() == hChildItem) {
			TRACE("#DropHilightItem OFF\n");
			SelectDropTarget(NULL);
		}
		ClearDropTarget(hChildItem);
		hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
	}
}
/*============================================================================*/
/*! リストコントロール（ドラッグ＆ドロップ関連）

-# ドロップされたデータをリストへ登録

@param	hDropItem	ドロップアイテム
@param	pDataObject	ドラッグデータ情報

@retval
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::DataObjectToList(HTREEITEM hDropItem, CCustomDropObject* pDataObject)
{
	DWORD dw = (DWORD)TYPEDATA((DWORD)GetItemData(hDropItem));
	HTREEITEM hParent = GetParentItem(hDropItem);
	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hDropItem);
	UINT count = 0;

	if (pDataObject->mDataType == CCustomDropObject::DT_TCHAR) {
		HTREEITEM hSortItem = NULL;
		TCHAR* pbuf = (TCHAR*)pDataObject->GetBuffer();
		if (pbuf == NULL)
			return FALSE;
		CString str, sItem, strItems = CString(pbuf);
		UINT index = 0;
		while (AfxExtractSubString(sItem, strItems, index++, ';')) {
			if (sItem.IsEmpty())
				continue;

			UINT item = 0;
			while (AfxExtractSubString(str, sItem, item, '\t')) {
				if (str.IsEmpty())
					continue;
				if (dw == CCustomDropObject::DK_SUBNODE) {
					// サブノードにドロップ
					// サブノードの子ノード先頭に追加する
					HTREEITEM item = InsertItem(str, NULL, NULL, hDropItem, TVI_SORT);
					hSortItem = hDropItem;
					SetItemData(item, MAKEDATA(CCustomDropObject::DK_LEAF, count));
					CTreeNode* new_node = pnodeDrop->CreateTreeNode(hDropItem, item, TVI_FIRST);
					if (pDataObject->mFormat == CCustomDropObject::DF_MONITOR) {
						swprintf_s(new_node->GetMonCtrl().mname, mNameSize, _T("%s"), (LPCTSTR)str);
					}
					else {
						swprintf_s(new_node->GetMonCtrl().cname, mNameSize, _T("%s"), (LPCTSTR)str);
					}
					new_node->SetParentNode(pnodeDrop);
					new_node->SetTreeItem(item);
					new_node->GetWindowInfo().tree = this;
					new_node->GetWindowInfo().wnd = mTreeParent;
					new_node->GetWindowInfo().type = eTreeItemType_Item;

					stColorData color;
					theApp.GetCustomControl().GetDataManager().GetNodeColor(mTreeParent, new_node->GetWindowInfo().type, color);
					memcpy(&(new_node->GetColor()), &color, sizeof(stColorData));
				}
				else {
					// リーフにドロップ
					// リーフの監視、制御を更新する
					CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hDropItem);
					if (pDataObject->mFormat == CCustomDropObject::DF_MONITOR) {
						swprintf_s(pnodeDrop->GetMonCtrl().mname, mNameSize, _T("%s"), (LPCTSTR)str);
					}
					else {
						swprintf_s(pnodeDrop->GetMonCtrl().cname, mNameSize, _T("%s"), (LPCTSTR)str);
					}
					CString strLeafstr = CreateLeafText(pnodeDrop->GetMonCtrl().display, pnodeDrop->GetMonCtrl().unit, pnodeDrop->GetMonCtrl().cname);
					SetItemText(hDropItem, strLeafstr);
				}
				break;
			}
			count++;
		}
		if (hSortItem != NULL) {
			SortLeafItem(hSortItem);
		}
	}

	if (pDataObject->mDataType == CCustomDropObject::DT_NODE) {
		DropCopyItem(hDropItem, pDataObject);
	}

	ExpandAllItems(hDropItem);
	EnsureVisible(hDropItem);

	return TRUE;
}
/*============================================================================*/
/*! リストコントロール（ドラッグ＆ドロップ関連）

-# ドロップ時のアイテムのコピー

@param	hDropItem	ドロップアイテム
@param	pDataObject	ドラッグデータ情報

@retval
*/
/*============================================================================*/
bool CCustomTreeListCtrl::DropCopyItem(HTREEITEM hDropItem, CCustomDropObject* pDataObject)
{
	bool ret = true;
	DWORD dw = (DWORD)TYPEDATA((DWORD)GetItemData(hDropItem));
	HTREEITEM hParent = GetParentItem(hDropItem);

	// ドラッグデータの先頭を取得
	CNode* node = pDataObject->mpNode;

	// 子ノード以下をコピーする
	ret = DropCopyChildItem(hDropItem, node, true);

	return ret;
}
/*============================================================================*/
/*! リストコントロール（ドラッグ＆ドロップ関連）

-# ドロップ時のアイテムのコピー

@param	hDropItem	ドロップアイテム
@param	pDataObject	ドラッグデータ情報

@retval
*/
/*============================================================================*/
bool CCustomTreeListCtrl::DropCopyChildItem(HTREEITEM hDropItem, CNode* node, bool bFirst)
{
	UINT count = 0;
	DWORD dw = (DWORD)TYPEDATA((DWORD)GetItemData(hDropItem));
	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hDropItem);

	// ドロップ先の親アイテム、親ノード情報を取得
	HTREEITEM hParent = GetParentItem(hDropItem);
	CTreeNode* pnodeParent = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hParent);

	// 子ノード以下をコピーする
	count = 0;
	HTREEITEM hSortItem = NULL;
	for (::std::vector<CNode*>::const_iterator itr = node->getChildren().begin(); itr != node->getChildren().end(); itr++) {
		CTreeNode* pnode = (*itr)->getNodeData();
		CString text = pnode->GetMonCtrl().display;
		if (pnode->GetWindowInfo().type == eTreeItemType_Item) {
			// リーフ用のテキストを作成
			text = CreateLeafText(pnode->GetMonCtrl().display, pnode->GetMonCtrl().unit, pnode->GetMonCtrl().cname);
		}
		if (dw == CCustomDropObject::DK_LEAF) {
			// ドロップ先がリーフの場合
			HTREEITEM item = InsertItem(text, NULL, NULL, hParent, TVI_SORT);
			SetItemData(item, MAKEDATA(dw + 1, pnodeDrop->GetWindowInfo().sortno + 1));
			CTreeNode* new_node = pnodeParent->CreateTreeNode(hParent, item, TVI_FIRST);
			new_node->DropCopyItem(pnode);
			stColorData color;
			theApp.GetCustomControl().GetDataManager().GetNodeColor(mTreeParent, new_node->GetWindowInfo().type, color);
			memcpy(&(new_node->GetColor()), &color, sizeof(stColorData));
			hSortItem = hParent;
		}
		else {
			// ドロップ先がリーフ以外の場合
			HTREEITEM item;
			if (pnode->GetWindowInfo().type == eTreeItemType_Item) {
				item = InsertItem(text, NULL, NULL, hDropItem, TVI_SORT);
				hSortItem = hDropItem;
			}
			else {
				item = InsertItem(text, NULL, NULL, hDropItem, (bFirst) ? TVI_FIRST : TVI_LAST);
			}
			SetItemData(item, MAKEDATA(dw + 1, count));
			CTreeNode* new_node = pnodeDrop->CreateTreeNode(hDropItem, item, (bFirst) ? TVI_FIRST : TVI_LAST);
			new_node->DropCopyItem(pnode);
			stColorData color;
			theApp.GetCustomControl().GetDataManager().GetNodeColor(mTreeParent, new_node->GetWindowInfo().type, color);
			memcpy(&(new_node->GetColor()), &color, sizeof(stColorData));
			DropCopyChildItem(item, (*itr), false);
		}
		count++;
	}

	if (hSortItem != NULL) {
		SortLeafItem(hSortItem);
	}

	return true;
}

/*============================================================================*/
/*! リストコントロール（ドラッグ＆ドロップ関連）

-# ツリーﾉｰﾄﾞの作成

@param

@retval
*/
/*============================================================================*/
CString CCustomTreeListCtrl::CreateLeafText(CString item, CString unit, CString control)
{
	CString str;
	str.Format(_T("%s\t\t%s\t%s"), (LPCTSTR)item, (LPCTSTR)unit, control.IsEmpty() == false ? mCOntrolSignString : _T(""));
	return str;
}

/*============================================================================*/
/*! リストコントロール（ドラッグ＆ドロップ関連）

-# ツリーコントロールのソートを行う

@param

@retval int
*/
/*============================================================================*/
void CCustomTreeListCtrl::SortLeafItem(HTREEITEM item)
{
	if (item != NULL && ItemHasChildren(item)) {

		TVSORTCB tvs;
		tvs.hParent = item;
		tvs.lpfnCompare = LeafCustomCompare;
		tvs.lParam = (LPARAM)this;

		SortChildrenCB(&tvs);
		// ソート番号の更新
		UpdateSortNo(item);
		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, item);
		pnode->SortTreeNode(item);
	}
}

/*============================================================================*/
/*! リストコントロール（ドラッグ＆ドロップ関連）

-# ツリーコントロールのソートを行うコールバック関数

@param

@retval int
*/
/*============================================================================*/
void CCustomTreeListCtrl::UpdateSortNo(HTREEITEM item)
{
	HTREEITEM child = GetChildItem(item);
	UINT pos = 1;
	while (child != NULL) {
		SetItemData(child, MAKEDATA(CCustomDropObject::DK_LEAF, pos * mSortRange));
		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, child);
		pnode->GetWindowInfo().sortno = (pos * mSortRange);
		child = GetNextItem(child, TVGN_NEXT);
		pos++;
	}
}

/*============================================================================*/
/*! リストコントロール（ドラッグ＆ドロップ関連）

-# ツリーコントロールのソートを行うコールバック関数

@param

@retval int
*/
/*============================================================================*/
int CALLBACK CCustomTreeListCtrl::LeafCustomCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CCustomTreeListCtrl* ptree = (CCustomTreeListCtrl*)lParamSort;
	DWORD sort1 = ptree->SORTDATA((DWORD)lParam1);
	DWORD sort2 = ptree->SORTDATA((DWORD)lParam2);
	TRACE("DragDrop Compare %d:%d > %d:%d\n", lParam1, sort1, lParam2, sort2);

	return (sort1 > sort2);
}
