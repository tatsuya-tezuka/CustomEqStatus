
// CustomCsvToXml.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CCustomCsvToXmlApp:
// ���̃N���X�̎����ɂ��ẮACustomCsvToXml.cpp ���Q�Ƃ��Ă��������B
//

class CCustomCsvToXmlApp : public CWinApp
{
public:
	CCustomCsvToXmlApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CCustomCsvToXmlApp theApp;