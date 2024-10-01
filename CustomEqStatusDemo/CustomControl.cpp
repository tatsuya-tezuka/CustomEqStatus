#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomControl.h"


//------------------------------------------------------------------------------------ 
//	�T�v�F
//------------------------------------------------------------------------------------
CCustomControl::CCustomControl()
{
}

CCustomControl::~CCustomControl()
{
}

/*============================================================================*/
/*! �J�X�^���R���g���[��

-# �J�X�^���R���g���[���̏�����

@param	appPath		�A�v���P�[�V�����p�X
@retval

*/
/*============================================================================*/
void CCustomControl::Initialize(CString appPath)
{
	/// �}�X�^�p�f�[�^�i�[�p�X
	mAppMasterDataPath = appPath + _T("\\") + mAppDataMasterPath;

	/// �f���p�f�[�^�i�[�p�X
	mAppUserDataPath = appPath + _T("\\") + mAppDataUserPath;

	/// �S�ẴE�B���h�E�n���h���A�m�[�h�̍폜
	GetDataManager().DeleteAll();

	/// �}�X�^�ݔ��ڍ׃f�[�^��ǂݍ���
	createMasterEquipment();

	/// ���[�U�ݔ��ڍ׃f�[�^��ǂݍ���
	createUserEquipment();
}

/*============================================================================*/
/*! �J�X�^���R���g���[��

-# �ݔ��ڍ׃E�B���h�E�̍쐬

@param		pnode		�m�[�h���
@retval

*/
/*============================================================================*/
CCustomDetail* CCustomControl::CreateEquipment(CTreeNode* pnode)
{
	// �ݔ��ڍ׉�ʂ̍쐬
	CCustomDetail* pitem = new CCustomDetail(theApp.GetMainWnd(), (pnode == NULL) ? false : true);
	if (pnode != NULL) {
		pnode->GetWindowInfo().wnd = pitem;
		pnode->GetWindowInfo().manager = &mCustomManager;
	}
	// �ݔ��ڍ׉�ʂ̍쐬
	pitem->Create(IDD_DIALOG_EQDETAIL, theApp.GetMainWnd());
	pitem->ShowWindow((pnode == NULL) ? SW_SHOW : pnode->GetWindowInfo().placement.showCmd);

	// �Ǘ��E�B���h�E�̐ݒ�
	if (pnode == NULL) {
		CTreeNode* pnodeitem = GetDataManager().SearchWndNode(pitem);
		pnodeitem->GetWindowInfo().wnd = pitem;
		pnodeitem->GetWindowInfo().manager = &mCustomManager;
		swprintf_s(pnodeitem->GetWindowInfo().groupname, mNameSize, _T("%s"), (LPCTSTR)_T("0"));
		pnodeitem->GetWindowInfo().groupno = 0;
		pnodeitem->GetWindowInfo().mode = eTreeItemMode_Monitor;
		pnodeitem->GetWindowInfo().kind = eTreeItemKind_User;
	}
	return pitem;
}

/*============================================================================*/
/*! �J�X�^���R���g���[��

-# �}�X�^�ݔ��ڍ׃f�[�^�̍쐬

@param
@retval

*/
/*============================================================================*/
void CCustomControl::createMasterEquipment()
{
	CWaitCursor wait;

	vector<CString> filelist;
	getMasterEquipmentFiles(filelist);
	vector<CString>::iterator itrmaster;
	for (itrmaster = filelist.begin(); itrmaster != filelist.end(); itrmaster++) {
		GetDataManager().LoadTreeDataXml((*itrmaster), eTreeItemKind_Master);
	}
}

/*============================================================================*/
/*! �J�X�^���R���g���[��

-# ���[�U�ݔ��ڍ׃f�[�^�̍쐬

@param
@retval

*/
/*============================================================================*/
void CCustomControl::createUserEquipment()
{
	CWaitCursor wait;

	vector<CString> filelist;
	getUserEquipmentFiles(filelist);
	vector<CString>::iterator itruser;
	for (itruser = filelist.begin(); itruser != filelist.end(); itruser++) {
		GetDataManager().LoadTreeDataXml((*itruser), eTreeItemKind_User);
	}
}

/*============================================================================*/
/*! �J�X�^���R���g���[��

-# �}�X�^�ݔ��ڍ׃t�@�C���̎擾

@param
@retval

*/
/*============================================================================*/
void CCustomControl::getMasterEquipmentFiles(vector<CString>& list)
{
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getMasterEquipmentFiles"), _T("Start"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;

	list.clear();

	//�g���q�̐ݒ�
	CString search_name = mAppMasterDataPath + _T("\\*.xml");

	hFind = FindFirstFile(search_name, &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		}
		else {
			list.push_back(mAppMasterDataPath + _T("\\") + CString(win32fd.cFileName));
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getMasterEquipmentFiles"), _T("Stop"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
}

/*============================================================================*/
/*! �J�X�^���R���g���[��

-# ���[�U�ݔ��ڍ׃t�@�C���̎擾

@param
@retval

*/
/*============================================================================*/
void CCustomControl::getUserEquipmentFiles(vector<CString>& list)
{
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getUserEquipmentFiles"), _T("Start"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;

	list.clear();

	//�g���q�̐ݒ�
	CString search_name = mAppUserDataPath + _T("\\*.xml");

	hFind = FindFirstFile(search_name, &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		}
		else {
			list.push_back(mAppUserDataPath + _T("\\") + CString(win32fd.cFileName));
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("getUserEquipmentFiles"), _T("Stop"), _T(""), nLogEx::debug);
	//������������������������ Log ������������������������//
	//=====================================================//
}
