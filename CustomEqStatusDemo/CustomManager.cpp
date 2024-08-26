/******************************************************************************
* @file    CustomManager.cpp
* @brief   �y�J�X�^�}�C�Y�@�\�z�ݔ��󋵊Ǘ��N���X
* @auther  SCC
* @date    2024/08/26 �V�K�쐬
* @par
******************************************************************************/
// CustomManager.cpp : �����t�@�C��
//
#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomManager.h"
#include "afxdialogex.h"


// CCustomManager �_�C�A���O

IMPLEMENT_DYNAMIC(CCustomManager, CCustomDialogBase)

CCustomManager::CCustomManager(CWnd* pParent /*=NULL*/)
: CCustomDialogBase(CCustomManager::IDD, pParent)
, mSelectType(0)
{

}

CCustomManager::~CCustomManager()
{
}

void CCustomManager::DoDataExchange(CDataExchange* pDX)
{
	CCustomDialogBase::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_USER, mSelectType);
	DDX_Control(pDX, IDC_LIST_MANAGER, mManagerList);
}


BEGIN_MESSAGE_MAP(CCustomManager, CCustomDialogBase)
	ON_BN_CLICKED(IDC_RADIO_USER, &CCustomManager::OnBnClickedRadioUser)
	ON_BN_CLICKED(IDC_RADIO_MASTER, &CCustomManager::OnBnClickedRadioMaster)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MANAGER, &CCustomManager::OnNMRClickListManager)
END_MESSAGE_MAP()


// CCustomManager ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# ������

@param

@retval
*/
/*============================================================================*/
BOOL CCustomManager::OnInitDialog()
{
	CCustomDialogBase::OnInitDialog();

	// TODO: �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �u���[�U�v�{�^�������C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnBnClickedRadioUser()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �u�}�X�^�v�{�^�������C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnBnClickedRadioMaster()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
}


void CCustomManager::OnNMRClickListManager(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	*pResult = 0;
}
