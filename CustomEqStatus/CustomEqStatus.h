
// CustomEqStatus.h : CustomEqStatus �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��
#include "LogTraceEx.h"
#include "DBAccess.h"


// CCustomEqStatusApp:
// ���̃N���X�̎����ɂ��ẮACustomEqStatus.cpp ���Q�Ƃ��Ă��������B
//

static const TCHAR* mAppDataSystem = { _T("NEC") };
static const TCHAR* mAppDataDataPath = { _T("DATA") };
static const TCHAR* mAppDataLogPath = { _T("LOG") };

class CCustomEqStatusApp : public CWinApp
{
public:
	CCustomEqStatusApp();

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
	/// ���W���[���t�@�C���p�X
	CString		mAppPath;
	/// AppData�p�X
	CString		mAppDataPath;

	/// �f�[�^�x�[�X�A�N�Z�X�N���X�iStationControl����̗��p�j
	CDBAccess	mDBAccess;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString&	GetAppPath() { return mAppPath; }
	CString&	GetAppDataPath() { return mAppDataPath; }
	CDBAccess&	GetDBAccess() { return mDBAccess; }

	void	InitializeApp();
	void	TerminateApp();

protected:

	/* ------------------------------------------------------------------------------------ */

	// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCustomEqStatusApp theApp;
