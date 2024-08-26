// CustomDetail.cpp : 実装ファイル
//

#include "stdafx.h"
#include "EquipmentStatusDemo2013.h"
#include "CustomDetail.h"
#include "afxdialogex.h"


// CCustomDetail ダイアログ

IMPLEMENT_DYNAMIC(CCustomDetail, CCustomDialogBase)

CCustomDetail::CCustomDetail(CWnd* pParent /*=NULL*/, bool bRestore/* = false*/)
: CCustomDialogBase(CCustomDetail::IDD, pParent)
{
	// 復元フラグ
	mRestore = bRestore;
	
	// ツリーコントロールフォントの作成
	mTreeFont.CreateStockObject(DEFAULT_GUI_FONT);
}

CCustomDetail::~CCustomDetail()
{
	mTreeFont.DeleteObject();
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
END_MESSAGE_MAP()


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
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
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
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CCustomDialogBase::OnHScroll(nSBCode, nPos, pScrollBar);
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
	mTreeCtrl.Create(this, mTreeFont);

	// コールバック関数の登録
	mTreeCtrl.SetClickCallback(messageClick);
	//mTreeCtrl.SetDragCallback(messageDrag);

	if (mRestore == true){
		restoreRoot();
	}
	else{
		createRoot();
	}

	// アイテムインデントの設定
	mTreeCtrl.SetIndent(5);

	mTreeCtrl.UpdateColumns();

	SetControlInfo(IDC_TREE_CTRL, ANCHORE_LEFTTOP | RESIZE_BOTH);
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

	CRect rect;
	mTreeCtrl.GetWindowRect(rect);
	ScreenToClient(rect);
	rect.right += dx;
	rect.bottom += dy;
	mTreeCtrl.MoveWindow(rect);
	Invalidate();
	return TRUE;
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
#ifndef _DEMO
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
#endif

	return FALSE;
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
#ifndef _DEMO
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
#endif
	return FALSE;
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
	mTreeFont.GetLogFont(&tn_root->GetColor().font);

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
	mTreeFont.GetLogFont(&item_node->GetColor().font);

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
	mTreeFont.GetLogFont(&item_node->GetColor().font);

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
	mTreeFont.GetLogFont(&item_node->GetColor().font);

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

	// ルート復元
	HTREEITEM rootItem = mTreeCtrl.InsertItem(pnode->GetWindowInfo().title, NULL, NULL, TVI_ROOT);
	mTreeCtrl.SetItemData(rootItem, (LPARAM)rootItem);
	pnode->SetTreeItem(rootItem);
	setNodeWindowInfo(pnode, eTreeItemType_Title, pnode->GetWindowInfo().title, NULL);
	// ツリーコントロールの背景色設定
	mTreeCtrl.SetBkColor(pnode->GetColor().back);

	// ツリーコントロールのフォント設定
	if (pnode->GetColor().font.lfHeight != 0){
		mTreeFont.DeleteObject();
		mTreeFont.CreateFontIndirect(&pnode->GetColor().font);
		mTreeCtrl.SetFont(&mTreeFont);
	}
	else{
		mTreeFont.GetLogFont(&pnode->GetColor().font);
	}

	restoreNode(pnode, rootItem);

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
