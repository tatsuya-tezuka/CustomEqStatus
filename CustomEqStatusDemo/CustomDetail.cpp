/******************************************************************************
* @file    CustomDetail.cpp
* @brief   【カスタマイズ機能】設備状況詳細画面クラス
* @auther  SCC
* @date    2024/08/26 新規作成
* @par
******************************************************************************/
#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomDetail.h"
#include "afxdialogex.h"
#include "CustomDetailConfig.h"

#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")


// CCustomDetail ダイアログ

IMPLEMENT_DYNAMIC(CCustomDetail, CCustomDialogBase)

CCustomDetail::CCustomDetail(CWnd* pParent /*=NULL*/, bool bRestore/* = false*/)
: CCustomDialogBase(CCustomDetail::IDD, pParent)
{
	// 復元フラグ
	mRestore = bRestore;
	
	// ツリーコントロールフォントの作成
	CFont temp;
	temp.CreateStockObject(DEFAULT_GUI_FONT);
	temp.GetLogFont(&mTreeLogFont);
	mTreeLogFont.lfHeight = -mTreeFontHeight;
	mTreeLogFont.lfWeight = FW_NORMAL;
	temp.DeleteObject();
}

CCustomDetail::~CCustomDetail()
{
}

void CCustomDetail::DoDataExchange(CDataExchange* pDX)
{
	CCustomDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CTRL, mTreeCtrl);
}


BEGIN_MESSAGE_MAP(CCustomDetail, CCustomDialogBase)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_CTRL, &CCustomDetail::OnNMRClickTreeCtrl)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_TREE_CTRL, &CCustomDetail::OnTvnGetdispinfoTreeCtrl)
	ON_WM_CLOSE()
	ON_WM_HSCROLL()
	//ON_NOTIFY(HDN_ITEMCHANGING, CCustomTreeListCtrl::eHeaderID, OnHeaderItemChanged)
	ON_NOTIFY(HDN_ITEMCHANGED, CCustomTreeListCtrl::eHeaderID, OnHeaderItemChanged)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, CCustomTreeListCtrl::eHeaderID, OnHeaderDividerdblclick)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_DETAIL_ADD, &CCustomDetail::OnDetailAdd)
	ON_COMMAND(ID_DETAIL_DELETE, &CCustomDetail::OnDetailDelete)
	ON_COMMAND(ID_DETAIL_RENAME, &CCustomDetail::OnDetailRename)
	ON_COMMAND(ID_DETAIL_MONCTRL, &CCustomDetail::OnDetailMonctrl)
	ON_COMMAND(ID_DETAIL_CONFIG, &CCustomDetail::OnDetailConfig)
END_MESSAGE_MAP()


// CCustomTreeListCtrl用のイベント処理
/*============================================================================*/
/*! 設備詳細

-# ツリーコントロールヘッダーのイベント処理（サイズ変更）
　※ツリーコントロールではイベントを拾えない

 @param

 @retval
 */
/*============================================================================*/
void CCustomDetail::OnHeaderItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	// カラム領域の再描画
	mTreeCtrl.UpdateColumns();

	// 再描画
	mTreeCtrl.Invalidate();
}
/*============================================================================*/
/*! 設備詳細

-# ヘッダーアイテム間のマウスダブルクリック
　※ツリーコントロールではイベントを拾えない

 @param

 @retval
 */
/*============================================================================*/
void CCustomDetail::OnHeaderDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	
	mTreeCtrl.DividerDblClick(phdr->iItem);

	*pResult = 0;
}

// CCustomDetail メッセージ ハンドラー

/*============================================================================*/
/*! 設備詳細

-# 初期化

@param

@retval
*/
/*============================================================================*/
BOOL CCustomDetail::OnInitDialog()
{
	CCustomDialogBase::OnInitDialog();

	// システムメニューに「リサイズフィット」を追加
	HMENU pSysMenu = ::GetSystemMenu(GetSafeHwnd(), FALSE);
	if (pSysMenu)
	{
		int count = ::GetMenuItemCount(pSysMenu);
		::InsertMenu(pSysMenu, 0, MF_BYPOSITION | MF_STRING, ID_DETAIL_RESIZEFIT, _T("ウィンドウサイズ最適化"));
		::InsertMenu(pSysMenu, 1, MF_BYPOSITION | MF_SEPARATOR, 0, _T(""));
	}

	createTreeControl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! 設備詳細

-# ツリーコントロールのマウス右クリックイベント（ポップアップメニュー表示）

@param

@retval
*/
/*============================================================================*/
void CCustomDetail::OnNMRClickTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	mMenuItem = NULL;

	CPoint pos;
	if (!::GetCursorPos(&pos))
	{
		ASSERT(FALSE);
		return;
	}

	mTreeCtrl.ScreenToClient(&pos);
	UINT col = 0;
	mMenuItem = mTreeCtrl.HitTestEx(pos, col);
	if (mMenuItem == NULL)
		return;

	mTreeCtrl.SelectItem(mMenuItem);

	CMenu menu;
	if (!menu.LoadMenu(IDR_POPUP_DETAIL))
	{
		ASSERT(FALSE);
		return;
	}

	CMenu* pMenu = menu.GetSubMenu(0);
	ASSERT(pMenu);

	CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(this, mMenuItem);
	// ここでツリーノード種別によってメニューの活性、非活性を行う
	switch (pnode->GetWindowInfo().type) {
	case	eTreeItemType_Title:
		pMenu->EnableMenuItem(ID_DETAIL_DELETE, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DETAIL_RENAME, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DETAIL_MONCTRL, MF_BYCOMMAND | MF_GRAYED);
		break;
	case	eTreeItemType_Main:
		pMenu->EnableMenuItem(ID_DETAIL_MONCTRL, MF_BYCOMMAND | MF_GRAYED);
		break;
	case	eTreeItemType_Sub:
		pMenu->EnableMenuItem(ID_DETAIL_MONCTRL, MF_BYCOMMAND | MF_GRAYED);
		break;
	case	eTreeItemType_Item:
		break;
	}

	mTreeCtrl.ClientToScreen(&pos);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);
}

/*============================================================================*/
/*! 設備詳細

-# ツールチップイベント

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnTvnGetdispinfoTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
}

/*============================================================================*/
/*! 設備詳細

-# 閉じるボタン押下イベント

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CCustomDialogBase::OnClose();
}

/*============================================================================*/
/*! 設備詳細

-# 水平スクロールイベント(CTreeListCtrlではキャッチできない)

@param

@retval
*/
/*============================================================================*/
void CCustomDetail::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// クライアント領域の取得
	CRect rcClient;
	GetClientRect(&rcClient);
	// クライアント領域幅の取得
	int cx = rcClient.Width();

	int xEndPos = mTreeCtrl.GetPosX();

	// スクロールバーのイベント処理
	// イベントによりスクロール量を調整する
	switch (nSBCode)
	{
	case SB_LINELEFT:
		mTreeCtrl.SetPosX(mTreeCtrl.GetPosX() - 15);
		break;
	case SB_LINERIGHT:
		mTreeCtrl.SetPosX(mTreeCtrl.GetPosX() + 15);
		break;
	case SB_PAGELEFT:
		mTreeCtrl.SetPosX(mTreeCtrl.GetPosX() - cx);
		break;
	case SB_PAGERIGHT:
		mTreeCtrl.SetPosX(mTreeCtrl.GetPosX() + cx);
		break;
	case SB_LEFT:
		mTreeCtrl.SetPosX(0);
		break;
	case SB_RIGHT:
		mTreeCtrl.SetPosX(mTreeCtrl.GetTotalCX() - cx);
		break;
	case SB_THUMBTRACK:
		mTreeCtrl.SetPosX(nPos);
		break;
	}

	if (mTreeCtrl.GetPosX() < 0)
		mTreeCtrl.SetPosX(0);
	else if (mTreeCtrl.GetPosX() > mTreeCtrl.GetTotalCX() - cx)
		mTreeCtrl.SetPosX(mTreeCtrl.GetTotalCX() - cx);

	if (xEndPos == mTreeCtrl.GetPosX())
		return;

	SetScrollPos(SB_HORZ, mTreeCtrl.GetPosX());
	// 各コントロールの再配置
	mTreeCtrl.RepositionControls();

	//CCustomDialogBase::OnHScroll(nSBCode, nPos, pScrollBar);
}
/*============================================================================*/
/*! 設備詳細

-# 個別リサイズのオーバーライド関数

@param		pWnd	対象コントロールハンドル
@param		dx		X増分値
@param		dy		Y増分値
@retval

*/
/*============================================================================*/
BOOL CCustomDetail::OnProcSize(CWnd* pWnd, int dx, int dy)
{
	if (mTreeCtrl.m_hWnd != pWnd->m_hWnd)
		return false;

	mTreeCtrl.ResizeControl(dx, dy);

	return TRUE;
}
/*============================================================================*/
/*! 設備詳細

-# リサイズフィットイベント

@param
@retval

*/
/*============================================================================*/
void CCustomDetail::OnSysCommand(UINT nID, LPARAM lParam)
{
	switch (nID){
	case	ID_DETAIL_RESIZEFIT:
		resizeFit();
		return;
	}

	CCustomDialogBase::OnSysCommand(nID, lParam);
}

//#############################################################################

/*============================================================================*/
/*! 設備詳細

-# ツリーリストコントロールの作成

@param

@retval
*/
/*============================================================================*/
void CCustomDetail::createTreeControl()
{
	// ツリーコントロールにデフォルトフォントを設定する
	mTreeCtrl.SetFontEx(eTreeItemType_Window, mTreeLogFont);
	mTreeCtrl.Create(this);

	// アイテムインデントの設定
	mTreeCtrl.SetIndent(7);

	// コールバック関数の登録
	mTreeCtrl.SetClickCallback(messageClick);
	//mTreeCtrl.SetDragCallback(messageDrag);

	if (mRestore == true){
		restoreRoot();
	}
	else{
		createRoot();
	}

	mTreeCtrl.UpdateColumns();

	SetControlInfo(IDC_TREE_CTRL, ANCHORE_LEFTTOP | RESIZE_BOTH);
}
/*============================================================================*/
/*! 設備詳細

-# コールバック関数

@param		hItem		ツリーアイテム
@param		nSubItem	ツリーサブアイテム
@param		point		マウスカーソル位置
@retval

*/
/*============================================================================*/
BOOL CALLBACK CCustomDetail::messageClick(CWnd* pwnd, HTREEITEM hItem, UINT nSubItem, CPoint point)
{
#ifdef _DEMO
	return FALSE;
#else
	//CCustomDetail* p = CCustomDetail::Instance();
	CCustomDetail* p = (CCustomDetail*)pwnd;

	UINT mask = 1 << CTreeListCtrl::eItem | 1 << CTreeListCtrl::eUnit;
	if ((1 << nSubItem) & mask)
		return TRUE;

	// 制御セルが押下されたかチェック
	bool bControl = p->mTreeCtrl.IsControl(point);
	if (bControl == false){
		return FALSE;
	}

	CString strText = p->mTreeCtrl.GetSubItemText(hItem, CTreeListCtrl::eControl);
	// 制御文字列の場合は制御コマンド実行
	if (strText.IsEmpty() == false && strText == CString(mCOntrolSignString)){
		// 制御コマンドを送信
		CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(p, hItem);
		CString strCntl = pnode->GetMonCtrl().cname;
		p->MessageBox(_T("（仮）制御コマンドを送信します\n") + strCntl);
	}
	return TRUE;
#endif
}
/*============================================================================*/
/*! 設備詳細

-# コールバック関数

@param		status		種別
@param		hItem		ツリーアイテム
@param		lParam1		パラメタ１
@param		lParam2		パラメタ２
@param		lParam3		パラメタ３
@retval

*/
/*============================================================================*/
BOOL CALLBACK CCustomDetail::messageDrag(CWnd* pwnd, UINT status, HTREEITEM hItem, LPARAM lParam1, LPARAM lParam2, LPARAM lParam3)
{
#ifdef _DEMO
	return FALSE;
#else
	//CCustomDetail* p = CCustomDetail::Instance();
	CCustomDetail* p = (CCustomDetail*)pwnd;

	CTreeNode* pnode;
	CTreeNode* pnodeDrop;
	BOOL bDropExecute = FALSE;

	switch (status){
	case	CTreeListCtrl::eEnable:
		pnode = theApp.GetDataManager().SearchItemNode(p, hItem);
		//if (pnode != NULL && pnode->getWindowInfo().type != eTreeItemType_Title)
		//	return TRUE;
		if (pnode != NULL && pnode->GetWindowInfo().type == eTreeItemType_Item)
			return TRUE;
		break;
	case	CTreeListCtrl::eSelect:
		// ・ドラッグアイテム種別がeTreeItemType_Mainの場合はeTreeItemType_Titleにのみドロップ可能
		// ・ドラッグアイテム種別がeTreeItemType_Subの場合はeTreeItemType_Mainにのみドロップ可能
		// ・ドラッグアイテム種別がeTreeItemType_Itemの場合はeTreeItemType_Subにのみドロップ可能
		pnode = theApp.GetDataManager().SearchItemNode(p, hItem);
		pnodeDrop = theApp.GetDataManager().SearchItemNode(p, (HTREEITEM)lParam1);
		if (pnode == NULL || pnodeDrop == NULL)
			return FALSE;
		if (pnode->GetWindowInfo().type == eTreeItemType_Main && pnodeDrop->GetWindowInfo().type == eTreeItemType_Title)
			bDropExecute = TRUE;
		else if (pnode->GetWindowInfo().type == eTreeItemType_Sub && pnodeDrop->GetWindowInfo().type == eTreeItemType_Main)
			bDropExecute = TRUE;
		else if (pnode->GetWindowInfo().type == eTreeItemType_Item && pnodeDrop->GetWindowInfo().type == eTreeItemType_Sub)
			bDropExecute = TRUE;

		return bDropExecute;
		break;
	case	CTreeListCtrl::eDrop:
		// 削除するノードの親ノードを取得（DropMoveItemを呼び出す前に取得）
		pnode = theApp.GetDataManager().SearchItemNode(p, p->mTreeCtrl.GetParentItem(hItem));
		// ドラッグノードをドロップする
		p->DropMoveItem(hItem, (HTREEITEM)lParam1);
		// ドラッグノードの親の子リストから削除する
		pnode->DeleteTreeNode(hItem);
		break;
	}
	return FALSE;
#endif
}
/*============================================================================*/
/*! 設備詳細

-# ルートの作成

@param

@retval void
*/
/*============================================================================*/
void CCustomDetail::createRoot()
{
	SetWindowText(mDefaultCustomTitle);
	// ルートアイテムの設定
	HTREEITEM rootItem = mTreeCtrl.InsertItem(mDefaultCustomTitle, NULL, NULL, TVI_ROOT);
	mTreeCtrl.SetItemData(rootItem, (LPARAM)rootItem);
	CTreeNode* tn_root = new CTreeNode(rootItem, this, &mTreeCtrl);
	setNodeWindowInfo(tn_root, eTreeItemType_Title, (LPWSTR)mDefaultCustomTitle, NULL);
	// デフォルトフォントの設定
	mTreeCtrl.SetFontEx(tn_root->GetWindowInfo().type, mTreeLogFont);
	// 論理フォントの取得
	mTreeCtrl.GetFontEx(eTreeItemType_Window).GetLogFont(&tn_root->GetColor().font);

	theApp.GetDataManager().AddTreeNode(tn_root);
}
/*============================================================================*/
/*! 設備詳細

-# メインノードの作成

@param		parentitem		親ツリーアイテム
@param		parentnode		親ノード

@retval void
*/
/*============================================================================*/
void CCustomDetail::createMainNode(HTREEITEM parentitem, CTreeNode* parentnode)
{
	CString str;
	str.Format(_T("%s"), mDefaultCustomMainText);
	HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, parentitem);
	mTreeCtrl.SetItemData(item, (LPARAM)item);
	CTreeNode* item_node = parentnode->CreateTreeNode(parentitem, item);
	CTreeNode* pcopyitem = theApp.GetDataManager().SearchItemNodeType(this, eTreeItemType_Main);
	if (pcopyitem != NULL)
		item_node->CopyItem(pcopyitem, true);
	setNodeWindowInfo(item_node, eTreeItemType_Main, (LPWSTR)mDefaultCustomMainText, parentnode);
	// デフォルトフォントの設定
	mTreeCtrl.SetFontEx(item_node->GetWindowInfo().type, mTreeLogFont);
	// 論理フォントの取得
	mTreeCtrl.GetFontEx(eTreeItemType_Window).GetLogFont(&item_node->GetColor().font);

	swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), mDefaultCustomMainText);
}
/*============================================================================*/
/*! 設備詳細

-# サブノードの作成

@param		parentitem		親ツリーアイテム
@param		parentnode		親ノード

@retval void
*/
/*============================================================================*/
void CCustomDetail::createSubNode(HTREEITEM parentitem, CTreeNode* parentnode)
{
	CString str;
	str.Format(_T("%s"), mDefaultCustomSubText);
	HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, parentitem);
	mTreeCtrl.SetItemData(item, (LPARAM)item);
	CTreeNode* item_node = parentnode->CreateTreeNode(parentitem, item);
	CTreeNode* pcopyitem = theApp.GetDataManager().SearchItemNodeType(this, eTreeItemType_Sub);
	if (pcopyitem != NULL)
		item_node->CopyItem(pcopyitem, true);
	setNodeWindowInfo(item_node, eTreeItemType_Sub, (LPWSTR)mDefaultCustomSubText, parentnode);
	// デフォルトフォントの設定
	mTreeCtrl.SetFontEx(item_node->GetWindowInfo().type, mTreeLogFont);
	// 論理フォントの取得
	mTreeCtrl.GetFontEx(eTreeItemType_Window).GetLogFont(&item_node->GetColor().font);

	swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), mDefaultCustomSubText);
}
/*============================================================================*/
/*! 設備詳細

-# リーフの作成

@param		parentitem		親ツリーアイテム
@param		parentnode		親ノード

@retval void
*/
/*============================================================================*/
void CCustomDetail::createLeaf(HTREEITEM parentitem, CTreeNode* parentnode)
{
	CString str;
	str = createLeafText(mDefaultCustomItemText, _T(""), _T(""));
	HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, parentitem);
	mTreeCtrl.SetItemData(item, (LPARAM)item);
	CTreeNode* item_node = parentnode->CreateTreeNode(parentitem, item);
	CTreeNode* pcopyitem = theApp.GetDataManager().SearchItemNodeType(this, eTreeItemType_Item);
	if (pcopyitem != NULL)
		item_node->CopyItem(pcopyitem, true);
	setNodeWindowInfo(item_node, eTreeItemType_Item, (LPWSTR)mDefaultCustomItemText, parentnode);
	// デフォルトフォントの設定
	mTreeCtrl.SetFontEx(item_node->GetWindowInfo().type, mTreeLogFont);
	// 論理フォントの取得
	mTreeCtrl.GetFontEx(eTreeItemType_Window).GetLogFont(&item_node->GetColor().font);

	swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), mDefaultCustomItemText);
}
/*============================================================================*/
/*! 設備詳細

-# ノード、リーフの復元

@param

@retval void
*/
/*============================================================================*/
void CCustomDetail::restoreRoot()
{
	CTreeNode* pnode = theApp.GetDataManager().SearchWndNode(this);

#if 0//(_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
	/*
	// Window attributes
	enum DWMWINDOWATTRIBUTE
	{
	DWMWA_NCRENDERING_ENABLED = 1,              // [get] Is non-client rendering enabled/disabled
	DWMWA_NCRENDERING_POLICY,                   // [set] DWMNCRENDERINGPOLICY - Non-client rendering policy
	DWMWA_TRANSITIONS_FORCEDISABLED,            // [set] Potentially enable/forcibly disable transitions
	DWMWA_ALLOW_NCPAINT,                        // [set] Allow contents rendered in the non-client area to be visible on the DWM-drawn frame.
	DWMWA_CAPTION_BUTTON_BOUNDS,                // [get] Bounds of the caption button area in window-relative space.
	DWMWA_NONCLIENT_RTL_LAYOUT,                 // [set] Is non-client content RTL mirrored
	DWMWA_FORCE_ICONIC_REPRESENTATION,          // [set] Force this window to display iconic thumbnails.
	DWMWA_FLIP3D_POLICY,                        // [set] Designates how Flip3D will treat the window.
	DWMWA_EXTENDED_FRAME_BOUNDS,                // [get] Gets the extended frame bounds rectangle in screen space
	DWMWA_HAS_ICONIC_BITMAP,                    // [set] Indicates an available bitmap when there is no better thumbnail representation.
	DWMWA_DISALLOW_PEEK,                        // [set] Don't invoke Peek on the window.
	DWMWA_EXCLUDED_FROM_PEEK,                   // [set] LivePreview exclusion information
	DWMWA_CLOAK,                                // [set] Cloak or uncloak the window
	DWMWA_CLOAKED,                              // [get] Gets the cloaked state of the window
	DWMWA_FREEZE_REPRESENTATION,                // [set] BOOL, Force this window to freeze the thumbnail without live update
	DWMWA_PASSIVE_UPDATE_MODE,                  // [set] BOOL, Updates the window only when desktop composition runs for other reasons
	DWMWA_USE_HOSTBACKDROPBRUSH,                // [set] BOOL, Allows the use of host backdrop brushes for the window.
	DWMWA_USE_IMMERSIVE_DARK_MODE = 20,         // [set] BOOL, Allows a window to either use the accent color, or dark, according to the user Color Mode preferences.
	DWMWA_WINDOW_CORNER_PREFERENCE = 33,        // [set] WINDOW_CORNER_PREFERENCE, Controls the policy that rounds top-level window corners
	DWMWA_BORDER_COLOR,                         // [set] COLORREF, The color of the thin border around a top-level window
	DWMWA_CAPTION_COLOR,                        // [set] COLORREF, The color of the caption
	DWMWA_TEXT_COLOR,                           // [set] COLORREF, The color of the caption text
	DWMWA_VISIBLE_FRAME_BORDER_THICKNESS,       // [get] UINT, width of the visible border around a thick frame window
	DWMWA_LAST
	};
	*/
	if (pnode->GetWindowInfo().kind == eTreeItemKind_Master){
		int windowColor = RGB(0, 0, 0);
		int textColor = RGB(255, 255, 255);
		//HWND window = GetActiveWindow();
		LONG exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		exStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, exStyle);
		DwmSetWindowAttribute(m_hWnd, 34/*DWMWA_BORDER_COLOR*/, &windowColor, sizeof(windowColor));
		DwmSetWindowAttribute(m_hWnd, 35/*DWMWA_CAPTION_COLOR*/, &windowColor, sizeof(windowColor));
		DwmSetWindowAttribute(m_hWnd, 36/*DWMWA_TEXT_COLOR*/, &textColor, sizeof(windowColor));
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}
#endif

	// ルート復元
	HTREEITEM rootItem = mTreeCtrl.InsertItem(pnode->GetWindowInfo().title, NULL, NULL, TVI_ROOT);
	mTreeCtrl.SetItemData(rootItem, (LPARAM)rootItem);
	pnode->SetTreeItem(rootItem);
	if (abs(mTreeLogFont.lfHeight) < abs(pnode->GetColor().font.lfHeight))
		mTreeLogFont.lfHeight = pnode->GetColor().font.lfHeight;

	setNodeWindowInfo(pnode, eTreeItemType_Title, pnode->GetWindowInfo().title, NULL);
	// フォントの設定
	mTreeCtrl.SetFontEx(eTreeItemType_Title, pnode->GetColor().font);

	// ツリーコントロールの背景色設定
	mTreeCtrl.SetBkColor(pnode->GetColor().back);

	restoreNode(pnode, rootItem);

	// ツリーコントロールのフォント設定
	mTreeCtrl.SetFontEx(eTreeItemType_Window, mTreeLogFont);

	if (pnode->GetWindowInfo().treeopen != 0){
		mTreeCtrl.Expand(rootItem, TVE_EXPAND);
	}

	// ウィンドウ状態の復元
	SetWindowPlacement(&(pnode->GetWindowInfo().placement));
	ShowWindow(pnode->GetWindowInfo().placement.showCmd);

	// ヘッダー幅再設定
	mTreeCtrl.SetHeaderWidth(pnode->GetWindowInfo().hwidth, mHeaderSize);
	mTreeCtrl.UpdateColumns();
}

/*============================================================================*/
/*! 設備詳細

-# ノード、リーフの復元

@param

@retval void
*/
/*============================================================================*/
void CCustomDetail::restoreNode(CTreeNode* pnode, HTREEITEM ptree)
{
	vector<CTreeNode*>::iterator itr;
	for (itr = pnode->GetChildren().begin(); itr != pnode->GetChildren().end(); itr++){
		CString str;
		if ((*itr)->GetWindowInfo().type == eTreeItemType_Item){
			str = createLeafText((*itr)->GetMonCtrl().display, (*itr)->GetMonCtrl().unit, (*itr)->GetMonCtrl().cname);
		}
		else{
			str.Format(_T("%s"), (*itr)->GetMonCtrl().display);
		}
		HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, ptree);
		if (abs(mTreeLogFont.lfHeight) < abs(pnode->GetColor().font.lfHeight))
			mTreeLogFont.lfHeight = pnode->GetColor().font.lfHeight;
		// フォントの設定
		mTreeCtrl.SetFontEx((*itr)->GetWindowInfo().type, (*itr)->GetColor().font);
		mTreeCtrl.SetItemData(item, (LPARAM)item);
		(*itr)->SetTreeItem(item);
		(*itr)->GetWindowInfo().tree = &mTreeCtrl;
		(*itr)->GetWindowInfo().wnd = this;
		restoreNode((*itr), item);

		if ((*itr)->GetWindowInfo().treeopen != 0){
			mTreeCtrl.Expand(item, TVE_EXPAND);
		}
	}
}
/*============================================================================*/
/*! 設備詳細

-# ノードパラメタの設定

@param		pnode		設定対象ノード
@param		type		ノード種別
@param		text		ノード文字列
@param		parent		親ノード

@retval void
*/
/*============================================================================*/
void CCustomDetail::setNodeWindowInfo(CTreeNode* pnode, UINT type, TCHAR* text, CTreeNode* parent)
{
	pnode->SetParentNode(parent);
	pnode->GetWindowInfo().wnd = this;
	pnode->GetWindowInfo().tree = &mTreeCtrl;
	pnode->GetWindowInfo().type = type;
	swprintf_s(pnode->GetWindowInfo().title, mTitleSize, _T("%s"), text);
	if (type == eTreeItemType_Title){
		SetWindowText(text);
	}
}

/*============================================================================*/
/*! 設備詳細

-# タイトル設定

@param

@retval
*/
/*============================================================================*/
void CCustomDetail::setTreeTitle(LPARAM lParam)
{
	CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(this, (HTREEITEM)lParam);

	if (pnode->GetWindowInfo().type == eTreeItemType_Title){
		swprintf_s(pnode->GetWindowInfo().title, mNameSize, _T("%s"), pnode->GetMonCtrl().display);
		SetWindowText(pnode->GetMonCtrl().display);
		if (pnode->GetWindowInfo().manager->GetSafeHwnd())
			pnode->GetWindowInfo().manager->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)this);
	}
}
/*============================================================================*/
/*! 設備詳細

-# 登録されている項目数でウィンドウを最適化する

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::resizeFit()
{
	UINT cxTotal, cyTotal;

	int CYCAPTION = GetSystemMetrics(SM_CYCAPTION);
	int CXSIZEFRAME = GetSystemMetrics(SM_CXSIZEFRAME);
	int CYSIZEFRAME = GetSystemMetrics(SM_CYSIZEFRAME);
	int CXEDGE = GetSystemMetrics(SM_CXEDGE);
	int CYEDGE = GetSystemMetrics(SM_CYEDGE);

	// ◆水平方向のサイズ変更
	// ヘッダーコントロールのカラム幅の取得
	cxTotal = (CYSIZEFRAME * 4);
	cxTotal += mTreeCtrl.GetHeaderWidth();
	//cxTotal += (nCnt * CYEDGE);
	cxTotal += (CYEDGE * 2);

	// ◆垂直方向のサイズ変更
	// アイテム高さの取得
	int nHeaderHeight = mTreeCtrl.GetHeaderHeight();

	HTREEITEM	hItem = NULL;

	mTreeCtrl.SelectSetFirstVisible(mTreeCtrl.GetRootItem());

	// 展開（表示）されているアイテム数を取得する
	hItem = mTreeCtrl.GetFirstVisibleItem();
	UINT itemCount = 0;
	while (hItem){
		itemCount++;
		hItem = mTreeCtrl.GetNextVisibleItem(hItem);
	}

	cyTotal = CYCAPTION + (CXSIZEFRAME * 4) + nHeaderHeight + (mTreeCtrl.GetItemHeight() * itemCount) + (CXEDGE * 2);

	// ◆ウィンドウサイズ変更
	WINDOWPLACEMENT	wPlacement;
	memset(&wPlacement, 0, sizeof(WINDOWPLACEMENT));
	wPlacement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wPlacement);

	// ウィンドウの位置情報更新
	CRect r = CRect(wPlacement.rcNormalPosition);
	wPlacement.rcNormalPosition.right = wPlacement.rcNormalPosition.left + cxTotal;
	wPlacement.rcNormalPosition.bottom = wPlacement.rcNormalPosition.top + cyTotal;

	SetWindowPlacement(&wPlacement);
}

/*============================================================================*/
/*! 設備詳細

-# ノード、リーフの登録

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnDetailAdd()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}

/*============================================================================*/
/*! 設備詳細

-# ノード、リーフの削除

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnDetailDelete()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}

/*============================================================================*/
/*! 設備詳細

-# 選択項目名の変更

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnDetailRename()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}

/*============================================================================*/
/*! 設備詳細

-# 監視制御一覧画面の表示

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnDetailMonctrl()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
}

/*============================================================================*/
/*! 設備詳細

-# カスタム画面の設定

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnDetailConfig()
{
	if (mMenuItem == NULL)
		return;

	CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(this, mMenuItem);

	CCustomDetailConfig config(this, pnode->GetWindowInfo().type);

	// 設定されている色情報を設定ダイアログに取得する
	for (int i = 0; i < sizeof(mColorConfig) / sizeof(mColorConfig[0]); i++) {

		stColorData color;
		if (theApp.GetDataManager().GetNodeColor(this, mColorConfig[i].type, color) == true) {
			switch (mColorConfig[i].type) {
			case	eTreeItemType_Window:
				config.mColor[eColorType_Window] = color.back;
				break;
			case	eTreeItemType_Title:
				config.mColor[eColorType_TitleBack] = color.textback;
				config.mColor[eColorType_TitleText] = color.text;
				config.mFont[eTreeItemType_Title] = color.font;
				config.mFont[eTreeItemType_Title] = color.font;
				break;
			case	eTreeItemType_Main:
				config.mColor[eColorType_MainBack] = color.textback;
				config.mColor[eColorType_MainText] = color.text;
				config.mFont[eTreeItemType_Main] = color.font;
				config.mFont[eTreeItemType_Main] = color.font;
				break;
			case	eTreeItemType_Sub:
				config.mColor[eColorType_SubBack] = color.textback;
				config.mColor[eColorType_SubText] = color.text;
				config.mFont[eTreeItemType_Sub] = color.font;
				config.mFont[eTreeItemType_Sub] = color.font;
				break;
			case	eTreeItemType_Item:
				config.mColor[eColorType_ItemBack] = color.textback;
				config.mColor[eColorType_ItemText] = color.text;
				config.mColor[eColorType_ValueText] = color.value;
				config.mColor[eColorType_UnitText] = color.unit;
				config.mFont[eTreeItemType_Item] = color.font;
				config.mFont[eTreeItemType_Item] = color.font;
				break;
			}
		}
	}

	// 設定画面表示
	if (config.DoModal() != IDOK) {
		return;
	}

	// 設定されたフォントから最大高さを求める
	mTreeLogFont.lfHeight = 0;
	for (int i = 0; i < sizeof(mColorConfig) / sizeof(mColorConfig[0]); i++) {
		switch (mColorConfig[i].type) {
		//case	eTreeItemType_Window:
		case	eTreeItemType_Title:
			if (abs(mTreeLogFont.lfHeight) < abs(config.mFont[eTreeItemType_Title].lfHeight)) {
				mTreeLogFont.lfHeight = config.mFont[eTreeItemType_Title].lfHeight;
			}
			break;
		case	eTreeItemType_Main:
			if (abs(mTreeLogFont.lfHeight) < abs(config.mFont[eTreeItemType_Main].lfHeight)) {
				mTreeLogFont.lfHeight = config.mFont[eTreeItemType_Main].lfHeight;
			}
			break;
		case	eTreeItemType_Sub:
			if (abs(mTreeLogFont.lfHeight) < abs(config.mFont[eTreeItemType_Sub].lfHeight)) {
				mTreeLogFont.lfHeight = config.mFont[eTreeItemType_Sub].lfHeight;
			}
			break;
		case	eTreeItemType_Item:
			if (abs(mTreeLogFont.lfHeight) < abs(config.mFont[eTreeItemType_Item].lfHeight)) {
				mTreeLogFont.lfHeight = config.mFont[eTreeItemType_Item].lfHeight;
			}
			break;
		}
	}
	// ツリーコントロールの背景色設定
	mTreeCtrl.SetFontEx(eTreeItemType_Window, mTreeLogFont);

	// 設定ダイアログで設定された色情報を保存する
	for (int i = 0; i < sizeof(mColorConfig) / sizeof(mColorConfig[0]); i++) {
		stColorData color;
		if (theApp.GetDataManager().GetNodeColor(this, mColorConfig[i].type, color) == true) {
			switch (mColorConfig[i].type) {
			case	eTreeItemType_Window:
				color.back = config.mColor[eColorType_Window];
				mTreeCtrl.SetBkColor(color.back);
				break;
			case	eTreeItemType_Title:
				color.textback = config.mColor[eColorType_TitleBack];
				color.text = config.mColor[eColorType_TitleText];
				color.font = config.mFont[eTreeItemType_Title];
				break;
			case	eTreeItemType_Main:
				color.textback = config.mColor[eColorType_MainBack];
				color.text = config.mColor[eColorType_MainText];
				color.font = config.mFont[eTreeItemType_Main];
				break;
			case	eTreeItemType_Sub:
				color.textback = config.mColor[eColorType_SubBack];
				color.text = config.mColor[eColorType_SubText];
				color.font = config.mFont[eTreeItemType_Sub];
				break;
			case	eTreeItemType_Item:
				color.textback = config.mColor[eColorType_ItemBack];
				color.text = config.mColor[eColorType_ItemText];
				color.value = config.mColor[eColorType_ValueText];
				color.unit = config.mColor[eColorType_UnitText];
				color.font = config.mFont[eTreeItemType_Item];
				break;
			}
			if (mColorConfig[i].type != eTreeItemType_Window) {
				theApp.GetDataManager().SetNodeColor(this, mColorConfig[i].type, color);
				mTreeCtrl.SetFontEx(mColorConfig[i].type, color.font);
			}
		}
	}

	mTreeCtrl.Invalidate();

	pnode = theApp.GetDataManager().SearchWndNode(this);
	if (pnode->GetWindowInfo().manager->GetSafeHwnd()) {
		pnode->GetWindowInfo().manager->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)this);
	}
}
