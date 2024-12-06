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
	//ON_NOTIFY(NM_RCLICK, IDC_LIST_MANAGER, &CCustomManager::OnNMRClickListManager)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MANAGER, &CCustomManager::OnNMDblclkListManager)
	ON_COMMAND(ID_MANAGER_NEW, &CCustomManager::OnManagerNew)
	ON_COMMAND(ID_MANAGER_DELETE, &CCustomManager::OnManagerDelete)
	ON_COMMAND(ID_MANAGER_SHOW, &CCustomManager::OnManagerShow)
	ON_COMMAND(ID_MANAGER_CREATE, &CCustomManager::OnManagerCreate)
	ON_WM_CONTEXTMENU()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENUMANAGER_CLOSE, &CCustomManager::OnMenumanagerClose)
	ON_NOTIFY(LVN_GETINFOTIP, IDC_LIST_MANAGER, &CCustomManager::OnLvnGetInfoTipListManager)
	ON_COMMAND(ID_MANGROUP_RENAME, &CCustomManager::OnMangroupRename)
	ON_COMMAND(ID_MANGROUP_RESET, &CCustomManager::OnMangroupReset)
	ON_COMMAND(ID_MANGROUP_SHOW, &CCustomManager::OnMangroupShow)
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
		createItem((int)eSelectUser, true);
		UpdateGroup();
	}

	//UpdateGroup();

	CenterWindow(GetDesktopWindow());

	SetWindowText(mMessage_Title_CustomManager);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
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
			UpdateGroup();
		}
		CRect rect;
		GetWindowRect(rect);
		theApp.GetCustomControl().UpdateCustomManagerPoint(rect);
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
	theApp.GetCustomControl().UpdateCustomManagerPoint(rect);
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
		UpdateGroup();
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
		UpdateGroup();
	}
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�\���C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if (pWnd != (CWnd*)&mManagerList) {
		// ���X�g�R���g���[����łȂ���Ή������Ȃ�
		return;
	}

	CPoint pt = point;
	mManagerList.ScreenToClient(&pt);

	int nItem, colnum;
	nItem = mManagerList.HitTestEx(pt, &colnum);
	TRACE("# OnContextMenu : INDEX=%d COL=%d\n", nItem, colnum);
	if (nItem < 0) {
		// ���X�g�R���g���[����łȂ���Ή������Ȃ�
		//return;
	}

	// �A�C�e���A�O���[�v���݂����`�F�b�N
	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	int nGroup = -1;
	while (pos) {
		int item = mManagerList.GetNextSelectedItem(pos);
		if (nGroup >= 0 && mManagerList.GetRowGroupId(item) != nGroup) {
			return;
		}
		nGroup = mManagerList.GetRowGroupId(item);
	}

	UINT nMenuRes = IDR_POPUP_MANAGER;

	bool bUser = false;
	bool bGroup = false;
	bool bNoGroup = false;
	mMenuSelectGroupID = -1;
	if (mManagerList.IsGroupViewEnabled() == TRUE) {
		// ���[�U
		bUser = true;
		TRACE("# OnContextMenu : USER\n");
		if (point.x != -1 && point.y != -1) {
			int nGroupId = mManagerList.GroupHitTest(pt);
			if (nGroupId >= 0) {
				// �O���[�v���x��
				CString str = mManagerList.GetGroupHeader(nGroupId);
				if (str == mNoGroupText) {
					return;
				}
				bGroup = true;
				nMenuRes = IDR_POPUP_MANAGER_GROUP;
				mMenuSelectGroupID = nGroupId;
				TRACE("# OnContextMenu : USER GroupID=%d\n", nGroupId);
			}
			else {
				// ���X�g����
				if (nItem >= 0) {
					nGroupId = mManagerList.GetRowGroupId(nItem);
					bNoGroup = (nGroupId == 0);
					TRACE("# OnContextMenu : USER Item=%d NoGroup=%d GroupID=%d\n", nItem, bNoGroup, nGroupId);
				}
			}
		}
	}
	else {
		// �}�X�^
		if (nItem < 0) {
			return;
		}
		TRACE("# OnContextMenu : MASTER\n");
	}

	CMenu menu;
	if (!menu.LoadMenu(nMenuRes)) {
		ASSERT(FALSE);
		return;
	}

	CMenu* pMenu = menu.GetSubMenu(0);
	ASSERT(pMenu);

	if (nItem < 0) {
		pMenu->EnableMenuItem(ID_MANAGER_NEW, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_MANAGER_DELETE, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_MANAGER_SHOW, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_MANAGER_CREATE, MF_BYCOMMAND | MF_GRAYED);
	}
	else {
		updateMenuItemStatus(pMenu, bUser, bGroup, bNoGroup);
	}

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
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
		if (HIWORD(pnode->GetManager().groupno) != 0) {
			nMenuRes = IDR_POPUP_MANAGER_GROUP;
		}
		TRACE("[MANAGER]Rclick Item=%d Group=%08X)\n", item, pnode->GetManager().groupno);
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

	//updateMenuItemStatus(pMenu);

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
	if (pnode == NULL)
		return;

	if (HIWORD(pnode->GetManager().groupno) != 0) {
		UINT nGroup = 0;
		for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
			CTreeNode* group = (CTreeNode*)mManagerList.GetItemData(item);
			if (HIWORD(group->GetManager().groupno) == HIWORD(pnode->GetManager().groupno)) {
				showCustomDetail(item, theApp.GetCustomControl().GetCascadePoint());
			}
		}
		UpdateGroup();
	}
	else {
		OnManagerShow();
	}

	TRACE("##### OnNMDblclkListManager\n");

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
#if _DEMO_PHASE < 110
	return;
#endif
	UpdateData(TRUE);
	createEqDetail(NULL);
	mManagerList.GroupByColumn(eManagerGroup, (mSelectType == eSelectUser) ? TRUE : FALSE);
	SyncEditMode();
	UpdateGroup();
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
	if (MessageBox(msg, mMessage_Title_CustomManager, MB_YESNO | MB_ICONQUESTION) == IDNO) {
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

		//CString msg;
		//msg.Format(mMessage_DetailDelete, pnode->GetEquipment().title);
		//if (MessageBox(msg, mMessage_Title_CustomManager, MB_YESNO | MB_ICONQUESTION) == IDNO) {
		//	continue;
		//}

		// �ݔ������ʂ̍폜
		CString xmlfile = CString(pnode->GetXmlFileName());
		theApp.GetCustomControl().GetDataManager().DeleteItemWnd(pnode->GetEquipment().wnd);
		theApp.GetCustomControl().GetDataManager().DeleteItemNode(pnode);
		DeleteFile(xmlfile);

		mManagerList.DeleteItem(nItem);
		pos = mManagerList.GetFirstSelectedItemPosition();
	}
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u�\���v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerShow()
{
	// �I������Ă���O���[�v���A�C�e�����擾����
	int nItem = -1;
	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	while (pos) {
		nItem = mManagerList.GetNextSelectedItem(pos);
		break;
	}
	if (nItem < 0)
		return;

	// �I������Ă���O���[�v�ԍ����擾
	int nGroupId = mManagerList.GetRowGroupId(nItem);
	if (nGroupId <= 0) {
		// �I������Ă���A�C�e����\��
		POSITION pos = mManagerList.GetFirstSelectedItemPosition();
		while (pos) {
			int nItem = mManagerList.GetNextSelectedItem(pos);
			showCustomDetail(nItem, theApp.GetCustomControl().GetCascadePoint());
		}
	}
	else {
		// �O���[�v���̃A�C�e���S�Ă�\�������ĉ�ʘA��������
		for (nItem = 0; nItem < mManagerList.GetItemCount(); nItem++) {
			int group = mManagerList.GetRowGroupId(nItem);
			if (group == nGroupId) {
				showCustomDetail(nItem, theApp.GetCustomControl().GetCascadePoint());
			}
		}
		UpdateGroup();
	}

	TRACE("##### OnManagerShow\n");
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �|�b�v�A�b�v���j���[�u��ʘA���쐬�v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnManagerCreate()
{
	// �O���[�v���̎擾
	CString str = _T("");
	CCustomGroupName dlg(str);
	while (1) {
		if (dlg.DoModal() == IDCANCEL)
			return;

		if (dlg.mGroupName.IsEmpty()) {
			MessageBox(mMessage_EmptyGroupName, mMessage_Title_CustomManager, MB_OK | MB_ICONWARNING);
			continue;
		}

		if (IsSameGroupName(dlg.mGroupName) == true) {
			MessageBox(mMessage_SameGroupName, mMessage_Title_CustomManager, MB_OK | MB_ICONWARNING);
			continue;
		}
		break;
	}

	// �O���[�v�ԍ��͍ő�l��ݒ肵�Ă��A�ēx�O���[�vID�͐U��Ȃ���
	UINT maxGroup = 100;
	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = mManagerList.GetNextSelectedItem(pos);
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(nItem);
		pnode->GetManager().groupno = MAKELONG(LOWORD(pnode->GetManager().groupno), maxGroup + 1);
		swprintf_s(pnode->GetManager().groupname, mNameSize, _T("%s"), (LPCTSTR)dlg.mGroupName);
	}
	createItem((int)eSelectUser);
	UpdateGroup();

	TRACE("##### OnManagerCreate\n");
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �O���[�v�|�b�v�A�b�v���j���[�u���̕ύX�v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnMangroupRename()
{
#if _DEMO_PHASE < 110
	return;
#endif
	CWaitCursor wait;

	CString str = mManagerList.GetGroupHeader(mMenuSelectGroupID);
	CCustomGroupName dlg(str);
	while (1) {
		if (dlg.DoModal() == IDCANCEL)
			return;

		if (dlg.mGroupName.IsEmpty()) {
			MessageBox(mMessage_EmptyGroupName, mMessage_Title_CustomManager, MB_OK | MB_ICONWARNING);
			continue;
		}

		if (dlg.mGroupName != str && IsSameGroupName(dlg.mGroupName) == true) {
			MessageBox(mMessage_SameGroupName, mMessage_Title_CustomManager, MB_OK | MB_ICONWARNING);
			continue;
		}
		break;
	}
	mManagerList.SetGroupHeader(mMenuSelectGroupID, dlg.mGroupName);

	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	while (pos) {
		int item = mManagerList.GetNextSelectedItem(pos);
		if (mMenuSelectGroupID == mManagerList.GetRowGroupId(item)) {
			CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
			swprintf_s(pnode->GetManager().groupname, mNameSize, _T("%s"), (LPCTSTR)dlg.mGroupName);
		}
	}
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �O���[�v�|�b�v�A�b�v���j���[�u��ʘA�������v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnMangroupReset()
{
	// �ΏۃO���[�v�̐擪�E�B���h�E�ʒu���擾����
	CPoint point;
	bool bFirstGroup = mSyncWindow.GetTopPoint(MAKELONG(0, mMenuSelectGroupID), point);
	for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		if (HIWORD(pnode->GetManager().groupno) == mMenuSelectGroupID) {
			pnode->GetManager().groupno = MAKELONG(LOWORD(pnode->GetManager().groupno), 0);
			if (pnode->GetEquipment().wnd != NULL) {
				pnode->GetEquipment().wnd->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
			}
		}
	}
	createItem((int)eSelectUser);
	UpdateGroup();

	TRACE("##### OnMangroupReset\n");
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �O���[�v�|�b�v�A�b�v���j���[�u�\���v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnMangroupShow()
{
#if _DEMO_PHASE < 100
	return;
#endif
	POSITION pos = mManagerList.GetFirstSelectedItemPosition();
	while (pos) {
		int nItem = mManagerList.GetNextSelectedItem(pos);
		showCustomDetail(nItem, theApp.GetCustomControl().GetCascadePoint());
	}
	UpdateGroup();

	TRACE("##### OnMangroupShow\n");
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �O���[�v�ԍ��̍ő�l���擾����

@param

@retval
*/
/*============================================================================*/
UINT CCustomManager::GetGroupMaxNo()
{
	UINT nGroup = 0;
	for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
		// �f�[�^�̃O���[�v�ԍ�����ΏۃJ�����̃e�L�X�g��ݒ肷��
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		if (pnode->GetEquipment().kind == eTreeItemKind_User) {
			nGroup = __max(nGroup, HIWORD(pnode->GetManager().groupno));
		}
	}
	return nGroup;
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �O���[�v���ԍ��̍ő�l���擾����

@param

@retval
*/
/*============================================================================*/
UINT CCustomManager::GetGroupInnerMaxNo()
{
	UINT nInnerNo = 0;
	for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
		// �f�[�^�̃O���[�v�ԍ�����ΏۃJ�����̃e�L�X�g��ݒ肷��
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		if (pnode->GetEquipment().kind == eTreeItemKind_User) {
			nInnerNo = __max(nInnerNo, LOWORD(pnode->GetManager().groupno));
		}
	}
	return nInnerNo;
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �����̃O���[�v�������݂��邩���m�F����

@param

@retval
*/
/*============================================================================*/
bool CCustomManager::IsSameGroupName(CString groupName)
{
	for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
		// �f�[�^�̃O���[�v�ԍ�����ΏۃJ�����̃e�L�X�g��ݒ肷��
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		CString str = CString(pnode->GetManager().groupname);
		if (str == groupName)
			return true;
	}
	return false;
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �O���[�v�ԍ��̍X�V

@param		point	�h���b�O���h���b�v���̃x�[�X�E�B���h�E�ʒu

@retval
*/
/*============================================================================*/
void CCustomManager::UpdateGroup(UINT nGroupId/*=0*/, CPoint * point/* = NULL*/)
{
#if _DEMO_PHASE < 110
	return;
#endif

	mSyncWindow.Dump();

	// 1�ł��O���[�v�E�B���h�E���\������Ă���ꍇ�͑S�ẴO���[�v�E�B���h�E��\��������
	ShowGroup();

	// �o�^����Ă���J�X�^����ʂ̃O���[�v�X�V
	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;

	if (nGroupId != 0 && point != NULL) {
		for (itr = treedata.begin(); itr != treedata.end(); itr++) {
			if ((*itr)->GetEquipment().wnd == NULL)
				continue;

			// �h���b�v���ꂽ�E�B���h�E�ɑ΂��Ă̏���
			if (HIWORD((*itr)->GetManager().groupno) == nGroupId){
				(*itr)->GetEquipment().wnd->SetWindowPos(NULL, point->x, point->y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
				TRACE("# UpdateGroup(Clear) : [%s] GroupNo=%d, GroupName=%s [%d,%d]\n", CStringA((*itr)->GetEquipment().title), HIWORD((*itr)->GetManager().groupno), CStringA((*itr)->GetManager().groupname), point->x, point->y);
			}
		}
	}

	// �S�ẴO���[�v�����폜
	mSyncWindow.Clear(0);
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		if ((*itr)->GetEquipment().wnd == NULL)
			continue;

		if ((*itr)->GetEquipment().kind == eTreeItemKind_User && HIWORD((*itr)->GetManager().groupno) != 0) {
			mSyncWindow.Set((*itr)->GetManager().groupno, (*itr)->GetEquipment().wnd);
			CRect rect;
			(*itr)->GetEquipment().wnd->GetWindowRect(rect);
			TRACE("# UpdateGroup(Set) : [%s] GroupNo=%d, GroupName=%s [%d,%d]\n", CStringA((*itr)->GetEquipment().title), HIWORD((*itr)->GetManager().groupno), CStringA((*itr)->GetManager().groupname), rect.left, rect.top);
		}
	}

	mSyncWindow.Start();

	// �x�[�X�E�B���h�E�𓮍삳����
	map< UINT, vector<GroupInfo> >& wlist = mSyncWindow.GetGroupWindowList();
	map< UINT, vector<GroupInfo> >::iterator witr;
	CRect rect;
	for (witr = wlist.begin(); witr != wlist.end(); witr++) {
		if ((*witr).second.size() != 0) {
			if ((*witr).second[0].wnd != 0) {
				(*witr).second[0].wnd->GetWindowRect(rect);
				TRACE("# UpdateGroup(Move) : [%d,%d]\n", rect.left, rect.top);
				mSyncWindow.Move(MAKELONG((*witr).second[0].innerno, (*witr).first), (*witr).second[0].wnd, rect);
			}
		}
	}

	TRACE("##### UpdateGroup\n");
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �O���[�v�E�B���h�E��1�ł��\������Ă���ꍇ�͋����I�ɕ\������

@param		ddGroup	�h���b�O���h���b�v���̏�ԁi��ʃ��[�h���h���b�O�O���[�v ���ʃ��[�h���h���b�v�O���[�v�j

@retval
*/
/*============================================================================*/
void CCustomManager::ShowGroup()
{
	for (UINT item = 0; item < (UINT)mManagerList.GetItemCount(); item++) {
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(item);
		CPoint point;
		if (pnode->GetEquipment().kind == eTreeItemKind_User && HIWORD(pnode->GetManager().groupno) != 0) {
			if (IsGroupVisible(HIWORD(pnode->GetManager().groupno)) == true) {
				if (pnode->GetEquipment().wnd == NULL || pnode->GetEquipment().wnd->IsWindowVisible() == false) {
					showCustomDetail(item, theApp.GetCustomControl().GetCascadePoint());
				}
			}
		}
	}

}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �O���[�v�E�B���h�E��1�ł��\������Ă��邩���`�F�b�N����

@param

@retval
*/
/*============================================================================*/
bool CCustomManager::IsGroupVisible(UINT nGroup)
{
	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		if ((*itr)->GetEquipment().kind == eTreeItemKind_User && HIWORD((*itr)->GetManager().groupno) == nGroup) {
			if ((*itr)->GetEquipment().wnd != NULL && (*itr)->GetEquipment().wnd->IsWindowVisible())
				return true;
		}
	}
	return false;
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# ���X�g�\������Ă��鏇�Ԃɒʂ��ԍ���U��Ȃ���

@param

@retval
*/
/*============================================================================*/
void CCustomManager::ResetGroupInnerNo()
{
	UINT groupno = mGroupRange;
	UINT maxgroup = GetGroupMaxNo() + 1;
	for (UINT group = 0; group < maxgroup; group++) {
		// �ΏۃO���[�v�̃O���[�v���ԍ����X�V����
		int count = mManagerList.GetItemCount();
		for (int pos = 0; pos < count; pos++) {
			CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(pos);
			if (pnode->GetEquipment().kind == eTreeItemKind_User) {
				if (HIWORD(pnode->GetManager().groupno) == group) {
					pnode->GetManager().groupno = MAKELONG(groupno, group);
					groupno += mGroupRange;
				}
			}
		}
	}

	SyncEditMode();

	TRACE("##### ResetGroupInnerNo\n");
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �ݔ��ڍׂ����X�g�֓o�^����

@param	nSelect		���[�Uor�}�X�^
@param	bResetInner	�O���[�v���ԍ��̃��Z�b�g

@retval
*/
/*============================================================================*/
void CCustomManager::createItem(UINT nSelect, bool bResetInner/* = false*/)
{
	mManagerList.SetRedraw(FALSE);

	mManagerList.RemoveAllGroups();
	mManagerList.DeleteAllItems();

	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	bool bFirstRead = false;
	for (itr = treedata.begin(); itr != treedata.end(); itr++){
		int count = mManagerList.GetItemCount();
		if ((*itr)->GetEquipment().kind == nSelect){
			mManagerList.AddItem(count, 0, (*itr)->GetEquipment().title, (LPARAM)(*itr));
			mManagerList.AddItem(count, 1, (*itr)->GetManager().memo);
			mManagerList.AddItem(count, 2, (*itr)->GetManager().groupname);
			mManagerList.SetItemData(count, (LPARAM)(*itr));
			(*itr)->GetEquipment().manager = this;
			if (LOWORD((*itr)->GetManager().groupno) == 0) {
				bFirstRead = true;
			}
		}
	}
	if (bFirstRead == true) {
		ResetGroupInnerNo();
	}

	mManagerList.SetRedraw(TRUE);

	// �O���[�v�̍č쐬���s���i�O���[�v�ԍ����U��Ȃ����j
	mManagerList.GroupByColumn(eManagerGroup, (nSelect == eSelectUser) ? TRUE : FALSE);
	SyncEditMode();

	if (nSelect == eSelectUser) {
		mManagerList.SortItem();
		mManagerList.SortGroup();
	}

	// �O���[�v���ԍ���U��Ȃ���
	if (bResetInner == true) {
		ResetGroupInnerNo();
	}

	//UpdateGroup();

#ifdef _DEBUG
	for (int pos = 0; pos < mManagerList.GetItemCount(); pos++){
		CTreeNode* pnode = (CTreeNode*)(mManagerList.GetItemData(pos));
		TRACE("# createItem Dump : [%d] Name=%s, GroupNo=%d, Inner=%d\n", pos, CStringA(pnode->GetEquipment().title), HIWORD(pnode->GetManager().groupno), LOWORD(pnode->GetManager().groupno));
	}
#endif

	mSelectType = nSelect;
	UpdateData(FALSE);

	TRACE("##### createItem\n");
	mSyncWindow.Dump();
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

	CTreeNode* pedit = theApp.GetCustomControl().GetDataManager().SearchWndNode(pitem);
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(pitem, false);
	if (pnode != NULL) {
		UINT maxno = GetGroupInnerMaxNo();
		pnode->GetManager().groupno = MAKELONG(maxno + mGroupRange, 0);
		SyncEditMode();
	}
	// createItem�����s���邱�ƂŊǗ���ʂɕ\�������
	createItem((int)mSelectType);

	UpdateGroup();
	if (pedit->GetEquipment().wnd != NULL) {
		CPoint point = theApp.GetCustomControl().GetCascadePoint();
		pedit->GetEquipment().wnd->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pedit->GetEquipment().wnd->ShowWindow(SW_SHOW);
	}
}
/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# ���j���[���ڂ̏�Ԃ��X�V����i�����j���[�\���j

@param	pMenu		���j���[�n���h��
@param	bUser		true�̏ꍇ���[�U
@param	bGroup		true�̏ꍇ�O���[�v�w�b�_�[
@param	bNoGroup	true�̏ꍇ[No Group]

@retval
*/
/*============================================================================*/
void CCustomManager::updateMenuItemStatus(CMenu* pMenu, bool bUser, bool bGroup, bool bNoGroup)
{
	UINT count = pMenu->GetMenuItemCount();
	for (UINT pos = 0; pos < count; pos++){
		UINT state = pMenu->GetMenuState(pos, MF_BYPOSITION);
		if (state & MF_POPUP){
			CMenu* pPopup = pMenu->GetSubMenu(pos);
			updateMenuItemStatus(pPopup, bUser, bGroup, bNoGroup);
		}
		else{
			bool bSelect = (mManagerList.GetSelectedCount() > 0) ? true : false;
			bool bMultiSelect = (mManagerList.GetSelectedCount() > 1) ? true : false;
			UINT nSelectCount = mManagerList.GetSelectedCount();
			pMenu->EnableMenuItem(ID_MANAGER_NEW, MF_BYCOMMAND | (bUser) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_DELETE, MF_BYCOMMAND | (bNoGroup && bUser && (bSelect || bMultiSelect)) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_SHOW, MF_BYCOMMAND | (bSelect || bMultiSelect) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANAGER_CREATE, MF_BYCOMMAND | (bNoGroup && bUser && bMultiSelect && nSelectCount <= mMaxEqSyncNum) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANGROUP_RENAME, MF_BYCOMMAND | (bGroup) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANGROUP_RESET, MF_BYCOMMAND | (bUser) ? MF_ENABLED : MF_GRAYED);
			pMenu->EnableMenuItem(ID_MANGROUP_SHOW, MF_BYCOMMAND | (bSelect || bMultiSelect) ? MF_ENABLED : MF_GRAYED);
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
		/*
		wParam : �h���b�v���̃h���b�v��O���[�v�ԍ�
		lParam : �ďo�� 0=�ݔ��ڍ� 1=�J�X�^�}�C�Y�Ǘ�
		*/
		if (lParam == 0 && mSelectType != eSelectUser) {
			break;
		}

		if (lParam == 0) {
			createItem((int)eSelectUser, true);
			UpdateGroup();
		}
		else if (lParam == 1) {
			if (wParam == 0) {
				createItem((int)eSelectUser, true);
				UpdateGroup();
			}
			else {
				CString str;
				str = mManagerList.GetGroupHeader((int)wParam);
				CPoint point;
				bool bFirstGroup = mSyncWindow.GetTopPoint(MAKELONG(0, wParam), point);
				TRACE("# Drop Point : %d, %d\n", point.x, point.y);
				createItem((int)eSelectUser, true);
				UINT nGroupId = mManagerList.GetHeaderGroupId(str);
				UpdateGroup(nGroupId, &point);
			}
		}

		break;
	case	eUserMessage_Manager_Delete:
		pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode((CWnd*)lParam);
		theApp.GetCustomControl().GetDataManager().DeleteItemWnd((CWnd*)lParam);
		//mSyncWindow.Close(pnode->GetWindowInfo().groupno, (CWnd*)lParam);
		if (wParam == 1) {
			theApp.GetCustomControl().GetDataManager().DeleteItemNode(pnode);
			createItem(mSelectType);
			UpdateGroup();
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
		mManagerList.SetItemText(i, 0, pnode->GetEquipment().title);
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
	int count = mManagerList.GetItemCount();
	for (int i = 0; i < count; i++) {
		CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(i);
		swprintf_s(pnode->GetManager().memo, mTitleSize, _T("%s"), (LPCTSTR)mManagerList.GetItemText(i, 1));

		CMarkup xml;
		xml.Load(pnode->GetXmlFileName());
		xml.FindElem(_T("ROOT"));
		xml.IntoElem();
		xml.FindElem(_T("EQUIPMENT"));
		xml.IntoElem();
		xml.FindElem(_T("EQUIPMENTINFO"));
		xml.IntoElem();

		xml.FindElem(_T("TYPE"));
		UINT type = _wtoi(xml.GetData());
		if (type == eTreeItemType_Title) {
			xml.FindElem(_T("TITLE"));
			// �u���l�v�ۑ�
			if (xml.FindElem(_T("MEMO")) == false) {
				xml.AddElem(_T("MEMO"), mManagerList.GetItemText(i, 1));
			}
			else {
				xml.SetData(mManagerList.GetItemText(i, 1));
			}
			// �O���[�v�ԍ��ۑ�
			if (xml.FindElem(_T("GROUPNO")) == false) {
				xml.AddElem(_T("GROUPNO"), pnode->GetManager().groupno);
			}
			else {
				xml.SetData(pnode->GetManager().groupno);
			}
			// �O���[�v���̕ۑ�
			if (xml.FindElem(_T("GROUPNAME")) == false) {
				xml.AddElem(_T("GROUPNAME"), pnode->GetManager().groupname);
			}
			else {
				xml.SetData(pnode->GetManager().groupname);
			}
		}

		xml.Save(pnode->GetXmlFileName());
	}
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �w��A�C�e���̐ݔ��ڍׂ��J��

@param	nItem		�A�C�e���ԍ�
@param	point		�\���J�n�ʒu

@retval
*/
/*============================================================================*/
void CCustomManager::showCustomDetail(int nItem, CPoint point)
{
	CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(nItem);
	showCustomDetail(pnode, point);
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �w��A�C�e���̐ݔ��ڍׂ��J��

@param	pnode		�m�[�h���
@param	point		�\���J�n�ʒu

@retval
*/
/*============================================================================*/
void CCustomManager::showCustomDetail(CTreeNode* pnode, CPoint point)
{
	//=====================================================//
	//������������������������ Log ������������������������//
	CLogTraceEx::Write(_T("***"), _T("CCustomManager"), _T("showCustomDetail"), CString(pnode->GetEquipment().title), _T(""), nLogEx::info);
	//������������������������ Log ������������������������//
	//=====================================================//
	bool bEditMode = false;
	if (pnode != NULL) {
		if (pnode->GetEquipment().wnd == NULL) {
			CCustomDetail* pitem = theApp.GetCustomControl().CreateEquipment(pnode);
			if (pitem == NULL)
				return;

			pnode->GetEquipment().wnd = pitem;
			if (pnode->GetEquipment().kind == eTreeItemKind_User)
				bEditMode = true;

			if (pnode->GetEquipment().wnd != NULL) {
				pnode->GetEquipment().wnd->ShowWindow(SW_HIDE);
				pnode->GetEquipment().wnd->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
		}
		else {
			//pnode->GetEquipment().wnd->GetActiveWindow();
		}
		pnode->GetEquipment().wnd->ShowWindow(SW_SHOWNA);
		if (bEditMode == true) {
			// �_�u���N���b�N���͏�ɕҏW���[�h�Ƃ���
			pnode->GetEquipment().wnd->PostMessage(eUserMessage_Detail_Mode, 0, (LPARAM)eTreeItemMode_Edit);
		}
	}
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �J�X�^���Ǘ���ʂ����

@param

@retval
*/
/*============================================================================*/
void CCustomManager::OnMenumanagerClose()
{
	CCustomDialogBase::OnClose();

	if (mMessageRet == IDCANCEL)
		return;

	updateXmlFile();
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �ʏ�m�[�h�ƕҏW�m�[�h�̃O���[�v���𓯊�������

@param

@retval
*/
/*============================================================================*/
void CCustomManager::SyncEditMode()
{
	vector<CTreeNode*>& treedata = theApp.GetCustomControl().GetDataManager().GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		if ((*itr)->GetEquipment().wnd == NULL)
			continue;

		CTreeNode* pedit = theApp.GetCustomControl().GetDataManager().SearchWndNode((*itr)->GetEquipment().wnd, true);
		if (pedit == NULL)
			continue;

		pedit->GetManager().groupno = (*itr)->GetManager().groupno;
		swprintf_s(pedit->GetManager().groupname, mNameSize, _T("%s"), (LPCTSTR)(*itr)->GetManager().groupname);
	}
}










void CCustomManager::OnLvnGetInfoTipListManager(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMLVGETINFOTIP>(pNMHDR);
	*pResult = 0;

	CTreeNode* pnode = (CTreeNode*)mManagerList.GetItemData(pGetInfoTip->iItem);
	if (pnode == NULL) {
		return;
	}
#ifdef _DEBUG
	int nGroupId = mManagerList.GetRowGroupId(pGetInfoTip->iItem);
	mToolText.Format(_T("(%d)NODE_GROUP=%d NODE_NO=%d NODE_GROUP_NAME=%s NAME=%s"), nGroupId, HIWORD(pnode->GetManager().groupno), LOWORD(pnode->GetManager().groupno), pnode->GetManager().groupname, pnode->GetEquipment().title);
	TRACE("%s\n", CStringA(mToolText));
#endif
	pGetInfoTip->pszText = (LPWSTR)(LPCTSTR)mToolText;
	pGetInfoTip->cchTextMax = mToolText.GetLength();

}
