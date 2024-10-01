#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomControl.h"


//------------------------------------------------------------------------------------ 
//	概要：
//------------------------------------------------------------------------------------
CCustomControl::CCustomControl()
{
}

CCustomControl::~CCustomControl()
{
}

/*============================================================================*/
/*! カスタムコントロール

-# カスタムコントロールの初期化

@param	appPath		アプリケーションパス
@retval

*/
/*============================================================================*/
void CCustomControl::Initialize(CString appPath)
{
	/// マスタ用データ格納パス
	mAppMasterDataPath = appPath + _T("\\") + mAppDataMasterPath;

	/// デモ用データ格納パス
	mAppUserDataPath = appPath + _T("\\") + mAppDataUserPath;

	/// 全てのウィンドウハンドル、ノードの削除
	GetDataManager().DeleteAll();

	/// マスタ設備詳細データを読み込む
	createMasterEquipment();

	/// ユーザ設備詳細データを読み込む
	createUserEquipment();
}

/*============================================================================*/
/*! カスタムコントロール

-# 設備詳細ウィンドウの作成

@param		pnode		ノード情報
@retval

*/
/*============================================================================*/
CCustomDetail* CCustomControl::CreateEquipment(CTreeNode* pnode)
{
	// 設備詳細画面の作成
	CCustomDetail* pitem = new CCustomDetail(theApp.GetMainWnd(), (pnode == NULL) ? false : true);
	if (pnode != NULL) {
		pnode->GetWindowInfo().wnd = pitem;
		pnode->GetWindowInfo().manager = &mCustomManager;
	}
	// 設備詳細画面の作成
	pitem->Create(IDD_DIALOG_EQDETAIL, theApp.GetMainWnd());
	pitem->ShowWindow((pnode == NULL) ? SW_SHOW : pnode->GetWindowInfo().placement.showCmd);

	// 管理ウィンドウの設定
	if (pnode == NULL) {
		CTreeNode* pnodeitem = GetDataManager().SearchWndNode(pitem);
		pnodeitem->GetWindowInfo().wnd = pitem;
		pnodeitem->GetWindowInfo().manager = &mCustomManager;
		swprintf_s(pnodeitem->GetWindowInfo().groupname, mNameSize, _T("%s"), (LPCTSTR)_T("0"));
		pnodeitem->GetWindowInfo().groupno = 0;
		pnodeitem->GetWindowInfo().mode = eTreeItemMode_Monitor;
		pnodeitem->GetWindowInfo().kind = eTreeItemKind_User;
	}
	return pitem;
}

/*============================================================================*/
/*! カスタムコントロール

-# マスタ設備詳細データの作成

@param
@retval

*/
/*============================================================================*/
void CCustomControl::createMasterEquipment()
{
	CWaitCursor wait;

	vector<CString> filelist;
	getMasterEquipmentFiles(filelist);
	vector<CString>::iterator itrmaster;
	for (itrmaster = filelist.begin(); itrmaster != filelist.end(); itrmaster++) {
		GetDataManager().LoadTreeDataXml((*itrmaster), eTreeItemKind_Master);
	}
}

/*============================================================================*/
/*! カスタムコントロール

-# ユーザ設備詳細データの作成

@param
@retval

*/
/*============================================================================*/
void CCustomControl::createUserEquipment()
{
	CWaitCursor wait;

	vector<CString> filelist;
	getUserEquipmentFiles(filelist);
	vector<CString>::iterator itruser;
	for (itruser = filelist.begin(); itruser != filelist.end(); itruser++) {
		GetDataManager().LoadTreeDataXml((*itruser), eTreeItemKind_User);
	}
}

/*============================================================================*/
/*! カスタムコントロール

-# マスタ設備詳細ファイルの取得

@param
@retval

*/
/*============================================================================*/
void CCustomControl::getMasterEquipmentFiles(vector<CString>& list)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getMasterEquipmentFiles"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;

	list.clear();

	//拡張子の設定
	CString search_name = mAppMasterDataPath + _T("\\*.xml");

	hFind = FindFirstFile(search_name, &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		}
		else {
			list.push_back(mAppMasterDataPath + _T("\\") + CString(win32fd.cFileName));
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getMasterEquipmentFiles"), _T("Stop"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
}

/*============================================================================*/
/*! カスタムコントロール

-# ユーザ設備詳細ファイルの取得

@param
@retval

*/
/*============================================================================*/
void CCustomControl::getUserEquipmentFiles(vector<CString>& list)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getUserEquipmentFiles"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;

	list.clear();

	//拡張子の設定
	CString search_name = mAppUserDataPath + _T("\\*.xml");

	hFind = FindFirstFile(search_name, &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		}
		else {
			list.push_back(mAppUserDataPath + _T("\\") + CString(win32fd.cFileName));
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getUserEquipmentFiles"), _T("Stop"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
}
