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
#include "CustomSelectSaveFile.h"

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
	//mBackupNode = NULL;
}

CCustomDetail::~CCustomDetail()
{
	//if(mBackupNode != NULL)
	//	delete mBackupNode;
}

void CCustomDetail::DoDataExchange(CDataExchange* pDX)
{
	CCustomDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CTRL, mTreeCtrl);
}


BEGIN_MESSAGE_MAP(CCustomDetail, CCustomDialogBase)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_CTRL, &CCustomDetail::OnNMRClickTreeCtrl)
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
	ON_NOTIFY(TVN_GETINFOTIP, IDC_TREE_CTRL, &CCustomDetail::OnTvnGetInfoTipTreeCtrl)
	ON_COMMAND(ID_MENUDETAIL_CLOSE, &CCustomDetail::OnMenudetailClose)
	ON_COMMAND(ID_MENUDETAIL_EDIT, &CCustomDetail::OnMenudetailEdit)
	ON_COMMAND(ID_MENUDETAIL_MONITOR, &CCustomDetail::OnMenudetailMonitor)
	ON_COMMAND(ID_MENUDETAIL_SAVE, &CCustomDetail::OnMenudetailSave)
	ON_COMMAND(ID_MENUDETAIL_SAVEAS, &CCustomDetail::OnMenudetailSaveas)
	ON_WM_INITMENUPOPUP()
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
#if _DEMO_PHASE < 50
	* pResult = 1;
	return;
#endif

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

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

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

#if _DEMO_PHASE < 50
	return;
#endif

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

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mMenuItem);
	// ここでツリーノード種別によってメニューの活性、非活性を行う
	if (pnode != NULL) {
		switch (pnode->GetWindowInfo().type) {
		case	eTreeItemType_Title:
			pMenu->EnableMenuItem(ID_DETAIL_DELETE, MF_BYCOMMAND | MF_GRAYED);
			pMenu->EnableMenuItem(ID_DETAIL_MONCTRL, MF_BYCOMMAND | MF_GRAYED);
			break;
		case	eTreeItemType_Main:
			pMenu->EnableMenuItem(ID_DETAIL_MONCTRL, MF_BYCOMMAND | MF_GRAYED);
			break;
		case	eTreeItemType_Sub:
			pMenu->EnableMenuItem(ID_DETAIL_MONCTRL, MF_BYCOMMAND | MF_GRAYED);
			break;
		case	eTreeItemType_Item:
			pMenu->EnableMenuItem(ID_DETAIL_ADD, MF_BYCOMMAND | MF_GRAYED);
			break;
		}

		pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
		if (pnode->GetWindowInfo().mode == eTreeItemMode_Monitor) {
			pMenu->EnableMenuItem(ID_DETAIL_ADD, MF_BYCOMMAND | MF_GRAYED);
			pMenu->EnableMenuItem(ID_DETAIL_DELETE, MF_BYCOMMAND | MF_GRAYED);
			pMenu->EnableMenuItem(ID_DETAIL_RENAME, MF_BYCOMMAND | MF_GRAYED);
			pMenu->EnableMenuItem(ID_DETAIL_MONCTRL, MF_BYCOMMAND | MF_GRAYED);
		}
	}
	else {
		pMenu->EnableMenuItem(ID_DETAIL_DELETE, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DETAIL_RENAME, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DETAIL_MONCTRL, MF_BYCOMMAND | MF_GRAYED);
	}

	mTreeCtrl.ClientToScreen(&pos);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);
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
	OnMenudetailClose();
}

/*============================================================================*/
/*! 設備詳細

-# メニュー閉じるボタン

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnMenudetailClose()
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

	//bool ret = mBackupNode->Equal(pnode);
	bool ret = theApp.GetCustomControl().GetDataManager().CompareEditNode(this);
	if (ret == true) {
		TRACE("Same Data\b");
	}
	else {
		TRACE("Different Data\n");
		int retmsg = CustomSaveDifferentMessageBoxHooked(m_hWnd, mMessage_DetailSaveDifferentData, pnode->GetWindowInfo().title, MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1, CString(pnode->GetXmlFileName()).IsEmpty() ? false : true);

		if (retmsg == IDCANCEL)
			return;

		if (retmsg == IDYES) {
			// 変更内容を保存する
			OnMenudetailSave();
		}
	}

	// 編集用ノードの削除
	theApp.GetCustomControl().GetDataManager().DeleteEditNode(this);

	CCustomDialogBase::OnClose();

	theApp.GetCustomControl().GetCustomManager().SendMessage(eUserMessage_Manager_Delete, CString(pnode->GetXmlFileName()).IsEmpty() == true ? 1 : 0, (LPARAM)this);
}

/*============================================================================*/
/*! 設備詳細

-# メニュー保存イベント

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnMenudetailSave()
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

	if (CString(pnode->GetXmlFileName()).IsEmpty() == true) {
		return;
	}

	// ツリーデータの保存
	saveHeaderWidth();

	// 編集用ノード情報の保存
	theApp.GetCustomControl().GetDataManager().RestoreEditNode(this);

	theApp.GetCustomControl().GetDataManager().SaveEquipmentData((UINT)eLayoutFileType_XML, pnode->GetXmlFileName(), this);

	// カスタム管理画面へ通知してタイトルを更新する
	if (pnode->GetWindowInfo().manager->GetSafeHwnd())
		pnode->GetWindowInfo().manager->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)this);
}

/*============================================================================*/
/*! 設備詳細

-# メニュー名前を付けて保存イベント

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnMenudetailSaveas()
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

	CCustomSelectSaveFile dlg;
	dlg.SetSavePathName(theApp.GetCustomControl().GetUserDataPath());
	dlg.SetSaveFileName(CString(pnode->GetXmlFileName()).Mid(theApp.GetCustomControl().GetUserDataPath().GetLength()+1));
	if (dlg.DoModal() == IDCANCEL)
		return;

	CString filename = dlg.GetSavePathName() + _T("\\") + dlg.GetSaveFileName();
	CString backxml = CString(pnode->GetXmlFileName());

	// ツリーデータの保存
	saveHeaderWidth();

	// 編集用ノード情報の保存
	swprintf_s(pnode->GetXmlFileName(), _MAX_PATH, _T("%s"), (LPCTSTR)filename);

	// ①編集用ノードを名前を付けて保存する
	theApp.GetCustomControl().GetDataManager().SaveasEditNode(this, (LPCTSTR)filename);

	// ②CCustomManagerの表示更新
	if (theApp.GetCustomControl().GetCustomManager().GetSafeHwnd() != NULL) {
		theApp.GetCustomControl().GetCustomManager().PostMessage(eUserMessage_Manager_Reset, 0, 1);
	}
}

/*============================================================================*/
/*! 設備詳細

-# ヘッダーサイズの保存

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::saveHeaderWidth()
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

	mTreeCtrl.GetHeaderWidth(pnode->GetWindowInfo().hwidth, mHeaderSize);
}

/*============================================================================*/
/*! 設備詳細

-# メニュー編集ー編集イベント

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnMenudetailEdit()
{
#if _DEMO_PHASE < 50
	return;
#endif
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	if (pnode != NULL) {
		pnode->GetWindowInfo().mode = eTreeItemMode_Edit;
		updateMode();
	}
}

/*============================================================================*/
/*! 設備詳細

-# メニュー編集ー監視イベント

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnMenudetailMonitor()
{
#if _DEMO_PHASE < 50
	return;
#endif
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	if (pnode != NULL) {
		pnode->GetWindowInfo().mode = eTreeItemMode_Monitor;
		updateMode();
	}
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
	//mTreeCtrl.SetIndent(5);
	mTreeCtrl.SetIndent(7);

	mTreeCtrl.ModifyStyle(TVS_EDITLABELS, 0);

#if _DEMO_PHASE < 50
	createRoot();
#else
	if (mRestore == true) {
		// 編集用ノードを使用するためにノードリストをバックする
		theApp.GetCustomControl().GetDataManager().BackupEditNode(this);
		restoreRoot();
	}
	else {
		createRoot();
	}
#endif

	mTreeCtrl.UpdateColumns();

	UpdateSortNo(mTreeCtrl.GetRootItem());

	SetControlInfo(IDC_TREE_CTRL, ANCHORE_LEFTTOP | RESIZE_BOTH);
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
	mTreeCtrl.SetItemData(rootItem, mTreeCtrl.MAKEDATA(CCustomDropObject::DK_TITLE, 0));

	CTreeNode* newnode = new CTreeNode(rootItem, this, &mTreeCtrl);
	setNodeWindowInfo(newnode, eTreeItemType_Title, (LPWSTR)mDefaultCustomTitle, NULL);
	// デフォルトフォントの設定
	mTreeCtrl.SetFontEx(newnode->GetWindowInfo().type, mTreeLogFont);
	// 論理フォントの取得
	mTreeCtrl.GetFontEx(eTreeItemType_Window).GetLogFont(&newnode->GetColor().font);

	newnode->GetWindowInfo().mode = eTreeItemMode_Edit;

	theApp.GetCustomControl().GetDataManager().AddTreeNode(newnode);

	// 編集用ノードを使用するためにノードリストをバックする
	theApp.GetCustomControl().GetDataManager().BackupEditNode(this);
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	pnode->SetTreeItem(rootItem);

	updateMode();
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
	mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(CCustomDropObject::DK_MAINNODE, 0));

	CTreeNode* item_node = parentnode->CreateTreeNode(parentitem, item);
	CTreeNode* pcopyitem = theApp.GetCustomControl().GetDataManager().SearchItemNodeType(this, eTreeItemType_Main);
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
	mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(CCustomDropObject::DK_SUBNODE, 0));

	CTreeNode* item_node = parentnode->CreateTreeNode(parentitem, item);
	CTreeNode* pcopyitem = theApp.GetCustomControl().GetDataManager().SearchItemNodeType(this, eTreeItemType_Sub);
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
	mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(CCustomDropObject::DK_LEAF, 0));

	CTreeNode* item_node = parentnode->CreateTreeNode(parentitem, item);
	CTreeNode* pcopyitem = theApp.GetCustomControl().GetDataManager().SearchItemNodeType(this, eTreeItemType_Item);
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
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

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
	mTreeCtrl.SetItemData(rootItem, mTreeCtrl.MAKEDATA(CCustomDropObject::DK_TITLE, 0));

	pnode->SetTreeItem(rootItem);
	mTreeLogFont = pnode->GetColor().font;

	setNodeWindowInfo(pnode, eTreeItemType_Title, pnode->GetWindowInfo().title, NULL);
	// フォントの設定
	mTreeCtrl.SetFontEx(eTreeItemType_Title, pnode->GetColor().font);

	// ツリーコントロールの背景色設定
	mTreeCtrl.SetBkColor(pnode->GetColor().back);

	restoreNode(pnode, rootItem);

	// ツリーコントロールのフォント設定
	mTreeCtrl.SetFontEx(eTreeItemType_Window, mTreeLogFont);

	// 常に展開モード
	//if (pnode->GetWindowInfo().treeopen != 0)
	{
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
	UINT sortno = 1;
	for (itr = pnode->GetChildren().begin(); itr != pnode->GetChildren().end(); itr++){
		CString str;
		str = generateTreeText((*itr));
		HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, ptree);
		switch ((*itr)->GetWindowInfo().type) {
		case	eTreeItemType_Main:
			mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(CCustomDropObject::DK_MAINNODE, 0));
			break;
		case	eTreeItemType_Sub:
			mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(CCustomDropObject::DK_SUBNODE, 0));
			break;
		case	eTreeItemType_Item:
			mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(CCustomDropObject::DK_LEAF, sortno * mSortRange));
			sortno++;
			break;
		}
		if (abs(mTreeLogFont.lfHeight) < abs(pnode->GetColor().font.lfHeight))
			mTreeLogFont.lfHeight = pnode->GetColor().font.lfHeight;
		// フォントの設定
		mTreeCtrl.SetFontEx((*itr)->GetWindowInfo().type, (*itr)->GetColor().font);
		(*itr)->SetTreeItem(item);
		(*itr)->GetWindowInfo().tree = &mTreeCtrl;
		(*itr)->GetWindowInfo().wnd = this;
		restoreNode((*itr), item);

		// 常に展開モードとする
		//if ((*itr)->GetWindowInfo().treeopen != 0)
		{
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
#if _DEMO_PHASE < 50
	return;
#endif
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mMenuItem);
	switch (pnode->GetWindowInfo().type) {
	case	eTreeItemType_Title:
		// メインノードアイテムの設定
		createMainNode(mMenuItem, pnode);
		mTreeCtrl.Expand(mMenuItem, TVE_EXPAND);
		break;
	case	eTreeItemType_Main:
		// サブノードアイテムの設定
		createSubNode(mMenuItem, pnode);
		mTreeCtrl.Expand(mMenuItem, TVE_EXPAND);
		break;
	case	eTreeItemType_Sub:
		// アイテムノードアイテムの設定
		createLeaf(mMenuItem, pnode);
		mTreeCtrl.Expand(mMenuItem, TVE_EXPAND);
		break;
	case	eTreeItemType_Item:
		break;
	}
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
#if _DEMO_PHASE < 50
	return;
#endif
	mTreeCtrl.SetRedraw(false);
	CTreeNode* pparent = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mTreeCtrl.GetParentItem(mMenuItem));
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mMenuItem);

	CString msg;
	msg.Format(mMessage_DetailDelete, pparent->GetMonCtrl().display);
	if (MessageBox(msg, mMessage_Title_CustomDetail, MB_YESNO | MB_ICONQUESTION) == IDYES) {
		switch (pnode->GetWindowInfo().type) {
		case	eTreeItemType_Main:
		case	eTreeItemType_Sub:
		case	eTreeItemType_Item:
			pparent->DeleteTreeNode(mMenuItem);
			mTreeCtrl.DeleteItem(mMenuItem);
			break;
		}
	}
	mTreeCtrl.SetRedraw(true);
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
#if _DEMO_PHASE < 50
	return;
#endif
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mMenuItem);
	switch (pnode->GetWindowInfo().type) {
	case	eTreeItemType_Title:
	case	eTreeItemType_Main:
	case	eTreeItemType_Sub:
		mTreeCtrl.SwitchEditMode(mMenuItem, CCustomTreeListCtrl::eItem, point);
		//mTreeCtrl.SelectItem(mMenuItem);
		//mTreeCtrl.EditLabel(mMenuItem);
		break;
	case	eTreeItemType_Item:
		mTreeCtrl.SwitchEditMode(mMenuItem, CCustomTreeListCtrl::eItem, point);
		break;
	}
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
#if _DEMO_PHASE < 50
	return;
#endif
	// 監視・制御一覧画面の呼出
	theApp.GetCustomControl().GetCustomMonCntl().ShowWindow(SW_SHOW);
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
#if _DEMO_PHASE < 50
	return;
#endif

	if (mMenuItem == NULL)
		return;

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mMenuItem);

	CCustomDetailConfig config(this, pnode->GetWindowInfo().type);

	// 設定されている色情報を設定ダイアログに取得する
	for (int i = 0; i < sizeof(mColorConfig) / sizeof(mColorConfig[0]); i++) {

		stColorData color;
		if (theApp.GetCustomControl().GetDataManager().GetNodeColor(this, mColorConfig[i].type, color) == true) {
			switch (mColorConfig[i].type) {
			case	eTreeItemType_Window:
				config.mColor[eColorType_Window] = color.back;
				config.mFont[eTreeItemType_Window] = color.font;
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

#if _DEMO_PHASE < 50
	return;
#endif

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

	// 設定ダイアログで設定された色情報を保存する
	for (int i = 0; i < sizeof(mColorConfig) / sizeof(mColorConfig[0]); i++) {
		stColorData color;
		if (theApp.GetCustomControl().GetDataManager().GetNodeColor(this, mColorConfig[i].type, color) == true) {
			switch (mColorConfig[i].type) {
			case	eTreeItemType_Window:
				color.back = config.mColor[eColorType_Window];
				mTreeCtrl.SetBkColor(color.back);
				color.font = config.mFont[eTreeItemType_Window];
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
			//if (mColorConfig[i].type != eTreeItemType_Window)
			{
				theApp.GetCustomControl().GetDataManager().SetNodeColor(this, mColorConfig[i].type, color);
				mTreeCtrl.SetFontEx(mColorConfig[i].type, color.font);
			}
		}
	}

	mTreeCtrl.Invalidate();
}

/*============================================================================*/
/*! 設備詳細

-# ツールチップイベント

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnTvnGetInfoTipTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMTVGETINFOTIP>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, pGetInfoTip->hItem);
	if (pnode == NULL) {
		return;
	}
	if (pnode->GetWindowInfo().type != eTreeItemType_Item) {
		return;
	}

	CString strmon = _T("EMPTY"), strcon = _T("EMPTY");
	if (CString(pnode->GetMonCtrl().mname).IsEmpty() == false) {
		strmon = pnode->GetMonCtrl().mname;
	}
	if (CString(pnode->GetMonCtrl().cname).IsEmpty() == false) {
		strcon = pnode->GetMonCtrl().cname;
	}
	mToolText.Format(_T("%s\n%s"), (LPCTSTR)strmon, (LPCTSTR)strcon);
	pGetInfoTip->pszText = (LPWSTR)(LPCTSTR)mToolText;
}

/*============================================================================*/
/*! 設備詳細

-# メッセージ処理

@param  なし

@retval なし
*/
/*============================================================================*/
LRESULT CCustomDetail::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case	eUserMessage_Detail_Mode:
		updateMode();
		break;
	case	eUserMessage_Manager_Update:
		setTreeTitle(lParam);
		break;
	default:
		return CCustomDialogBase::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}

/*============================================================================*/
/*! 設備詳細

-# タイトル文字の設定

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::setTreeTitle(LPARAM lParam)
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, (HTREEITEM)lParam);

	if (pnode->GetWindowInfo().type == eTreeItemType_Title) {
		swprintf_s(pnode->GetWindowInfo().title, mNameSize, _T("%s"), pnode->GetMonCtrl().display);
		updateMode();
	}
}

/*============================================================================*/
/*! 設備詳細

-# タイトル文字の変更

@param  なし

@retval なし
*/
/*============================================================================*/
void CCustomDetail::updateMode()
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	CString title = pnode->GetWindowInfo().title;
	mTreeCtrl.ModifyStyle(TVS_EDITLABELS, 0);
	if (pnode->GetWindowInfo().mode == eTreeItemMode_Edit) {
		title += CString(mEditModeString);
		mTreeCtrl.ModifyStyle(0, TVS_EDITLABELS);
	}

	// ウィンドウテキストの変更
	SetWindowText(title);
}

/*============================================================================*/
/*! 設備詳細

-# メニューの初期化

@param

@retval なし
*/
/*============================================================================*/
void CCustomDetail::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CCustomDialogBase::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	updateMenu(pPopupMenu);
}

/*============================================================================*/
/*! 設備詳細

-# メニュー項目の活性/非活性の設定

@param

@retval なし
*/
/*============================================================================*/
void CCustomDetail::updateMenu(CMenu* pMenu)
{
	int menuCount = pMenu->GetMenuItemCount();
	for (int i = 0; i < menuCount; i++){
		MENUITEMINFO menuItemInfo;
		::memset(&menuItemInfo, 0, sizeof(MENUITEMINFO));
		menuItemInfo.cbSize = sizeof(MENUITEMINFO);
		menuItemInfo.fMask = MIIM_STATE | MIIM_ID;
		if (pMenu->GetMenuItemInfo(i, &menuItemInfo, TRUE))
		{
			if (updateMenuItem(&menuItemInfo))
			{
				pMenu->SetMenuItemInfo(i, &menuItemInfo, TRUE);
			}
		}
	}
}

/*============================================================================*/
/*! 設備詳細

-# メニュー項目のチェック状態の取得

@param

@retval なし
*/
/*============================================================================*/
bool CCustomDetail::updateMenuItem(MENUITEMINFO* pMenuItemInfo)
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	switch (pMenuItemInfo->wID)
	{
	case ID_MENUDETAIL_EDIT:
		pMenuItemInfo->fState = pnode->GetWindowInfo().mode == eTreeItemMode_Edit ? MFS_CHECKED : MFS_UNCHECKED;
		return true;
	case ID_MENUDETAIL_MONITOR:
		pMenuItemInfo->fState = pnode->GetWindowInfo().mode == eTreeItemMode_Monitor ? MFS_CHECKED : MFS_UNCHECKED;
		return true;
	case ID_MENUDETAIL_SAVE:
		pMenuItemInfo->fState = CString(pnode->GetXmlFileName()).IsEmpty() == true ? MFS_GRAYED : MF_ENABLED;
		return true;
	}
	return false;
}

#ifdef _DRAGDROP
/*============================================================================*/
/*! 設備詳細

-# リーフの登録

@param		targetitem		登録対象のアイテム（親）
@param		tagetnode		登録対象のノード
@param		pdata			ドロップデータ

@retval void
*/
/*============================================================================*/
void CCustomDetail::DragDrop_AddLeaf(HTREEITEM targetitem, CTreeNode* targetnode, stDragData* pdata)
{
	CString str, strmon, strcon;
	str = createLeafText(mDefaultCustomItemText, _T(""), _T(""));

	vector<int>::iterator itr;
	for (itr = pdata->indexes.begin(); itr != pdata->indexes.end(); itr++) {
		// 仮アイテムの作成
		HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, targetitem);
		switch (targetnode->GetWindowInfo().type) {
		case	eTreeItemType_Main:
			mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(CCustomDropObject::DK_MAINNODE, 0));
			break;
		case	eTreeItemType_Sub:
			mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(CCustomDropObject::DK_SUBNODE, 0));
			break;
		case	eTreeItemType_Item:
			mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(CCustomDropObject::DK_LEAF, 0));
			break;
		}

		// 仮アイテムからノードを作成する
		CTreeNode* item_node = targetnode->CreateTreeNode(targetitem, item);
		CTreeNode* pcopyitem = theApp.GetCustomControl().GetDataManager().SearchItemNodeType(this, eTreeItemType_Item);
		if (pcopyitem != NULL)
			item_node->CopyItem(pcopyitem, true);

		// ノード情報の作成
		setNodeWindowInfo(item_node, eTreeItemType_Item, (LPWSTR)mDefaultCustomItemText, targetnode);

		// ドロップされた情報からノード情報を作成する
		DragDrop_GetDataName(pdata->type, (*itr), strmon, strcon);

		if (strmon.IsEmpty() == false) {
			swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strmon);
		}
		else if (strcon.IsEmpty() == false) {
			swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strcon);
		}
		else {
			swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)mDefaultCustomItemText);
		}
		swprintf_s(item_node->GetMonCtrl().mname, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strmon);
		swprintf_s(item_node->GetMonCtrl().cname, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strcon);

		// 仮アイテムへドロップされた情報をもとに再設定
		mTreeCtrl.SetSubItemText(item, CCustomTreeListCtrl::eItem, item_node->GetMonCtrl().display);
		if (strcon.IsEmpty() == false) {
			mTreeCtrl.SetSubItemText(item, CCustomTreeListCtrl::eControl, mCOntrolSignString);
		}
	}
	mTreeCtrl.Expand(targetitem, TVE_EXPAND);
}
/*============================================================================*/
/*! 設備詳細

-# リーフの更新

@param		targetitem		登録対象のアイテム
@param		tagetnode		登録対象のノード
@param		pdata			ドロップデータ

@retval void
*/
/*============================================================================*/
void CCustomDetail::DragDrop_UpdateLeaf(HTREEITEM targetitem, CTreeNode* targetnode, stDragData* pdata)
{
	CString str, strmon, strcon;

	vector<int>::iterator itr;
	for (itr = pdata->indexes.begin(); itr != pdata->indexes.end(); itr++) {
		DragDrop_GetDataName(pdata->type, (*itr), strmon, strcon);

		if (strmon.IsEmpty() == false) {
			swprintf_s(targetnode->GetMonCtrl().mname, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strmon);
		}
		else if (strcon.IsEmpty() == false) {
			swprintf_s(targetnode->GetMonCtrl().cname, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strcon);
		}

		if (strcon.IsEmpty() == false) {
			mTreeCtrl.SetSubItemText(targetitem, CCustomTreeListCtrl::eControl, mCOntrolSignString);
		}
		else {
			mTreeCtrl.SetSubItemText(targetitem, CCustomTreeListCtrl::eControl, _T(""));
		}
	}
}
/*============================================================================*/
/*! 設備詳細

-# ヘッダー幅取得

@param		item	ツリーアイテム

@retval
*/
/*============================================================================*/
void CCustomDetail::DragDrop_GetDataName(UINT type, UINT pos, CString& strmon, CString& strcon)
{
	strmon.Empty();
	strcon.Empty();

	switch (type) {
	case	eFromType_Mon:
		// 監視名取得
		//strmon = theApp.GetCustomControl().GetDataManager().GetDataMonitor().GetEqList()[pos];
		break;
	case	eFromType_Cntl:
		// 制御名取得
		//strcon = theApp.GetCustomControl().GetDataManager().GetDataControl().GetEqList()[pos];
		break;
	}
}

/*============================================================================*/
/*! 設備詳細

-# 画面間のドラッグ＆ドロップにおいて、ドロップ可能かを調べる

@param  todrag	ドラッグ元（0:監視・制御 1:カスタム）
@param  lParam	ドロップ情報（ドラッグ元によって異なる）

@retval なし
*/
/*============================================================================*/
BOOL CCustomDetail::DragDrop_SetSelectTarget(UINT todrag, LPARAM lParam)
{
	BOOL ret = TRUE;
	if (todrag == eFromType_Custom) {
		// カスタム画面からのドラッグ
		ret = DragDrop_SetSelectCustom(todrag, lParam);
	}
	else {
		// 監視・制御一覧からのドラッグ
		stDragData* pdata = (stDragData*)lParam;

		CPoint pt = CPoint(pdata->point);
		mTreeCtrl.ScreenToClient(&pt);
		// ドロップ位置情報から対象のアイテムを取得する
		HTREEITEM hItem = mTreeCtrl.HitTest(pt);
		if (hItem == NULL) {
			mTreeCtrl.SelectDropTarget(NULL);
			ret = FALSE;
		}
		else {
			CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);
			TRACE("=== Select Target Display : %s\n", CStringA(pnode->GetMonCtrl().display));
			UINT type = pnode->GetWindowInfo().type;
			if (pnode == NULL) {
				mTreeCtrl.SelectDropTarget(NULL);
				ret = FALSE;
			}
			else {
				switch (type) {
				case	eTreeItemType_Sub:
				case	eTreeItemType_Item:
					mTreeCtrl.SelectDropTarget(hItem);
					break;
				default:
					mTreeCtrl.SelectDropTarget(NULL);
					ret = FALSE;
				}
				mTreeCtrl.Expand(hItem, TVE_EXPAND);
			}
		}
	}
	return ret;
}
/*============================================================================*/
/*! 設備詳細

-# カスタム画面間のドラッグ＆ドロップにおいて、ドロップ可能かを調べる

@param  todrag	ドラッグ元（0:監視・制御 1:カスタム）
@param  lParam	ドロップ情報（ドラッグ元によって異なる）

@retval なし
*/
/*============================================================================*/
BOOL CCustomDetail::DragDrop_SetSelectCustom(UINT todrag, LPARAM lParam)
{
	CTreeNode* pnodeDrag = (CTreeNode*)lParam;
	if (pnodeDrag == NULL) {
		mTreeCtrl.SelectDropTarget(NULL);
		return FALSE;
	}

	// カーソル位置からドロップ先のアイテムを取得する
	CPoint pt;
	GetCursorPos(&pt);
	mTreeCtrl.ScreenToClient(&pt);
	HTREEITEM hItem = mTreeCtrl.HitTest(pt);
	if (hItem == NULL) {
		mTreeCtrl.SelectDropTarget(NULL);
		return FALSE;
	}

	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);
	TRACE("=== Select Target Display : %s\n", CStringA(pnodeDrop->GetMonCtrl().display));
	if (pnodeDrop == NULL) {
		mTreeCtrl.SelectDropTarget(NULL);
		return FALSE;
	}

	if (DragDrop_IsDropExecute(pnodeDrag, pnodeDrop) == false) {
		mTreeCtrl.SelectDropTarget(NULL);
		return FALSE;

	}

	mTreeCtrl.Expand(hItem, TVE_EXPAND);
	mTreeCtrl.SelectDropTarget(hItem);

	return TRUE;
}
/*============================================================================*/
/*! 設備詳細

-# ドロップ時のアイテム種別の関係性からドロップ可能かを調べる

@param	pnodeDrag	ドラッグノード
@param	hitemDrop	ドロップアイテム

@retval
*/
/*============================================================================*/
bool CCustomDetail::DragDrop_IsDropExecute(CTreeNode* pnodeDrag, CTreeNode* pnodeDrop)
{
	// ・ドラッグアイテム種別がeTreeItemType_Mainの場合はeTreeItemType_Titleにのみドロップ可能
	// ・ドラッグアイテム種別がeTreeItemType_Subの場合はeTreeItemType_Mainにのみドロップ可能
	// ・ドラッグアイテム種別がeTreeItemType_Itemの場合はeTreeItemType_Subにのみドロップ可能
	bool bDropExecute = false;
	if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Main && pnodeDrop->GetWindowInfo().type == eTreeItemType_Title)
		bDropExecute = true;
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Sub && pnodeDrop->GetWindowInfo().type == eTreeItemType_Main)
		bDropExecute = true;
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Item && pnodeDrop->GetWindowInfo().type == eTreeItemType_Sub)
		bDropExecute = true;

	return bDropExecute;
}

/*============================================================================*/
/*! 設備詳細

-# ドラッグ＆ドロップ対象の設定

@param  todrag	ドラッグ元（0:監視・制御 1:カスタム）
@param  lParam	ドロップ情報（ドラッグ元によって異なる）
@param  bMove	ドラッグ元のデータをコピーするのか移動するのかのフラグ

@retval なし
*/
/*============================================================================*/
void CCustomDetail::DragDrop_SetDropTarget(UINT todrag, LPARAM lParam, bool bMove/* = true*/)
{
	if (todrag == eFromType_Custom) {
		CPoint pt;
		GetCursorPos(&pt);
		mTreeCtrl.ScreenToClient(&pt);
		HTREEITEM hItem = mTreeCtrl.HitTest(pt);
		if (hItem == NULL) {
			mTreeCtrl.SelectDropTarget(NULL);
			return;
		}

		if (bMove == true) {
			DragDrop_Move(hItem, lParam);
		}
		else {
			DragDrop_Copy(hItem, lParam);
		}
	}
	else {
		// 監視・制御一覧からのドラッグ
		stDragData* pdata = (stDragData*)lParam;

		CPoint pt = CPoint(pdata->point);
		mTreeCtrl.ScreenToClient(&pt);
		// ドロップ位置情報から対象のアイテムを取得する
		HTREEITEM hItem = mTreeCtrl.HitTest(pt);
		if (hItem == NULL)
			return;

		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);
		if (pnode->GetWindowInfo().type != eTreeItemType_Sub && pnode->GetWindowInfo().type != eTreeItemType_Item)
			return;

		// ドロップされたアイテムを追加（作成）する
		CString strSub, strItem;

		if (pnode->GetWindowInfo().type == eTreeItemType_Sub) {
			// 追加
			DragDrop_AddLeaf(hItem, pnode, pdata);
		}
		if (pnode->GetWindowInfo().type == eTreeItemType_Item) {
			// 更新
			DragDrop_UpdateLeaf(hItem, pnode, pdata);
		}
		mTreeCtrl.Expand(hItem, TVE_EXPAND);
		mTreeCtrl.SelectDropTarget(NULL);
	}
}
/*============================================================================*/
/*! 設備詳細

-# 設備詳細同志のドラッグデータのコピーを行う

@param  hItem	ドロップ先アイテム
@param  lParam	ドロップ情報（ドラッグ元によって異なる）

@retval なし
*/
/*============================================================================*/
void CCustomDetail::DragDrop_Copy(HTREEITEM hItem, LPARAM lParam)
{
	/*
		ドラッグコピー条件
		・ドラッグ：メインノード　→　ドロップ：タイトルノード（メインノードの最上位にコピー）
		・ドラッグ：サブノード　→　ドロップ：メインノード（サブノードの最上位にコピー）
		・ドラッグ：リーフ　→　ドロップ：サブノード（リーフの最上位にコピー）
		・ドラッグ：リーフ　→　ドロップ：リーフ（リーフの下ににコピー）
	*/
	CTreeNode* pnodeDrag = (CTreeNode*)lParam;
	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);

	bool bSort = false;
	if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Main && pnodeDrop->GetWindowInfo().type == eTreeItemType_Title) {
		// ドラッグ：メインノード　→　ドロップ：タイトルノード（メインノードの最上位にコピー）
		TRACE("Drag_Drop Main -> Title\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Sub && pnodeDrop->GetWindowInfo().type == eTreeItemType_Main) {
		// ドラッグ：サブノード　→　ドロップ：メインノード（サブノードの最上位にコピー）
		TRACE("Drag_Drop Sub -> Main\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Item && pnodeDrop->GetWindowInfo().type == eTreeItemType_Sub) {
		// ドラッグ：リーフ　→　ドロップ：サブノード（リーフの最上位にコピー）
		TRACE("Drag_Drop Leaf -> Sub\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Item && pnodeDrop->GetWindowInfo().type == eTreeItemType_Item) {
		// ドラッグ：リーフ　→　ドロップ：リーフ（リーフの下ににコピー）
		bSort = true;
		TRACE("Drag_Drop Leaf -> Leaf\n");
	}
	else {
		TRACE("Drag_Drop Error %d -> %d\n", pnodeDrag->GetWindowInfo().type, pnodeDrop->GetWindowInfo().type);
		mTreeCtrl.SelectDropTarget(NULL);
		return;
	}

	// ここまできたらコピー可能
	CWnd* dragWnd = pnodeDrag->GetWindowInfo().wnd;
	CWnd* dropWnd = pnodeDrop->GetWindowInfo().wnd;
	HTREEITEM dragItem = pnodeDrag->GetTreeItem();
	HTREEITEM dropItem = pnodeDrop->GetTreeItem();
	DragDrop_CopyItem(dragWnd, dragItem, dropWnd, dropItem, bSort);
}
/*============================================================================*/
/*! 設備詳細

-# 設備詳細同志のドラッグデータの移動を行う

@param  hItem	ドロップ先アイテム
@param  lParam	ドロップ情報（ドラッグ元によって異なる）

@retval なし
*/
/*============================================================================*/
void CCustomDetail::DragDrop_Move(HTREEITEM hItem, LPARAM lParam)
{
	/*
		ドラッグ移動条件
		・ドラッグ：メインノード　→　ドロップ：タイトルノード（メインノードの最上位にコピー）
		・ドラッグ：サブノード　→　ドロップ：メインノード（サブノードの最上位にコピー）
		・ドラッグ：リーフ　→　ドロップ：サブノード（リーフの最上位にコピー）
		・ドラッグ：リーフ　→　ドロップ：リーフ（リーフの下ににコピー）
	*/
	CTreeNode* pnodeDrag = (CTreeNode*)lParam;
	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);

	bool bSort = false;
	if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Main && pnodeDrop->GetWindowInfo().type == eTreeItemType_Title) {
		// ドラッグ：メインノード　→　ドロップ：タイトルノード（メインノードの最上位にコピー）
		TRACE("Drag_Drop Main -> Title\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Sub && pnodeDrop->GetWindowInfo().type == eTreeItemType_Main) {
		// ドラッグ：サブノード　→　ドロップ：メインノード（サブノードの最上位にコピー）
		TRACE("Drag_Drop Sub -> Main\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Item && pnodeDrop->GetWindowInfo().type == eTreeItemType_Sub) {
		// ドラッグ：リーフ　→　ドロップ：サブノード（リーフの最上位にコピー）
		TRACE("Drag_Drop Leaf -> Sub\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Item && pnodeDrop->GetWindowInfo().type == eTreeItemType_Item) {
		// ドラッグ：リーフ　→　ドロップ：リーフ（リーフの下ににコピー）
		bSort = true;
		TRACE("Drag_Drop Leaf -> Leaf\n");
	}
	else {
		TRACE("Drag_Drop Error %d -> %d\n", pnodeDrag->GetWindowInfo().type, pnodeDrop->GetWindowInfo().type);
		mTreeCtrl.SelectDropTarget(NULL);
		return;
	}

	// ここまできたら移動可能
	CWnd* dragWnd = pnodeDrag->GetWindowInfo().wnd;
	CWnd* dropWnd = pnodeDrop->GetWindowInfo().wnd;
	HTREEITEM dragItem = pnodeDrag->GetTreeItem();
	HTREEITEM dropItem = pnodeDrop->GetTreeItem();
	DragDrop_MoveItem(dragWnd, dragItem, dropWnd, dropItem, bSort);
}
/*============================================================================*/
/*! ツリーコントロール拡張機能

-# ドロップ時のアイテムの移動

@param	dragWnd		ドラッグウィンドウハンドル
@param	pnodeDrag	ドラッグアイテム
@param	dropWnd		ドロップウィンドウハンドル
@param	pnodeDrop	ドロップアイテム
@param	bSort		ドロップ処理後にソートするかのフラグ

@retval
*/
/*============================================================================*/
bool CCustomDetail::DragDrop_CopyItem(CWnd* dragWnd, HTREEITEM dragItem, CWnd* dropWnd, HTREEITEM dropItem, bool bSort, HTREEITEM hInsertAfter/* = TVI_FIRST*/)
{
	CString str;
	HTREEITEM	hNewItem;

	CTreeNode* pnodeDrag = theApp.GetCustomControl().GetDataManager().SearchItemNode(dragWnd, dragItem);
	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(dropWnd, dropItem);
	CCustomTreeListCtrl* dragTree = (CCustomTreeListCtrl*)pnodeDrag->GetWindowInfo().tree;
	CCustomTreeListCtrl* dropTree = (CCustomTreeListCtrl*)pnodeDrop->GetWindowInfo().tree;

	CTreeNode* pnode;
	CTreeNode* pnodeParent = NULL;
	HTREEITEM parent=NULL;
	if (bSort == true) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("[Drag]%s -> [Drop]%s"), pnodeDrag->GetMonCtrl().display, pnodeDrop->GetMonCtrl().display);
		CLogTraceEx::Write(_T("***"), _T("CCustomDetail"), _T("DragDrop_CopyItem"), _T("Leaf"), msg, nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		// リーフからリーフ
		// ドロップアイテムの親を取得する
		parent = dropTree->GetParentItem(dropItem);
		pnodeParent = theApp.GetCustomControl().GetDataManager().SearchItemNode(dropWnd, parent);
		str = generateTreeText(pnodeDrag);
		hNewItem = dropTree->InsertItem(str, NULL, NULL, parent, TVI_SORT);
		mTreeCtrl.SetItemData(hNewItem, (LPARAM)hNewItem);
		pnode = pnodeParent->CreateTreeNode(parent, hNewItem);
	}
	else {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("[Drag]%s -> [Drop]%s"), pnodeDrag->GetMonCtrl().display, pnodeDrop->GetMonCtrl().display);
		CLogTraceEx::Write(_T("***"), _T("CCustomDetail"), _T("DragDrop_CopyItem"), _T("Node"), msg, nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		// メインノード、サブノードへのドロップ
		str = generateTreeText(pnodeDrag);
		hNewItem = dropTree->InsertItem(str, NULL, NULL, dropItem, hInsertAfter);
		mTreeCtrl.SetItemData(hNewItem, (LPARAM)hNewItem);
		pnode = pnodeDrop->CreateTreeNode(dropItem, hNewItem, hInsertAfter);
	}

	pnode->CopyItem(pnodeDrag);
	pnode->GetWindowInfo().wnd = dropWnd;
	pnode->GetWindowInfo().tree = dropTree;

	HTREEITEM hChildItem = dragTree->GetNextItem(dragItem, TVGN_CHILD);
	while (hChildItem) {
		DragDrop_CopyItem(dragWnd, hChildItem, dropWnd, hNewItem, bSort, TVI_LAST);
		hChildItem = dragTree->GetNextItem(hChildItem, TVGN_NEXT);
	}

	dropTree->Expand(hNewItem, TVE_EXPAND);
	dropTree->SelectDropTarget(NULL);

	if (bSort == true) {
		TRACE("DragDrop Sort Start(%08x) DropItem(%08x)\n", dropTree, dropItem);
		pnode->GetWindowInfo().sortno = pnodeDrop->GetWindowInfo().sortno + 1;
		HTREEITEM hParent = dropTree->GetParentItem(dropItem);
		DragDrop_SortItem(hParent);
		if (pnodeParent != NULL) {
			pnodeParent->SortTreeNode(parent);
		}

		// ソート番号の更新
		//DragDrop_UpdateSortNo(hParent);
	}

	UpdateSortNo(mTreeCtrl.GetRootItem());
	dropTree->ExpandAll();

	return true;
}
/*============================================================================*/
/*! ツリーコントロール拡張機能

-# ドロップ時のアイテムの移動

@param	dragWnd		ドラッグウィンドウハンドル
@param	pnodeDrag	ドラッグアイテム
@param	dropWnd		ドロップウィンドウハンドル
@param	pnodeDrop	ドロップアイテム
@param	bSort		ドロップ処理後にソートするかのフラグ

@retval
*/
/*============================================================================*/
bool CCustomDetail::DragDrop_MoveItem(CWnd* dragWnd, HTREEITEM dragItem, CWnd* dropWnd, HTREEITEM dropItem, bool bSort, HTREEITEM hInsertAfter/* = TVI_FIRST*/)
{
	CString str;
	HTREEITEM	hNewItem, hFirstChild;

	CTreeNode* pnodeWnd = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	CTreeNode* pnodeDrag = theApp.GetCustomControl().GetDataManager().SearchItemNode(dragWnd, dragItem);
	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(dropWnd, dropItem);
	CCustomTreeListCtrl* dragTree = (CCustomTreeListCtrl*)pnodeDrag->GetWindowInfo().tree;
	CCustomTreeListCtrl* dropTree = (CCustomTreeListCtrl*)pnodeDrop->GetWindowInfo().tree;

	CTreeNode* pnode;
	CTreeNode* pnodeParent = NULL;
	HTREEITEM parent = NULL;
	if (bSort == true) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("[Drag]%s -> [Drop]%s"), pnodeDrag->GetMonCtrl().display, pnodeDrop->GetMonCtrl().display);
		CLogTraceEx::Write(_T("***"), _T("CCustomDetail"), _T("DragDrop_MoveItem"), _T("Leaf"), msg, nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		// リーフからリーフ
		// ドロップアイテムの親を取得する
		parent = dropTree->GetParentItem(dropItem);
		pnodeParent = theApp.GetCustomControl().GetDataManager().SearchItemNode(dropWnd, parent);
		str = generateTreeText(pnodeDrag);
		hNewItem = dropTree->InsertItem(str, NULL, NULL, parent, TVI_SORT);
		mTreeCtrl.SetItemData(hNewItem, (LPARAM)hNewItem);
		pnode = pnodeParent->CreateTreeNode(parent, hNewItem);
	}
	else {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CString msg;
		msg.Format(_T("[Drag]%s -> [Drop]%s"), pnodeDrag->GetMonCtrl().display, pnodeDrop->GetMonCtrl().display);
		CLogTraceEx::Write(_T("***"), _T("CCustomDetail"), _T("DragDrop_MoveItem"), _T("Node"), msg, nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		str = generateTreeText(pnodeDrag);
		hNewItem = dropTree->InsertItem(str, NULL, NULL, dropItem, hInsertAfter);
		mTreeCtrl.SetItemData(hNewItem, (LPARAM)hNewItem);
		pnode = pnodeDrop->CreateTreeNode(dropItem, hNewItem, hInsertAfter);
	}

	pnode->CopyItem(pnodeDrag);
	pnode->GetWindowInfo().wnd = dropWnd;
	pnode->GetWindowInfo().tree = dropTree;

	while ((hFirstChild = dragTree->GetChildItem(dragItem)) != NULL)
	{
		DragDrop_MoveItem(dragWnd, hFirstChild, dropWnd, hNewItem, bSort, TVI_LAST);
	}
	dropTree->Expand(hNewItem, TVE_EXPAND);
	dropTree->SelectDropTarget(NULL);

	dragTree->DeleteItem(dragItem);
	pnodeWnd->DeleteItemNode(pnodeDrag);

	if (bSort == true) {
		pnode->GetWindowInfo().sortno = pnodeDrop->GetWindowInfo().sortno + 1;
		DragDrop_SortItem(parent);
		if (pnodeParent != NULL) {
			pnodeParent->SortTreeNode(parent);
		}

		// ソート番号の更新
		//DragDrop_UpdateSortNo(parent);
	}

	UpdateSortNo(mTreeCtrl.GetRootItem());
	dropTree->ExpandAll();

	return true;
}
/*============================================================================*/
/*! 設備詳細

-# ツリーコントロールのソート処理（リーフのみ）

@param

@retval int
*/
/*============================================================================*/
void CCustomDetail::DragDrop_SortItem(HTREEITEM item)
{
	if (item != NULL && mTreeCtrl.ItemHasChildren(item)) {

		TVSORTCB tvs;
		tvs.hParent = item;
		tvs.lpfnCompare = DragDrop_Compare;
		tvs.lParam = (LPARAM)&mTreeCtrl;

		CTreeCtrl* ptree = (CTreeCtrl*)tvs.lParam;
		CWnd* p = ptree->GetParent();

		mTreeCtrl.SortChildrenCB(&tvs);
	}
}
/*============================================================================*/
/*! 設備詳細

-# ツリーコントロールのソートを行うコールバック関数

@param

@retval int
*/
/*============================================================================*/
int CALLBACK CCustomDetail::DragDrop_Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CTreeCtrl* ptree = (CTreeCtrl*)lParamSort;
	CWnd* p = ptree->GetParent();
	CTreeNode* pdata1 = theApp.GetCustomControl().GetDataManager().SearchItemNode(ptree->GetParent(), (HTREEITEM)lParam1);
	CTreeNode* pdata2 = theApp.GetCustomControl().GetDataManager().SearchItemNode(ptree->GetParent(), (HTREEITEM)lParam2);

	TRACE("DragDrop Compare(%08x) %08x:%d > %08x:%d\n", ptree, lParam1, (pdata1 == NULL) ? -1 : pdata1->GetWindowInfo().sortno, lParam2, (pdata2 == NULL) ? -1 : pdata2->GetWindowInfo().sortno);

	return (pdata1 != NULL && pdata2 != NULL) ? (pdata1->GetWindowInfo().sortno > pdata2->GetWindowInfo().sortno) : 0;
}
/*============================================================================*/
/*! 設備詳細

-# ツリーコントロールのソートを行うコールバック関数

@param

@retval int
*/
/*============================================================================*/
void CCustomDetail::DragDrop_UpdateSortNo(HTREEITEM item)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDetail"), _T("DragDrop_UpdateSortNo"), _T(""), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	CTreeNode* pSubNode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, item);
	if (pSubNode->GetWindowInfo().type != eTreeItemType_Sub)
		return;

	HTREEITEM child = mTreeCtrl.GetChildItem(item);
	UINT pos = 0;
	while (child != NULL) {
		CTreeNode* node = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, child);
		if (node == NULL)
			break;

		node->GetWindowInfo().sortno = pos * mSortRange;

		child = mTreeCtrl.GetNextItem(child, TVGN_NEXT);
		pos++;
	}
}
#endif

/*============================================================================*/
/*! 設備詳細

-# ツリーテキストの生成

@param	pnode	ツリーﾉｰﾄﾞ

@retval void
*/
/*============================================================================*/
CString CCustomDetail::generateTreeText(CTreeNode* pnode)
{
	CString ret;

	if (pnode->GetWindowInfo().type == eTreeItemType_Item) {
		ret = createLeafText(pnode->GetMonCtrl().display, pnode->GetMonCtrl().unit, pnode->GetMonCtrl().cname);
	}
	else {
		ret.Format(_T("%s"), pnode->GetMonCtrl().display);
	}
	return ret;
}

/*============================================================================*/
/*! 設備詳細

-# ソート番号の更新

@param	hItem	ツリーアイテム

@retval void
*/
/*============================================================================*/
void CCustomDetail::UpdateSortNo(HTREEITEM hItem)
{
	HTREEITEM hSubItem = mTreeCtrl.GetChildItem(hItem);
	while (hSubItem) {
		UpdateSortNo(hSubItem);
		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hSubItem);
		if (pnode->GetWindowInfo().type == eTreeItemType_Sub) {
			HTREEITEM hItem = mTreeCtrl.GetChildItem(hSubItem);
			UINT pos = 1;
			while (hItem) {
				CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);
				pnode->GetWindowInfo().sortno = pos * mSortRange;
				pos++;
				hItem = mTreeCtrl.GetNextSiblingItem(hItem);
			}
		}
		hSubItem = mTreeCtrl.GetNextSiblingItem(hSubItem);
	}
}
