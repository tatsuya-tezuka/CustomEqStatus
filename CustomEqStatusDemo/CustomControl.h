#pragma once

#include "CustomDataManager.h"
#include "CustomManager.h"
#include "CustomDetail.h"
#include "CustomMonCntl.h"
#include "CustomDropTarget.h"

// メモリ管理
#include <windows.h>
#include <psapi.h>

// timeGetTime使用
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// MessageBox Hook
#include <Windows.h>
#include <afxwin.h >

// フックハンドル用の変数をグローバルで宣言する。
static HHOOK CustomHookHandle;	// フックハンドル変数
static HHOOK CustomMsgHookHandle;	// フックハンドル変数
static LPCWSTR mMsgCustomOK = L"実行";				// OKボタンの文字
static LPCWSTR mMsgCustomYes = L"保存";	// キャンセルボタンの文字
static LPCWSTR mMsgCustomNo = L"保存しない";	// NOボタンの文字
static LPCWSTR mMsgCustomCancel = L"キャンセル";	// キャンセルボタンの文字
static BOOL    mMsgCustomSaveEnable=TRUE;
// プロトタイプ宣言
static LRESULT CALLBACK CustomSaveDifferentMsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK CustomMessageBoxHooked(int nCode, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////
//	編集処理後のメッセージボックス（保存、保存しない、キャンセル）
static LRESULT CALLBACK CustomSaveDifferentMsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// コード判断
	if (nCode >= 0)
	{
		if (nCode == HCBT_ACTIVATE)
		{
			SetDlgItemText((HWND)wParam, IDYES, mMsgCustomYes);
			SetDlgItemText((HWND)wParam, IDNO, mMsgCustomNo);
			SetDlgItemText((HWND)wParam, IDCANCEL, mMsgCustomCancel);
			EnableWindow(GetDlgItem((HWND)wParam, IDYES), mMsgCustomSaveEnable);

			// フック関数をアンインストール(フック解除！）
			BOOL ret = UnhookWindowsHookEx(CustomHookHandle);

		}
	}
	// 次のフックに渡す
	return CallNextHookEx(CustomHookHandle, nCode, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////
//	編集処理後のメッセージボックス（保存、保存しない、キャンセル）
static int CustomSaveDifferentMessageBoxHooked(HWND handle, LPCTSTR message, LPCTSTR title, UINT nType, BOOL bSaveEnable)
{
	mMsgCustomSaveEnable = bSaveEnable;
	// フック関数(MsgBoxHookProc)をインストールする SetWindowHookEx
	CustomHookHandle = SetWindowsHookEx(WH_CBT, CustomSaveDifferentMsgBoxHookProc, NULL, GetCurrentThreadId());
	return (MessageBox(handle, message, title, nType));
}

/////////////////////////////////////////////////////////////////////
//	ダイアログの終了時のメッセージボックス（実行、キャンセル）
static LRESULT CALLBACK CustomMsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// コード判断
	if (nCode >= 0)
	{
		if (nCode == HCBT_ACTIVATE)
		{
			// OKボタン(IDOK)の内容を書き換える
			SetDlgItemText((HWND)wParam, IDOK, mMsgCustomOK);
			// キャンセルボタン(IDCANCEL)の内容を書き換える
			SetDlgItemText((HWND)wParam, IDCANCEL, mMsgCustomCancel);
			BOOL ret;
			// フック関数をアンインストール(フック解除！）
			ret = UnhookWindowsHookEx(CustomMsgHookHandle);

		}
	}
	// 次のフックに渡す
	return CallNextHookEx(CustomMsgHookHandle, nCode, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////
//	ダイアログの終了時のメッセージボックス（実行、キャンセル）
static int CustomMessageBoxHooked(HWND handle, LPCTSTR message, LPCTSTR title, UINT nType)
{
	// フック関数(MsgBoxHookProc)をインストールする SetWindowHookEx
	CustomMsgHookHandle = SetWindowsHookEx(WH_CBT, CustomMsgBoxHookProc, NULL, GetCurrentThreadId());
	return (MessageBox(handle, message, title, nType));
}

class CCustomMonitors
{
public:
	CCustomMonitors();
	virtual ~CCustomMonitors();

private:
	typedef struct {
		RECT			rect;
		HMONITOR		hMonitor;
		MONITORINFOEX	info;
	} mMonData;
	vector<mMonData> m_MonitorList;

	static BOOL CALLBACK CustomMonitorNumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

public:
	//CMonitor GetMonitor(const int index) const;
	int		GetCount() const { return (int)m_MonitorList.size(); }
	void	AddMonitor(HMONITOR hMonitor, CRect rect, MONITORINFOEX* info)
	{
		mMonData data;
		data.rect.left = rect.left;
		data.rect.top = rect.top;
		data.rect.right = rect.right;
		data.rect.bottom = rect.bottom;
		data.hMonitor = hMonitor;
		memcpy(&data.info, info, sizeof(MONITORINFOEX));
		m_MonitorList.push_back(data);
	}
	bool	GetMonitor(const UINT nMonNo, RECT* prect, MONITORINFOEX* pinfo)
	{
		vector<mMonData>::iterator itr;
		for (itr = m_MonitorList.begin(); itr != m_MonitorList.end(); itr++) {
			if (nMonNo == 0 && (*itr).info.dwFlags == MONITORINFOF_PRIMARY) {
				memcpy(prect, &((*itr).rect), sizeof(RECT));
				memcpy(pinfo, &((*itr).info), sizeof(MONITORINFOEX));
				return true;
			}
			if (nMonNo == 1 && (*itr).info.dwFlags != MONITORINFOF_PRIMARY) {
				memcpy(prect, &((*itr).rect), sizeof(RECT));
				memcpy(pinfo, &((*itr).info), sizeof(MONITORINFOEX));
				return true;
			}
		}
		return false;
	}
};


class CCustomControl
{
public:

	CCustomControl();
	virtual ~CCustomControl();

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
	/// メインウィンドウハンドル
	CWnd* mMainWnd;
	/// マスタデータ格納パス
	CString					mAppMasterDataPath;
	/// ユーザデータ格納パス
	CString					mAppUserDataPath;
	/// データベースパス
	CString					mAppDataBasePath;
	/// カスタムデータ管理関連 
	CCustomDataManager		mDataManager;
	/// カスタマイズ管理
	CCustomManager			mCustomManager;
	// 監視・制御一覧
	CCustomMonCntl			mCustmMonCntl;
	/// カスタム管理画面位置
	CRect					mManagerRect;
	CPoint					mCascadePoint;
	/// ドラッグ＆ドロップ
	CCustomDropTarget		mCustomDragTarget;

	// マルチモニタ情報
	CCustomMonitors			mCustomMonitor;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	Initialize(CWnd* pParent, CString appPath);

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
	void	SetMasterDataPath(CString name) { mAppMasterDataPath = name; }
	/// ユーザ設備詳細データ格納パス
	CString	GetUserDataPath() { return mAppUserDataPath; }
	void	SetUserDataPath(CString name) { mAppUserDataPath = name; }
	/// カスタムデータ管理関連
	CCustomDataManager& GetDataManager() { return mDataManager; }
	/// カスタマイズ管理
	CCustomManager& GetCustomManager()
	{
		if (mCustomManager.GetSafeHwnd() == NULL) {
			mCustomManager.Create(IDD_DIALOG_MANAGER, mMainWnd);
		}
		return mCustomManager;
	}
	// 監視・制御一覧
	CCustomMonCntl& GetCustomMonCntl()
	{
		if (mCustmMonCntl.GetSafeHwnd() == NULL) {
			mCustmMonCntl.Create(IDD_DIALOG_MONCNTL, mMainWnd);
		}
		return mCustmMonCntl;
	}
	/// 設備詳細ウィンドウの作成
	CCustomDetail* CreateEquipment(CTreeNode* pnode, UINT mode= eTreeItemMode_Monitor);
	/// カスタム管理画面位置の更新
	void UpdateCustomManagerPoint(CRect rect)
	{
		mManagerRect = rect;
		mCascadePoint = CPoint(mManagerRect.right + GetSystemMetrics(SM_CYSIZEFRAME), mManagerRect.top);
	}
	/// カスケード表示位置の取得
	CPoint GetCascadePoint()
	{
		CPoint point = mCascadePoint;
		int CYCAPTION = GetSystemMetrics(SM_CYCAPTION);
		mCascadePoint.x += CYCAPTION;
		mCascadePoint.y += CYCAPTION;

		CWnd* pWnd = CWnd::GetDesktopWindow();
		CRect rectDesktop;
		pWnd->GetClientRect(rectDesktop);

		// カスタム管理画面の左上の位置がどのモニタ内にあるか確認して、
		// 存在するモニタサイズを取得する
		int countMonitor = GetCustomMonitor().GetCount();
		RECT monRect;
		MONITORINFOEX monInfo;
		for (int i = 0; i < countMonitor; i++) {
			if (GetCustomMonitor().GetMonitor(i, &monRect, &monInfo) == true) {
				CRect rc = CRect(monInfo.rcWork);
				if (rc.PtInRect(CPoint(mManagerRect.left, mManagerRect.top)) == TRUE) {
					rectDesktop = CRect(monInfo.rcWork);
					break;
				}
			}
		}

		if (point.x >= rectDesktop.right || point.y >= (rectDesktop.bottom - GetSystemMetrics(SM_CYCAPTION))) {
			UpdateCustomManagerPoint(mManagerRect);
			if (point.x >= rectDesktop.right) {
				mCascadePoint = CPoint(mManagerRect.left - mManagerRect.Width() - GetSystemMetrics(SM_CYSIZEFRAME), mManagerRect.top);
			}
			point = mCascadePoint;
			int CYCAPTION = GetSystemMetrics(SM_CYCAPTION);
			mCascadePoint.x += CYCAPTION;
			mCascadePoint.y += CYCAPTION;
		}

		return point;
	}

	CCustomDropTarget& GetCustomDragTarget() { return mCustomDragTarget; }

	// マルチモニタ情報
	CCustomMonitors& GetCustomMonitor() { return mCustomMonitor; }

protected:
	void	createMasterEquipment();
	void	createUserEquipment();
	void	getMasterEquipmentFiles(vector<CString>& list);
	void	getUserEquipmentFiles(vector<CString>& list);

	/* ------------------------------------------------------------------------------------ */
};

