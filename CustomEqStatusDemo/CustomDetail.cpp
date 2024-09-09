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
}

CCustomDetail::~CCustomDetail()
{
}

void CCustomDetail::DoDataExchange(CDataExchange* pDX)
{
	CCustomDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_CTRL, mTreeCtrl);
}


BEGIN_MESSAGE_MAP(CCustomDetail, CCustomDialogBase)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_CTRL, &CCustomDetail::OnNMRClickTreeCtrl)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_TREE_CTRL, &CCustomDetail::OnTvnGetdispinfoTreeCtrl)
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

	CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(this, mMenuItem);
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

-# �c�[���`�b�v�C�x���g

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomDetail::OnTvnGetdispinfoTreeCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	*pResult = 0;
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
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CCustomDialogBase::OnClose();
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
	mTreeCtrl.SetIndent(7);

	// �R�[���o�b�N�֐��̓o�^
	mTreeCtrl.SetClickCallback(messageClick);
	//mTreeCtrl.SetDragCallback(messageDrag);

	if (mRestore == true){
		restoreRoot();
	}
	else{
		createRoot();
	}

	mTreeCtrl.UpdateColumns();

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
#ifdef _DEMO
	return FALSE;
#else
	//CCustomDetail* p = CCustomDetail::Instance();
	CCustomDetail* p = (CCustomDetail*)pwnd;

	UINT mask = 1 << CTreeListCtrl::eItem | 1 << CTreeListCtrl::eUnit;
	if ((1 << nSubItem) & mask)
		return TRUE;

	// ����Z�����������ꂽ���`�F�b�N
	bool bControl = p->mTreeCtrl.IsControl(point);
	if (bControl == false){
		return FALSE;
	}

	CString strText = p->mTreeCtrl.GetSubItemText(hItem, CTreeListCtrl::eControl);
	// ���䕶����̏ꍇ�͐���R�}���h���s
	if (strText.IsEmpty() == false && strText == CString(mCOntrolSignString)){
		// ����R�}���h�𑗐M
		CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(p, hItem);
		CString strCntl = pnode->GetMonCtrl().cname;
		p->MessageBox(_T("�i���j����R�}���h�𑗐M���܂�\n") + strCntl);
	}
	return TRUE;
#endif
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
#ifdef _DEMO
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

	theApp.GetDataManager().AddTreeNode(tn_root);
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
	CTreeNode* pcopyitem = theApp.GetDataManager().SearchItemNodeType(this, eTreeItemType_Main);
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
	CTreeNode* pcopyitem = theApp.GetDataManager().SearchItemNodeType(this, eTreeItemType_Sub);
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
	CTreeNode* pcopyitem = theApp.GetDataManager().SearchItemNodeType(this, eTreeItemType_Item);
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
	CTreeNode* pnode = theApp.GetDataManager().SearchWndNode(this);

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
		if ((*itr)->GetWindowInfo().type == eTreeItemType_Item){
			str = createLeafText((*itr)->GetMonCtrl().display, (*itr)->GetMonCtrl().unit, (*itr)->GetMonCtrl().cname);
		}
		else{
			str.Format(_T("%s"), (*itr)->GetMonCtrl().display);
		}
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

-# �^�C�g���ݒ�

@param

@retval
*/
/*============================================================================*/
void CCustomDetail::setTreeTitle(LPARAM lParam)
{
	CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(this, (HTREEITEM)lParam);

	if (pnode->GetWindowInfo().type == eTreeItemType_Title){
		swprintf_s(pnode->GetWindowInfo().title, mNameSize, _T("%s"), pnode->GetMonCtrl().display);
		SetWindowText(pnode->GetMonCtrl().display);
		if (pnode->GetWindowInfo().manager->GetSafeHwnd())
			pnode->GetWindowInfo().manager->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)this);
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

	CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(this, mMenuItem);

	CCustomDetailConfig config(this, pnode->GetWindowInfo().type);

	// �ݒ肳��Ă���F����ݒ�_�C�A���O�Ɏ擾����
	for (int i = 0; i < sizeof(mColorConfig) / sizeof(mColorConfig[0]); i++) {

		stColorData color;
		if (theApp.GetDataManager().GetNodeColor(this, mColorConfig[i].type, color) == true) {
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
		if (theApp.GetDataManager().GetNodeColor(this, mColorConfig[i].type, color) == true) {
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
				theApp.GetDataManager().SetNodeColor(this, mColorConfig[i].type, color);
				mTreeCtrl.SetFontEx(mColorConfig[i].type, color.font);
			}
		}
	}

	mTreeCtrl.Invalidate();

	pnode = theApp.GetDataManager().SearchWndNode(this);
	if (pnode->GetWindowInfo().manager->GetSafeHwnd()) {
		pnode->GetWindowInfo().manager->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)this);
	}
}
