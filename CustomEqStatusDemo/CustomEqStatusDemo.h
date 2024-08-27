
// CustomEqStatusDemo.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル
#include "LogTraceEx.h"
#include "CustomDataManager.h"
#include "CustomManager.h"
#include "CustomDetail.h"

// CCustomEqStatusDemoApp:
// このクラスの実装については、CustomEqStatusDemo.cpp を参照してください。
//

class CCustomEqStatusDemoApp : public CWinApp
{
public:
	CCustomEqStatusDemoApp();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

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
	/// デモデータ格納パス
	CString		mAppDemoDataPath;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	InitializeApp();
	void	TerminateApp();
	CString	GetDemoDataPath() { return mAppDemoDataPath; }

protected:

	/* ------------------------------------------------------------------------------------ */

	//////////////////////////////////////////////////////////////////////////////////////////
	/// ◆カスタマイズ機能
	//////////////////////////////////////////////////////////////////////////////////////////
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
	/// カスタムデータ管理関連 
	CCustomDataManager		mDataManager;
	/// カスタマイズ管理
	CCustomManager			mCustomManager;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	/// カスタムデータ管理関連
	CCustomDataManager&	GetDataManager() { return mDataManager; }
	/// カスタマイズ管理
	CCustomManager&	GetCustomManager() { return mCustomManager; }
	/// 設備詳細ウィンドウの作成
	CCustomDetail*	CreateEquipment(CTreeNode* pnode);

protected:

	//////////////////////////////////////////////////////////////////////////////////////////

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CCustomEqStatusDemoApp theApp;