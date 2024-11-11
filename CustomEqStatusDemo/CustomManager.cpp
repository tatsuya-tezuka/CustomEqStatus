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
	ON_COMMAND(ID_MANAGER_SHOW, &CCustomManager::OnManagerShow)
	ON_COMMAND(ID_MANAGER_CREATE, &CCustomManager::OnManagerCreate)
	ON_COMMAND(ID_MANAGER_CANCEL, &CCustomManager::OnManagerCancel)
	ON_WM_CONTEXTMENU()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENUMANAGER_CLOSE, &CCustomManager::OnMenumanagerClose)
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

	mManagerList.CreateGroupControl(this);
	SetControlInfo(IDC_LIST_MANAGER, ANCHORE_LEFTTOP | RESIZE_BOTH);

	// ���X�g�ɓo�^
	if (theApp.GetCustomControl().GetDataManager().GetTreeNode().size() != 0){
		createItem((int)eSelectUser);
	}

	UpdateGroup();

	CenterWindow(GetDesktopWindow());

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
		if (theApp.GetCustomControl().GetDataManager().GetTreeNode().size() != 0){
			createItem((int)mSelectType);
		}
		CRect rect;
		GetWindowRect(rect);
		theApp.GetCustomControl().UpdateCustomManagerPoint(CPoint(rect.left, rect.top));
	}
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �E�B���h�E�̈ړ�

@param
@retval

*/
/*============================================================================*/
void CCustomManager::OnMove(int x, int y)
{
	CCustomDialogBase::OnMove(x, y);

	CRect rect;
	GetWindowRect(rect);
	theApp.GetCustomControl().UpdateCustomManagerPoint(CPoint(rect.left, rect.top));
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
	if (theApp.GetCustomControl().GetDataManager().GetTreeNode().size() != 0){
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
	if (theApp.GetCustomControl().GetDataManager().GetTreeNode().size() != 0){
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

	UINT nMenuRes = IDR_POPUP_MANAGER;
	int item = mManagerList.HitTest(pos);
	if(item >= 0){
		// �A�C�e������m�[�h�����擾����
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		if (HIWORD(pnode->GetWindowInfo().groupno) != 0) {
			nMenuRes = IDR_POPUP_MANAGER_GROUP;
		}
		TRACE("[MANAGER]Rclick Item=%d Group=%08X)\n", item, pnode->GetWindowInfo().groupno);
	}
	else {
		TRACE("[MANAGER]Rclick Item=--)\n");
	}

	CMenu menu;
	if (!menu.LoadMenu(nMenuRes)){
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
#if _DEMO_PHASE < 40
	return;
#endif

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;
	if (nItem < 0)
		return;

	showCustomDetail(nItem, true);

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
#if _DEMO_PHASE < 30
	return;
#endif
	UpdateData(TRUE);
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
	CString msg;
	msg.Format(mMessage_ManagerDelete);
	if (MessageBox(msg, mMessage_Title_CustomDetail, MB_YESNO | MB_ICONQUESTION) == IDNO) {
		return;
	}

#if _DEMO_PHASE < 110
	return;
#endif
	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = mManagerList.GetNextSelectedItem(pos);
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(nItem);
		if (pnode == NULL)
			continue;

		CString msg;
		msg.Format(mMessage_DetailDelete, pnode->GetWindowInfo().title);
		if (MessageBox(msg, mMessage_Title_CustomManager, MB_YESNO | MB_ICONQUESTION) == IDNO) {
			continue;
		}

		// �ݔ������ʂ̍폜
		CString xmlfile = CString(pnode->GetXmlFileName());
		theApp.GetCustomControl().GetDataManager().DeleteItemWnd(pnode->GetWindowInfo().wnd);
		theApp.GetCustomControl().GetDataManager().DeleteItemNode(pnode);
		DeleteFile(xmlfile);

		mManagerList.DeleteItem(nItem);
		pos = mManagerList.GetFirstSelectedItemPosition();
	}
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
#if _DEMO_PHASE < 100
	return;
#endif
	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = mManagerList.GetNextSelectedItem(pos);
		showCustomDetail(nItem, true);
	}
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

-# �ݔ��ڍׂ̍쐬

@param

@retval
*/
/*============================================================================*/
void CCustomManager::UpdateGroup()
{
	// �o�^����Ă���J�X�^����ʂ̃O���[�v�X�V
	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	// �S�ẴO���[�v�����폜
	mSyncWindow.Clear(0);
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		if ((*itr)->GetWindowInfo().kind == eTreeItemKind_User && HIWORD((*itr)->GetWindowInfo().groupno) != 0) {
			mSyncWindow.Set((*itr)->GetWindowInfo().groupno, (*itr)->GetWindowInfo().wnd);
		}
	}

	mSyncWindow.Start();
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �O���[�v�����ԍ��̃��Z�b�g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::ResetGroupInnerNo()
{
	map<UINT, UINT>	maxlist;
	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	// �S�ẴO���[�v�����폜
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		if ((*itr)->GetWindowInfo().kind == eTreeItemKind_User) {
			map<UINT, UINT>::iterator itrmax = maxlist.find((UINT)HIWORD((*itr)->GetWindowInfo().groupno));
			if (itrmax == maxlist.end()) {
				maxlist.insert(map<UINT, UINT>::value_type(HIWORD((*itr)->GetWindowInfo().groupno), 1));
				(*itr)->GetWindowInfo().groupno = HIWORD((*itr)->GetWindowInfo().groupno) << 16 | 1;
			}
			else {
				// ���[�N�̃O���[�v�ԍ����X�V����
				(*itrmax).second += mGroupRange;
				// �m�[�h�̃O���[�v�ԍ����X�V����
				(*itr)->GetWindowInfo().groupno = HIWORD((*itr)->GetWindowInfo().groupno) << 16 | (*itrmax).second;
			}
		}
	}
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �ݔ��ڍׂ����X�g�֓o�^����

@param	nSelect		���[�Uor�}�X�^

@retval
*/
/*============================================================================*/
void CCustomManager::createItem(UINT nSelect)
{
	mManagerList.SetRedraw(FALSE);

	mManagerList.RemoveAllGroups();
	mManagerList.DeleteAllItems();

	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	for (itr = treedata.begin(); itr != treedata.end(); itr++){
		int count = mManagerList.GetItemCount();
		if ((*itr)->GetWindowInfo().kind == nSelect){
			mManagerList.AddItem(count, 0, (*itr)->GetWindowInfo().title, (LPARAM)(*itr));
			mManagerList.AddItem(count, 1, (*itr)->GetWindowInfo().memo);
			mManagerList.AddItem(count, 2, (*itr)->GetWindowInfo().groupname);
			mManagerList.SetItemData(count, (LPARAM)(*itr));
			(*itr)->GetWindowInfo().manager = this;
		}
	}

	ResetGroupInnerNo();

	mManagerList.SetRedraw(TRUE);

	mManagerList.GroupByColumn(eManagerGroup, (nSelect == eSelectUser) ? TRUE : FALSE);

	if (nSelect == eSelectUser) {
		mManagerList.SortGroup();
	}

	mSelectType = nSelect;
	UpdateData(FALSE);
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �ݔ��ڍׂ̍쐬

@param

@retval
*/
/*============================================================================*/
void CCustomManager::createEquipment()
{
	// �o�^����Ă���J�X�^����ʂ̍쐬
	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		// �ݔ��ڍ׉�ʂ̍쐬
		CCustomDetail* pitem = theApp.GetCustomControl().CreateEquipment((*itr));
	}
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
	CCustomDetail* pitem = theApp.GetCustomControl().CreateEquipment(NULL, eTreeItemMode_Edit);

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(pitem);
	// createItem�����s���邱�ƂŊǗ���ʂɕ\�������
	createItem((int)mSelectType);
	if (pnode->GetWindowInfo().wnd != NULL) {
		CPoint point = theApp.GetCustomControl().GetCascadePoint();
		pnode->GetWindowInfo().wnd->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}
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
			pMenu->EnableMenuItem(ID_MANAGER_DELETE, MF_BYCOMMAND | (bKind) && (bSelect || bMultiSelect) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_SHOW, MF_BYCOMMAND | (bSelect || bMultiSelect) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_CREATE, MF_BYCOMMAND | (bKind) && (bMultiSelect) ? MF_ENABLED : MF_GRAYED);
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

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# ���b�Z�[�W����

@param
@retval

*/
/*============================================================================*/
LRESULT CCustomManager::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	CTreeNode* pnode;
	switch (message) {
	case	eUserMessage_Manager_Update:
		updateItemData(lParam);
		break;
	case	eUserMessage_Manager_Reset:
		if (lParam == 1 && mSelectType != eSelectUser) {
			break;
		}
		createItem((int)eSelectUser);
		UpdateGroup();
		break;
	case	eUserMessage_Manager_Delete:
		pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode((CWnd*)lParam);
		theApp.GetCustomControl().GetDataManager().DeleteItemWnd((CWnd*)lParam);
		if (wParam == 1) {
			theApp.GetCustomControl().GetDataManager().DeleteItemNode(pnode);
			createItem(mSelectType);
		}
		break;
	default:
		return CCustomDialogBase::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �ݔ��ڍׂ̍X�V

@param
@retval

*/
/*============================================================================*/
void CCustomManager::updateItemData(LPARAM lParam)
{
	int count = mManagerList.GetItemCount();
	for (int i = 0; i < count; i++) {
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(i);
		mManagerList.SetItemText(i, 0, pnode->GetWindowInfo().title);
	}
	mManagerList.Invalidate();
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �E�B���h�E�uX�v�{�^������

@param
@retval

*/
/*============================================================================*/
void CCustomManager::OnClose()
{
	OnMenumanagerClose();
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# ���l���̍X�V

@param
@retval

*/
/*============================================================================*/
void CCustomManager::updateXmlFile()
{
#if _DEMO_PHASE < 60
	return;
#endif
	int count = mManagerList.GetItemCount();
	for (int i = 0; i < count; i++) {
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(i);
		swprintf_s(pnode->GetWindowInfo().memo, mTitleSize, _T("%s"), (LPCTSTR)mManagerList.GetItemText(i, 1));

		CMarkup xml;
		xml.Load(pnode->GetXmlFileName());
		xml.FindElem(_T("ROOT"));
		xml.IntoElem();
		xml.FindElem(_T("EQUIPMENT"));
		xml.IntoElem();
		xml.FindElem(_T("WINDOWINFO"));
		xml.IntoElem();

		xml.FindElem(_T("TYPE"));
		UINT type = _wtoi(xml.GetData());
		if (type == eTreeItemType_Title) {
			xml.FindElem(_T("TITLE"));
			if (xml.FindElem(_T("MEMO")) == false) {
				xml.AddElem(_T("MEMO"), mManagerList.GetItemText(i, 1));
			}
			else {
				xml.SetData(mManagerList.GetItemText(i, 1));
			}
		}

		xml.Save(pnode->GetXmlFileName());
	}
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �w��A�C�e���̐ݔ��ڍׂ��J��

@param	nItem		�A�C�e���ԍ�
@param	bDblClick	�_�u���N���b�N�t���O

@retval
*/
/*============================================================================*/
void CCustomManager::showCustomDetail(int nItem, bool bDblClick)
{
	CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(nItem);
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T("***"), _T("CCustomManager"), _T("showCustomDetail"), CString(pnode->GetWindowInfo().title), _T(""), nLogEx::info);
	//������������������������ Log ������������������������//
	//=====================================================//
	bool bEditMode = false;
	if (pnode != NULL) {
		if (pnode->GetWindowInfo().wnd == NULL) {
			CCustomDetail* pitem = theApp.GetCustomControl().CreateEquipment(pnode);
			if (pitem == NULL)
				return;

			pnode->GetWindowInfo().wnd = pitem;
			bEditMode = true;

			if (bDblClick == true) {
				// �_�u���N���b�N���̓J�X�P�[�h�\��
				CPoint point = theApp.GetCustomControl().GetCascadePoint();
				if (pnode->GetWindowInfo().wnd != NULL) {
					pnode->GetWindowInfo().wnd->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				}
			}
		}
		pnode->GetWindowInfo().wnd->ShowWindow(SW_SHOWNA);
		pnode->GetWindowInfo().wnd->SetActiveWindow();
		if (bEditMode == true) {
			// �_�u���N���b�N���͏�ɕҏW���[�h�Ƃ���
			pnode->GetWindowInfo().wnd->PostMessageW(eUserMessage_Detail_Mode, 0, (LPARAM)eTreeItemMode_Edit);
		}
	}
}

void CCustomManager::OnMenumanagerClose()
{
	updateXmlFile();

	CCustomDialogBase::OnClose();
}
