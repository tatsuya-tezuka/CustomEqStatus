#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomControl.h"


BOOL CALLBACK CCustomMonitors::CustomMonitorNumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	CCustomMonitors* pmon = (CCustomMonitors*)dwData;

	// ���j�^���̎擾
	MONITORINFOEX mInfo;
	mInfo.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &mInfo);

	// ���j�^�̈�̎擾
	CRect rect(lprcMonitor);
	pmon->AddMonitor(hMonitor, rect, &mInfo);

	return TRUE;
}

CCustomMonitors::CCustomMonitors()
{
	// �}���`���j�^�Ή�
	EnumDisplayMonitors(NULL, NULL, CustomMonitorNumProc, (LPARAM)this);
}

CCustomMonitors::~CCustomMonitors()
{
}

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

@param	pParent		���C���E�B���h�E�n���h��
@param	appPath		�A�v���P�[�V�����p�X
@retval

*/
/*============================================================================*/
void CCustomControl::Initialize(CWnd* pParent, CString appPath)
{
	mMainWnd = pParent;

	/// �}�X�^�p�f�[�^�i�[�p�X
	mAppMasterDataPath = appPath + _T("\\") + mAppDataMasterPath;

	/// �f���p�f�[�^�i�[�p�X
	mAppUserDataPath = appPath + _T("\\") + mAppDataUserPath;

	/// �f�[�^�x�[�X�p�X
	mAppDataBasePath = appPath + _T("\\") + _T("db");

	/// �S�ẴE�B���h�E�n���h���A�m�[�h�̍폜
	GetDataManager().DeleteAll();

	/// �}�X�^�ݔ��ڍ׃f�[�^��ǂݍ���
	createMasterEquipment();

	/// ���[�U�ݔ��ڍ׃f�[�^��ǂݍ���
	createUserEquipment();

	// ���f�[�^�x�[�X�擾
	const CString mpDBName[] = {
		_T("eqmon.db.obj"),
		_T("eqctl.db.obj"),
	};
	// �Ď�DB�擾
	CString str = mAppDataBasePath + _T("\\") + mpDBName[0];
	GetDataManager().GetDataMonitor().eq_db_read(str);
	// ����DB�擾
	str = mAppDataBasePath + _T("\\") + mpDBName[1];
	GetDataManager().GetDataControl().eq_db_read(str);
}

/*============================================================================*/
/*! �J�X�^���R���g���[��

-# �ݔ��ڍ׃E�B���h�E�̍쐬

@param		pnode		�m�[�h���
@retval

*/
/*============================================================================*/
CCustomDetail* CCustomControl::CreateEquipment(CTreeNode* pnode, UINT mode/* = eTreeItemMode_Monitor*/)
{
	// �ݔ��ڍ׉�ʂ̍쐬
	CCustomDetail* pitem = new CCustomDetail(theApp.GetMainWnd(), (pnode == NULL) ? false : true);
	if (pnode != NULL) {
		pnode->GetEquipment().wnd = pitem;
		pnode->GetEquipment().manager = &mCustomManager;
	}
	// �ݔ��ڍ׉�ʂ̍쐬
	pitem->Create(IDD_DIALOG_EQDETAIL, theApp.GetMainWnd());
	pitem->ShowWindow((pnode == NULL) ? SW_SHOW : pnode->GetEquipment().placement.showCmd);

	// �Ǘ��E�B���h�E�̐ݒ�
	if (pnode == NULL) {
		CTreeNode* pnodeitem = GetDataManager().SearchWndNode(pitem);
		pnodeitem->GetEquipment().wnd = pitem;
		pnodeitem->GetEquipment().manager = &mCustomManager;
		//swprintf_s(pnodeitem->GetWindowInfo().groupname, mNameSize, _T("%s"), (LPCTSTR)mNoGroupText);
		pnodeitem->GetManager().groupno = 0;
		pnodeitem->GetEquipment().mode = mode;
		pnodeitem->GetEquipment().kind = eTreeItemKind_User;
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
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("createMasterEquipment"), (*itrmaster), _T(""), nLogEx::info);
		//������������������������ Log ������������������������//
		//=====================================================//
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
		//=====================================================//
		//������������������������ Log ������������������������//
		CLogTraceEx::Write(_T("***"), _T("CCustomControl"), _T("createUserEquipment"), (*itruser), _T(""), nLogEx::info);
		//������������������������ Log ������������������������//
		//=====================================================//
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
