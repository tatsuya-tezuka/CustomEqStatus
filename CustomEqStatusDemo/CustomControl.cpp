#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomControl.h"


BOOL CALLBACK CCustomMonitors::CustomMonitorNumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	CCustomMonitors* pmon = (CCustomMonitors*)dwData;

	// モニタ情報の取得
	MONITORINFOEX mInfo;
	mInfo.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &mInfo);

	// モニタ領域の取得
	CRect rect(lprcMonitor);
	pmon->AddMonitor(hMonitor, rect, &mInfo);

	return TRUE;
}

CCustomMonitors::CCustomMonitors()
{
	// マルチモニタ対応
	EnumDisplayMonitors(NULL, NULL, CustomMonitorNumProc, (LPARAM)this);
}

CCustomMonitors::~CCustomMonitors()
{
}

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

@param	pParent		メインウィンドウハンドル
@param	appPath		アプリケーションパス
@retval

*/
/*============================================================================*/
void CCustomControl::Initialize(CWnd* pParent, CString appPath)
{
	mMainWnd = pParent;

	/// マスタ用データ格納パス
	mAppMasterDataPath = appPath + _T("\\") + mAppDataMasterPath;

	/// デモ用データ格納パス
	mAppUserDataPath = appPath + _T("\\") + mAppDataUserPath;

	/// データベースパス
	mAppDataBasePath = appPath + _T("\\") + _T("db");

	/// 全てのウィンドウハンドル、ノードの削除
	GetDataManager().DeleteAll();

	/// マスタ設備詳細データを読み込む
	createMasterEquipment();

	/// ユーザ設備詳細データを読み込む
	createUserEquipment();

	// ◆データベース取得
	const CString mpDBName[] = {
		_T("eqmon.db.obj"),
		_T("eqctl.db.obj"),
	};
	// 監視DB取得
	CString str = mAppDataBasePath + _T("\\") + mpDBName[0];
	GetDataManager().GetDataMonitor().eq_db_read(str);
	// 制御DB取得
	str = mAppDataBasePath + _T("\\") + mpDBName[1];
	GetDataManager().GetDataControl().eq_db_read(str);
}

/*============================================================================*/
/*! カスタムコントロール

-# 設備詳細ウィンドウの作成

@param		pnode		ノード情報
@retval

*/
/*============================================================================*/
CCustomDetail* CCustomControl::CreateEquipment(CTreeNode* pnode, UINT mode/* = eTreeItemMode_Monitor*/)
{
	// 設備詳細画面の作成
	CCustomDetail* pitem = new CCustomDetail(theApp.GetMainWnd(), (pnode == NULL) ? false : true);
	if (pnode != NULL) {
		pnode->GetEquipment().wnd = pitem;
		pnode->GetEquipment().manager = &mCustomManager;
	}
	// 設備詳細画面の作成
	pitem->Create(IDD_DIALOG_EQDETAIL, theApp.GetMainWnd());
	pitem->ShowWindow((pnode == NULL) ? SW_SHOW : pnode->GetEquipment().placement.showCmd);

	// 管理ウィンドウの設定
	if (pnode == NULL) {
		CTreeNode* pnodeitem = GetDataManager().SearchWndNode(pitem);
		pnodeitem->GetEquipment().wnd = pitem;
		pnodeitem->GetEquipment().manager = &mCustomManager;
		//swprintf_s(pnodeitem->GetWindowInfo().groupname, mNameSize, _T("%s"), (LPCTSTR)mNoGroupText);
		pnodeitem->GetManager().groupno = 0;
		pnodeitem->GetEquipment().mode = mode;
		pnodeitem->GetEquipment().kind = eTreeItemKind_User;
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
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("createMasterEquipment"), (*itrmaster), _T(""), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
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
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("createUserEquipment"), (*itruser), _T(""), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
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
