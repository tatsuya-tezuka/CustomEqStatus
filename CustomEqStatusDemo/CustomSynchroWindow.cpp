#include "stdafx.h"
#include "CustomSynchroWindow.h"

CCustomSynchroWindow::CCustomSynchroWindow()
{
	mGroupWindowList.clear();
}


CCustomSynchroWindow::~CCustomSynchroWindow()
{
}

/*============================================================================*/
/*! ウィンドウ連動

-# 画面連結の設定

@param	group		グループ番号(1-)
@param	pbase		ウィンドウハンドル

@retval
*/
/*============================================================================*/
void CCustomSynchroWindow::Set(UINT group, CWnd* pbase)
{
	GroupInfo search = GroupInfo(LOWORD(group), pbase);
	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr != mGroupWindowList.end()) {
		vector<GroupInfo>::iterator itrlist;
		itrlist = std::find((*itr).second.begin(), (*itr).second.end(), search);
		if (itrlist == (*itr).second.end()) {
			(*itr).second.push_back(search);
		}
	}
	else {
		vector<GroupInfo> temp;
		temp.push_back(search);
		mGroupWindowList.insert(map<UINT, vector<GroupInfo>>::value_type(HIWORD(group), temp));
		itr = mGroupWindowList.find(HIWORD(group));
	}

	sort((*itr).second.begin(), (*itr).second.end());
}

/*============================================================================*/
/*! ウィンドウ連動

-# 画面連結の先頭ウィンドウの取得

@param	group		グループ番号(1-)

@retval
*/
/*============================================================================*/
void CCustomSynchroWindow::Start()
{
	CRect rect;
	map< UINT, vector<GroupInfo> >::iterator itr;
	for (itr = mGroupWindowList.begin(); itr != mGroupWindowList.end(); itr++) {
		vector<GroupInfo>::iterator itrlist;
		for (itrlist = (*itr).second.begin(); itrlist != (*itr).second.end(); itrlist++) {
			if ((*itrlist).innerno == 1 && (*itrlist).wnd) {
				(*itrlist).wnd->GetWindowRect(rect);
				Move(((*itr).first<<16 | (*itrlist).innerno), (*itrlist).wnd, rect);
				break;
			}
		}
	}
}

/*============================================================================*/
/*! ウィンドウ連動

-# 任意のウィンドウの移動

@param	group		グループ番号(1-)
@param	pbase		移動対象のウィンドウ
@param	rectbase	移動対象のウィンドウ矩形

@retval
*/
/*============================================================================*/
void CCustomSynchroWindow::Move(UINT group, CWnd* pbase, CRect rectbase)
{
	CRect rect, rc;
	rect = rectbase;

	GroupInfo search = GroupInfo(LOWORD(group), pbase);

	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr == mGroupWindowList.end()) {
		// 対象グループが存在しないので何もしない
		return;
	}
	vector<GroupInfo>::iterator itrLeft;
	itrLeft = std::find((*itr).second.begin(), (*itr).second.end(), search);
	if (itrLeft == (*itr).second.end()) {
		// 対象ウィンドウが存在しないので何もしない
		return;
	}

	// 対象ウィンドウの左側移動
	vector<GroupInfo>::reverse_iterator itrRight;
	bool bfind = false;
	rect = rectbase;
	for (itrRight = (*itr).second.rbegin(); itrRight != (*itr).second.rend(); itrRight++) {
		if (bfind == true) {
			(*itrRight).wnd->GetWindowRect(rc);
			rect.right = rect.left + (GetSystemMetrics(SM_CXSIZEFRAME) * 3);
			rect.left = rect.right - rc.Width();
			(*itrRight).wnd->MoveWindow(rect.left, rect.top, rc.Width(), rc.Height());
			SetWindowPos((*itrRight).wnd->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE| SWP_NOACTIVATE);
			//rect.right -= rc.Width();
		}
		else {
			if ((*itrRight).wnd == pbase) {
				bfind = true;
			}
		}
	}

	// 対象ウィンドウの右側移動
	bfind = false;
	rect = rectbase;
	for (itrLeft = (*itr).second.begin(); itrLeft != (*itr).second.end(); itrLeft++) {
		if (bfind == true) {
			(*itrLeft).wnd->GetWindowRect(rc);
			rect.right -= (GetSystemMetrics(SM_CXSIZEFRAME) * 3);
			(*itrLeft).wnd->MoveWindow(rect.right, rect.top, rc.Width(), rc.Height());
			SetWindowPos((*itrLeft).wnd->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE| SWP_NOACTIVATE);
			rect.right += rc.Width();
		}
		else {
			if ((*itrLeft).wnd == pbase) {
				bfind = true;
			}
		}
	}
}

/*============================================================================*/
/*! ウィンドウ連動

-# 任意のウィンドウを閉じる

@param	group		グループ番号(1-)
@param	pbase		移動対象のウィンドウ

@retval
*/
/*============================================================================*/
void CCustomSynchroWindow::Close(UINT group, CWnd* pbase)
{
	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr == mGroupWindowList.end()) {
		// 対象グループが存在しないので何もしない
		return;
	}

	vector<GroupInfo>::iterator itrlist;
	for (itrlist = (*itr).second.begin(); itrlist != (*itr).second.end(); itrlist++) {
		if ((*itrlist).wnd != pbase) {
			(*itrlist).wnd->PostMessageW(WM_CLOSE, 0, 0);
		}
	}
}

/*============================================================================*/
/*! ウィンドウ連動

-# 任意のウィンドウの削除

@param	group		グループ番号(1-)
@param	pbase		対象のウィンドウ

@retval
*/
/*============================================================================*/
void CCustomSynchroWindow::Clear(UINT group, CWnd* pbase/* = NULL*/)
{
	GroupInfo search = GroupInfo(LOWORD(group), pbase);

	if (group == 0) {
		// 全て削除
		mGroupWindowList.clear();
		return;
	}

	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr != mGroupWindowList.end()) {
		if (pbase == NULL) {
			// 全て削除する
			(*itr).second.clear();
		}
		else {
			vector<GroupInfo>::iterator itrlist;
			itrlist = std::find((*itr).second.begin(), (*itr).second.end(), search);
			if (itrlist != (*itr).second.end()) {
				(*itr).second.erase(itrlist);
			}
		}
	}
}

/*============================================================================*/
/*! ウィンドウ連動

-# 任意のウィンドウが画面連結されているかの確認

@param	group		グループ番号(1-)
@param	pbase		対象のウィンドウ

@retval
*/
/*============================================================================*/
BOOL CCustomSynchroWindow::IsSync(UINT group, CWnd* pbase)
{
	GroupInfo search = GroupInfo(LOWORD(group), pbase);

	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr != mGroupWindowList.end()) {
		vector<GroupInfo>::iterator itrlist;
		itrlist = std::find((*itr).second.begin(), (*itr).second.end(), search);
		if (itrlist != (*itr).second.end()) {
			return TRUE;
		}
	}
	return FALSE;
}

/*============================================================================*/
/*! ウィンドウ連動

-# 画面連結数の取得

@param	group		グループ番号(1-)

@retval
*/
/*============================================================================*/
int CCustomSynchroWindow::Size(UINT group)
{
	map< UINT, vector<GroupInfo> >::iterator itr;
	itr = mGroupWindowList.find(HIWORD(group));
	if (itr != mGroupWindowList.end()) {
		return (int)(*itr).second.size();
	}

	return 0;
}

/*============================================================================*/
/*! ウィンドウ連動

-# 任意のウィンドウのグループ番号取得

@param	pbase		移動対象のウィンドウ

@retval
*/
/*============================================================================*/
UINT CCustomSynchroWindow::Group(CWnd* pbase)
{
	map< UINT, vector<GroupInfo> >::iterator itr;
	for (itr = mGroupWindowList.begin(); itr != mGroupWindowList.end(); itr++) {
		vector<GroupInfo>::iterator itrlist;
		for (itrlist = (*itr).second.begin(); itrlist != (*itr).second.end(); itrlist++) {
			if ((*itrlist).wnd == pbase) {
				return (*itr).first;
			}
		}

	}
	return 0; // グループなし
}
