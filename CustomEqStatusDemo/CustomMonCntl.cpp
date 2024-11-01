// CustomMonCntl.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomMonCntl.h"
#include "afxdialogex.h"


// CCustomMonCntl ダイアログ

IMPLEMENT_DYNAMIC(CCustomMonCntl, CCustomDialogBase)

CCustomMonCntl::CCustomMonCntl(CWnd* pParent /*=nullptr*/)
	: CCustomDialogBase(IDD_DIALOG_MONCNTL, pParent)
	, mSearchText(_T(""))
{

}

CCustomMonCntl::~CCustomMonCntl()
{
}

void CCustomMonCntl::DoDataExchange(CDataExchange* pDX)
{
	CCustomDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DATA, mTreeData);
	DDX_Control(pDX, IDC_LIST_DATA, mListData);
	DDX_Control(pDX, IDC_EDIT_SEARCH, mEditSearch);
	DDX_Text(pDX, IDC_EDIT_SEARCH, mSearchText);
	DDV_MaxChars(pDX, mSearchText, 64);
}


BEGIN_MESSAGE_MAP(CCustomMonCntl, CCustomDialogBase)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DATA, &CCustomMonCntl::OnTvnSelchangedTreeData)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DATA, &CCustomMonCntl::OnLvnGetdispinfoListData)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, &CCustomMonCntl::OnEnChangeEditSearch)
END_MESSAGE_MAP()


// CCustomMonCntl メッセージ ハンドラー

/*============================================================================*/
/*! 監視・制御一覧

-# 初期化

@param

@retval なし
*/
/*============================================================================*/
BOOL CCustomMonCntl::OnInitDialog()
{
	CCustomDialogBase::OnInitDialog();

	createControl();

	CenterWindow(GetDesktopWindow());
	mListData.SetDragFormat(CCustomDropObject::DF_MONITOR);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/*============================================================================*/
/*! 監視・制御一覧

-# コントロールの初期化

@param

@retval なし
*/
/*============================================================================*/
void CCustomMonCntl::createControl()
{
	const TCHAR* mMonCtrlListHeader[] = {
		_T("名称"),
	};

	// 拡張スタイルの取得
	DWORD  dwStyle = ListView_GetExtendedListViewStyle(mListData.m_hWnd);
	//  拡張スタイルのセット
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER;
	//  新しい拡張スタイルを設定
	ListView_SetExtendedListViewStyle(mListData.m_hWnd, dwStyle);

	int width = 0;
	for (int i = 0; i < sizeof(mMonCtrlListHeader) / sizeof(mMonCtrlListHeader[0]); i++) {
		switch (i) {
		case	0:	width = 300; break;
		case	1:	width = 100; break;
		case	2:	width = 200; break;
		}
		mListData.InsertColumn(i, mMonCtrlListHeader[i], LVCFMT_LEFT, width);
	}

	createTreeData();

	SetControlInfo(IDC_TREE_DATA, ANCHORE_LEFTTOP | RESIZE_VER);
	SetControlInfo(IDC_LIST_DATA, ANCHORE_LEFTTOP | RESIZE_BOTH);
	SetControlInfo(IDC_EDIT_SEARCH, ANCHORE_LEFTTOP | RESIZE_HOR);
}

/*============================================================================*/
/*! 監視・制御一覧

-# ツリーデータの作成

@param

@retval なし
*/
/*============================================================================*/
void CCustomMonCntl::createTreeData()
{
	HTREEITEM hMon, hCtrl;

	hMon = mTreeData.InsertItem(_T("監視"), NULL, NULL, TVI_ROOT);
	mTreeData.SetItemData(hMon, (DWORD)eType_Mon);
	createTreeMon(hMon);
	hCtrl = mTreeData.InsertItem(_T("制御"), NULL, NULL, TVI_ROOT);
	mTreeData.SetItemData(hCtrl, (DWORD)eType_Cntl);
	createTreeCntl(hCtrl);

	mTreeData.SelectItem(hMon);
}

/*============================================================================*/
/*! 監視・制御一覧

-# ツリー監視データの作成

@param

@retval なし
*/
/*============================================================================*/
void CCustomMonCntl::createTreeMon(HTREEITEM hMon)
{
	vector<CString> nodelist;
	vector<CString>& data = theApp.GetCustomControl().GetDataManager().GetDataMonitor().GetEqList();
	vector<CString>::iterator itr, itrfind;
	// 監視値から装置名を取得してツリーに登録する
	CString str;
	HTREEITEM hItem;
	for (itr = data.begin(); itr != data.end(); itr++) {
		AfxExtractSubString(str, (*itr), 0, '.');
		if (str.IsEmpty() == true)
			str = (*itr);

		itrfind = std::find(nodelist.begin(), nodelist.end(), str);
		if (itrfind != nodelist.end())
			continue;

		hItem = mTreeData.InsertItem(str, NULL, NULL, hMon);
		mTreeData.SetItemData(hItem, (DWORD)eType_Mon);
		nodelist.push_back(str);
	}
	mTreeData.Expand(hMon, TVE_EXPAND);
}

/*============================================================================*/
/*! 監視・制御一覧

-# ツリー制御データの作成

@param

@retval なし
*/
/*============================================================================*/
void CCustomMonCntl::createTreeCntl(HTREEITEM hCntl)
{
	vector<CString> nodelist;
	vector<CString>& data = theApp.GetCustomControl().GetDataManager().GetDataControl().GetEqList();
	vector<CString>::iterator itr, itrfind;
	// 制御値から装置名を取得してツリーに登録する
	CString str;
	HTREEITEM hItem;
	for (itr = data.begin(); itr != data.end(); itr++) {
		AfxExtractSubString(str, (*itr), 0, '.');
		if (str.IsEmpty() == true)
			str = (*itr);

		itrfind = std::find(nodelist.begin(), nodelist.end(), str);
		if (itrfind != nodelist.end())
			continue;

		hItem = mTreeData.InsertItem(str, NULL, NULL, hCntl);
		mTreeData.SetItemData(hItem, (DWORD)eType_Cntl);
		nodelist.push_back(str);
	}
	mTreeData.Expand(hCntl, TVE_EXPAND);
}

/*============================================================================*/
/*! 監視・制御一覧

-# ツリー項目の選択

@param

@retval なし
*/
/*============================================================================*/
void CCustomMonCntl::OnTvnSelchangedTreeData(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	mEditSearch.SetWindowText(_T(""));

	int count = 0;
	switch (mTreeData.GetItemData(mTreeData.GetSelectedItem())) {
	case	eType_Mon:
		// 監視を選択
		mDbData.clear();
		mDbData.insert(mDbData.end(), theApp.GetCustomControl().GetDataManager().GetDataMonitor().GetEqList().begin(), theApp.GetCustomControl().GetDataManager().GetDataMonitor().GetEqList().end());
		count = (int)theApp.GetCustomControl().GetDataManager().GetDataMonitor().GetEqList().size();
		if (mTreeData.ItemHasChildren(mTreeData.GetSelectedItem()) == FALSE) {
			CString str;
			str = mTreeData.GetItemText(mTreeData.GetSelectedItem());
			mEditSearch.SetWindowText(str+_T("."));
		}
		mListData.SetDragFormat(CCustomDropObject::DF_MONITOR);
		break;
	case	eType_Cntl:
		// 制御を選択
		mDbData.clear();
		mDbData.insert(mDbData.end(), theApp.GetCustomControl().GetDataManager().GetDataControl().GetEqList().begin(), theApp.GetCustomControl().GetDataManager().GetDataControl().GetEqList().end());
		count = (int)theApp.GetCustomControl().GetDataManager().GetDataControl().GetEqList().size();
		if (mTreeData.ItemHasChildren(mTreeData.GetSelectedItem()) == FALSE) {
			CString str;
			str = mTreeData.GetItemText(mTreeData.GetSelectedItem());
			mEditSearch.SetWindowText(str + _T("."));
		}
		mListData.SetDragFormat(CCustomDropObject::DF_CONTROL);
		break;
	default:
		count = 0;
		mListData.ShowWindow(SW_SHOW);
		return;
	}

	mListData.DeleteAllItems();
	mListData.SetItemCount(count);

	OnEnChangeEditSearch();

	*pResult = 0;
}

/*============================================================================*/
/*! 監視・制御一覧

-# 仮想リストデータの設定

@param

@retval なし
*/
/*============================================================================*/
void CCustomMonCntl::OnLvnGetdispinfoListData(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	LV_ITEM* pItem = &(pDispInfo)->item;

	if (pItem->mask & LVIF_TEXT) {
		CString text;

		switch (mTreeData.GetItemData(mTreeData.GetSelectedItem())) {
		case	eType_Mon:
			// 監視を選択
			if (pItem->iSubItem == 0)
			{
				text = mDbData[pItem->iItem];// theApp.GetDataManager().GetDataMonitor().GetEqList()[pItem->iItem];
			}
			else
			{
				text.Empty();
			}
			break;
		case	eType_Cntl:
			// 制御を選択
			if (pItem->iSubItem == 0)
			{
				text = mDbData[pItem->iItem];// theApp.GetDataManager().GetDataControl().GetEqList()[pItem->iItem];
			}
			else
			{
				text.Empty();
			}
			break;
		default:
			text.Empty();
		}
		lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
	}

	*pResult = 0;
}

/*============================================================================*/
/*! 監視・制御一覧

-# 検索文字列の編集イベント

@param

@retval
*/
/*============================================================================*/
void CCustomMonCntl::OnEnChangeEditSearch()
{
	UpdateData(TRUE);

	if (mTreeData.GetItemData(mTreeData.GetSelectedItem()) == eType_Mon) {
		mDbData.clear();
		mDbData.insert(mDbData.end(), theApp.GetCustomControl().GetDataManager().GetDataMonitor().GetEqList().begin(), theApp.GetCustomControl().GetDataManager().GetDataMonitor().GetEqList().end());
	}
	else if (mTreeData.GetItemData(mTreeData.GetSelectedItem()) == eType_Cntl) {
		mDbData.clear();
		mDbData.insert(mDbData.end(), theApp.GetCustomControl().GetDataManager().GetDataControl().GetEqList().begin(), theApp.GetCustomControl().GetDataManager().GetDataControl().GetEqList().end());
	}
	else {
		return;
	}

	CString strSearch = mSearchText;
	strSearch.MakeUpper();

	bool bMatch = false;
	if (strSearch.Find(_T("*")) >= 0)
		bMatch = true;
	if (strSearch.Find(_T("?")) >= 0)
		bMatch = true;

	if (strSearch.IsEmpty() == false) {
		mDbData.erase(std::remove_if(mDbData.begin(), mDbData.end(), [&](const CString& str)
			{
				if (bMatch)
					return WildcardMatch(str, strSearch) != true;
				else
					return str.Find(strSearch) != 0;
			}), mDbData.end());
	}

	int count = (int)mDbData.size();
	mListData.DeleteAllItems();
	mListData.SetItemCount(count);
}
