/******************************************************************************
* @file    CustomManager.h
* @brief   �y�J�X�^�}�C�Y�@�\�z�ݔ��󋵊Ǘ��N���X
* @auther  SCC
* @date    2024/08/26 �V�K�쐬
* @par
******************************************************************************/
#pragma once

#include "CustomDialogBase.h"

// CCustomManager �_�C�A���O

class CCustomManager : public CCustomDialogBase
{
	DECLARE_DYNAMIC(CCustomManager)

public:
	CCustomManager(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CCustomManager();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
