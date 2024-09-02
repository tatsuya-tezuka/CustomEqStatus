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
	ON_COMMAND(ID_MANAGER_NEW, &CCustomManager::OnManagerNew)
	ON_COMMAND(ID_MANAGER_DELETE, &CCustomManager::OnManagerDelete)
	ON_COMMAND(ID_MANAGER_MONITOR, &CCustomManager::OnManagerMonitor)
	ON_COMMAND(ID_MANAGER_EDIT, &CCustomManager::OnManagerEdit)
	ON_COMMAND(ID_MANAGER_SHOW, &CCustomManager::OnManagerShow)
	ON_COMMAND(ID_MANAGER_HIDE, &CCustomManager::OnManagerHide)
	ON_COMMAND(ID_MANAGER_CREATE, &CCustomManager::OnManagerCreate)
	ON_COMMAND(ID_MANAGER_CANCEL, &CCustomManager::OnManagerCancel)
	ON_COMMAND(ID_MANAGER_LOAD, &CCustomManager::OnManagerLoad)
	ON_COMMAND(ID_MANAGER_SAVE, &CCustomManager::OnManagerSave)
	ON_WM_CONTEXTMENU()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
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
		mManagerList.AddItem(item, 2, _T("0"));
		item++;
		mManagerList.AddItem(item, 0, _T("S�ё��M"), 0);
		mManagerList.AddItem(item, 1, _T("Demo"));
		mManagerList.AddItem(item, 2, _T("1"));
		item++;
		mManagerList.AddItem(item, 0, _T("S�ё���"), 0);
		mManagerList.AddItem(item, 1, _T("Demo"));
		mManagerList.AddItem(item, 2, _T("1"));
		item++;
		mManagerList.AddItem(item, 0, _T("X-TX"), 0);
		mManagerList.AddItem(item, 1, _T("Demo"));
		mManagerList.AddItem(item, 2, _T("0"));
	}
	else{
		mManagerList.AddItem(item, 0, _T("#�A���e�i"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("0"));
		item++;
		mManagerList.AddItem(item, 0, _T("#S�ё��M"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("1"));
		item++;
		mManagerList.AddItem(item, 0, _T("#S�ё���"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("1"));
		item++;
		mManagerList.AddItem(item, 0, _T("#X-TX"), 0);
		mManagerList.AddItem(item, 1, _T("#Demo"));
		mManagerList.AddItem(item, 2, _T("0"));
	}

	mManagerList.SetRedraw(TRUE);
#ifdef _TRIAL
	return;
#endif

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
	SetControlInfo(IDC_LIST_MANAGER, ANCHORE_LEFTTOP | RESIZE_BOTH);

#ifdef _TRIAL
	_CreateDemo((int)eSelectUser);
	return TRUE;
#endif
	if (theApp.GetDataManager().GetTreeNode().size() != 0){
		createItem((int)eSelectUser);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�\���C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �E�B���h�E�̕\��/��\���C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CCustomDialogBase::OnShowWindow(bShow, nStatus);

	if (bShow == TRUE){
		// �\��
		UpdateData(TRUE);
#ifdef _TRIAL
		_CreateDemo((int)mSelectType);
		return;
#endif
		if (theApp.GetDataManager().GetTreeNode().size() != 0){
			createItem((int)mSelectType);
		}
	}
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
	UpdateData(TRUE);
#ifdef _TRIAL
	_CreateDemo((int)eSelectUser);
	return;
#endif
	if (theApp.GetDataManager().GetTreeNode().size() != 0){
		createItem((int)mSelectType);
	}
	UpdateData(FALSE);
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
	UpdateData(TRUE);
#ifdef _TRIAL
	_CreateDemo((int)eSelectMaster);
	return;
#endif
	if (theApp.GetDataManager().GetTreeNode().size() != 0){
		createItem((int)mSelectType);
	}
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
	*pResult = 0;

	CPoint pos;
	if (::GetCursorPos(&pos) == FALSE)
	{
		ASSERT(FALSE);
		return;
	}

	mManagerList.ScreenToClient(&pos);
	int item = mManagerList.HitTest(pos);

	CMenu menu;
	if (!menu.LoadMenu(IDR_POPUP_MANAGER)){
		ASSERT(FALSE);
		return;
	}

	UpdateData(TRUE);

	CMenu* pMenu = menu.GetSubMenu(0);
	ASSERT(pMenu);

	updateMenuItemStatus(pMenu);

	mManagerList.ClientToScreen(&pos);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);
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

	int nItem = pNMItemActivate->iItem;
	if (nItem < 0)
		return;
	CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(nItem);
	if (pnode != NULL){
		if (pnode->GetWindowInfo().wnd == NULL){
			CCustomDetail* pitem = theApp.CreateEquipment(pnode);
			if (pitem == NULL)
				return;
		}
		pnode->GetWindowInfo().wnd->ShowWindow(SW_SHOWNA);
		pnode->GetWindowInfo().wnd->SetActiveWindow();
	}

	*pResult = 0;
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u�쐬�v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerNew()
{
	UpdateData(TRUE);
#ifdef _TRIAL
	return;
#endif
	createEqDetail(NULL);
	mManagerList.GroupByColumn(eManagerGroup, (mSelectType == eSelectUser) ? TRUE : FALSE);
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u�폜�v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerDelete()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u���[�h-�Ď��v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerMonitor()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u���[�h-�ҏW�v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerEdit()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u�\��-�\���v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerShow()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u�\��-��\���v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerHide()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u��ʘA��-�쐬�v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerCreate()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u��ʘA��-�����v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerCancel()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u�ďo�v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerLoad()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u�ۑ��v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerSave()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �ݔ��ڍׂ����X�g�֓o�^����

@param

@retval
*/
/*============================================================================*/
void CCustomManager::createItem(int nSelect)
{
	mManagerList.SetRedraw(FALSE);

	mManagerList.RemoveAllGroups();
	mManagerList.DeleteAllItems();

	vector<CTreeNode*>& treedata = theApp.GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	for (itr = treedata.begin(); itr != treedata.end(); itr++){
		int count = mManagerList.GetItemCount();
		if ((*itr)->GetWindowInfo().kind == nSelect){
			mManagerList.AddItem(count, 0, (*itr)->GetWindowInfo().title, (LPARAM)(*itr));
			mManagerList.AddItem(count, 1, (*itr)->GetWindowInfo().memo);
			mManagerList.AddItem(count, 2, (*itr)->GetWindowInfo().group);
			mManagerList.SetItemData(count, (LPARAM)(*itr));
			(*itr)->GetWindowInfo().manager = this;
		}
	}

	mManagerList.SetRedraw(TRUE);

	mManagerList.GroupByColumn(eManagerGroup, (nSelect == eSelectUser) ? TRUE : FALSE);
}

/*============================================================================*/
/*! �J�X�^�}�C�Y�Ǘ�

-# �ݔ��ڍׂ̓o�^

@param		node		�ڍ׉�ʂ��쐬���錳�m�[�h���i�V�K�쐬����NULL�j
@retval

*/
/*============================================================================*/
void CCustomManager::createEqDetail(CTreeNode* node/*=NULL*/)
{
	// �ݔ��ڍ׉�ʂ̍쐬
	CCustomDetail* pitem = theApp.CreateEquipment(NULL);

	CTreeNode* pnode = theApp.GetDataManager().SearchWndNode(pitem);
	createItem((int)mSelectType);
	//int count = mManagerList.GetItemCount();
	//mManagerList.AddItem(count, 0, mDefaultCustomTitle, (LPARAM)node);
	//mManagerList.AddItem(count, 1, _T(""));
	//mManagerList.AddItem(count, 2, _T("0"));
	//mManagerList.SetItemData(count, (LPARAM)pnode);
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# ���j���[���ڂ̏�Ԃ��X�V����i�����j���[�\���j

@param	pMenu	���j���[�n���h��

@retval
*/
/*============================================================================*/
void CCustomManager::updateMenuItemStatus(CMenu* pMenu)
{
	UINT count = pMenu->GetMenuItemCount();
	for (UINT pos = 0; pos < count; pos++){
		UINT state = pMenu->GetMenuState(pos, MF_BYPOSITION);
		if (state & MF_POPUP){
			CMenu* pPopup = pMenu->GetSubMenu(pos);
			updateMenuItemStatus(pPopup);
		}
		else{
			BOOL bKind = (mSelectType == eSelectUser) ? TRUE : FALSE;
			BOOL bSelect = (mManagerList.GetSelectedCount() > 0) ? TRUE : FALSE;
			BOOL bMultiSelect = (mManagerList.GetSelectedCount() > 1) ? TRUE : FALSE;
			pMenu->EnableMenuItem(ID_MANAGER_NEW, MF_BYCOMMAND | (bKind) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_DELETE, MF_BYCOMMAND | (bSelect) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_MONITOR, MF_BYCOMMAND | (bSelect) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_EDIT, MF_BYCOMMAND | (bSelect) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_SHOW, MF_BYCOMMAND | (bSelect) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_HIDE, MF_BYCOMMAND | (bSelect) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_CREATE, MF_BYCOMMAND | (bSelect) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_CANCEL, MF_BYCOMMAND | (bSelect) ? MF_ENABLED : MF_GRAYED);
		}
	}
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �}�E�X���{�^������

@param
@retval

*/
/*============================================================================*/
void CCustomManager::OnLButtonDown(UINT nFlags, CPoint point)
{
	int pos = 0, index;
	int colnum, strpos = 0;

	if ((index = mManagerList.HitTestEx(point, &colnum)) != -1 && colnum != 0){
		UINT flag = LVIS_SELECTED;
		if ((mManagerList.GetItemState(index, flag) & flag) == flag){
			if (mManagerList.EditExecute(index, colnum) == false){
				mManagerList.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
		else{
			mManagerList.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}

	CCustomDialogBase::OnLButtonDown(nFlags, point);
}
