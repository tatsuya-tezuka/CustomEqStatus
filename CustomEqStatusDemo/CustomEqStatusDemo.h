
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

// timeGetTime使用
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// メモリ管理
#include <windows.h>
#include <psapi.h>

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

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	InitializeApp();
	void	TerminateApp();

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
	/// マスタデータ格納パス
	CString					mAppMasterDataPath;
	/// ユーザデータ格納パス
	CString					mAppUserDataPath;
	/// カスタムデータ管理関連 
	CCustomDataManager		mDataManager;
	/// カスタマイズ管理
	CCustomManager			mCustomManager;
	/// カスタム管理画面位置
	CPoint					mManagerPoint;
	CPoint					mCascadePoint;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void PrintMemoryInfo()
	{
		HANDLE hProcess;
		PROCESS_MEMORY_COUNTERS_EX pmc;

		// Print the process identifier.
		hProcess = GetCurrentProcess();

		// Print information about the memory usage of the process.
		if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
		{
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CString msg;
			msg.Format(_T("PageFaultCount: 0x%08X"), pmc.PageFaultCount);
			CLogTraceEx::Write(_T("***"), _T("CCustomEquipmentApp"), _T("Process Memory Info."), msg, _T(""), nLogEx::debug);
			msg.Format(_T("PeakWorkingSetSize: 0x % 08X"), pmc.PeakWorkingSetSize);
			CLogTraceEx::Write(_T("***"), _T("CCustomEquipmentApp"), _T("Process Memory Info."), msg, _T(""), nLogEx::debug);
			msg.Format(_T("WorkingSetSize: 0x%08X"), pmc.WorkingSetSize);
			CLogTraceEx::Write(_T("***"), _T("CCustomEquipmentApp"), _T("Process Memory Info."), msg, _T(""), nLogEx::debug);
			msg.Format(_T("QuotaPeakPagedPoolUsage: 0x%08X"), pmc.QuotaPeakPagedPoolUsage);
			CLogTraceEx::Write(_T("***"), _T("CCustomEquipmentApp"), _T("Process Memory Info."), msg, _T(""), nLogEx::debug);
			msg.Format(_T("QuotaPagedPoolUsage: 0x%08X"), pmc.QuotaPagedPoolUsage);
			CLogTraceEx::Write(_T("***"), _T("CCustomEquipmentApp"), _T("Process Memory Info."), msg, _T(""), nLogEx::debug);
			msg.Format(_T("QuotaPeakNonPagedPoolUsage: 0x%08X"), pmc.QuotaPeakNonPagedPoolUsage);
			CLogTraceEx::Write(_T("***"), _T("CCustomEquipmentApp"), _T("Process Memory Info."), msg, _T(""), nLogEx::debug);
			msg.Format(_T("QuotaNonPagedPoolUsage: 0x%08X"), pmc.QuotaNonPagedPoolUsage);
			CLogTraceEx::Write(_T("***"), _T("CCustomEquipmentApp"), _T("Process Memory Info."), msg, _T(""), nLogEx::debug);
			msg.Format(_T("PagefileUsage: 0x%08X"), pmc.PagefileUsage);
			CLogTraceEx::Write(_T("***"), _T("CCustomEquipmentApp"), _T("Process Memory Info."), msg, _T(""), nLogEx::debug);
			msg.Format(_T("PeakPagefileUsage: 0x%08X"), pmc.PeakPagefileUsage);
			CLogTraceEx::Write(_T("***"), _T("CCustomEquipmentApp"), _T("Process Memory Info."), msg, _T(""), nLogEx::debug);
			msg.Format(_T("PrivateUsage: 0x%08X(%dMB)"), pmc.PrivateUsage, pmc.PrivateUsage / 1000 / 1000);
			CLogTraceEx::Write(_T("***"), _T("CCustomEquipmentApp"), _T("Process Memory Info."), msg, _T(""), nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
		}

		CloseHandle(hProcess);
	}

	/// マスタ設備詳細データ格納パス
	CString	GetMasterDataPath() { return mAppMasterDataPath; }
	/// ユーザ設備詳細データ格納パス
	CString	GetUserDataPath() { return mAppUserDataPath; }
	/// カスタムデータ管理関連
	CCustomDataManager&	GetDataManager() { return mDataManager; }
	/// カスタマイズ管理
	CCustomManager&	GetCustomManager() { return mCustomManager; }
	/// 設備詳細ウィンドウの作成
	CCustomDetail*	CreateEquipment(CTreeNode* pnode);
	/// カスタム管理画面位置の更新
	void UpdateCustomManagerPoint(CPoint point)
	{
		mManagerPoint = point;
		CRect rect;
		mCustomManager.GetWindowRect(rect);
		mCascadePoint = CPoint(rect.right + 5, rect.top);
	}
	/// カスケード表示位置の取得
	CPoint GetCascadePoint()
	{
		CPoint point = mCascadePoint;
		int CYCAPTION = GetSystemMetrics(SM_CYCAPTION);
		mCascadePoint.x += CYCAPTION;
		mCascadePoint.y += CYCAPTION;
		return point;
	}

protected:
	void	createMasterEquipment();
	void	createUserEquipment();
	void	getMasterEquipmentFiles(vector<CString>& list);
	void	getUserEquipmentFiles(vector<CString>& list);

	//////////////////////////////////////////////////////////////////////////////////////////

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CCustomEqStatusDemoApp theApp;