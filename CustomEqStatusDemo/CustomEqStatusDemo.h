
// CustomEqStatusDemo.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��
#include "LogTraceEx.h"
#include "CustomDataManager.h"
#include "CustomManager.h"
#include "CustomDetail.h"

// timeGetTime�g�p
#include <mmsystem.h>
#pragma comment( lib, "winmm.lib" )

// �������Ǘ�
#include <windows.h>
#include <psapi.h>

// CCustomEqStatusDemoApp:
// ���̃N���X�̎����ɂ��ẮACustomEqStatusDemo.cpp ���Q�Ƃ��Ă��������B
//

class CCustomEqStatusDemoApp : public CWinApp
{
public:
	CCustomEqStatusDemoApp();

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
	/// ���W���[���t�@�C���p�X
	CString		mAppPath;
	/// AppData�p�X
	CString		mAppDataPath;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	InitializeApp();
	void	TerminateApp();

protected:

	/* ------------------------------------------------------------------------------------ */

	//////////////////////////////////////////////////////////////////////////////////////////
	/// ���J�X�^�}�C�Y�@�\
	//////////////////////////////////////////////////////////////////////////////////////////
	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum { ePopMon = 1, ePopCon };

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	/// �}�X�^�f�[�^�i�[�p�X
	CString					mAppMasterDataPath;
	/// ���[�U�f�[�^�i�[�p�X
	CString					mAppUserDataPath;
	/// �J�X�^���f�[�^�Ǘ��֘A 
	CCustomDataManager		mDataManager;
	/// �J�X�^�}�C�Y�Ǘ�
	CCustomManager			mCustomManager;
	/// �J�X�^���Ǘ���ʈʒu
	CPoint					mManagerPoint;
	CPoint					mCascadePoint;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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
	/// ���[�U�ݔ��ڍ׃f�[�^�i�[�p�X
	CString	GetUserDataPath() { return mAppUserDataPath; }
	/// �J�X�^���f�[�^�Ǘ��֘A
	CCustomDataManager&	GetDataManager() { return mDataManager; }
	/// �J�X�^�}�C�Y�Ǘ�
	CCustomManager&	GetCustomManager() { return mCustomManager; }
	/// �ݔ��ڍ׃E�B���h�E�̍쐬
	CCustomDetail*	CreateEquipment(CTreeNode* pnode);
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

protected:
	void	createMasterEquipment();
	void	createUserEquipment();
	void	getMasterEquipmentFiles(vector<CString>& list);
	void	getUserEquipmentFiles(vector<CString>& list);

	//////////////////////////////////////////////////////////////////////////////////////////

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CCustomEqStatusDemoApp theApp;