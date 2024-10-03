/******************************************************************************
* @file    CustomDetail.cpp
* @brief   �y�J�X�^�}�C�Y�@�\�z�ݔ��󋵏ڍ׉�ʃN���X
* @auther  SCC
* @date    2024/08/26 �V�K�쐬
* @par
******************************************************************************/
#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomDetail.h"
#include "afxdialogex.h"
#include "CustomDetailConfig.h"
#include "CustomSelectSaveFile.h"

#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")

// CCustomDetail �_�C�A���O

IMPLEMENT_DYNAMIC(CCustomDetail, CCustomDialogBase)

CCustomDetail::CCustomDetail(CWnd* pParent /*=NULL*/, bool bRestore/* = false*/)
: CCustomDialogBase(CCustomDetail::IDD, pParent)
{
	// �����t���O
	mRestore = bRestore;
	
	// �c���[�R���g���[���t�H���g�̍쐬
	CFont temp;
	temp.CreateStockObject(DEFAULT_GUI_FONT);
	temp.GetLogFont(&mTreeLogFont);
	mTreeLogFont.lfHeight = -mTreeFontHeight;
	mTreeLogFont.lfWeight = FW_NORMAL;
	temp.DeleteObject();
	mMode = eTreeItemMode_Monitor;
	mBackupNode = NULL;
}

CCustomDetail::~CCustomDetail()
{
	if(mBackupNode != NULL)
		delete mBackupNode;
}

void CCustomDetail::DoDataExchange(CDataExchange* pDX)
{
	CCustomDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CTRL, mTreeCtrl);
}


BEGIN_MESSAGE_MAP(CCustomDetail, CCustomDialogBase)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_CTRL, &CCustomDetail::OnNMRClickTreeCtrl)
	ON_WM_CLOSE()
	ON_WM_HSCROLL()
	//ON_NOTIFY(HDN_ITEMCHANGING, CCustomTreeListCtrl::eHeaderID, OnHeaderItemChanged)
	ON_NOTIFY(HDN_ITEMCHANGED, CCustomTreeListCtrl::eHeaderID, OnHeaderItemChanged)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, CCustomTreeListCtrl::eHeaderID, OnHeaderDividerdblclick)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_DETAIL_ADD, &CCustomDetail::OnDetailAdd)
	ON_COMMAND(ID_DETAIL_DELETE, &CCustomDetail::OnDetailDelete)
	ON_COMMAND(ID_DETAIL_RENAME, &CCustomDetail::OnDetailRename)
	ON_COMMAND(ID_DETAIL_MONCTRL, &CCustomDetail::OnDetailMonctrl)
	ON_COMMAND(ID_DETAIL_CONFIG, &CCustomDetail::OnDetailConfig)
	ON_NOTIFY(TVN_GETINFOTIP, IDC_TREE_CTRL, &CCustomDetail::OnTvnGetInfoTipTreeCtrl)
	ON_COMMAND(ID_MENUDETAIL_CLOSE, &CCustomDetail::OnMenudetailClose)
	ON_COMMAND(ID_MENUDETAIL_EDIT, &CCustomDetail::OnMenudetailEdit)
	ON_COMMAND(ID_MENUDETAIL_MONITOR, &CCustomDetail::OnMenudetailMonitor)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_MENUDETAIL_SAVE, &CCustomDetail::OnMenudetailSave)
	ON_COMMAND(ID_MENUDETAIL_SAVEAS, &CCustomDetail::OnMenudetailSaveas)
END_MESSAGE_MAP()


// CCustomTreeListCtrl�p�̃C�x���g����
/*============================================================================*/
/*! �ݔ��ڍ�

-# �c���[�R���g���[���w�b�_�[�̃C�x���g�����i�T�C�Y�ύX�j
�@���c���[�R���g���[���ł̓C�x���g���E���Ȃ�

 @param

 @retval
 */
/*============================================================================*/
void CCustomDetail::OnHeaderItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	// �J�����̈�̍ĕ`��
	mTreeCtrl.UpdateColumns();

	// �ĕ`��
	mTreeCtrl.Invalidate();
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �w�b�_�[�A�C�e���Ԃ̃}�E�X�_�u���N���b�N
�@���c���[�R���g���[���ł̓C�x���g���E���Ȃ�

 @param

 @retval
 */
/*============================================================================*/
void CCustomDetail::OnHeaderDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	
	mTreeCtrl.DividerDblClick(phdr->iItem);

	*pResult = 0;
}

// CCustomDetail ���b�Z�[�W �n���h���[

/*============================================================================*/
/*! �ݔ��ڍ�

-# ������

@param

@retval
*/
/*============================================================================*/
BOOL CCustomDetail::OnInitDialog()
{
	CCustomDialogBase::OnInitDialog();

	// �V�X�e�����j���[�Ɂu���T�C�Y�t�B�b�g�v��ǉ�
	HMENU pSysMenu = ::GetSystemMenu(GetSafeHwnd(), FALSE);
	if (pSysMenu)
	{
		int count = ::GetMenuItemCount(pSysMenu);
		::InsertMenu(pSysMenu, 0, MF_BYPOSITION | MF_STRING, ID_DETAIL_RESIZEFIT, _T("�E�B���h�E�T�C�Y�œK��"));
		::InsertMenu(pSysMenu, 1, MF_BYPOSITION | MF_SEPARATOR, 0, _T(""));
	}

	createTreeControl();

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

	// �m�[�h���̃N���[��
	mBackupNode = theApp.GetCustomControl().GetDataManager().CloneItemNode(pnode, mBackupNode);
	//mBackupNode = new CTreeNode(0, NULL, NULL);
	//mBackupNode->CopyTreeNode(pnode);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �c���[�R���g���[���̃}�E�X�E�N���b�N�C�x���g�i�|�b�v�A�b�v���j���[�\���j

@param

@retval
*/
/*============================================================================*/
void CCustomDetail::OnNMRClickTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	mMenuItem = NULL;

	CPoint pos;
	if (!::GetCursorPos(&pos))
	{
		ASSERT(FALSE);
		return;
	}

	mTreeCtrl.ScreenToClient(&pos);
	UINT col = 0;
	mMenuItem = mTreeCtrl.HitTestEx(pos, col);
	if (mMenuItem == NULL)
		return;

	mTreeCtrl.SelectItem(mMenuItem);

	CMenu menu;
	if (!menu.LoadMenu(IDR_POPUP_DETAIL))
	{
		ASSERT(FALSE);
		return;
	}

	CMenu* pMenu = menu.GetSubMenu(0);
	ASSERT(pMenu);

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mMenuItem);
	// �����Ńc���[�m�[�h��ʂɂ���ă��j���[�̊����A�񊈐����s��
	switch (pnode->GetWindowInfo().type) {
	case	eTreeItemType_Title:
		pMenu->EnableMenuItem(ID_DETAIL_DELETE, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DETAIL_RENAME, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DETAIL_MONCTRL, MF_BYCOMMAND | MF_GRAYED);
		break;
	case	eTreeItemType_Main:
		pMenu->EnableMenuItem(ID_DETAIL_MONCTRL, MF_BYCOMMAND | MF_GRAYED);
		break;
	case	eTreeItemType_Sub:
		pMenu->EnableMenuItem(ID_DETAIL_MONCTRL, MF_BYCOMMAND | MF_GRAYED);
		break;
	case	eTreeItemType_Item:
		break;
	}

	mTreeCtrl.ClientToScreen(&pos);
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, this);
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# ����{�^�������C�x���g

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnClose()
{
	OnMenudetailClose();
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# ���j���[����{�^��

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnMenudetailClose()
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

	bool ret = mBackupNode->Equal(pnode);
	if (ret == true) {
		TRACE("Same Data\b");
	}
	else {
		TRACE("Different Data\n");
	}


	int retmsg = CustomSaveDifferentMessageBoxHooked(m_hWnd, mMessage_DetailSaveDifferentData, pnode->GetWindowInfo().title, MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON1);

	if (retmsg == IDCANCEL)
		return;

	if (retmsg == IDYES) {
		// �ύX���e��ۑ�����
		OnMenudetailSave();
	}
	else {
		// �m�[�h���̃N���[��
		theApp.GetCustomControl().GetDataManager().CloneItemNode(mBackupNode, pnode);
		// �Ώۃm�[�h���N���A����
		//theApp.GetCustomControl().GetDataManager().ClearItemNode(pnode);
		// �o�b�N�A�b�v�m�[�h���猳�ɖ߂�
		//pnode->CopyTreeNode(mBackupNode);
	}

	theApp.GetCustomControl().GetCustomManager().PostMessageW(eUserMessage_Manager_Delete, 0, (LPARAM)this);

	CCustomDialogBase::OnClose();
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# ���j���[�ۑ��C�x���g

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnMenudetailSave()
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

	// �c���[�f�[�^�̕ۑ�
	saveHeaderWidth();
	theApp.GetCustomControl().GetDataManager().SaveEquipmentData((UINT)eLayoutFileType_XML, pnode->GetXmlFileName(), this);

	// �J�X�^���Ǘ���ʂ֒ʒm���ă^�C�g�����X�V����
	if (pnode->GetWindowInfo().manager->GetSafeHwnd())
		pnode->GetWindowInfo().manager->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)this);
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# ���j���[���O��t���ĕۑ��C�x���g

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnMenudetailSaveas()
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

	CCustomSelectSaveFile dlg;
	dlg.SetSavePathName(theApp.GetCustomControl().GetUserDataPath());
	dlg.SetSaveFileName(CString(pnode->GetXmlFileName()).Mid(theApp.GetCustomControl().GetUserDataPath().GetLength()+1));
	if (dlg.DoModal() == IDCANCEL)
		return;

	CString filename = dlg.GetSavePathName() + _T("\\") + dlg.GetSaveFileName();
	CString backxml = CString(pnode->GetXmlFileName());

	saveHeaderWidth();

	// �@CTreeNode���w��t�@�C�����ŕۑ�����
	theApp.GetCustomControl().GetDataManager().SaveEquipmentData((UINT)eLayoutFileType_XML, filename, this);
	// �ACTreeNode��XML�t�@�C������ύX����
	swprintf_s(pnode->GetXmlFileName(), _MAX_PATH, _T("%s"), (LPCTSTR)filename);
	// �B�ۑ��O��XML���ă��[�h����
	CTreeNode* pnode_new = theApp.GetCustomControl().GetDataManager().LoadTreeDataXml(backxml, eTreeItemKind_User);
	// �����O��t���ĕۑ������A�C�e���ƍēx�ǂݍ��݂����A�C�e��SWAP����

	// �CCCustomManager�̕\���X�V
	if (theApp.GetCustomControl().GetCustomManager().GetSafeHwnd() != NULL) {
		theApp.GetCustomControl().GetCustomManager().PostMessage(eUserMessage_Manager_Reset, 0, 1);
	}
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �w�b�_�[�T�C�Y�̕ۑ�

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::saveHeaderWidth()
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

	mTreeCtrl.GetHeaderWidth(pnode->GetWindowInfo().hwidth, mHeaderSize);
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# ���j���[�ҏW�[�ҏW�C�x���g

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnMenudetailEdit()
{
#ifdef _NOPROC
	return;
#endif
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	if (pnode != NULL) {
		pnode->GetWindowInfo().mode = eTreeItemMode_Edit;
		updateMode();
	}
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# ���j���[�ҏW�[�Ď��C�x���g

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnMenudetailMonitor()
{
#ifdef _NOPROC
	return;
#endif
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	if (pnode != NULL) {
		pnode->GetWindowInfo().mode = eTreeItemMode_Monitor;
		updateMode();
	}
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �����X�N���[���C�x���g(CTreeListCtrl�ł̓L���b�`�ł��Ȃ�)

@param

@retval
*/
/*============================================================================*/
void CCustomDetail::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// �N���C�A���g�̈�̎擾
	CRect rcClient;
	GetClientRect(&rcClient);
	// �N���C�A���g�̈敝�̎擾
	int cx = rcClient.Width();

	int xEndPos = mTreeCtrl.GetPosX();

	// �X�N���[���o�[�̃C�x���g����
	// �C�x���g�ɂ��X�N���[���ʂ𒲐�����
	switch (nSBCode)
	{
	case SB_LINELEFT:
		mTreeCtrl.SetPosX(mTreeCtrl.GetPosX() - 15);
		break;
	case SB_LINERIGHT:
		mTreeCtrl.SetPosX(mTreeCtrl.GetPosX() + 15);
		break;
	case SB_PAGELEFT:
		mTreeCtrl.SetPosX(mTreeCtrl.GetPosX() - cx);
		break;
	case SB_PAGERIGHT:
		mTreeCtrl.SetPosX(mTreeCtrl.GetPosX() + cx);
		break;
	case SB_LEFT:
		mTreeCtrl.SetPosX(0);
		break;
	case SB_RIGHT:
		mTreeCtrl.SetPosX(mTreeCtrl.GetTotalCX() - cx);
		break;
	case SB_THUMBTRACK:
		mTreeCtrl.SetPosX(nPos);
		break;
	}

	if (mTreeCtrl.GetPosX() < 0)
		mTreeCtrl.SetPosX(0);
	else if (mTreeCtrl.GetPosX() > mTreeCtrl.GetTotalCX() - cx)
		mTreeCtrl.SetPosX(mTreeCtrl.GetTotalCX() - cx);

	if (xEndPos == mTreeCtrl.GetPosX())
		return;

	SetScrollPos(SB_HORZ, mTreeCtrl.GetPosX());
	// �e�R���g���[���̍Ĕz�u
	mTreeCtrl.RepositionControls();

	//CCustomDialogBase::OnHScroll(nSBCode, nPos, pScrollBar);
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �ʃ��T�C�Y�̃I�[�o�[���C�h�֐�

@param		pWnd	�ΏۃR���g���[���n���h��
@param		dx		X�����l
@param		dy		Y�����l
@retval

*/
/*============================================================================*/
BOOL CCustomDetail::OnProcSize(CWnd* pWnd, int dx, int dy)
{
	if (mTreeCtrl.m_hWnd != pWnd->m_hWnd)
		return false;

	mTreeCtrl.ResizeControl(dx, dy);

	return TRUE;
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# ���T�C�Y�t�B�b�g�C�x���g

@param
@retval

*/
/*============================================================================*/
void CCustomDetail::OnSysCommand(UINT nID, LPARAM lParam)
{
	switch (nID){
	case	ID_DETAIL_RESIZEFIT:
		resizeFit();
		return;
	}

	CCustomDialogBase::OnSysCommand(nID, lParam);
}

//#############################################################################

/*============================================================================*/
/*! �ݔ��ڍ�

-# �c���[���X�g�R���g���[���̍쐬

@param

@retval
*/
/*============================================================================*/
void CCustomDetail::createTreeControl()
{
	// �c���[�R���g���[���Ƀf�t�H���g�t�H���g��ݒ肷��
	mTreeCtrl.SetFontEx(eTreeItemType_Window, mTreeLogFont);
	mTreeCtrl.Create(this);

	// �A�C�e���C���f���g�̐ݒ�
	//mTreeCtrl.SetIndent(5);
	mTreeCtrl.SetIndent(7);

	// �R�[���o�b�N�֐��̓o�^
	mTreeCtrl.SetClickCallback(messageClick);
	//mTreeCtrl.SetDragCallback(messageDrag);

	mTreeCtrl.ModifyStyle(TVS_EDITLABELS, 0);

	if (mRestore == true){
		restoreRoot();
	}
	else{
		createRoot();
	}

	mTreeCtrl.UpdateColumns();

	UpdateSortNo(mTreeCtrl.GetRootItem());

	SetControlInfo(IDC_TREE_CTRL, ANCHORE_LEFTTOP | RESIZE_BOTH);
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �R�[���o�b�N�֐�

@param		hItem		�c���[�A�C�e��
@param		nSubItem	�c���[�T�u�A�C�e��
@param		point		�}�E�X�J�[�\���ʒu
@retval

*/
/*============================================================================*/
BOOL CALLBACK CCustomDetail::messageClick(CWnd* pwnd, HTREEITEM hItem, UINT nSubItem, CPoint point)
{
#ifdef _NOPROC
	//return FALSE;
#endif
	//CCustomDetail* p = CCustomDetail::Instance();
	CCustomDetail* p = (CCustomDetail*)pwnd;

	if (p->mMode != eTreeItemMode_Edit) {
		return FALSE;
	}


	UINT mask = 1 << eTreeItemSubType_Item | 1 << eTreeItemSubType_Unit;
	if ((1 << nSubItem) & mask)
		return TRUE;

	// ����Z�����������ꂽ���`�F�b�N
	bool bControl = p->mTreeCtrl.IsControl(point);
	if (bControl == false){
		return FALSE;
	}

	CString strText = p->mTreeCtrl.GetSubItemText(hItem, eTreeItemSubType_Control);
	// ���䕶����̏ꍇ�͐���R�}���h���s
	if (strText.IsEmpty() == false && strText == CString(mCOntrolSignString)){
		// ����R�}���h�𑗐M
		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(p, hItem);
		CString strCntl = pnode->GetMonCtrl().cname;
		p->MessageBox(_T("�i���j����R�}���h�𑗐M���܂�\n") + strCntl);
		return FALSE;
	}
	return TRUE;
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �R�[���o�b�N�֐�

@param		status		���
@param		hItem		�c���[�A�C�e��
@param		lParam1		�p�����^�P
@param		lParam2		�p�����^�Q
@param		lParam3		�p�����^�R
@retval

*/
/*============================================================================*/
BOOL CALLBACK CCustomDetail::messageDrag(CWnd* pwnd, UINT status, HTREEITEM hItem, LPARAM lParam1, LPARAM lParam2, LPARAM lParam3)
{
#ifdef _NOPROC
	return FALSE;
#else
	//CCustomDetail* p = CCustomDetail::Instance();
	CCustomDetail* p = (CCustomDetail*)pwnd;

	CTreeNode* pnode;
	CTreeNode* pnodeDrop;
	BOOL bDropExecute = FALSE;

	switch (status){
	case	CTreeListCtrl::eEnable:
		pnode = theApp.GetDataManager().SearchItemNode(p, hItem);
		//if (pnode != NULL && pnode->getWindowInfo().type != eTreeItemType_Title)
		//	return TRUE;
		if (pnode != NULL && pnode->GetWindowInfo().type == eTreeItemType_Item)
			return TRUE;
		break;
	case	CTreeListCtrl::eSelect:
		// �E�h���b�O�A�C�e����ʂ�eTreeItemType_Main�̏ꍇ��eTreeItemType_Title�ɂ̂݃h���b�v�\
		// �E�h���b�O�A�C�e����ʂ�eTreeItemType_Sub�̏ꍇ��eTreeItemType_Main�ɂ̂݃h���b�v�\
		// �E�h���b�O�A�C�e����ʂ�eTreeItemType_Item�̏ꍇ��eTreeItemType_Sub�ɂ̂݃h���b�v�\
		pnode = theApp.GetDataManager().SearchItemNode(p, hItem);
		pnodeDrop = theApp.GetDataManager().SearchItemNode(p, (HTREEITEM)lParam1);
		if (pnode == NULL || pnodeDrop == NULL)
			return FALSE;
		if (pnode->GetWindowInfo().type == eTreeItemType_Main && pnodeDrop->GetWindowInfo().type == eTreeItemType_Title)
			bDropExecute = TRUE;
		else if (pnode->GetWindowInfo().type == eTreeItemType_Sub && pnodeDrop->GetWindowInfo().type == eTreeItemType_Main)
			bDropExecute = TRUE;
		else if (pnode->GetWindowInfo().type == eTreeItemType_Item && pnodeDrop->GetWindowInfo().type == eTreeItemType_Sub)
			bDropExecute = TRUE;

		return bDropExecute;
		break;
	case	CTreeListCtrl::eDrop:
		// �폜����m�[�h�̐e�m�[�h���擾�iDropMoveItem���Ăяo���O�Ɏ擾�j
		pnode = theApp.GetDataManager().SearchItemNode(p, p->mTreeCtrl.GetParentItem(hItem));
		// �h���b�O�m�[�h���h���b�v����
		p->DropMoveItem(hItem, (HTREEITEM)lParam1);
		// �h���b�O�m�[�h�̐e�̎q���X�g����폜����
		pnode->DeleteTreeNode(hItem);
		break;
	}
	return FALSE;
#endif
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# ���[�g�̍쐬

@param

@retval void
*/
/*============================================================================*/
void CCustomDetail::createRoot()
{
	SetWindowText(mDefaultCustomTitle);
	// ���[�g�A�C�e���̐ݒ�
	HTREEITEM rootItem = mTreeCtrl.InsertItem(mDefaultCustomTitle, NULL, NULL, TVI_ROOT);
	mTreeCtrl.SetItemData(rootItem, (LPARAM)rootItem);
	CTreeNode* tn_root = new CTreeNode(rootItem, this, &mTreeCtrl);
	setNodeWindowInfo(tn_root, eTreeItemType_Title, (LPWSTR)mDefaultCustomTitle, NULL);
	// �f�t�H���g�t�H���g�̐ݒ�
	mTreeCtrl.SetFontEx(tn_root->GetWindowInfo().type, mTreeLogFont);
	// �_���t�H���g�̎擾
	mTreeCtrl.GetFontEx(eTreeItemType_Window).GetLogFont(&tn_root->GetColor().font);

	theApp.GetCustomControl().GetDataManager().AddTreeNode(tn_root);
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# ���C���m�[�h�̍쐬

@param		parentitem		�e�c���[�A�C�e��
@param		parentnode		�e�m�[�h

@retval void
*/
/*============================================================================*/
void CCustomDetail::createMainNode(HTREEITEM parentitem, CTreeNode* parentnode)
{
	CString str;
	str.Format(_T("%s"), mDefaultCustomMainText);
	HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, parentitem);
	mTreeCtrl.SetItemData(item, (LPARAM)item);
	CTreeNode* item_node = parentnode->CreateTreeNode(parentitem, item);
	CTreeNode* pcopyitem = theApp.GetCustomControl().GetDataManager().SearchItemNodeType(this, eTreeItemType_Main);
	if (pcopyitem != NULL)
		item_node->CopyItem(pcopyitem, true);
	setNodeWindowInfo(item_node, eTreeItemType_Main, (LPWSTR)mDefaultCustomMainText, parentnode);
	// �f�t�H���g�t�H���g�̐ݒ�
	mTreeCtrl.SetFontEx(item_node->GetWindowInfo().type, mTreeLogFont);
	// �_���t�H���g�̎擾
	mTreeCtrl.GetFontEx(eTreeItemType_Window).GetLogFont(&item_node->GetColor().font);

	swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), mDefaultCustomMainText);
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �T�u�m�[�h�̍쐬

@param		parentitem		�e�c���[�A�C�e��
@param		parentnode		�e�m�[�h

@retval void
*/
/*============================================================================*/
void CCustomDetail::createSubNode(HTREEITEM parentitem, CTreeNode* parentnode)
{
	CString str;
	str.Format(_T("%s"), mDefaultCustomSubText);
	HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, parentitem);
	mTreeCtrl.SetItemData(item, (LPARAM)item);
	CTreeNode* item_node = parentnode->CreateTreeNode(parentitem, item);
	CTreeNode* pcopyitem = theApp.GetCustomControl().GetDataManager().SearchItemNodeType(this, eTreeItemType_Sub);
	if (pcopyitem != NULL)
		item_node->CopyItem(pcopyitem, true);
	setNodeWindowInfo(item_node, eTreeItemType_Sub, (LPWSTR)mDefaultCustomSubText, parentnode);
	// �f�t�H���g�t�H���g�̐ݒ�
	mTreeCtrl.SetFontEx(item_node->GetWindowInfo().type, mTreeLogFont);
	// �_���t�H���g�̎擾
	mTreeCtrl.GetFontEx(eTreeItemType_Window).GetLogFont(&item_node->GetColor().font);

	swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), mDefaultCustomSubText);
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# ���[�t�̍쐬

@param		parentitem		�e�c���[�A�C�e��
@param		parentnode		�e�m�[�h

@retval void
*/
/*============================================================================*/
void CCustomDetail::createLeaf(HTREEITEM parentitem, CTreeNode* parentnode)
{
	CString str;
	str = createLeafText(mDefaultCustomItemText, _T(""), _T(""));
	HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, parentitem);
	mTreeCtrl.SetItemData(item, (LPARAM)item);
	CTreeNode* item_node = parentnode->CreateTreeNode(parentitem, item);
	CTreeNode* pcopyitem = theApp.GetCustomControl().GetDataManager().SearchItemNodeType(this, eTreeItemType_Item);
	if (pcopyitem != NULL)
		item_node->CopyItem(pcopyitem, true);
	setNodeWindowInfo(item_node, eTreeItemType_Item, (LPWSTR)mDefaultCustomItemText, parentnode);
	// �f�t�H���g�t�H���g�̐ݒ�
	mTreeCtrl.SetFontEx(item_node->GetWindowInfo().type, mTreeLogFont);
	// �_���t�H���g�̎擾
	mTreeCtrl.GetFontEx(eTreeItemType_Window).GetLogFont(&item_node->GetColor().font);

	swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), mDefaultCustomItemText);
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �m�[�h�A���[�t�̕���

@param

@retval void
*/
/*============================================================================*/
void CCustomDetail::restoreRoot()
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);

#if 0//(_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
	/*
	// Window attributes
	enum DWMWINDOWATTRIBUTE
	{
	DWMWA_NCRENDERING_ENABLED = 1,              // [get] Is non-client rendering enabled/disabled
	DWMWA_NCRENDERING_POLICY,                   // [set] DWMNCRENDERINGPOLICY - Non-client rendering policy
	DWMWA_TRANSITIONS_FORCEDISABLED,            // [set] Potentially enable/forcibly disable transitions
	DWMWA_ALLOW_NCPAINT,                        // [set] Allow contents rendered in the non-client area to be visible on the DWM-drawn frame.
	DWMWA_CAPTION_BUTTON_BOUNDS,                // [get] Bounds of the caption button area in window-relative space.
	DWMWA_NONCLIENT_RTL_LAYOUT,                 // [set] Is non-client content RTL mirrored
	DWMWA_FORCE_ICONIC_REPRESENTATION,          // [set] Force this window to display iconic thumbnails.
	DWMWA_FLIP3D_POLICY,                        // [set] Designates how Flip3D will treat the window.
	DWMWA_EXTENDED_FRAME_BOUNDS,                // [get] Gets the extended frame bounds rectangle in screen space
	DWMWA_HAS_ICONIC_BITMAP,                    // [set] Indicates an available bitmap when there is no better thumbnail representation.
	DWMWA_DISALLOW_PEEK,                        // [set] Don't invoke Peek on the window.
	DWMWA_EXCLUDED_FROM_PEEK,                   // [set] LivePreview exclusion information
	DWMWA_CLOAK,                                // [set] Cloak or uncloak the window
	DWMWA_CLOAKED,                              // [get] Gets the cloaked state of the window
	DWMWA_FREEZE_REPRESENTATION,                // [set] BOOL, Force this window to freeze the thumbnail without live update
	DWMWA_PASSIVE_UPDATE_MODE,                  // [set] BOOL, Updates the window only when desktop composition runs for other reasons
	DWMWA_USE_HOSTBACKDROPBRUSH,                // [set] BOOL, Allows the use of host backdrop brushes for the window.
	DWMWA_USE_IMMERSIVE_DARK_MODE = 20,         // [set] BOOL, Allows a window to either use the accent color, or dark, according to the user Color Mode preferences.
	DWMWA_WINDOW_CORNER_PREFERENCE = 33,        // [set] WINDOW_CORNER_PREFERENCE, Controls the policy that rounds top-level window corners
	DWMWA_BORDER_COLOR,                         // [set] COLORREF, The color of the thin border around a top-level window
	DWMWA_CAPTION_COLOR,                        // [set] COLORREF, The color of the caption
	DWMWA_TEXT_COLOR,                           // [set] COLORREF, The color of the caption text
	DWMWA_VISIBLE_FRAME_BORDER_THICKNESS,       // [get] UINT, width of the visible border around a thick frame window
	DWMWA_LAST
	};
	*/
	if (pnode->GetWindowInfo().kind == eTreeItemKind_Master){
		int windowColor = RGB(0, 0, 0);
		int textColor = RGB(255, 255, 255);
		//HWND window = GetActiveWindow();
		LONG exStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		exStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, exStyle);
		DwmSetWindowAttribute(m_hWnd, 34/*DWMWA_BORDER_COLOR*/, &windowColor, sizeof(windowColor));
		DwmSetWindowAttribute(m_hWnd, 35/*DWMWA_CAPTION_COLOR*/, &windowColor, sizeof(windowColor));
		DwmSetWindowAttribute(m_hWnd, 36/*DWMWA_TEXT_COLOR*/, &textColor, sizeof(windowColor));
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}
#endif

	// ���[�g����
	HTREEITEM rootItem = mTreeCtrl.InsertItem(pnode->GetWindowInfo().title, NULL, NULL, TVI_ROOT);
	mTreeCtrl.SetItemData(rootItem, (LPARAM)rootItem);
	pnode->SetTreeItem(rootItem);
	if (abs(mTreeLogFont.lfHeight) < abs(pnode->GetColor().font.lfHeight))
		mTreeLogFont.lfHeight = pnode->GetColor().font.lfHeight;

	setNodeWindowInfo(pnode, eTreeItemType_Title, pnode->GetWindowInfo().title, NULL);
	// �t�H���g�̐ݒ�
	mTreeCtrl.SetFontEx(eTreeItemType_Title, pnode->GetColor().font);

	// �c���[�R���g���[���̔w�i�F�ݒ�
	mTreeCtrl.SetBkColor(pnode->GetColor().back);

	restoreNode(pnode, rootItem);

	// �c���[�R���g���[���̃t�H���g�ݒ�
	mTreeCtrl.SetFontEx(eTreeItemType_Window, mTreeLogFont);

	if (pnode->GetWindowInfo().treeopen != 0){
		mTreeCtrl.Expand(rootItem, TVE_EXPAND);
	}

	// �E�B���h�E��Ԃ̕���
	SetWindowPlacement(&(pnode->GetWindowInfo().placement));
	ShowWindow(pnode->GetWindowInfo().placement.showCmd);

	// �w�b�_�[���Đݒ�
	mTreeCtrl.SetHeaderWidth(pnode->GetWindowInfo().hwidth, mHeaderSize);
	mTreeCtrl.UpdateColumns();
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �m�[�h�A���[�t�̕���

@param

@retval void
*/
/*============================================================================*/
void CCustomDetail::restoreNode(CTreeNode* pnode, HTREEITEM ptree)
{
	vector<CTreeNode*>::iterator itr;
	for (itr = pnode->GetChildren().begin(); itr != pnode->GetChildren().end(); itr++){
		CString str;
		str = generateTreeText((*itr));
		HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, ptree);
		if (abs(mTreeLogFont.lfHeight) < abs(pnode->GetColor().font.lfHeight))
			mTreeLogFont.lfHeight = pnode->GetColor().font.lfHeight;
		// �t�H���g�̐ݒ�
		mTreeCtrl.SetFontEx((*itr)->GetWindowInfo().type, (*itr)->GetColor().font);
		mTreeCtrl.SetItemData(item, (LPARAM)item);
		(*itr)->SetTreeItem(item);
		(*itr)->GetWindowInfo().tree = &mTreeCtrl;
		(*itr)->GetWindowInfo().wnd = this;
		restoreNode((*itr), item);

		if ((*itr)->GetWindowInfo().treeopen != 0){
			mTreeCtrl.Expand(item, TVE_EXPAND);
		}
	}
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �m�[�h�p�����^�̐ݒ�

@param		pnode		�ݒ�Ώۃm�[�h
@param		type		�m�[�h���
@param		text		�m�[�h������
@param		parent		�e�m�[�h

@retval void
*/
/*============================================================================*/
void CCustomDetail::setNodeWindowInfo(CTreeNode* pnode, UINT type, TCHAR* text, CTreeNode* parent)
{
	pnode->SetParentNode(parent);
	pnode->GetWindowInfo().wnd = this;
	pnode->GetWindowInfo().tree = &mTreeCtrl;
	pnode->GetWindowInfo().type = type;
	swprintf_s(pnode->GetWindowInfo().title, mTitleSize, _T("%s"), text);
	if (type == eTreeItemType_Title){
		SetWindowText(text);
	}
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �o�^����Ă��鍀�ڐ��ŃE�B���h�E���œK������

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::resizeFit()
{
	UINT cxTotal, cyTotal;

	int CYCAPTION = GetSystemMetrics(SM_CYCAPTION);
	int CXSIZEFRAME = GetSystemMetrics(SM_CXSIZEFRAME);
	int CYSIZEFRAME = GetSystemMetrics(SM_CYSIZEFRAME);
	int CXEDGE = GetSystemMetrics(SM_CXEDGE);
	int CYEDGE = GetSystemMetrics(SM_CYEDGE);

	// �����������̃T�C�Y�ύX
	// �w�b�_�[�R���g���[���̃J�������̎擾
	cxTotal = (CYSIZEFRAME * 4);
	cxTotal += mTreeCtrl.GetHeaderWidth();
	//cxTotal += (nCnt * CYEDGE);
	cxTotal += (CYEDGE * 2);

	// �����������̃T�C�Y�ύX
	// �A�C�e�������̎擾
	int nHeaderHeight = mTreeCtrl.GetHeaderHeight();

	HTREEITEM	hItem = NULL;

	mTreeCtrl.SelectSetFirstVisible(mTreeCtrl.GetRootItem());

	// �W�J�i�\���j����Ă���A�C�e�������擾����
	hItem = mTreeCtrl.GetFirstVisibleItem();
	UINT itemCount = 0;
	while (hItem){
		itemCount++;
		hItem = mTreeCtrl.GetNextVisibleItem(hItem);
	}

	cyTotal = CYCAPTION + (CXSIZEFRAME * 4) + nHeaderHeight + (mTreeCtrl.GetItemHeight() * itemCount) + (CXEDGE * 2);

	// ���E�B���h�E�T�C�Y�ύX
	WINDOWPLACEMENT	wPlacement;
	memset(&wPlacement, 0, sizeof(WINDOWPLACEMENT));
	wPlacement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wPlacement);

	// �E�B���h�E�̈ʒu���X�V
	CRect r = CRect(wPlacement.rcNormalPosition);
	wPlacement.rcNormalPosition.right = wPlacement.rcNormalPosition.left + cxTotal;
	wPlacement.rcNormalPosition.bottom = wPlacement.rcNormalPosition.top + cyTotal;

	SetWindowPlacement(&wPlacement);
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �m�[�h�A���[�t�̓o�^

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnDetailAdd()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �m�[�h�A���[�t�̍폜

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnDetailDelete()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �I�����ږ��̕ύX

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnDetailRename()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �Ď�����ꗗ��ʂ̕\��

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnDetailMonctrl()
{
	// TODO: �����ɃR�}���h �n���h���[ �R�[�h��ǉ����܂��B
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �J�X�^����ʂ̐ݒ�

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnDetailConfig()
{
	if (mMenuItem == NULL)
		return;

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, mMenuItem);

	CCustomDetailConfig config(this, pnode->GetWindowInfo().type);

	// �ݒ肳��Ă���F����ݒ�_�C�A���O�Ɏ擾����
	for (int i = 0; i < sizeof(mColorConfig) / sizeof(mColorConfig[0]); i++) {

		stColorData color;
		if (theApp.GetCustomControl().GetDataManager().GetNodeColor(this, mColorConfig[i].type, color) == true) {
			switch (mColorConfig[i].type) {
			case	eTreeItemType_Window:
				config.mColor[eColorType_Window] = color.back;
				break;
			case	eTreeItemType_Title:
				config.mColor[eColorType_TitleBack] = color.textback;
				config.mColor[eColorType_TitleText] = color.text;
				config.mFont[eTreeItemType_Title] = color.font;
				config.mFont[eTreeItemType_Title] = color.font;
				break;
			case	eTreeItemType_Main:
				config.mColor[eColorType_MainBack] = color.textback;
				config.mColor[eColorType_MainText] = color.text;
				config.mFont[eTreeItemType_Main] = color.font;
				config.mFont[eTreeItemType_Main] = color.font;
				break;
			case	eTreeItemType_Sub:
				config.mColor[eColorType_SubBack] = color.textback;
				config.mColor[eColorType_SubText] = color.text;
				config.mFont[eTreeItemType_Sub] = color.font;
				config.mFont[eTreeItemType_Sub] = color.font;
				break;
			case	eTreeItemType_Item:
				config.mColor[eColorType_ItemBack] = color.textback;
				config.mColor[eColorType_ItemText] = color.text;
				config.mColor[eColorType_ValueText] = color.value;
				config.mColor[eColorType_UnitText] = color.unit;
				config.mFont[eTreeItemType_Item] = color.font;
				config.mFont[eTreeItemType_Item] = color.font;
				break;
			}
		}
	}

	// �ݒ��ʕ\��
	if (config.DoModal() != IDOK) {
		return;
	}

#ifdef _NOPROC
	return;
#endif

	// �ݒ肳�ꂽ�t�H���g����ő卂�������߂�
	mTreeLogFont.lfHeight = 0;
	for (int i = 0; i < sizeof(mColorConfig) / sizeof(mColorConfig[0]); i++) {
		switch (mColorConfig[i].type) {
		//case	eTreeItemType_Window:
		case	eTreeItemType_Title:
			if (abs(mTreeLogFont.lfHeight) < abs(config.mFont[eTreeItemType_Title].lfHeight)) {
				mTreeLogFont.lfHeight = config.mFont[eTreeItemType_Title].lfHeight;
			}
			break;
		case	eTreeItemType_Main:
			if (abs(mTreeLogFont.lfHeight) < abs(config.mFont[eTreeItemType_Main].lfHeight)) {
				mTreeLogFont.lfHeight = config.mFont[eTreeItemType_Main].lfHeight;
			}
			break;
		case	eTreeItemType_Sub:
			if (abs(mTreeLogFont.lfHeight) < abs(config.mFont[eTreeItemType_Sub].lfHeight)) {
				mTreeLogFont.lfHeight = config.mFont[eTreeItemType_Sub].lfHeight;
			}
			break;
		case	eTreeItemType_Item:
			if (abs(mTreeLogFont.lfHeight) < abs(config.mFont[eTreeItemType_Item].lfHeight)) {
				mTreeLogFont.lfHeight = config.mFont[eTreeItemType_Item].lfHeight;
			}
			break;
		}
	}
	// �c���[�R���g���[���̔w�i�F�ݒ�
	mTreeCtrl.SetFontEx(eTreeItemType_Window, mTreeLogFont);

	// �ݒ�_�C�A���O�Őݒ肳�ꂽ�F����ۑ�����
	for (int i = 0; i < sizeof(mColorConfig) / sizeof(mColorConfig[0]); i++) {
		stColorData color;
		if (theApp.GetCustomControl().GetDataManager().GetNodeColor(this, mColorConfig[i].type, color) == true) {
			switch (mColorConfig[i].type) {
			case	eTreeItemType_Window:
				color.back = config.mColor[eColorType_Window];
				mTreeCtrl.SetBkColor(color.back);
				break;
			case	eTreeItemType_Title:
				color.textback = config.mColor[eColorType_TitleBack];
				color.text = config.mColor[eColorType_TitleText];
				color.font = config.mFont[eTreeItemType_Title];
				break;
			case	eTreeItemType_Main:
				color.textback = config.mColor[eColorType_MainBack];
				color.text = config.mColor[eColorType_MainText];
				color.font = config.mFont[eTreeItemType_Main];
				break;
			case	eTreeItemType_Sub:
				color.textback = config.mColor[eColorType_SubBack];
				color.text = config.mColor[eColorType_SubText];
				color.font = config.mFont[eTreeItemType_Sub];
				break;
			case	eTreeItemType_Item:
				color.textback = config.mColor[eColorType_ItemBack];
				color.text = config.mColor[eColorType_ItemText];
				color.value = config.mColor[eColorType_ValueText];
				color.unit = config.mColor[eColorType_UnitText];
				color.font = config.mFont[eTreeItemType_Item];
				break;
			}
			if (mColorConfig[i].type != eTreeItemType_Window) {
				theApp.GetCustomControl().GetDataManager().SetNodeColor(this, mColorConfig[i].type, color);
				mTreeCtrl.SetFontEx(mColorConfig[i].type, color.font);
			}
		}
	}

	mTreeCtrl.Invalidate();

	//pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	//if (pnode->GetWindowInfo().manager->GetSafeHwnd()) {
	//	pnode->GetWindowInfo().manager->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)this);
	//}
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �c�[���`�b�v�C�x���g

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnTvnGetInfoTipTreeCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMTVGETINFOTIP>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	*pResult = 0;

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, pGetInfoTip->hItem);
	if (pnode == NULL) {
		return;
	}
	if (pnode->GetWindowInfo().type != eTreeItemType_Item) {
		return;
	}

	CString strmon = _T("EMPTY"), strcon = _T("EMPTY");
	if (CString(pnode->GetMonCtrl().mname).IsEmpty() == false) {
		strmon = pnode->GetMonCtrl().mname;
	}
	if (CString(pnode->GetMonCtrl().cname).IsEmpty() == false) {
		strcon = pnode->GetMonCtrl().cname;
	}
	mToolText.Format(_T("%s\n%s"), strmon, strcon);
	pGetInfoTip->pszText = (LPWSTR)(LPCTSTR)mToolText;
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# ���b�Z�[�W����

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
LRESULT CCustomDetail::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case	eUserMessage_Detail_Mode:
		updateMode();
		break;
	case	eUserMessage_Manager_Update:
		setTreeTitle(lParam);
		break;
	default:
		return CCustomDialogBase::WindowProc(message, wParam, lParam);
	}
	return TRUE;
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �^�C�g�������̐ݒ�

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::setTreeTitle(LPARAM lParam)
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, (HTREEITEM)lParam);

	if (pnode->GetWindowInfo().type == eTreeItemType_Title) {
		swprintf_s(pnode->GetWindowInfo().title, mNameSize, _T("%s"), pnode->GetMonCtrl().display);
		updateMode();
	}
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �^�C�g�������̕ύX

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::updateMode()
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	CString title = pnode->GetWindowInfo().title;
	mMode = eTreeItemMode_Monitor;
	mTreeCtrl.ModifyStyle(TVS_EDITLABELS, 0);
	if (pnode->GetWindowInfo().mode == eTreeItemMode_Edit) {
		title += CString(mEditModeString);
		mMode = eTreeItemMode_Edit;
		mTreeCtrl.ModifyStyle(0, TVS_EDITLABELS);
	}

	// �E�B���h�E�e�L�X�g�̕ύX
	SetWindowText(title);
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# ���j���[�̏�����

@param

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CCustomDialogBase::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	updateMenu(pPopupMenu);
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# ���j���[���ڂ̊���/�񊈐��̐ݒ�

@param

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::updateMenu(CMenu* pMenu)
{
	int menuCount = pMenu->GetMenuItemCount();
	for (int i = 0; i < menuCount; i++){
		MENUITEMINFO menuItemInfo;
		::memset(&menuItemInfo, 0, sizeof(MENUITEMINFO));
		menuItemInfo.cbSize = sizeof(MENUITEMINFO);
		menuItemInfo.fMask = MIIM_STATE | MIIM_ID;
		if (pMenu->GetMenuItemInfo(i, &menuItemInfo, TRUE))
		{
			if (updateMenuItem(&menuItemInfo))
			{
				pMenu->SetMenuItemInfo(i, &menuItemInfo, TRUE);
			}
		}
	}
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# ���j���[���ڂ̃`�F�b�N��Ԃ̎擾

@param

@retval �Ȃ�
*/
/*============================================================================*/
bool CCustomDetail::updateMenuItem(MENUITEMINFO* pMenuItemInfo)
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	switch (pMenuItemInfo->wID)
	{
	case ID_MENUDETAIL_EDIT:
		pMenuItemInfo->fState = pnode->GetWindowInfo().mode == eTreeItemMode_Edit ? MFS_CHECKED : MFS_UNCHECKED;
		return true;
	case ID_MENUDETAIL_MONITOR:
		pMenuItemInfo->fState = pnode->GetWindowInfo().mode == eTreeItemMode_Monitor ? MFS_CHECKED : MFS_UNCHECKED;
		return true;
	}
	return false;
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �h���b�v���̃A�C�e���̈ړ�

@param	hitemDrag	�h���b�O�A�C�e��
@param	hitemDrop	�h���b�v�A�C�e��

@retval
*/
/*============================================================================*/
bool CCustomDetail::DropMoveItem(HTREEITEM hItemDrag, HTREEITEM hItemDrop)
{
	HTREEITEM	hNewItem, hFirstChild;
	CString str;

	CTreeNode* pnodeDrag = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItemDrag);
	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItemDrop);

	str = generateTreeText(pnodeDrag);
	hNewItem = mTreeCtrl.InsertItem(str, NULL, NULL, hItemDrop, TVI_FIRST);
	mTreeCtrl.SetItemData(hNewItem, (LPARAM)hNewItem);

	CTreeNode* tn_item = pnodeDrop->CreateTreeNode(hItemDrop, hNewItem, TVI_FIRST);
	swprintf_s(tn_item->GetMonCtrl().display, mNameSize, _T("%s"), pnodeDrag->GetMonCtrl().display);
	if (tn_item->GetWindowInfo().type == eTreeItemType_Item) {
		str = generateTreeText(pnodeDrag);
		mTreeCtrl.SetItemText(hNewItem, str);
	}

	// �m�[�h�R�s�[
	tn_item->CopyItem(pnodeDrag);

	// ���[�t�̏ꍇ�A���䕶�����ݒ�
	if (tn_item->GetWindowInfo().type == eTreeItemType_Item) {
		str = generateTreeText(pnodeDrag);
		mTreeCtrl.SetItemText(hNewItem, str);
	}

	while ((hFirstChild = mTreeCtrl.GetChildItem(hItemDrag)) != NULL)
	{
		DropMoveItem(hFirstChild, hNewItem);
	}
	mTreeCtrl.Expand(hNewItem, TVE_EXPAND);

	mTreeCtrl.DeleteItem(hItemDrag);

	// �h���b�O�m�[�h�̐e�̎q���X�g����폜����
	pnodeDrag->DeleteTreeNode(hItemDrag);

	return true;
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �h���b�v���̃A�C�e���̃R�s�[�i�J�X�^����ʊԁj

@param	pnodeDrag	�h���b�O�m�[�h
@param	hitemDrop	�h���b�v�A�C�e��

@retval
*/
/*============================================================================*/
bool CCustomDetail::DropCopyItem(CTreeNode* pnodeDrag, HTREEITEM hItemDrop)
{
	HTREEITEM	hNewItem;
	CString str;

	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItemDrop);
	UINT sortno = 0;

	bool bLeafDrop = false;
	if (pnodeDrop->GetWindowInfo().type == eTreeItemType_Item) {
		sortno = pnodeDrop->GetWindowInfo().sortno;
		hItemDrop = mTreeCtrl.GetParentItem(hItemDrop);
		pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItemDrop);
		bLeafDrop = true;
	}
	else {
	}

	str = generateTreeText(pnodeDrag);
	hNewItem = mTreeCtrl.InsertItem(str, NULL, NULL, hItemDrop, TVI_FIRST);
	mTreeCtrl.SetItemData(hNewItem, (LPARAM)hNewItem);
	CTreeNode* new_node = pnodeDrop->CreateTreeNode(hItemDrop, hNewItem, TVI_FIRST);
	swprintf_s(new_node->GetMonCtrl().display, mNameSize, _T("%s"), pnodeDrag->GetMonCtrl().display);
	if (new_node->GetWindowInfo().type == eTreeItemType_Item) {
		str = generateTreeText(pnodeDrag);
		mTreeCtrl.SetItemText(hNewItem, str);
	}

	// �m�[�h�R�s�[
	new_node->CopyItem(pnodeDrag);

	// ���[�t�̏ꍇ�A���䕶�����ݒ�
	if (new_node->GetWindowInfo().type == eTreeItemType_Item) {
		str = generateTreeText(pnodeDrag);
		mTreeCtrl.SetItemText(hNewItem, str);
	}

	if (bLeafDrop == true) {
		// �h���b�v�悪���[�t�̏ꍇ
		new_node->GetWindowInfo().sortno = sortno + 1;
		DragDrop_SortItem(hItemDrop);
	}

	// �\�[�g�ԍ��̍X�V
	DragDrop_UpdateSortNo(hItemDrop);

	return true;
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# ���[�t�̓o�^

@param		targetitem		�o�^�Ώۂ̃A�C�e���i�e�j
@param		tagetnode		�o�^�Ώۂ̃m�[�h
@param		pdata			�h���b�v�f�[�^

@retval void
*/
/*============================================================================*/
void CCustomDetail::DragDrop_AddLeaf(HTREEITEM targetitem, CTreeNode* targetnode, stDragData* pdata)
{
	CString str, strmon, strcon;
	str = createLeafText(mDefaultCustomItemText, _T(""), _T(""));

	vector<int>::iterator itr;
	for (itr = pdata->indexes.begin(); itr != pdata->indexes.end(); itr++) {
		// ���A�C�e���̍쐬
		HTREEITEM item = mTreeCtrl.InsertItem(str, NULL, NULL, targetitem);
		mTreeCtrl.SetItemData(item, (LPARAM)item);

		// ���A�C�e������m�[�h���쐬����
		CTreeNode* item_node = targetnode->CreateTreeNode(targetitem, item);
		CTreeNode* pcopyitem = theApp.GetCustomControl().GetDataManager().SearchItemNodeType(this, eTreeItemType_Item);
		if (pcopyitem != NULL)
			item_node->CopyItem(pcopyitem, true);

		// �m�[�h���̍쐬
		setNodeWindowInfo(item_node, eTreeItemType_Item, (LPWSTR)mDefaultCustomItemText, targetnode);

		// �h���b�v���ꂽ��񂩂�m�[�h�����쐬����
		DragDrop_GetDataName(pdata->type, (*itr), strmon, strcon);

		if (strmon.IsEmpty() == false) {
			swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strmon);
		}
		else if (strcon.IsEmpty() == false) {
			swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strcon);
		}
		else {
			swprintf_s(item_node->GetMonCtrl().display, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)mDefaultCustomItemText);
		}
		swprintf_s(item_node->GetMonCtrl().mname, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strmon);
		swprintf_s(item_node->GetMonCtrl().cname, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strcon);

		// ���A�C�e���փh���b�v���ꂽ�������ƂɍĐݒ�
		mTreeCtrl.SetSubItemText(item, CCustomTreeListCtrl::eItem, item_node->GetMonCtrl().display);
		if (strcon.IsEmpty() == false) {
			mTreeCtrl.SetSubItemText(item, CCustomTreeListCtrl::eControl, mCOntrolSignString);
		}
	}
	mTreeCtrl.Expand(targetitem, TVE_EXPAND);
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# ���[�t�̍X�V

@param		targetitem		�o�^�Ώۂ̃A�C�e��
@param		tagetnode		�o�^�Ώۂ̃m�[�h
@param		pdata			�h���b�v�f�[�^

@retval void
*/
/*============================================================================*/
void CCustomDetail::DragDrop_UpdateLeaf(HTREEITEM targetitem, CTreeNode* targetnode, stDragData* pdata)
{
	CString str, strmon, strcon;

	vector<int>::iterator itr;
	for (itr = pdata->indexes.begin(); itr != pdata->indexes.end(); itr++) {
		DragDrop_GetDataName(pdata->type, (*itr), strmon, strcon);

		if (strmon.IsEmpty() == false) {
			swprintf_s(targetnode->GetMonCtrl().mname, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strmon);
		}
		else if (strcon.IsEmpty() == false) {
			swprintf_s(targetnode->GetMonCtrl().cname, mNameSize, _T("%s"), (LPWSTR)(LPCTSTR)strcon);
		}

		if (strcon.IsEmpty() == false) {
			mTreeCtrl.SetSubItemText(targetitem, CCustomTreeListCtrl::eControl, mCOntrolSignString);
		}
		else {
			mTreeCtrl.SetSubItemText(targetitem, CCustomTreeListCtrl::eControl, _T(""));
		}
	}
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �w�b�_�[���擾

@param		item	�c���[�A�C�e��

@retval
*/
/*============================================================================*/
void CCustomDetail::DragDrop_GetDataName(UINT type, UINT pos, CString& strmon, CString& strcon)
{
	strmon.Empty();
	strcon.Empty();

	switch (type) {
	case	eFromType_Mon:
		// �Ď����擾
		//strmon = theApp.GetCustomControl().GetDataManager().GetDataMonitor().GetEqList()[pos];
		break;
	case	eFromType_Cntl:
		// ���䖼�擾
		//strcon = theApp.GetCustomControl().GetDataManager().GetDataControl().GetEqList()[pos];
		break;
	}
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �c���[�A�C�e���̓W�J

@param  todrag	�h���b�O���i0:�Ď��E���� 1:�J�X�^���j
@param  lParam	�h���b�v���i�h���b�O���ɂ���ĈقȂ�j

@retval �Ȃ�
*/
/*============================================================================*/
BOOL CCustomDetail::DragDrop_SetSelectTarget(UINT todrag, LPARAM lParam)
{
	BOOL ret = TRUE;
	if (todrag == eFromType_Custom) {
		// ���̃J�X�^����ʂ���̃h���b�O
		CTreeNode* pnode = (CTreeNode*)lParam;

		CPoint      pt;
		GetCursorPos(&pt);
		mTreeCtrl.ScreenToClient(&pt);
		HTREEITEM hItem = mTreeCtrl.HitTest(pt);
		if (hItem == NULL) {
			mTreeCtrl.SelectDropTarget(NULL);
			ret = FALSE;
		}
		else {
			CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);
			TRACE("=== Select Target Display : %s\n", CStringA(pnode->GetMonCtrl().display));
			if (pnode == NULL) {
				mTreeCtrl.SelectDropTarget(NULL);
				ret = FALSE;
			}
			else {
				UINT type = pnode->GetWindowInfo().type;
				if (pnode == NULL) {
					mTreeCtrl.SelectDropTarget(NULL);
					ret = FALSE;
				}
				else {
					switch (type) {
					case	eTreeItemType_Sub:
					case	eTreeItemType_Item:
						mTreeCtrl.SelectDropTarget(hItem);
						break;
					default:
						mTreeCtrl.SelectDropTarget(NULL);
						ret = FALSE;
					}
					mTreeCtrl.Expand(hItem, TVE_EXPAND);
				}
			}
		}
	}
	else {
		// �Ď��E����ꗗ����̃h���b�O
		stDragData* pdata = (stDragData*)lParam;

		CPoint pt = CPoint(pdata->point);
		mTreeCtrl.ScreenToClient(&pt);
		// �h���b�v�ʒu��񂩂�Ώۂ̃A�C�e�����擾����
		HTREEITEM hItem = mTreeCtrl.HitTest(pt);
		if (hItem == NULL) {
			mTreeCtrl.SelectDropTarget(NULL);
			ret = FALSE;
		}
		else {
			CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);
			TRACE("=== Select Target Display : %s\n", CStringA(pnode->GetMonCtrl().display));
			UINT type = pnode->GetWindowInfo().type;
			if (pnode == NULL) {
				mTreeCtrl.SelectDropTarget(NULL);
				ret = FALSE;
			}
			else {
				switch (type) {
				case	eTreeItemType_Sub:
				case	eTreeItemType_Item:
					mTreeCtrl.SelectDropTarget(hItem);
					break;
				default:
					mTreeCtrl.SelectDropTarget(NULL);
					ret = FALSE;
				}
				mTreeCtrl.Expand(hItem, TVE_EXPAND);
			}
		}
	}
	return ret;
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �h���b�O���h���b�v�Ώۂ̐ݒ�

@param  todrag	�h���b�O���i0:�Ď��E���� 1:�J�X�^���j
@param  lParam	�h���b�v���i�h���b�O���ɂ���ĈقȂ�j
@param  bMove	�h���b�O���̃f�[�^���R�s�[����̂��ړ�����̂��̃t���O

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::DragDrop_SetDropTarget(UINT todrag, LPARAM lParam, bool bMove/* = true*/)
{
	if (todrag == eFromType_Custom) {
		CPoint pt;
		GetCursorPos(&pt);
		mTreeCtrl.ScreenToClient(&pt);
		HTREEITEM hItem = mTreeCtrl.HitTest(pt);
		if (hItem == NULL) {
			mTreeCtrl.SelectDropTarget(NULL);
			return;
		}

		if (bMove == true) {
			DragDrop_Move(hItem, lParam);
		}
		else {
			DragDrop_Copy(hItem, lParam);
		}
	}
	else {
		// �Ď��E����ꗗ����̃h���b�O
		stDragData* pdata = (stDragData*)lParam;

		CPoint pt = CPoint(pdata->point);
		mTreeCtrl.ScreenToClient(&pt);
		// �h���b�v�ʒu��񂩂�Ώۂ̃A�C�e�����擾����
		HTREEITEM hItem = mTreeCtrl.HitTest(pt);
		if (hItem == NULL)
			return;

		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);
		if (pnode->GetWindowInfo().type != eTreeItemType_Sub && pnode->GetWindowInfo().type != eTreeItemType_Item)
			return;

		// �h���b�v���ꂽ�A�C�e����ǉ��i�쐬�j����
		CString strSub, strItem;

		if (pnode->GetWindowInfo().type == eTreeItemType_Sub) {
			// �ǉ�
			DragDrop_AddLeaf(hItem, pnode, pdata);
		}
		if (pnode->GetWindowInfo().type == eTreeItemType_Item) {
			// �X�V
			DragDrop_UpdateLeaf(hItem, pnode, pdata);
		}
		mTreeCtrl.Expand(hItem, TVE_EXPAND);
		mTreeCtrl.SelectDropTarget(NULL);
	}
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �ݔ��ڍד��u�̃h���b�O�f�[�^�̃R�s�[���s��

@param  hItem	�h���b�v��A�C�e��
@param  lParam	�h���b�v���i�h���b�O���ɂ���ĈقȂ�j

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::DragDrop_Copy(HTREEITEM hItem, LPARAM lParam)
{
	/*
		�h���b�O�R�s�[����
		�E�h���b�O�F���C���m�[�h�@���@�h���b�v�F�^�C�g���m�[�h�i���C���m�[�h�̍ŏ�ʂɃR�s�[�j
		�E�h���b�O�F�T�u�m�[�h�@���@�h���b�v�F���C���m�[�h�i�T�u�m�[�h�̍ŏ�ʂɃR�s�[�j
		�E�h���b�O�F���[�t�@���@�h���b�v�F�T�u�m�[�h�i���[�t�̍ŏ�ʂɃR�s�[�j
		�E�h���b�O�F���[�t�@���@�h���b�v�F���[�t�i���[�t�̉��ɂɃR�s�[�j
	*/
	CTreeNode* pnodeDrag = (CTreeNode*)lParam;
	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);

	bool bSort = false;
	if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Main && pnodeDrop->GetWindowInfo().type == eTreeItemType_Title) {
		// �h���b�O�F���C���m�[�h�@���@�h���b�v�F�^�C�g���m�[�h�i���C���m�[�h�̍ŏ�ʂɃR�s�[�j
		TRACE("Drag_Drop Main -> Title\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Sub && pnodeDrop->GetWindowInfo().type == eTreeItemType_Main) {
		// �h���b�O�F�T�u�m�[�h�@���@�h���b�v�F���C���m�[�h�i�T�u�m�[�h�̍ŏ�ʂɃR�s�[�j
		TRACE("Drag_Drop Sub -> Main\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Item && pnodeDrop->GetWindowInfo().type == eTreeItemType_Sub) {
		// �h���b�O�F���[�t�@���@�h���b�v�F�T�u�m�[�h�i���[�t�̍ŏ�ʂɃR�s�[�j
		TRACE("Drag_Drop Leaf -> Sub\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Item && pnodeDrop->GetWindowInfo().type == eTreeItemType_Item) {
		// �h���b�O�F���[�t�@���@�h���b�v�F���[�t�i���[�t�̉��ɂɃR�s�[�j
		bSort = true;
		TRACE("Drag_Drop Leaf -> Leaf\n");
	}
	else {
		TRACE("Drag_Drop Error %d -> %d\n", pnodeDrag->GetWindowInfo().type, pnodeDrop->GetWindowInfo().type);
		mTreeCtrl.SelectDropTarget(NULL);
		return;
	}

	// �����܂ł�����R�s�[�\
	CWnd* dragWnd = pnodeDrag->GetWindowInfo().wnd;
	CWnd* dropWnd = pnodeDrop->GetWindowInfo().wnd;
	HTREEITEM dragItem = pnodeDrag->GetTreeItem();
	HTREEITEM dropItem = pnodeDrop->GetTreeItem();
	DragDrop_CopyItem(dragWnd, dragItem, dropWnd, dropItem, bSort);
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �ݔ��ڍד��u�̃h���b�O�f�[�^�̈ړ����s��

@param  hItem	�h���b�v��A�C�e��
@param  lParam	�h���b�v���i�h���b�O���ɂ���ĈقȂ�j

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::DragDrop_Move(HTREEITEM hItem, LPARAM lParam)
{
	/*
		�h���b�O�ړ�����
		�E�h���b�O�F���C���m�[�h�@���@�h���b�v�F�^�C�g���m�[�h�i���C���m�[�h�̍ŏ�ʂɃR�s�[�j
		�E�h���b�O�F�T�u�m�[�h�@���@�h���b�v�F���C���m�[�h�i�T�u�m�[�h�̍ŏ�ʂɃR�s�[�j
		�E�h���b�O�F���[�t�@���@�h���b�v�F�T�u�m�[�h�i���[�t�̍ŏ�ʂɃR�s�[�j
		�E�h���b�O�F���[�t�@���@�h���b�v�F���[�t�i���[�t�̉��ɂɃR�s�[�j
	*/
	CTreeNode* pnodeDrag = (CTreeNode*)lParam;
	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);

	bool bSort = false;
	if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Main && pnodeDrop->GetWindowInfo().type == eTreeItemType_Title) {
		// �h���b�O�F���C���m�[�h�@���@�h���b�v�F�^�C�g���m�[�h�i���C���m�[�h�̍ŏ�ʂɃR�s�[�j
		TRACE("Drag_Drop Main -> Title\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Sub && pnodeDrop->GetWindowInfo().type == eTreeItemType_Main) {
		// �h���b�O�F�T�u�m�[�h�@���@�h���b�v�F���C���m�[�h�i�T�u�m�[�h�̍ŏ�ʂɃR�s�[�j
		TRACE("Drag_Drop Sub -> Main\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Item && pnodeDrop->GetWindowInfo().type == eTreeItemType_Sub) {
		// �h���b�O�F���[�t�@���@�h���b�v�F�T�u�m�[�h�i���[�t�̍ŏ�ʂɃR�s�[�j
		TRACE("Drag_Drop Leaf -> Sub\n");
	}
	else if (pnodeDrag->GetWindowInfo().type == eTreeItemType_Item && pnodeDrop->GetWindowInfo().type == eTreeItemType_Item) {
		// �h���b�O�F���[�t�@���@�h���b�v�F���[�t�i���[�t�̉��ɂɃR�s�[�j
		bSort = true;
		TRACE("Drag_Drop Leaf -> Leaf\n");
	}
	else {
		TRACE("Drag_Drop Error %d -> %d\n", pnodeDrag->GetWindowInfo().type, pnodeDrop->GetWindowInfo().type);
		mTreeCtrl.SelectDropTarget(NULL);
		return;
	}

	// �����܂ł�����ړ��\
	CWnd* dragWnd = pnodeDrag->GetWindowInfo().wnd;
	CWnd* dropWnd = pnodeDrop->GetWindowInfo().wnd;
	HTREEITEM dragItem = pnodeDrag->GetTreeItem();
	HTREEITEM dropItem = pnodeDrop->GetTreeItem();
	DragDrop_MoveItem(dragWnd, dragItem, dropWnd, dropItem, bSort);
}
/*============================================================================*/
/*! �c���[�R���g���[���g���@�\

-# �h���b�v���̃A�C�e���̈ړ�

@param	dragWnd		�h���b�O�E�B���h�E�n���h��
@param	pnodeDrag	�h���b�O�A�C�e��
@param	dropWnd		�h���b�v�E�B���h�E�n���h��
@param	pnodeDrop	�h���b�v�A�C�e��
@param	bSort		�h���b�v������Ƀ\�[�g���邩�̃t���O

@retval
*/
/*============================================================================*/
bool CCustomDetail::DragDrop_CopyItem(CWnd* dragWnd, HTREEITEM dragItem, CWnd* dropWnd, HTREEITEM dropItem, bool bSort)
{
	CString str;
	HTREEITEM	hNewItem, hFirstChild;

	CTreeNode* pnodeDrag = theApp.GetCustomControl().GetDataManager().SearchItemNode(dragWnd, dragItem);
	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(dropWnd, dropItem);
	CCustomTreeListCtrl* dragTree = (CCustomTreeListCtrl*)pnodeDrag->GetWindowInfo().tree;
	CCustomTreeListCtrl* dropTree = (CCustomTreeListCtrl*)pnodeDrop->GetWindowInfo().tree;

	CTreeNode* pnode;
	CTreeNode* pnodeParent = NULL;
	HTREEITEM parent=NULL;
	if (bSort == true) {
		// ���[�t���烊�[�t
		// �h���b�v�A�C�e���̐e���擾����
		parent = dropTree->GetParentItem(dropItem);
		pnodeParent = theApp.GetCustomControl().GetDataManager().SearchItemNode(dropWnd, parent);
		str = generateTreeText(pnodeDrag);
		hNewItem = dropTree->InsertItem(str, NULL, NULL, parent, TVI_SORT);
		mTreeCtrl.SetItemData(hNewItem, (LPARAM)hNewItem);
		pnode = pnodeParent->CreateTreeNode(parent, hNewItem);
	}
	else {
		str = generateTreeText(pnodeDrag);
		hNewItem = dropTree->InsertItem(str, NULL, NULL, (bSort) ? dropTree->GetParentItem(dropItem) : dropItem, TVI_FIRST);
		mTreeCtrl.SetItemData(hNewItem, (LPARAM)hNewItem);
		pnode = pnodeDrop->CreateTreeNode(dropItem, hNewItem, TVI_FIRST);
	}

	pnode->CopyItem(pnodeDrag);
	pnode->GetWindowInfo().wnd = dropWnd;
	pnode->GetWindowInfo().tree = dropTree;

	while ((hFirstChild = dragTree->GetChildItem(dragItem)) != NULL)
	{
		DragDrop_CopyItem(dragWnd, hFirstChild, dropWnd, hNewItem, bSort);
	}
	dropTree->Expand(hNewItem, TVE_EXPAND);
	dropTree->SelectDropTarget(NULL);

	if (bSort == true) {
		TRACE("DragDrop Sort Start(%08x) DropItem(%08x)\n", dropTree, dropItem);
		pnode->GetWindowInfo().sortno = pnodeDrop->GetWindowInfo().sortno + 1;
		HTREEITEM hParent = dropTree->GetParentItem(dropItem);
		DragDrop_SortItem(hParent);
		if (pnodeParent != NULL) {
			pnodeParent->SortTreeNode(parent);
		}

		// �\�[�g�ԍ��̍X�V
		//DragDrop_UpdateSortNo(hParent);
	}

	UpdateSortNo(mTreeCtrl.GetRootItem());

	return true;
}
/*============================================================================*/
/*! �c���[�R���g���[���g���@�\

-# �h���b�v���̃A�C�e���̈ړ�

@param	dragWnd		�h���b�O�E�B���h�E�n���h��
@param	pnodeDrag	�h���b�O�A�C�e��
@param	dropWnd		�h���b�v�E�B���h�E�n���h��
@param	pnodeDrop	�h���b�v�A�C�e��
@param	bSort		�h���b�v������Ƀ\�[�g���邩�̃t���O

@retval
*/
/*============================================================================*/
bool CCustomDetail::DragDrop_MoveItem(CWnd* dragWnd, HTREEITEM dragItem, CWnd* dropWnd, HTREEITEM dropItem, bool bSort)
{
	CString str;
	HTREEITEM	hNewItem, hFirstChild;

	CTreeNode* pnodeDrag = theApp.GetCustomControl().GetDataManager().SearchItemNode(dragWnd, dragItem);
	CTreeNode* pnodeDrop = theApp.GetCustomControl().GetDataManager().SearchItemNode(dropWnd, dropItem);
	CCustomTreeListCtrl* dragTree = (CCustomTreeListCtrl*)pnodeDrag->GetWindowInfo().tree;
	CCustomTreeListCtrl* dropTree = (CCustomTreeListCtrl*)pnodeDrop->GetWindowInfo().tree;

	CTreeNode* pnode;
	CTreeNode* pnodeParent = NULL;
	HTREEITEM parent = NULL;
	if (bSort == true) {
		// ���[�t���烊�[�t
		// �h���b�v�A�C�e���̐e���擾����
		parent = dropTree->GetParentItem(dropItem);
		pnodeParent = theApp.GetCustomControl().GetDataManager().SearchItemNode(dropWnd, parent);
		str = generateTreeText(pnodeDrag);
		hNewItem = dropTree->InsertItem(str, NULL, NULL, parent, TVI_SORT);
		mTreeCtrl.SetItemData(hNewItem, (LPARAM)hNewItem);
		pnode = pnodeParent->CreateTreeNode(parent, hNewItem);
	}
	else {
		str = generateTreeText(pnodeDrag);
		hNewItem = dropTree->InsertItem(str, NULL, NULL, (bSort) ? dropTree->GetParentItem(dropItem) : dropItem, TVI_FIRST);
		mTreeCtrl.SetItemData(hNewItem, (LPARAM)hNewItem);
		pnode = pnodeDrop->CreateTreeNode(dropItem, hNewItem, TVI_FIRST);
	}

	pnode->CopyItem(pnodeDrag);
	pnode->GetWindowInfo().wnd = dropWnd;
	pnode->GetWindowInfo().tree = dropTree;

	while ((hFirstChild = dragTree->GetChildItem(dragItem)) != NULL)
	{
		DragDrop_MoveItem(dragWnd, hFirstChild, dropWnd, hNewItem, bSort);
	}
	dropTree->Expand(hNewItem, TVE_EXPAND);
	dropTree->SelectDropTarget(NULL);

	dragTree->DeleteItem(dragItem);

	if (bSort == true) {
		pnode->GetWindowInfo().sortno = pnodeDrop->GetWindowInfo().sortno + 1;
		DragDrop_SortItem(parent);
		if (pnodeParent != NULL) {
			pnodeParent->SortTreeNode(parent);
		}

		// �\�[�g�ԍ��̍X�V
		//DragDrop_UpdateSortNo(parent);
	}

	UpdateSortNo(mTreeCtrl.GetRootItem());

	return true;
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �c���[�R���g���[���̃\�[�g���s���R�[���o�b�N�֐�

@param

@retval int
*/
/*============================================================================*/
void CCustomDetail::DragDrop_SortItem(HTREEITEM item)
{
	if (item != NULL && mTreeCtrl.ItemHasChildren(item)) {

		TVSORTCB tvs;
		tvs.hParent = item;
		tvs.lpfnCompare = DragDrop_Compare;
		tvs.lParam = (LPARAM)&mTreeCtrl;

		CTreeCtrl* ptree = (CTreeCtrl*)tvs.lParam;
		CWnd* p = ptree->GetParent();

		mTreeCtrl.SortChildrenCB(&tvs);
	}
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �c���[�R���g���[���̃\�[�g���s���R�[���o�b�N�֐�

@param

@retval int
*/
/*============================================================================*/
int CALLBACK CCustomDetail::DragDrop_Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CTreeCtrl* ptree = (CTreeCtrl*)lParamSort;
	CWnd* p = ptree->GetParent();
	CTreeNode* pdata1 = theApp.GetCustomControl().GetDataManager().SearchItemNode(ptree->GetParent(), (HTREEITEM)lParam1);
	CTreeNode* pdata2 = theApp.GetCustomControl().GetDataManager().SearchItemNode(ptree->GetParent(), (HTREEITEM)lParam2);

	TRACE("DragDrop Compare(%08x) %08x:%d > %08x:%d\n", ptree, lParam1, (pdata1 == NULL) ? -1 : pdata1->GetWindowInfo().sortno, lParam2, (pdata2 == NULL) ? -1 : pdata2->GetWindowInfo().sortno);

	return (pdata1 != NULL && pdata2 != NULL) ? (pdata1->GetWindowInfo().sortno > pdata2->GetWindowInfo().sortno) : 0;
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �c���[�R���g���[���̃\�[�g���s���R�[���o�b�N�֐�

@param

@retval int
*/
/*============================================================================*/
void CCustomDetail::DragDrop_UpdateSortNo(HTREEITEM item)
{
	CTreeNode* pSubNode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, item);
	if (pSubNode->GetWindowInfo().type != eTreeItemType_Sub)
		return;

	HTREEITEM child = mTreeCtrl.GetChildItem(item);
	UINT pos = 0;
	while (child != NULL) {
		CTreeNode* node = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, child);
		if (node == NULL)
			break;

		node->GetWindowInfo().sortno = pos * mSortRange;

		child = mTreeCtrl.GetNextItem(child, TVGN_NEXT);
		pos++;
	}
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �c���[�e�L�X�g�̐���

@param	pnode	�c���[ɰ��

@retval void
*/
/*============================================================================*/
CString CCustomDetail::generateTreeText(CTreeNode* pnode)
{
	CString ret;

	if (pnode->GetWindowInfo().type == eTreeItemType_Item) {
		ret = createLeafText(pnode->GetMonCtrl().display, pnode->GetMonCtrl().unit, pnode->GetMonCtrl().cname);
	}
	else {
		ret.Format(_T("%s"), pnode->GetMonCtrl().display);
	}
	return ret;
}

/*============================================================================*/
/*! �ݔ��ڍ�

-# �\�[�g�ԍ��̍X�V

@param	hItem	�c���[�A�C�e��

@retval void
*/
/*============================================================================*/
void CCustomDetail::UpdateSortNo(HTREEITEM hItem)
{
	HTREEITEM hSubItem = mTreeCtrl.GetChildItem(hItem);
	while (hSubItem) {
		UpdateSortNo(hSubItem);
		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hSubItem);
		if (pnode->GetWindowInfo().type == eTreeItemType_Sub) {
			HTREEITEM hItem = mTreeCtrl.GetChildItem(hSubItem);
			UINT pos = 0;
			while (hItem) {
				CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(this, hItem);
				pnode->GetWindowInfo().sortno = pos * mSortRange;
				pos++;
				hItem = mTreeCtrl.GetNextSiblingItem(hItem);
			}
		}
		hSubItem = mTreeCtrl.GetNextSiblingItem(hSubItem);
	}
}
