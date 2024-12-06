/******************************************************************************
* @file    CustomManager.cpp
* @brief   【カスタマイズ機能】設備状況管理クラス
* @auther  SCC
* @date    2024/08/26 新規作成
* @par
******************************************************************************/
// CustomManager.cpp : 実装ファイル
//
#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomManager.h"
#include "afxdialogex.h"


// CCustomManager ダイアログ

IMPLEMENT_DYNAMIC(CCustomManager, CCustomDialogBase)

CCustomManager::CCustomManager(CWnd* pParent /*=NULL*/)
: CCustomDialogBase(CCustomManager::IDD, pParent)
, mSelectType(0)
{

}

CCustomManager::~CCustomManager()
{
}

void CCustomManager::DoDataExchange(CDataExchange* pDX)
{
	CCustomDialogBase::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_USER, mSelectType);
	DDX_Control(pDX, IDC_LIST_MANAGER, mManagerList);
}


BEGIN_MESSAGE_MAP(CCustomManager, CCustomDialogBase)
	ON_BN_CLICKED(IDC_RADIO_USER, &CCustomManager::OnBnClickedRadioUser)
	ON_BN_CLICKED(IDC_RADIO_MASTER, &CCustomManager::OnBnClickedRadioMaster)
	//ON_NOTIFY(NM_RCLICK, IDC_LIST_MANAGER, &CCustomManager::OnNMRClickListManager)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MANAGER, &CCustomManager::OnNMDblclkListManager)
	ON_COMMAND(ID_MANAGER_NEW, &CCustomManager::OnManagerNew)
	ON_COMMAND(ID_MANAGER_DELETE, &CCustomManager::OnManagerDelete)
	ON_COMMAND(ID_MANAGER_SHOW, &CCustomManager::OnManagerShow)
	ON_COMMAND(ID_MANAGER_CREATE, &CCustomManager::OnManagerCreate)
	ON_WM_CONTEXTMENU()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENUMANAGER_CLOSE, &CCustomManager::OnMenumanagerClose)
	ON_NOTIFY(LVN_GETINFOTIP, IDC_LIST_MANAGER, &CCustomManager::OnLvnGetInfoTipListManager)
	ON_COMMAND(ID_MANGROUP_RENAME, &CCustomManager::OnMangroupRename)
	ON_COMMAND(ID_MANGROUP_RESET, &CCustomManager::OnMangroupReset)
	ON_COMMAND(ID_MANGROUP_SHOW, &CCustomManager::OnMangroupShow)
END_MESSAGE_MAP()

// CCustomManager メッセージ ハンドラー

/*============================================================================*/
/*! 設備詳細管理

-# 初期化

@param

@retval
*/
/*============================================================================*/
BOOL CCustomManager::OnInitDialog()
{
	CCustomDialogBase::OnInitDialog();

	mManagerList.CreateGroupControl(this);
	SetControlInfo(IDC_LIST_MANAGER, ANCHORE_LEFTTOP | RESIZE_BOTH);

	// リストに登録
	if (theApp.GetCustomControl().GetDataManager().GetTreeNode().size() != 0){
		createItem((int)eSelectUser, true);
		UpdateGroup();
	}

	//UpdateGroup();

	CenterWindow(GetDesktopWindow());

	SetWindowText(mMessage_Title_CustomManager);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
/*============================================================================*/
/*! 設備詳細管理

-# ウィンドウの表示/非表示イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CCustomDialogBase::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE){
		// 表示
		UpdateData(TRUE);
		if (theApp.GetCustomControl().GetDataManager().GetTreeNode().size() != 0){
			createItem((int)mSelectType);
			UpdateGroup();
		}
		CRect rect;
		GetWindowRect(rect);
		theApp.GetCustomControl().UpdateCustomManagerPoint(rect);
	}
}

/*============================================================================*/
/*! 設備詳細管理

-# ウィンドウの移動

@param
@retval

*/
/*============================================================================*/
void CCustomManager::OnMove(int x, int y)
{
	CCustomDialogBase::OnMove(x, y);

	CRect rect;
	GetWindowRect(rect);
	theApp.GetCustomControl().UpdateCustomManagerPoint(rect);
}

/*============================================================================*/
/*! 設備詳細管理

-# 「ユーザ」ボタン押下イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnBnClickedRadioUser()
{
	UpdateData(TRUE);
	if (theApp.GetCustomControl().GetDataManager().GetTreeNode().size() != 0){
		createItem((int)mSelectType);
		UpdateGroup();
	}
	UpdateData(FALSE);
}
/*============================================================================*/
/*! 設備詳細管理

-# 「マスタ」ボタン押下イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnBnClickedRadioMaster()
{
	UpdateData(TRUE);
	if (theApp.GetCustomControl().GetDataManager().GetTreeNode().size() != 0){
		createItem((int)mSelectType);
		UpdateGroup();
	}
}
/*============================================================================*/
/*! 設備詳細管理

-# ポップアップメニュー表示イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (pWnd != (CWnd*)&mManagerList) {
		// リストコントロール上でなければ何もしない
		return;
	}

	CPoint pt = point;
	mManagerList.ScreenToClient(&pt);

	int nItem, colnum;
	nItem = mManagerList.HitTestEx(pt, &colnum);
	TRACE("# OnContextMenu : INDEX=%d COL=%d\n", nItem, colnum);
	if (nItem < 0) {
		// リストコントロール上でなければ何もしない
		//return;
	}

	// アイテム、グループ混在かをチェック
	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	int nGroup = -1;
	while (pos) {
		int item = mManagerList.GetNextSelectedItem(pos);
		if (nGroup >= 0 && mManagerList.GetRowGroupId(item) != nGroup) {
			return;
		}
		nGroup = mManagerList.GetRowGroupId(item);
	}

	UINT nMenuRes = IDR_POPUP_MANAGER;

	bool bUser = false;
	bool bGroup = false;
	bool bNoGroup = false;
	mMenuSelectGroupID = -1;
	if (mManagerList.IsGroupViewEnabled() == TRUE) {
		// ユーザ
		bUser = true;
		TRACE("# OnContextMenu : USER\n");
		if (point.x != -1 && point.y != -1) {
			int nGroupId = mManagerList.GroupHitTest(pt);
			if (nGroupId >= 0) {
				// グループラベル
				CString str = mManagerList.GetGroupHeader(nGroupId);
				if (str == mNoGroupText) {
					return;
				}
				bGroup = true;
				nMenuRes = IDR_POPUP_MANAGER_GROUP;
				mMenuSelectGroupID = nGroupId;
				TRACE("# OnContextMenu : USER GroupID=%d\n", nGroupId);
			}
			else {
				// リスト項目
				if (nItem >= 0) {
					nGroupId = mManagerList.GetRowGroupId(nItem);
					bNoGroup = (nGroupId == 0);
					TRACE("# OnContextMenu : USER Item=%d NoGroup=%d GroupID=%d\n", nItem, bNoGroup, nGroupId);
				}
			}
		}
	}
	else {
		// マスタ
		if (nItem < 0) {
			return;
		}
		TRACE("# OnContextMenu : MASTER\n");
	}

	CMenu menu;
	if (!menu.LoadMenu(nMenuRes)) {
		ASSERT(FALSE);
		return;
	}

	CMenu* pMenu = menu.GetSubMenu(0);
	ASSERT(pMenu);

	if (nItem < 0) {
		pMenu->EnableMenuItem(ID_MANAGER_NEW, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_MANAGER_DELETE, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_MANAGER_SHOW, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_MANAGER_CREATE, MF_BYCOMMAND | MF_GRAYED);
	}
	else {
		updateMenuItemStatus(pMenu, bUser, bGroup, bNoGroup);
	}

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}
/*============================================================================*/
/*! 設備詳細管理

-# リストコントロールでのマウス右クリックイベント（※メニュー表示）

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnNMRClickListManager(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	*pResult = 0;

	CPoint pos;
	if (::GetCursorPos(&pos) == FALSE)
	{
		ASSERT(FALSE);
		return;
	}

	mManagerList.ScreenToClient(&pos);

	UINT nMenuRes = IDR_POPUP_MANAGER;
	int item = mManagerList.HitTest(pos);
	if(item >= 0){
		// アイテムからノード情報を取得する
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		if (HIWORD(pnode->GetManager().groupno) != 0) {
			nMenuRes = IDR_POPUP_MANAGER_GROUP;
		}
		TRACE("[MANAGER]Rclick Item=%d Group=%08X)\n", item, pnode->GetManager().groupno);
	}
	else {
		TRACE("[MANAGER]Rclick Item=--)\n");
	}

	CMenu menu;
	if (!menu.LoadMenu(nMenuRes)){
		ASSERT(FALSE);
		return;
	}

	UpdateData(TRUE);

	CMenu* pMenu = menu.GetSubMenu(0);
	ASSERT(pMenu);

	//updateMenuItemStatus(pMenu);

	mManagerList.ClientToScreen(&pos);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);
}
/*============================================================================*/
/*! 設備詳細管理

-# リストコントロールでのマウスダブルクリック（※対象ウィンドウのアクティブ化）

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnNMDblclkListManager(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;
	if (nItem < 0)
		return;

	CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(nItem);
	if (pnode == NULL)
		return;

	if (HIWORD(pnode->GetManager().groupno) != 0) {
		UINT nGroup = 0;
		for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
			CTreeNode* group = (CTreeNode*)mManagerList.GetItemData(item);
			if (HIWORD(group->GetManager().groupno) == HIWORD(pnode->GetManager().groupno)) {
				showCustomDetail(item, theApp.GetCustomControl().GetCascadePoint());
			}
		}
		UpdateGroup();
	}
	else {
		OnManagerShow();
	}

	TRACE("##### OnNMDblclkListManager\n");

	*pResult = 0;
}
/*============================================================================*/
/*! 設備詳細管理

-# ポップアップメニュー「作成」イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerNew()
{
#if _DEMO_PHASE < 110
	return;
#endif
	UpdateData(TRUE);
	createEqDetail(NULL);
	mManagerList.GroupByColumn(eManagerGroup, (mSelectType == eSelectUser) ? TRUE : FALSE);
	SyncEditMode();
	UpdateGroup();
}
/*============================================================================*/
/*! 設備詳細管理

-# ポップアップメニュー「削除」イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerDelete()
{
	CString msg;
	msg.Format(mMessage_ManagerDelete);
	if (MessageBox(msg, mMessage_Title_CustomManager, MB_YESNO | MB_ICONQUESTION) == IDNO) {
		return;
	}

#if _DEMO_PHASE < 110
	return;
#endif
	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = mManagerList.GetNextSelectedItem(pos);
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(nItem);
		if (pnode == NULL)
			continue;

		//CString msg;
		//msg.Format(mMessage_DetailDelete, pnode->GetEquipment().title);
		//if (MessageBox(msg, mMessage_Title_CustomManager, MB_YESNO | MB_ICONQUESTION) == IDNO) {
		//	continue;
		//}

		// 設備制御画面の削除
		CString xmlfile = CString(pnode->GetXmlFileName());
		theApp.GetCustomControl().GetDataManager().DeleteItemWnd(pnode->GetEquipment().wnd);
		theApp.GetCustomControl().GetDataManager().DeleteItemNode(pnode);
		DeleteFile(xmlfile);

		mManagerList.DeleteItem(nItem);
		pos = mManagerList.GetFirstSelectedItemPosition();
	}
}
/*============================================================================*/
/*! 設備詳細管理

-# ポップアップメニュー「表示」イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerShow()
{
	// 選択されているグループ内アイテムを取得する
	int nItem = -1;
	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	while (pos) {
		nItem = mManagerList.GetNextSelectedItem(pos);
		break;
	}
	if (nItem < 0)
		return;

	// 選択されているグループ番号を取得
	int nGroupId = mManagerList.GetRowGroupId(nItem);
	if (nGroupId <= 0) {
		// 選択されているアイテムを表示
		POSITION pos = mManagerList.GetFirstSelectedItemPosition();
		while (pos) {
			int nItem = mManagerList.GetNextSelectedItem(pos);
			showCustomDetail(nItem, theApp.GetCustomControl().GetCascadePoint());
		}
	}
	else {
		// グループ内のアイテム全てを表示させて画面連結させる
		for (nItem = 0; nItem < mManagerList.GetItemCount(); nItem++) {
			int group = mManagerList.GetRowGroupId(nItem);
			if (group == nGroupId) {
				showCustomDetail(nItem, theApp.GetCustomControl().GetCascadePoint());
			}
		}
		UpdateGroup();
	}

	TRACE("##### OnManagerShow\n");
}
/*============================================================================*/
/*! 設備詳細管理

-# ポップアップメニュー「画面連結作成」イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerCreate()
{
	// グループ名の取得
	CString str = _T("");
	CCustomGroupName dlg(str);
	while (1) {
		if (dlg.DoModal() == IDCANCEL)
			return;

		if (dlg.mGroupName.IsEmpty()) {
			MessageBox(mMessage_EmptyGroupName, mMessage_Title_CustomManager, MB_OK | MB_ICONWARNING);
			continue;
		}

		if (IsSameGroupName(dlg.mGroupName) == true) {
			MessageBox(mMessage_SameGroupName, mMessage_Title_CustomManager, MB_OK | MB_ICONWARNING);
			continue;
		}
		break;
	}

	// グループ番号は最大値を設定しても、再度グループIDは振りなおす
	UINT maxGroup = 100;
	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = mManagerList.GetNextSelectedItem(pos);
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(nItem);
		pnode->GetManager().groupno = MAKELONG(LOWORD(pnode->GetManager().groupno), maxGroup + 1);
		swprintf_s(pnode->GetManager().groupname, mNameSize, _T("%s"), (LPCTSTR)dlg.mGroupName);
	}
	createItem((int)eSelectUser);
	UpdateGroup();

	TRACE("##### OnManagerCreate\n");
}

/*============================================================================*/
/*! 設備詳細管理

-# グループポップアップメニュー「名称変更」イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnMangroupRename()
{
#if _DEMO_PHASE < 110
	return;
#endif
	CWaitCursor wait;

	CString str = mManagerList.GetGroupHeader(mMenuSelectGroupID);
	CCustomGroupName dlg(str);
	while (1) {
		if (dlg.DoModal() == IDCANCEL)
			return;

		if (dlg.mGroupName.IsEmpty()) {
			MessageBox(mMessage_EmptyGroupName, mMessage_Title_CustomManager, MB_OK | MB_ICONWARNING);
			continue;
		}

		if (dlg.mGroupName != str && IsSameGroupName(dlg.mGroupName) == true) {
			MessageBox(mMessage_SameGroupName, mMessage_Title_CustomManager, MB_OK | MB_ICONWARNING);
			continue;
		}
		break;
	}
	mManagerList.SetGroupHeader(mMenuSelectGroupID, dlg.mGroupName);

	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	while (pos) {
		int item = mManagerList.GetNextSelectedItem(pos);
		if (mMenuSelectGroupID == mManagerList.GetRowGroupId(item)) {
			CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
			swprintf_s(pnode->GetManager().groupname, mNameSize, _T("%s"), (LPCTSTR)dlg.mGroupName);
		}
	}
}

/*============================================================================*/
/*! 設備詳細管理

-# グループポップアップメニュー「画面連結解除」イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnMangroupReset()
{
	// 対象グループの先頭ウィンドウ位置を取得する
	CPoint point;
	bool bFirstGroup = mSyncWindow.GetTopPoint(MAKELONG(0, mMenuSelectGroupID), point);
	for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		if (HIWORD(pnode->GetManager().groupno) == mMenuSelectGroupID) {
			pnode->GetManager().groupno = MAKELONG(LOWORD(pnode->GetManager().groupno), 0);
			if (pnode->GetEquipment().wnd != NULL) {
				pnode->GetEquipment().wnd->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
			}
		}
	}
	createItem((int)eSelectUser);
	UpdateGroup();

	TRACE("##### OnMangroupReset\n");
}

/*============================================================================*/
/*! 設備詳細管理

-# グループポップアップメニュー「表示」イベント

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnMangroupShow()
{
#if _DEMO_PHASE < 100
	return;
#endif
	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = mManagerList.GetNextSelectedItem(pos);
		showCustomDetail(nItem, theApp.GetCustomControl().GetCascadePoint());
	}
	UpdateGroup();

	TRACE("##### OnMangroupShow\n");
}

/*============================================================================*/
/*! 設備詳細管理

-# グループ番号の最大値を取得する

@param

@retval
*/
/*============================================================================*/
UINT CCustomManager::GetGroupMaxNo()
{
	UINT nGroup = 0;
	for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
		// データのグループ番号から対象カラムのテキストを設定する
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		if (pnode->GetEquipment().kind == eTreeItemKind_User) {
			nGroup = __max(nGroup, HIWORD(pnode->GetManager().groupno));
		}
	}
	return nGroup;
}

/*============================================================================*/
/*! 設備詳細管理

-# グループ内番号の最大値を取得する

@param

@retval
*/
/*============================================================================*/
UINT CCustomManager::GetGroupInnerMaxNo()
{
	UINT nInnerNo = 0;
	for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
		// データのグループ番号から対象カラムのテキストを設定する
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		if (pnode->GetEquipment().kind == eTreeItemKind_User) {
			nInnerNo = __max(nInnerNo, LOWORD(pnode->GetManager().groupno));
		}
	}
	return nInnerNo;
}

/*============================================================================*/
/*! 設備詳細管理

-# 同名のグループ名が存在するかを確認する

@param

@retval
*/
/*============================================================================*/
bool CCustomManager::IsSameGroupName(CString groupName)
{
	for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
		// データのグループ番号から対象カラムのテキストを設定する
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		CString str = CString(pnode->GetManager().groupname);
		if (str == groupName)
			return true;
	}
	return false;
}

/*============================================================================*/
/*! 設備詳細管理

-# グループ番号の更新

@param		point	ドラッグ＆ドロップ時のベースウィンドウ位置

@retval
*/
/*============================================================================*/
void CCustomManager::UpdateGroup(UINT nGroupId/*=0*/, CPoint * point/* = NULL*/)
{
#if _DEMO_PHASE < 110
	return;
#endif

	mSyncWindow.Dump();

	// 1つでもグループウィンドウが表示されている場合は全てのグループウィンドウを表示させる
	ShowGroup();

	// 登録されているカスタム画面のグループ更新
	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;

	if (nGroupId != 0 && point != NULL) {
		for (itr = treedata.begin(); itr != treedata.end(); itr++) {
			if ((*itr)->GetEquipment().wnd == NULL)
				continue;

			// ドロップされたウィンドウに対しての処理
			if (HIWORD((*itr)->GetManager().groupno) == nGroupId){
				(*itr)->GetEquipment().wnd->SetWindowPos(NULL, point->x, point->y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
				TRACE("# UpdateGroup(Clear) : [%s] GroupNo=%d, GroupName=%s [%d,%d]\n", CStringA((*itr)->GetEquipment().title), HIWORD((*itr)->GetManager().groupno), CStringA((*itr)->GetManager().groupname), point->x, point->y);
			}
		}
	}

	// 全てのグループ情報を削除
	mSyncWindow.Clear(0);
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		if ((*itr)->GetEquipment().wnd == NULL)
			continue;

		if ((*itr)->GetEquipment().kind == eTreeItemKind_User && HIWORD((*itr)->GetManager().groupno) != 0) {
			mSyncWindow.Set((*itr)->GetManager().groupno, (*itr)->GetEquipment().wnd);
			CRect rect;
			(*itr)->GetEquipment().wnd->GetWindowRect(rect);
			TRACE("# UpdateGroup(Set) : [%s] GroupNo=%d, GroupName=%s [%d,%d]\n", CStringA((*itr)->GetEquipment().title), HIWORD((*itr)->GetManager().groupno), CStringA((*itr)->GetManager().groupname), rect.left, rect.top);
		}
	}

	mSyncWindow.Start();

	// ベースウィンドウを動作させる
	map< UINT, vector<GroupInfo> >& wlist = mSyncWindow.GetGroupWindowList();
	map< UINT, vector<GroupInfo> >::iterator witr;
	CRect rect;
	for (witr = wlist.begin(); witr != wlist.end(); witr++) {
		if ((*witr).second.size() != 0) {
			if ((*witr).second[0].wnd != 0) {
				(*witr).second[0].wnd->GetWindowRect(rect);
				TRACE("# UpdateGroup(Move) : [%d,%d]\n", rect.left, rect.top);
				mSyncWindow.Move(MAKELONG((*witr).second[0].innerno, (*witr).first), (*witr).second[0].wnd, rect);
			}
		}
	}

	TRACE("##### UpdateGroup\n");
}

/*============================================================================*/
/*! 設備詳細管理

-# グループウィンドウが1つでも表示されている場合は強制的に表示する

@param		ddGroup	ドラッグ＆ドロップ時の状態（上位ワード＝ドラッググループ 下位ワード＝ドロップグループ）

@retval
*/
/*============================================================================*/
void CCustomManager::ShowGroup()
{
	for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		CPoint point;
		if (pnode->GetEquipment().kind == eTreeItemKind_User && HIWORD(pnode->GetManager().groupno) != 0) {
			if (IsGroupVisible(HIWORD(pnode->GetManager().groupno)) == true) {
				if (pnode->GetEquipment().wnd == NULL || pnode->GetEquipment().wnd->IsWindowVisible() == false) {
					showCustomDetail(item, theApp.GetCustomControl().GetCascadePoint());
				}
			}
		}
	}

}

/*============================================================================*/
/*! 設備詳細管理

-# グループウィンドウが1つでも表示されているかをチェックする

@param

@retval
*/
/*============================================================================*/
bool CCustomManager::IsGroupVisible(UINT nGroup)
{
	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		if ((*itr)->GetEquipment().kind == eTreeItemKind_User && HIWORD((*itr)->GetManager().groupno) == nGroup) {
			if ((*itr)->GetEquipment().wnd != NULL && (*itr)->GetEquipment().wnd->IsWindowVisible())
				return true;
		}
	}
	return false;
}

/*============================================================================*/
/*! 設備詳細管理

-# リスト表示されている順番に通し番号を振りなおす

@param

@retval
*/
/*============================================================================*/
void CCustomManager::ResetGroupInnerNo()
{
	UINT groupno = mGroupRange;
	UINT maxgroup = GetGroupMaxNo() + 1;
	for (UINT group = 0; group < maxgroup; group++) {
		// 対象グループのグループ内番号を更新する
		int count = mManagerList.GetItemCount();
		for (int pos = 0; pos < count; pos++) {
			CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(pos);
			if (pnode->GetEquipment().kind == eTreeItemKind_User) {
				if (HIWORD(pnode->GetManager().groupno) == group) {
					pnode->GetManager().groupno = MAKELONG(groupno, group);
					groupno += mGroupRange;
				}
			}
		}
	}

	SyncEditMode();

	TRACE("##### ResetGroupInnerNo\n");
}

/*============================================================================*/
/*! 設備詳細管理

-# 設備詳細をリストへ登録する

@param	nSelect		ユーザorマスタ
@param	bResetInner	グループ内番号のリセット

@retval
*/
/*============================================================================*/
void CCustomManager::createItem(UINT nSelect, bool bResetInner/* = false*/)
{
	mManagerList.SetRedraw(FALSE);

	mManagerList.RemoveAllGroups();
	mManagerList.DeleteAllItems();

	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	bool bFirstRead = false;
	for (itr = treedata.begin(); itr != treedata.end(); itr++){
		int count = mManagerList.GetItemCount();
		if ((*itr)->GetEquipment().kind == nSelect){
			mManagerList.AddItem(count, 0, (*itr)->GetEquipment().title, (LPARAM)(*itr));
			mManagerList.AddItem(count, 1, (*itr)->GetManager().memo);
			mManagerList.AddItem(count, 2, (*itr)->GetManager().groupname);
			mManagerList.SetItemData(count, (LPARAM)(*itr));
			(*itr)->GetEquipment().manager = this;
			if (LOWORD((*itr)->GetManager().groupno) == 0) {
				bFirstRead = true;
			}
		}
	}
	if (bFirstRead == true) {
		ResetGroupInnerNo();
	}

	mManagerList.SetRedraw(TRUE);

	// グループの再作成を行う（グループ番号も振りなおす）
	mManagerList.GroupByColumn(eManagerGroup, (nSelect == eSelectUser) ? TRUE : FALSE);
	SyncEditMode();

	if (nSelect == eSelectUser) {
		mManagerList.SortItem();
		mManagerList.SortGroup();
	}

	// グループ内番号を振りなおす
	if (bResetInner == true) {
		ResetGroupInnerNo();
	}

	//UpdateGroup();

#ifdef _DEBUG
	for (int pos = 0; pos < mManagerList.GetItemCount(); pos++){
		CTreeNode* pnode = (CTreeNode*)(mManagerList.GetItemData(pos));
		TRACE("# createItem Dump : [%d] Name=%s, GroupNo=%d, Inner=%d\n", pos, CStringA(pnode->GetEquipment().title), HIWORD(pnode->GetManager().groupno), LOWORD(pnode->GetManager().groupno));
	}
#endif

	mSelectType = nSelect;
	UpdateData(FALSE);

	TRACE("##### createItem\n");
	mSyncWindow.Dump();
}

/*============================================================================*/
/*! 設備詳細管理

-# 設備詳細の作成

@param

@retval
*/
/*============================================================================*/
void CCustomManager::createEquipment()
{
	// 登録されているカスタム画面の作成
	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		// 設備詳細画面の作成
		CCustomDetail* pitem = theApp.GetCustomControl().CreateEquipment((*itr));
	}
}

/*============================================================================*/
/*! カスタマイズ管理

-# 設備詳細の登録

@param		node		詳細画面を作成する元ノード情報（新規作成時はNULL）
@retval

*/
/*============================================================================*/
void CCustomManager::createEqDetail(CTreeNode* node/*=NULL*/)
{
	// 設備詳細画面の作成
	CCustomDetail* pitem = theApp.GetCustomControl().CreateEquipment(NULL, eTreeItemMode_Edit);

	CTreeNode* pedit = theApp.GetCustomControl().GetDataManager().SearchWndNode(pitem);
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(pitem, false);
	if (pnode != NULL) {
		UINT maxno = GetGroupInnerMaxNo();
		pnode->GetManager().groupno = MAKELONG(maxno + mGroupRange, 0);
		SyncEditMode();
	}
	// createItemを実行することで管理画面に表示される
	createItem((int)mSelectType);

	UpdateGroup();
	if (pedit->GetEquipment().wnd != NULL) {
		CPoint point = theApp.GetCustomControl().GetCascadePoint();
		pedit->GetEquipment().wnd->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pedit->GetEquipment().wnd->ShowWindow(SW_SHOW);
	}
}
/*============================================================================*/
/*! 設備詳細管理

-# メニュー項目の状態を更新する（※メニュー表示）

@param	pMenu		メニューハンドル
@param	bUser		trueの場合ユーザ
@param	bGroup		trueの場合グループヘッダー
@param	bNoGroup	trueの場合[No Group]

@retval
*/
/*============================================================================*/
void CCustomManager::updateMenuItemStatus(CMenu* pMenu, bool bUser, bool bGroup, bool bNoGroup)
{
	UINT count = pMenu->GetMenuItemCount();
	for (UINT pos = 0; pos < count; pos++){
		UINT state = pMenu->GetMenuState(pos, MF_BYPOSITION);
		if (state & MF_POPUP){
			CMenu* pPopup = pMenu->GetSubMenu(pos);
			updateMenuItemStatus(pPopup, bUser, bGroup, bNoGroup);
		}
		else{
			bool bSelect = (mManagerList.GetSelectedCount() > 0) ? true : false;
			bool bMultiSelect = (mManagerList.GetSelectedCount() > 1) ? true : false;
			UINT nSelectCount = mManagerList.GetSelectedCount();
			pMenu->EnableMenuItem(ID_MANAGER_NEW, MF_BYCOMMAND | (bUser) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_DELETE, MF_BYCOMMAND | (bNoGroup && bUser && (bSelect || bMultiSelect)) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_SHOW, MF_BYCOMMAND | (bSelect || bMultiSelect) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_CREATE, MF_BYCOMMAND | (bNoGroup && bUser && bMultiSelect && nSelectCount <= mMaxEqSyncNum) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANGROUP_RENAME, MF_BYCOMMAND | (bGroup) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANGROUP_RESET, MF_BYCOMMAND | (bUser) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANGROUP_SHOW, MF_BYCOMMAND | (bSelect || bMultiSelect) ? MF_ENABLED : MF_GRAYED);
		}
	}
}

/*============================================================================*/
/*! 設備詳細管理

-# マウス左ボタン押下

@param
@retval

*/
/*============================================================================*/
void CCustomManager::OnLButtonDown(UINT nFlags, CPoint point)
{
	int pos = 0, index;
	int colnum, strpos = 0;

	if ((index = mManagerList.HitTestEx(point, &colnum)) != -1 && colnum != 0){
		UINT flag = LVIS_SELECTED;
		if ((mManagerList.GetItemState(index, flag) & flag) == flag){
			if (mManagerList.EditExecute(index, colnum) == false){
				mManagerList.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
		else{
			mManagerList.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}

	CCustomDialogBase::OnLButtonDown(nFlags, point);
}

/*============================================================================*/
/*! 設備詳細管理

-# メッセージ処理

@param
@retval

*/
/*============================================================================*/
LRESULT CCustomManager::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CTreeNode* pnode;

	switch (message) {
	case	eUserMessage_Manager_Update:
		updateItemData(lParam);
		break;
	case	eUserMessage_Manager_Reset:
		/*
		wParam : ドロップ時のドロップ先グループ番号
		lParam : 呼出元 0=設備詳細 1=カスタマイズ管理
		*/
		if (lParam == 0 && mSelectType != eSelectUser) {
			break;
		}

		if (lParam == 0) {
			createItem((int)eSelectUser, true);
			UpdateGroup();
		}
		else if (lParam == 1) {
			if (wParam == 0) {
				createItem((int)eSelectUser, true);
				UpdateGroup();
			}
			else {
				CString str;
				str = mManagerList.GetGroupHeader((int)wParam);
				CPoint point;
				bool bFirstGroup = mSyncWindow.GetTopPoint(MAKELONG(0, wParam), point);
				TRACE("# Drop Point : %d, %d\n", point.x, point.y);
				createItem((int)eSelectUser, true);
				UINT nGroupId = mManagerList.GetHeaderGroupId(str);
				UpdateGroup(nGroupId, &point);
			}
		}

		break;
	case	eUserMessage_Manager_Delete:
		pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode((CWnd*)lParam);
		theApp.GetCustomControl().GetDataManager().DeleteItemWnd((CWnd*)lParam);
		//mSyncWindow.Close(pnode->GetWindowInfo().groupno, (CWnd*)lParam);
		if (wParam == 1) {
			theApp.GetCustomControl().GetDataManager().DeleteItemNode(pnode);
			createItem(mSelectType);
			UpdateGroup();
		}
		break;
	default:
		return CCustomDialogBase::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}

/*============================================================================*/
/*! 設備詳細管理

-# 設備詳細の更新

@param
@retval

*/
/*============================================================================*/
void CCustomManager::updateItemData(LPARAM lParam)
{
	int count = mManagerList.GetItemCount();
	for (int i = 0; i < count; i++) {
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(i);
		mManagerList.SetItemText(i, 0, pnode->GetEquipment().title);
	}
	mManagerList.Invalidate();
}

/*============================================================================*/
/*! 設備詳細管理

-# ウィンドウ「X」ボタン押下

@param
@retval

*/
/*============================================================================*/
void CCustomManager::OnClose()
{
	OnMenumanagerClose();
}

/*============================================================================*/
/*! 設備詳細管理

-# 備考欄の更新

@param
@retval

*/
/*============================================================================*/
void CCustomManager::updateXmlFile()
{
	int count = mManagerList.GetItemCount();
	for (int i = 0; i < count; i++) {
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(i);
		swprintf_s(pnode->GetManager().memo, mTitleSize, _T("%s"), (LPCTSTR)mManagerList.GetItemText(i, 1));

		CMarkup xml;
		xml.Load(pnode->GetXmlFileName());
		xml.FindElem(_T("ROOT"));
		xml.IntoElem();
		xml.FindElem(_T("EQUIPMENT"));
		xml.IntoElem();
		xml.FindElem(_T("EQUIPMENTINFO"));
		xml.IntoElem();

		xml.FindElem(_T("TYPE"));
		UINT type = _wtoi(xml.GetData());
		if (type == eTreeItemType_Title) {
			xml.FindElem(_T("TITLE"));
			// 「備考」保存
			if (xml.FindElem(_T("MEMO")) == false) {
				xml.AddElem(_T("MEMO"), mManagerList.GetItemText(i, 1));
			}
			else {
				xml.SetData(mManagerList.GetItemText(i, 1));
			}
			// グループ番号保存
			if (xml.FindElem(_T("GROUPNO")) == false) {
				xml.AddElem(_T("GROUPNO"), pnode->GetManager().groupno);
			}
			else {
				xml.SetData(pnode->GetManager().groupno);
			}
			// グループ名称保存
			if (xml.FindElem(_T("GROUPNAME")) == false) {
				xml.AddElem(_T("GROUPNAME"), pnode->GetManager().groupname);
			}
			else {
				xml.SetData(pnode->GetManager().groupname);
			}
		}

		xml.Save(pnode->GetXmlFileName());
	}
}

/*============================================================================*/
/*! 設備詳細管理

-# 指定アイテムの設備詳細を開く

@param	nItem		アイテム番号
@param	point		表示開始位置

@retval
*/
/*============================================================================*/
void CCustomManager::showCustomDetail(int nItem, CPoint point)
{
	CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(nItem);
	showCustomDetail(pnode, point);
}

/*============================================================================*/
/*! 設備詳細管理

-# 指定アイテムの設備詳細を開く

@param	pnode		ノード情報
@param	point		表示開始位置

@retval
*/
/*============================================================================*/
void CCustomManager::showCustomDetail(CTreeNode* pnode, CPoint point)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomManager"), _T("showCustomDetail"), CString(pnode->GetEquipment().title), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	bool bEditMode = false;
	if (pnode != NULL) {
		if (pnode->GetEquipment().wnd == NULL) {
			CCustomDetail* pitem = theApp.GetCustomControl().CreateEquipment(pnode);
			if (pitem == NULL)
				return;

			pnode->GetEquipment().wnd = pitem;
			if (pnode->GetEquipment().kind == eTreeItemKind_User)
				bEditMode = true;

			if (pnode->GetEquipment().wnd != NULL) {
				pnode->GetEquipment().wnd->ShowWindow(SW_HIDE);
				pnode->GetEquipment().wnd->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
		}
		else {
			//pnode->GetEquipment().wnd->GetActiveWindow();
		}
		pnode->GetEquipment().wnd->ShowWindow(SW_SHOWNA);
		if (bEditMode == true) {
			// ダブルクリック時は常に編集モードとする
			pnode->GetEquipment().wnd->PostMessage(eUserMessage_Detail_Mode, 0, (LPARAM)eTreeItemMode_Edit);
		}
	}
}

/*============================================================================*/
/*! 設備詳細管理

-# カスタム管理画面を閉じる

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnMenumanagerClose()
{
	CCustomDialogBase::OnClose();

	if (mMessageRet == IDCANCEL)
		return;

	updateXmlFile();
}

/*============================================================================*/
/*! 設備詳細管理

-# 通常ノードと編集ノードのグループ情報を同期させる

@param

@retval
*/
/*============================================================================*/
void CCustomManager::SyncEditMode()
{
	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		if ((*itr)->GetEquipment().wnd == NULL)
			continue;

		CTreeNode* pedit = theApp.GetCustomControl().GetDataManager().SearchWndNode((*itr)->GetEquipment().wnd, true);
		if (pedit == NULL)
			continue;

		pedit->GetManager().groupno = (*itr)->GetManager().groupno;
		swprintf_s(pedit->GetManager().groupname, mNameSize, _T("%s"), (LPCTSTR)(*itr)->GetManager().groupname);
	}
}










void CCustomManager::OnLvnGetInfoTipListManager(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMLVGETINFOTIP>(pNMHDR);
	*pResult = 0;

	CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(pGetInfoTip->iItem);
	if (pnode == NULL) {
		return;
	}
#ifdef _DEBUG
	int nGroupId = mManagerList.GetRowGroupId(pGetInfoTip->iItem);
	mToolText.Format(_T("(%d)NODE_GROUP=%d NODE_NO=%d NODE_GROUP_NAME=%s NAME=%s"), nGroupId, HIWORD(pnode->GetManager().groupno), LOWORD(pnode->GetManager().groupno), pnode->GetManager().groupname, pnode->GetEquipment().title);
	TRACE("%s\n", CStringA(mToolText));
#endif
	pGetInfoTip->pszText = (LPWSTR)(LPCTSTR)mToolText;
	pGetInfoTip->cchTextMax = mToolText.GetLength();

}
