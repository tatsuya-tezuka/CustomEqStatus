
// CustomEqStatus.h : CustomEqStatus アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル
#include "LogTraceEx.h"
#include "DBAccess.h"


// CCustomEqStatusApp:
// このクラスの実装については、CustomEqStatus.cpp を参照してください。
//

static const TCHAR* mAppDataSystem = { _T("NEC") };
static const TCHAR* mAppDataDataPath = { _T("DATA") };
static const TCHAR* mAppDataLogPath = { _T("LOG") };

class CCustomEqStatusApp : public CWinApp
{
public:
	CCustomEqStatusApp();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum { ePopMon = 1, ePopCon };

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	/// モジュールファイルパス
	CString		mAppPath;
	/// AppDataパス
	CString		mAppDataPath;

	/// データベースアクセスクラス（StationControlからの流用）
	CDBAccess	mDBAccess;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString&	GetAppPath() { return mAppPath; }
	CString&	GetAppDataPath() { return mAppDataPath; }
	CDBAccess&	GetDBAccess() { return mDBAccess; }

	void	InitializeApp();
	void	TerminateApp();

protected:

	/* ------------------------------------------------------------------------------------ */

	// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCustomEqStatusApp theApp;
