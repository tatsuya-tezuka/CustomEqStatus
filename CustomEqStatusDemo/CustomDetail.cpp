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
	mTreeLogFont.lfWeight = FW_BOLD;
	mTreeLogFont.lfCharSet = DEFAULT_CHARSET;
	mTreeLogFont.lfUnderline = 0;
	mTreeLogFont.lfStrikeOut = 0;
	swprintf_s(mTreeLogFont.lfFaceName, LF_FACESIZE, _T("%s"), (LPCTSTR)mDefaultCustomFontName);
	temp.GetLogFont(&mTreeLeafLogFont);
	mTreeLeafLogFont.lfHeight = -mTreeFontHeight;
	mTreeLeafLogFont.lfWeight = FW_BOLD;
	mTreeLeafLogFont.lfCharSet = DEFAULT_CHARSET;
	mTreeLeafLogFont.lfUnderline = 0;
	mTreeLeafLogFont.lfStrikeOut = 0;
	swprintf_s(mTreeLeafLogFont.lfFaceName, LF_FACESIZE, _T("%s"), (LPCTSTR)mDefaultMonitorFontName);
	temp.DeleteObject();

	// 文字フォントの作成
	CFont defFont;
	defFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (defFont.GetLogFont(&lf)) {
		lf.lfHeight = -14;
		lf.lfWeight = FW_NORMAL;
		lf.lfCharSet = DEFAULT_CHARSET;
		lf.lfUnderline = 0;
		lf.lfStrikeOut = 0;
		swprintf_s(lf.lfFaceName, LF_FACESIZE, _T("%s"), (LPCTSTR)mDefaultCustomFontName);
		mToolFont.DeleteObject();
		mToolFont.CreateFontIndirect(&lf);
		defFont.DeleteObject();
	}

	//mBackupNode = NULL;
}

CCustomDetail::~CCustomDetail()
{
	mToolFont.DeleteObject();

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
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_CTRL, &CCustomDetail::OnTvnItemexpandingTreeCtrl)
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
#if _DEMO_PHASE < 80
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
		switch (pnode->GetEquipment().type) {
		case	eTreeItemType_Title:
			pMenu->EnableMenuItem(ID_DETAIL_DELETE, MF_BYCOMMAND | MF_GRAYED);
			break;
		case	eTreeItemType_Main:
			break;
		case	eTreeItemType_Sub:
			break;
		case	eTreeItemType_Item:
			pMenu->EnableMenuItem(ID_DETAIL_ADD, MF_BYCOMMAND | MF_GRAYED);
			break;
		}

		pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
		if (pnode->GetEquipment().mode == eTreeItemMode_Monitor) {
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

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDetail"), _T("OnMenudetailClose"), CString(pnode->GetEquipment().title), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

#if _DEMO_PHASE < 80
	int retmsg = CustomSaveDifferentMessageBoxHooked(m_hWnd, mMessage_DetailSaveDifferentData, pnode->GetEquipment().title, MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1, CString(pnode->GetXmlFileName()).IsEmpty() ? false : true);
	if (retmsg == IDCANCEL) {
		return;
	}
	CCustomDialogBase::OnClose();
	return;
#endif

	/*
		ウィンドウ位置の保存はレイアウト保存時だけ
	*/
	// ウィンドウサイズ、ヘッダーコントロールの幅を保存する
	WINDOWPLACEMENT	placement;			// ウィンドウ位置情報
	memset(&placement, 0, sizeof(WINDOWPLACEMENT));
	placement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&placement);
	memcpy(&pnode->GetEquipment().placement.rcNormalPosition, &placement.rcNormalPosition, sizeof(RECT));
	saveHeaderWidth();

	//bool ret = mBackupNode->Equal(pnode);
	bool ret = theApp.GetCustomControl().GetDataManager().CompareEditNode(this);
	if (ret == true && CString(pnode->GetXmlFileName()).IsEmpty() == false) {
		TRACE("Same Data\b");
	}
	else {
		TRACE("Different Data\n");

		CCustomSelectSaveFile dlg;
		dlg.SetSavePathName(theApp.GetCustomControl().GetUserDataPath());
		dlg.SetSaveFileName(CString(pnode->GetXmlFileName()));
		INT_PTR ret = dlg.DoModal();

		if (ret == IDCANCEL) {
			return;
		}
		if (ret == IDOK) {
			// 変更内容を保存する
			OnMenudetailSave();
		}
	}

	CCustomDialogBase::OnClose();

	if (mMessageRet == IDCANCEL)
		return;

	// 編集用ノードの削除
	// 画面連結対応
	if (HIWORD(pnode->GetManager().groupno) != 0) {
		vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
		vector<CTreeNode*>::iterator itr;
		for (itr = treedata.begin(); itr != treedata.end(); itr++) {
			if ((*itr)->GetEquipment().wnd != pnode->GetEquipment().wnd && HIWORD((*itr)->GetManager().groupno) == HIWORD(pnode->GetManager().groupno)) {
				CWnd* pwnd = (*itr)->GetEquipment().wnd;
				theApp.GetCustomControl().GetDataManager().DeleteEditNode(pwnd);
				theApp.GetCustomControl().GetDataManager().DeleteItemWnd(pwnd);
			}
		}
	}

	theApp.GetCustomControl().GetDataManager().DeleteEditNode(this);

	// 新規作成時のウィンドウ時、保存しない場合は削除する
	pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
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
#if _DEMO_PHASE < 80
	return;
#endif
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDetail"), _T("OnMenudetailSave"), CString(pnode->GetEquipment().title), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	if (CString(pnode->GetXmlFileName()).IsEmpty() == true) {
		return;
	}

	// ツリーデータの保存
	saveHeaderWidth();

	// 編集用ノード情報の保存
	theApp.GetCustomControl().GetDataManager().RestoreEditNode(this);

	theApp.GetCustomControl().GetDataManager().SaveEquipmentData((UINT)eLayoutFileType_XML, pnode->GetXmlFileName(), this);

	// カスタム管理画面へ通知してタイトルを更新する
	if (pnode->GetEquipment().manager->GetSafeHwnd())
		pnode->GetEquipment().manager->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)this);
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
#if _DEMO_PHASE < 80
	return;
#endif
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDetail"), _T("OnMenudetailSaveas"), CString(pnode->GetEquipment().title), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	CCustomSelectSaveFile dlg;
	dlg.SetSavePathName(theApp.GetCustomControl().GetUserDataPath());
	dlg.SetSaveFileName(CString(pnode->GetXmlFileName()));
	INT_PTR ret = dlg.DoModal();

	CString filename = dlg.GetSavePathName() + _T("\\") + dlg.GetSaveFileName();
	CString backxml = CString(pnode->GetXmlFileName());
	switch (ret) {
	case	IDCANCEL:
		return;
	case	IDNO:
		return;
		break;
	case	IDOK:
		break;
	}

	// ツリーヘッダー幅の保存
	saveHeaderWidth();

	// 編集用ノード情報の保存
	swprintf_s(pnode->GetXmlFileName(), _MAX_PATH, _T("%s"), (LPCTSTR)filename);

	// ①編集用ノードを名前を付けて保存する
	theApp.GetCustomControl().GetDataManager().SaveasEditNode(this, (LPCTSTR)filename);

	// ②CCustomManagerの表示更新
	if (theApp.GetCustomControl().GetCustomManager().GetSafeHwnd() != NULL) {
		theApp.GetCustomControl().GetCustomManager().PostMessage(eUserMessage_Manager_Reset, 0, 0);
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

	mTreeCtrl.GetHeaderWidth(pnode->GetEquipment().hwidth, mHeaderSize);
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
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	if (pnode != NULL) {
		pnode->GetEquipment().mode = eTreeItemMode_Edit;
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
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	int retmsg = CustomSaveDifferentMessageBoxHooked(m_hWnd, mMessage_DetailSaveDifferentData, pnode->GetEquipment().title, MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1, CString(pnode->GetXmlFileName()).IsEmpty() ? false : true);
	if (retmsg == IDCANCEL) {
		return;
}
#if _DEMO_PHASE < 80
	return;
#endif

	bool ret = theApp.GetCustomControl().GetDataManager().CompareEditNode(this);
	if (ret != true) {
	}

	pnode->GetEquipment().mode = eTreeItemMode_Monitor;
	updateMode();
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

	if (mRestore == true) {
		// 編集用ノードを使用するためにノードリストをバックする
		theApp.GetCustomControl().GetDataManager().BackupEditNode(this);
		restoreRoot();
	}
	else {
		createRoot();
	}

	mTreeCtrl.UpdateColumns();

	UpdateSortNo(mTreeCtrl.GetRootItem());

	SetControlInfo(IDC_TREE_CTRL, ANCHORE_LEFTTOP | RESIZE_BOTH);

	CToolTipCtrl* ptip = mTreeCtrl.GetToolTips();
	if (ptip != NULL) {
		ptip->SetFont(&mToolFont);
		ptip->SetDelayTime(TTDT_AUTOPOP, 50000);
	}
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
	mTreeCtrl.SetItemData(rootItem, mTreeCtrl.MAKEDATA(eTreeItemType_Title, 0));

	CTreeNode* newnode = new CTreeNode(rootItem, this, &mTreeCtrl, eTreeItemType_Title);
	setNodeWindowInfo(newnode, eTreeItemType_Title, (LPWSTR)mDefaultCustomTitle, NULL);
	// デフォルトフォントの設定
	mTreeCtrl.SetFontEx(newnode->GetEquipment().type, mTreeLogFont);
	// 論理フォントの取得
	mTreeCtrl.GetFontEx(eTreeItemType_Title).GetLogFont(&newnode->GetColor().font);

	newnode->GetEquipment().mode = eTreeItemMode_Edit;

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
	mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(eTreeItemType_Main, 0));

	CTreeNode* item_node = parentnode->CreateTreeNode(eTreeItemType_Main, parentitem, item);
	CTreeNode* pcopyitem = theApp.GetCustomControl().GetDataManager().SearchItemNodeType(this, eTreeItemType_Main);
	if (pcopyitem != NULL)
		item_node->CopyItem(pcopyitem, true);
	setNodeWindowInfo(item_node, eTreeItemType_Main, (LPWSTR)mDefaultCustomMainText, parentnode);
	// デフォルトフォントの設定
	mTreeCtrl.SetFontEx(item_node->GetEquipment().type, mTreeLogFont);
	// 論理フォントの取得
	mTreeCtrl.GetFontEx(eTreeItemType_Main).GetLogFont(&item_node->GetColor().font);

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
	mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(eTreeItemType_Sub, 0));

	CTreeNode* item_node = parentnode->CreateTreeNode(eTreeItemType_Sub, parentitem, item);
	CTreeNode* pcopyitem = theApp.GetCustomControl().GetDataManager().SearchItemNodeType(this, eTreeItemType_Sub);
	if (pcopyitem != NULL)
		item_node->CopyItem(pcopyitem, true);
	setNodeWindowInfo(item_node, eTreeItemType_Sub, (LPWSTR)mDefaultCustomSubText, parentnode);
	// デフォルトフォントの設定
	mTreeCtrl.SetFontEx(item_node->GetEquipment().type, mTreeLogFont);
	// 論理フォントの取得
	mTreeCtrl.GetFontEx(eTreeItemType_Sub).GetLogFont(&item_node->GetColor().font);

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
	str = createLeafText(mDefaultCustomItemText, _T(""), _T(""), false);
	HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, parentitem);
	mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(eTreeItemType_Item, 0));

	CTreeNode* item_node = parentnode->CreateTreeNode(eTreeItemType_Item, parentitem, item);
	CTreeNode* pcopyitem = theApp.GetCustomControl().GetDataManager().SearchItemNodeType(this, eTreeItemType_Item);
	if (pcopyitem != NULL)
		item_node->CopyItem(pcopyitem, true);
	setNodeWindowInfo(item_node, eTreeItemType_Item, (LPWSTR)mDefaultCustomItemText, parentnode);
	// デフォルトフォントの設定
	mTreeCtrl.SetFontEx(item_node->GetEquipment().type, mTreeLeafLogFont);
	// 論理フォントの取得
	mTreeCtrl.GetFontEx(eTreeItemType_Item).GetLogFont(&item_node->GetColor().font);

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
	HTREEITEM rootItem = mTreeCtrl.InsertItem(pnode->GetEquipment().title, NULL, NULL, TVI_ROOT);
	mTreeCtrl.SetItemData(rootItem, mTreeCtrl.MAKEDATA(eTreeItemType_Title, 0));

	pnode->SetTreeItem(rootItem);
	mTreeLogFont = pnode->GetColor().font;

	setNodeWindowInfo(pnode, eTreeItemType_Title, pnode->GetEquipment().title, NULL);
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
	SetWindowPlacement(&(pnode->GetEquipment().placement));
	ShowWindow(pnode->GetEquipment().placement.showCmd);

	// ヘッダー幅再設定
	mTreeCtrl.SetHeaderWidth(pnode->GetEquipment().hwidth, mHeaderSize);
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
		switch ((*itr)->GetEquipment().type) {
		case	eTreeItemType_Main:
			mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(eTreeItemType_Main, (*itr)->GetEquipment().sortno));
			break;
		case	eTreeItemType_Sub:
			mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(eTreeItemType_Sub, (*itr)->GetEquipment().sortno));
			break;
		case	eTreeItemType_Item:
			mTreeCtrl.SetItemData(item, mTreeCtrl.MAKEDATA(eTreeItemType_Item, (*itr)->GetEquipment().sortno/*sortno * mSortRange*/));
			sortno++;
			break;
		}
		if (abs(mTreeLogFont.lfHeight) < abs(pnode->GetColor().font.lfHeight))
			mTreeLogFont.lfHeight = pnode->GetColor().font.lfHeight;
		// フォントの設定
		mTreeCtrl.SetFontEx((*itr)->GetEquipment().type, (*itr)->GetColor().font);
		(*itr)->SetTreeItem(item);
		(*itr)->GetEquipment().tree = &mTreeCtrl;
		(*itr)->GetEquipment().wnd = this;
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
	pnode->GetEquipment().wnd = this;
	pnode->GetEquipment().tree = &mTreeCtrl;
	pnode->GetEquipment().type = type;
	swprintf_s(pnode->GetEquipment().title, mTitleSize, _T("%s"), text);
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
	int CYMENU = GetSystemMetrics(SM_CYMENU);
	int CXSIZEFRAME = GetSystemMetrics(SM_CXSIZEFRAME);
	int CYSIZEFRAME = GetSystemMetrics(SM_CYSIZEFRAME);
	int CXEDGE = GetSystemMetrics(SM_CXEDGE);
	int CYEDGE = GetSystemMetrics(SM_CYEDGE);

	// ヘッダーの最適化を行う
	int nColsCnt = mTreeCtrl.GetHeaderCount();
	for (int i = 0; i < nColsCnt; i++) {
		mTreeCtrl.DividerDblClick(i);
	}

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

	cyTotal = CYCAPTION + CYMENU + (CXSIZEFRAME * 4) + nHeaderHeight + (mTreeCtrl.GetItemHeight() * itemCount) + (CXEDGE * 2);

	// ◆ウィンドウサイズ変更
	WINDOWPLACEMENT	wPlacement;
	memset(&wPlacement, 0, sizeof(WINDOWPLACEMENT));
	wPlacement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wPlacement);

	// ウィンドウの位置情報更新
	CRect r = CRect(wPlacement.rcNormalPosition);
	wPlacement.rcNormalPosition.right = wPlacement.rcNormalPosition.left + cxTotal;
	wPlacement.rcNormalPosition.bottom = wPlacement.rcNormalPosition.top + cyTotal;
	r = CRect(wPlacement.rcNormalPosition);

	SetWindowPlacement(&wPlacement);

	mTreeCtrl.ResizeControl(r.Width(), r.Height());

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	theApp.GetCustomControl().GetCustomManager().GetCustomSyncWindow().Sync(this, pnode->GetManager().groupno);
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
#if _DEMO_PHASE < 80
	return;
#endif
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mMenuItem);
	switch (pnode->GetEquipment().type) {
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
#if _DEMO_PHASE < 80
	return;
#endif
	mTreeCtrl.SetRedraw(false);
	CTreeNode* pparent = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mTreeCtrl.GetParentItem(mMenuItem));
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mMenuItem);

	CString msg;
	msg.Format(mMessage_DetailDelete, pparent->GetMonCtrl().display);
	if (MessageBox(msg, mMessage_Title_CustomDetail, MB_YESNO | MB_ICONQUESTION) == IDYES) {
		switch (pnode->GetEquipment().type) {
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
#if _DEMO_PHASE < 80
	return;
#endif
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mMenuItem);
	switch (pnode->GetEquipment().type) {
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
#if _DEMO_PHASE < 80
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
#if _DEMO_PHASE < 110
	return;
#endif

	if (mMenuItem == NULL)
		return;

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mMenuItem);

	CCustomDetailConfig config(this, pnode->GetEquipment().type);

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
				color.font.lfItalic = 0;
				color.font.lfUnderline = 0;
				color.font.lfStrikeOut = 0;
				break;
			case	eTreeItemType_Title:
				color.textback = config.mColor[eColorType_TitleBack];
				color.text = config.mColor[eColorType_TitleText];
				color.font = config.mFont[eTreeItemType_Title];
				color.font.lfItalic = 0;
				color.font.lfUnderline = 0;
				color.font.lfStrikeOut = 0;
				break;
			case	eTreeItemType_Main:
				color.textback = config.mColor[eColorType_MainBack];
				color.text = config.mColor[eColorType_MainText];
				color.font = config.mFont[eTreeItemType_Main];
				color.font.lfItalic = 0;
				color.font.lfUnderline = 0;
				color.font.lfStrikeOut = 0;
				break;
			case	eTreeItemType_Sub:
				color.textback = config.mColor[eColorType_SubBack];
				color.text = config.mColor[eColorType_SubText];
				color.font = config.mFont[eTreeItemType_Sub];
				color.font.lfItalic = 0;
				color.font.lfUnderline = 0;
				color.font.lfStrikeOut = 0;
				break;
			case	eTreeItemType_Item:
				color.textback = config.mColor[eColorType_ItemBack];
				color.text = config.mColor[eColorType_ItemText];
				color.value = config.mColor[eColorType_ValueText];
				color.unit = config.mColor[eColorType_UnitText];
				color.font = config.mFont[eTreeItemType_Item];
				color.font.lfItalic = 0;
				color.font.lfUnderline = 0;
				color.font.lfStrikeOut = 0;
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
		ASSERT(FALSE);
		return;
	}

	// マウス位置を取得して、先頭アイテムにあるときだけツールヒントを表示する
	CPoint pos;
	if (!::GetCursorPos(&pos)){
		ASSERT(FALSE);
		return;
	}
	mTreeCtrl.ScreenToClient(&pos);
	UINT col = 0;
	HTREEITEM item = mTreeCtrl.HitTestEx(pos, col);
	if (item == NULL || col != 0)
		return;

	if (pnode->GetEquipment().type != eTreeItemType_Item) {
#ifdef _DEBUG
		mToolText.Format(_T("%s\nSORT(%d)\n%08X"), (LPCTSTR)pnode->GetMonCtrl().display, pnode->GetEquipment().sortno, mTreeCtrl.GetItemData(pGetInfoTip->hItem));
		pGetInfoTip->pszText = (LPWSTR)(LPCTSTR)mToolText;
#endif
		return;
	}

	CString strmon = _T("EMPTY"), strcon = _T("EMPTY");
	if (CString(pnode->GetMonCtrl().mname).IsEmpty() == false) {
		strmon = pnode->GetMonCtrl().mname;
	}
	if (CString(pnode->GetMonCtrl().cname).IsEmpty() == false) {
		strcon = pnode->GetMonCtrl().cname;
	}

#ifdef _DEBUG
	mToolText.Format(_T("%s\n%s\nSORT(%d)\n%08X"), (LPCTSTR)strmon, (LPCTSTR)strcon, pnode->GetEquipment().sortno, mTreeCtrl.GetItemData(pGetInfoTip->hItem));
#else
	if (strmon == _T("EMPTY") && strcon == _T("EMPTY") && CString(pnode->GetMonCtrl().display).IsEmpty())
		return;
	mToolText.Format(_T("%s\n%s"), (LPCTSTR)strmon, (LPCTSTR)strcon);
#endif
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
		OnMenudetailEdit();
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

	if (pnode->GetEquipment().type == eTreeItemType_Title) {
		swprintf_s(pnode->GetEquipment().title, mNameSize, _T("%s"), pnode->GetMonCtrl().display);
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
#if _DEMO_PHASE < 80
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	CString title = pnode->GetEquipment().title;
	if (pnode->GetEquipment().mode == eTreeItemMode_Edit) {
		title += CString(mEditModeString);
	}

	// ウィンドウテキストの変更
	SetWindowText(title);
#else
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	CString title = pnode->GetEquipment().title;
	mTreeCtrl.ModifyStyle(TVS_EDITLABELS, 0);
	if (pnode->GetEquipment().mode == eTreeItemMode_Edit) {
		title += CString(mEditModeString);
		mTreeCtrl.ModifyStyle(0, TVS_EDITLABELS);
	}

	// ウィンドウテキストの変更
	SetWindowText(title);
#endif
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
		pMenuItemInfo->fState = pnode->GetEquipment().mode == eTreeItemMode_Edit ? MFS_CHECKED : MFS_UNCHECKED;
		return true;
	case ID_MENUDETAIL_MONITOR:
		pMenuItemInfo->fState = pnode->GetEquipment().mode == eTreeItemMode_Monitor ? MFS_CHECKED : MFS_UNCHECKED;
		return true;
	case ID_MENUDETAIL_SAVE:
		pMenuItemInfo->fState = CString(pnode->GetXmlFileName()).IsEmpty() == true ? MFS_GRAYED : MF_ENABLED;
		return true;
	}
	return false;
}

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

	if (pnode->GetEquipment().type == eTreeItemType_Item) {
#ifdef _ORG
		ret = createLeafText(pnode->GetMonCtrl().display, pnode->GetMonCtrl().unit, pnode->GetMonCtrl().cname, CString(pnode->GetMonCtrl().mname).IsEmpty()?false:true);
		int nRand = rand();
		if ((nRand % 5) == 0) {
			pnode->GetColor().value = mDefaultErrorValueTextColor;
		} else if ((nRand % 5) == 1) {
			pnode->GetColor().value = mDefaultWarningValueTextColor;
		}
		else {
			pnode->GetColor().value = mDefaultValueTextColor;

		}
#else
		ret = createLeafText(pnode->GetMonCtrl().display, pnode->GetMonCtrl().unit, pnode->GetMonCtrl().cname, CString(pnode->GetMonCtrl().mname).IsEmpty() ? false : true);
		CString data;
		COLORREF col = theApp.GetCustomControl().GetCustomData(pnode->GetMonCtrl().mname, data);

		if (data.IsEmpty() == false) {
			ret.Format(_T("%s\t%s\t%s\t%s"), (LPCTSTR)pnode->GetMonCtrl().display, (LPCTSTR)data, (LPCTSTR)pnode->GetMonCtrl().unit, CString(pnode->GetMonCtrl().cname).IsEmpty() == false ? mCOntrolSignString : _T(""));
			pnode->GetColor().value = col;
		}
		else {
			int nRand = rand();
			if ((nRand % 5) == 0) {
				pnode->GetColor().value = mDefaultErrorValueTextColor;
			}
			else if ((nRand % 5) == 1) {
				pnode->GetColor().value = mDefaultWarningValueTextColor;
			}
			else {
				pnode->GetColor().value = mDefaultValueTextColor;
			}
		}
#endif
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
		if (pnode->GetEquipment().type == eTreeItemType_Sub) {
			HTREEITEM hItem = mTreeCtrl.GetChildItem(hSubItem);
			UINT pos = 1;
			while (hItem) {
				CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);
				pnode->GetEquipment().sortno = pos * mSortRange;
				pos++;
				hItem = mTreeCtrl.GetNextSiblingItem(hItem);
			}
		}
		hSubItem = mTreeCtrl.GetNextSiblingItem(hSubItem);
	}
}


void CCustomDetail::OnTvnItemexpandingTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	if (pNMTreeView->action == TVE_COLLAPSE){
		*pResult = 1;
		return;
	}

	*pResult = 0;
}
