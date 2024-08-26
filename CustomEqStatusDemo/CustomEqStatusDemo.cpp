
// CustomEqStatusDemo.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomEqStatusDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCustomEqStatusDemoApp

BEGIN_MESSAGE_MAP(CCustomEqStatusDemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCustomEqStatusDemoApp コンストラクション

CCustomEqStatusDemoApp::CCustomEqStatusDemoApp()
{
	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CCustomEqStatusDemoApp オブジェクトです。

CCustomEqStatusDemoApp theApp;


// CCustomEqStatusDemoApp 初期化

BOOL CCustomEqStatusDemoApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ダイアログにシェル ツリー ビューまたはシェル リスト ビュー コントロールが
	// 含まれている場合にシェル マネージャーを作成します。
	CShellManager *pShellManager = new CShellManager;

	// MFC コントロールでテーマを有効にするために、"Windows ネイティブ" のビジュアル マネージャーをアクティブ化
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));

	InitializeApp();

	CCustomEqStatusDemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//  記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <キャンセル> で消された時のコードを
		//  記述してください。
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: ダイアログの作成に失敗しました。アプリケーションは予期せずに終了します。\n");
		TRACE(traceAppMsg, 0, "警告: ダイアログで MFC コントロールを使用している場合、#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS を指定できません。\n");
	}

	// 上で作成されたシェル マネージャーを削除します。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	//  アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

int CCustomEqStatusDemoApp::ExitInstance()
{
	TerminateApp();

	return CWinApp::ExitInstance();
}

/*============================================================================*/
/*! アプリケーション

-# アプリケーションの初期化

@param
@retval

*/
/*============================================================================*/
void CCustomEqStatusDemoApp::InitializeApp()
{
	// モジュール名からEXEが存在するパスを取得する
	TCHAR path[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, path, sizeof(path));
	PathRemoveFileSpec(path);
	mAppPath = path;

	// AppDataパスの作成＆取得
	SHGetSpecialFolderPath(NULL, path, CSIDL_APPDATA, TRUE);
	CString strPath = path;
	strPath += _T("\\") + CString(mAppDataSystem);
	CreateDirectory(strPath, NULL);
	strPath += _T("\\") + CString(mAppDataDataPath);
	CreateDirectory(strPath, NULL);
	mAppDataPath = strPath;

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CString logpath = path;
	logpath += _T("\\") + CString(mAppDataSystem);
	CreateDirectory(logpath, NULL);
	logpath += _T("\\") + CString(mAppDataLogPath);
	CreateDirectory(logpath, NULL);
	logpath += _T("\\");
	CLogTraceEx::Create(logpath, _T("CCustomEquipment"), nLogEx::debug, nLogEx::text);
	CLogTraceEx::Write(_T("###"), _T("CCustomEquipment"), _T("Start"), _T(""), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
}

/*============================================================================*/
/*! アプリケーション

-# アプリケーションの終了

@param
@retval

*/
/*============================================================================*/
void CCustomEqStatusDemoApp::TerminateApp()
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("###"), _T("CCustomEquipment"), _T("Finish"), _T(""), _T(""), nLogEx::info);
	CLogTraceEx::Close();
	CLogTraceEx::Delete();
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
}

/*============================================================================*/
/*! アプリケーション

-# 設備詳細ウィンドウの作成

@param		pnode		ノード情報
@retval

*/
/*============================================================================*/
CCustomDetail* CCustomEqStatusDemoApp::CreateEquipment(CTreeNode* pnode)
{
	// 設備詳細画面の作成
	CCustomDetail *pitem = new CCustomDetail(theApp.GetMainWnd(), (pnode == NULL) ? false : true);
	if (pnode != NULL){
		pnode->GetWindowInfo().wnd = pitem;
		pnode->GetWindowInfo().manager = &mCustomManager;
	}
	// 設備詳細画面の作成
	pitem->Create(IDD_DIALOG_EQDETAIL, theApp.GetMainWnd());
	pitem->ShowWindow((pnode == NULL) ? SW_SHOW : pnode->GetWindowInfo().placement.showCmd);

	// 管理ウィンドウの設定
	if (pnode == NULL){
		CTreeNode* pnodeitem = theApp.GetDataManager().SearchWndNode(pitem);
		pnodeitem->GetWindowInfo().wnd = pitem;
		pnodeitem->GetWindowInfo().manager = &mCustomManager;
	}
	return pitem;
}
