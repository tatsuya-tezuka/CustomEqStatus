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
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MANAGER, &CCustomManager::OnNMDblclkListManager)
END_MESSAGE_MAP()


void CCustomManager::_CreateDemo(int nSelect)
{
	mManagerList.SetRedraw(FALSE);

	mManagerList.RemoveAllGroups();
	mManagerList.DeleteAllItems();

	int item = 0;
	if (nSelect == eSelectUser){
		mManagerList.AddItem(item, 0, _T("�A���e�i"), 0);
		mManagerList.AddItem(item, 1, _T("Demo"));
		mManagerList.AddItem(item, 2, _T("�O���[�v�Ȃ�"));
		item++;
		mManagerList.AddItem(item, 0, _T("S�ё��M"), 0);
		mManagerList.AddItem(item, 1, _T("Demo"));
		mManagerList.AddItem(item, 2, _T("�O���[�v�P"));
		item++;
		mManagerList.AddItem(item, 0, _T("S�ё���"), 0);
		mManagerList.AddItem(item, 1, _T("Demo"));
		mManagerList.AddItem(item, 2, _T("�O���[�v�P"));
		item++;
		mManagerList.AddItem(item, 0, _T("X-TX"), 0);
		mManagerList.AddItem(item, 1, _T("Demo"));
		mManagerList.AddItem(item, 2, _T("�O���[�v�Ȃ�"));
	}
	else{
		mManagerList.AddItem(item, 0, _T("#�A���e�i"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("�O���[�v�Ȃ�"));
		item++;
		mManagerList.AddItem(item, 0, _T("#S�ё��M"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("�O���[�v�P"));
		item++;
		mManagerList.AddItem(item, 0, _T("#S�ё���"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("�O���[�v�P"));
		item++;
		mManagerList.AddItem(item, 0, _T("#X-TX"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("�O���[�v�Ȃ�"));
	}

	mManagerList.SetRedraw(TRUE);

	mManagerList.GroupByColumn(eManagerGroup, (nSelect == eSelectUser)?TRUE:FALSE);
}

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

	mManagerList.CreateGroupControl();

#ifdef _DEMO
	_CreateDemo((int)eSelectUser);
#endif


	SetControlInfo(IDC_LIST_MANAGER, ANCHORE_LEFTTOP | RESIZE_BOTH);

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

#ifdef _DEMO
	_CreateDemo((int)eSelectUser);
#endif
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

#ifdef _DEMO
	_CreateDemo((int)eSelectMaster);
#endif
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# ���X�g�R���g���[���ł̃}�E�X�E�N���b�N�C�x���g�i�����j���[�\���j

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnNMRClickListManager(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	*pResult = 0;
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# ���X�g�R���g���[���ł̃}�E�X�_�u���N���b�N�i���ΏۃE�B���h�E�̃A�N�e�B�u���j

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnNMDblclkListManager(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	*pResult = 0;
}
