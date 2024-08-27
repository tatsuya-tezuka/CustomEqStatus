
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
	/// �f���f�[�^�i�[�p�X
	CString		mAppDemoDataPath;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	InitializeApp();
	void	TerminateApp();
	CString	GetDemoDataPath() { return mAppDemoDataPath; }

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
	/// �J�X�^���f�[�^�Ǘ��֘A 
	CCustomDataManager		mDataManager;
	/// �J�X�^�}�C�Y�Ǘ�
	CCustomManager			mCustomManager;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	/// �J�X�^���f�[�^�Ǘ��֘A
	CCustomDataManager&	GetDataManager() { return mDataManager; }
	/// �J�X�^�}�C�Y�Ǘ�
	CCustomManager&	GetCustomManager() { return mCustomManager; }
	/// �ݔ��ڍ׃E�B���h�E�̍쐬
	CCustomDetail*	CreateEquipment(CTreeNode* pnode);

protected:

	//////////////////////////////////////////////////////////////////////////////////////////

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CCustomEqStatusDemoApp theApp;