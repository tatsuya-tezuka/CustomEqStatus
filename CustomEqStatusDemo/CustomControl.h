#pragma once

#include "CustomDataManager.h"
#include "CustomManager.h"
#include "CustomDetail.h"
#include "CustomMonCntl.h"
#include "CustomDropTarget.h"

// �������Ǘ�
#include <windows.h>
#include <psapi.h>

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// MessageBox Hook
#include <Windows.h>
#include <afxwin.h >

// �t�b�N�n���h���p�̕ϐ����O���[�o���Ő錾����B
static HHOOK CustomHookHandle;	// �t�b�N�n���h���ϐ�
static HHOOK CustomMsgHookHandle;	// �t�b�N�n���h���ϐ�
static LPCWSTR mMsgCustomOK = L"���s";				// OK�{�^���̕���
static LPCWSTR mMsgCustomYes = L"�ۑ�";	// �L�����Z���{�^���̕���
static LPCWSTR mMsgCustomNo = L"�ۑ����Ȃ�";	// �L�����Z���{�^���̕���
static LPCWSTR mMsgCustomCancel = L"�L�����Z��";	// �L�����Z���{�^���̕���
static BOOL    mMsgCustomSaveEnable=TRUE;
// �v���g�^�C�v�錾
static LRESULT CALLBACK CustomSaveDifferentMsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK CustomMessageBoxHooked(int nCode, WPARAM wParam, LPARAM lParam);

/////////////////////////////////////////////////////////////////////
//	�ҏW������̃��b�Z�[�W�{�b�N�X�i�ۑ��A�ۑ����Ȃ��A�L�����Z���j
static LRESULT CALLBACK CustomSaveDifferentMsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// �R�[�h���f
	if (nCode >= 0)
	{
		if (nCode == HCBT_ACTIVATE)
		{
			SetDlgItemText((HWND)wParam, IDYES, mMsgCustomYes);
			SetDlgItemText((HWND)wParam, IDNO, mMsgCustomNo);
			SetDlgItemText((HWND)wParam, IDCANCEL, mMsgCustomCancel);
			EnableWindow(GetDlgItem((HWND)wParam, IDYES), mMsgCustomSaveEnable);

			// �t�b�N�֐����A���C���X�g�[��(�t�b�N�����I�j
			BOOL ret = UnhookWindowsHookEx(CustomHookHandle);

		}
	}
	// ���̃t�b�N�ɓn��
	return CallNextHookEx(CustomHookHandle, nCode, wParam, lParam);
}
/////////////////////////////////////////////////////////////////////
//	�ҏW������̃��b�Z�[�W�{�b�N�X�i�ۑ��A�ۑ����Ȃ��A�L�����Z���j
static int CustomSaveDifferentMessageBoxHooked(HWND handle, LPCTSTR message, LPCTSTR title, UINT nType, BOOL bSaveEnable)
{
	mMsgCustomSaveEnable = bSaveEnable;
	// �t�b�N�֐�(MsgBoxHookProc)���C���X�g�[������ SetWindowHookEx
	CustomHookHandle = SetWindowsHookEx(WH_CBT, CustomSaveDifferentMsgBoxHookProc, NULL, GetCurrentThreadId());
	return (MessageBox(handle, message, title, nType));
}

/////////////////////////////////////////////////////////////////////
//	�_�C�A���O�̏I�����̃��b�Z�[�W�{�b�N�X�i���s�A�L�����Z���j
static LRESULT CALLBACK CustomMsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// �R�[�h���f
	if (nCode >= 0)
	{
		if (nCode == HCBT_ACTIVATE)
		{
			// OK�{�^��(IDOK)�̓��e������������
			SetDlgItemText((HWND)wParam, IDOK, mMsgCustomOK);
			// �L�����Z���{�^��(IDCANCEL)�̓��e������������
			SetDlgItemText((HWND)wParam, IDCANCEL, mMsgCustomCancel);
			BOOL ret;
			// �t�b�N�֐����A���C���X�g�[��(�t�b�N�����I�j
			ret = UnhookWindowsHookEx(CustomMsgHookHandle);

		}
	}
	// ���̃t�b�N�ɓn��
	return CallNextHookEx(CustomMsgHookHandle, nCode, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////
//	�_�C�A���O�̏I�����̃��b�Z�[�W�{�b�N�X�i���s�A�L�����Z���j
static int CustomMessageBoxHooked(HWND handle, LPCTSTR message, LPCTSTR title, UINT nType)
{
	// �t�b�N�֐�(MsgBoxHookProc)���C���X�g�[������ SetWindowHookEx
	CustomMsgHookHandle = SetWindowsHookEx(WH_CBT, CustomMsgBoxHookProc, NULL, GetCurrentThreadId());
	return (MessageBox(handle, message, title, nType));
}


class CCustomControl
{
public:

	CCustomControl();
	virtual ~CCustomControl();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	/// ���C���E�B���h�E�n���h��
	CWnd* mMainWnd;
	/// �}�X�^�f�[�^�i�[�p�X
	CString					mAppMasterDataPath;
	/// ���[�U�f�[�^�i�[�p�X
	CString					mAppUserDataPath;
	/// �f�[�^�x�[�X�p�X
	CString					mAppDataBasePath;
	/// �J�X�^���f�[�^�Ǘ��֘A 
	CCustomDataManager		mDataManager;
	/// �J�X�^�}�C�Y�Ǘ�
	CCustomManager			mCustomManager;
	// �Ď��E����ꗗ
	CCustomMonCntl			mCustmMonCntl;
	/// �J�X�^���Ǘ���ʈʒu
	CPoint					mManagerPoint;
	CPoint					mCascadePoint;
	/// �h���b�O���h���b�v
	CCustomDropTarget		mCustomDragTarget;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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
			//������������������������ Log ������������������������//
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
			//������������������������ Log ������������������������//
			//=====================================================//
		}

		CloseHandle(hProcess);
	}

	/// �}�X�^�ݔ��ڍ׃f�[�^�i�[�p�X
	CString	GetMasterDataPath() { return mAppMasterDataPath; }
	void	SetMasterDataPath(CString name) { mAppMasterDataPath = name; }
	/// ���[�U�ݔ��ڍ׃f�[�^�i�[�p�X
	CString	GetUserDataPath() { return mAppUserDataPath; }
	void	SetUserDataPath(CString name) { mAppUserDataPath = name; }
	/// �J�X�^���f�[�^�Ǘ��֘A
	CCustomDataManager& GetDataManager() { return mDataManager; }
	/// �J�X�^�}�C�Y�Ǘ�
	CCustomManager& GetCustomManager()
	{
		if (mCustomManager.GetSafeHwnd() == NULL) {
			mCustomManager.Create(IDD_DIALOG_MANAGER, mMainWnd);
		}
		return mCustomManager;
	}
	// �Ď��E����ꗗ
	CCustomMonCntl& GetCustomMonCntl()
	{
		if (mCustmMonCntl.GetSafeHwnd() == NULL) {
			mCustmMonCntl.Create(IDD_DIALOG_MONCNTL, mMainWnd);
		}
		return mCustmMonCntl;
	}
	/// �ݔ��ڍ׃E�B���h�E�̍쐬
	CCustomDetail* CreateEquipment(CTreeNode* pnode, UINT mode= eTreeItemMode_Monitor);
	/// �J�X�^���Ǘ���ʈʒu�̍X�V
	void UpdateCustomManagerPoint(CPoint point)
	{
		mManagerPoint = point;
		CRect rect;
		mCustomManager.GetWindowRect(rect);
		mCascadePoint = CPoint(rect.right + 5, rect.top);
	}
	/// �J�X�P�[�h�\���ʒu�̎擾
	CPoint GetCascadePoint()
	{
		CPoint point = mCascadePoint;
		int CYCAPTION = GetSystemMetrics(SM_CYCAPTION);
		mCascadePoint.x += CYCAPTION;
		mCascadePoint.y += CYCAPTION;
		return point;
	}

	CCustomDropTarget& GetCustomDragTarget() { return mCustomDragTarget; }

protected:
	void	createMasterEquipment();
	void	createUserEquipment();
	void	getMasterEquipmentFiles(vector<CString>& list);
	void	getUserEquipmentFiles(vector<CString>& list);

	/* ------------------------------------------------------------------------------------ */
};

