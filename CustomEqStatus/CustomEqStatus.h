
// CustomEqStatus.h : CustomEqStatus �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��


// CCustomEqStatusApp:
// ���̃N���X�̎����ɂ��ẮACustomEqStatus.cpp ���Q�Ƃ��Ă��������B
//

class CCustomEqStatusApp : public CWinApp
{
public:
	CCustomEqStatusApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCustomEqStatusApp theApp;
