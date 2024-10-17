/******************************************************************************
* @file    CustomTreeListCtrl.cpp
* @brief   �y�J�X�^�}�C�Y�@�\�z�ݔ��󋵏ڍ׃c���[�N���X
* @auther  SCC
* @date    2024/08/26 �V�K�쐬
* @par
******************************************************************************/
#include "stdafx.h"
#include "CustomEqStatusDemo.h"

#include "CustomTreeListCtrl.h"


//=============================================================================
// CCustomTreeEdit
//=============================================================================
/*============================================================================*/
/*! �c���[���G�f�B�b�g�{�b�N�X

-# �R���X�g���N�^�[

@param
@retval

*/
/*============================================================================*/
CCustomTreeEdit::CCustomTreeEdit(HTREEITEM iItem, int iSubItem, CString sInitText)
:msInitText(sInitText)
{
	miItem = iItem;
	miSubItem = iSubItem;
	mbESC = FALSE;
	mbKeyReturn = FALSE;
	mbKeyShift = FALSE;
	mbNotify = FALSE;
	mnNumberLimit = 100;
}

/*============================================================================*/
/*! �c���[���G�f�B�b�g�{�b�N�X

-# �f�X�g���N�^�[

@param
@retval

*/
/*============================================================================*/
CCustomTreeEdit::~CCustomTreeEdit()
{
}

/*============================================================================*/
/*! �c���[���G�f�B�b�g�{�b�N�X

-# ���b�Z�[�W �}�b�v�̒�`

@param
@retval

*/
/*============================================================================*/
BEGIN_MESSAGE_MAP(CCustomTreeEdit, CEdit)
	//{{AFX_MSG_MAP(CCustomTreeEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListEdit message handlers

/*============================================================================*/
/*! �c���[���G�f�B�b�g�{�b�N�X

-# Window���b�Z�[�W���f�B�X�p�b�`�O�ɏ�������

@param	pMsg	Window���b�Z�[�W
@retval	BOOL

*/
/*============================================================================*/
BOOL CCustomTreeEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN){
		SHORT sKey = GetKeyState(VK_CONTROL);
		if (pMsg->wParam == VK_RETURN
			|| pMsg->wParam == VK_DELETE
			|| pMsg->wParam == VK_ESCAPE
			|| sKey
			)
		{
			if (pMsg->wParam == VK_RETURN)
				mbKeyReturn = TRUE;
			if (GetKeyState(VK_SHIFT) & 0xff00)
				mbKeyShift = TRUE;
			else
				mbKeyShift = FALSE;

			::TranslateMessage(pMsg);
			if (!(GetStyle() & ES_MULTILINE) || pMsg->wParam != VK_ESCAPE){
				::DispatchMessage(pMsg);
			}
			return TRUE;
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

/*============================================================================*/
/*! ���X�g�R���g���[�����G�f�B�b�g�{�b�N�X

-# ���l�̍ő包����ݒ肷��

@param	len		����
@retval

*/
/*============================================================================*/
void CCustomTreeEdit::SetNumberLimit(UINT len)
{
	mnNumberLimit = len;
}

/*============================================================================*/
/*! �c���[���G�f�B�b�g�{�b�N�X

-# ���̓t�H�[�J�X���������O�ɌĂяo�����

@param pNewWnd	���̓t�H�[�J�X���󂯎��E�B���h�E�ւ̃|�C���^�[
@retval

*/
/*============================================================================*/
void CCustomTreeEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CString str;
	GetWindowText(str);

	if (mbNotify == TRUE)
	{
		return;
	}
	mbNotify = TRUE;

	// ���x���ҏW�̏I����ʒm
	TV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = TVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.hItem = miItem;
	dispinfo.item.pszText = mbESC ? NULL : LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = mbESC ? 0 : str.GetLength();

	GetParent()->SetFocus();
	GetParent()->GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
}

/*============================================================================*/
/*! �c���[���G�f�B�b�g�{�b�N�X

-# �L�[�X�g���[�N�������ɕϊ������Ƃ��Ăяo�����

@param nChar	�L�[�̕����R�[�h
@param nRepCnt	�J��Ԃ���
@param nFlags
@retval

*/
/*============================================================================*/
void CCustomTreeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 0x03 || nChar == 0x16 || nChar == 0x18 || nChar == 0x08)//Ctrl+C; Ctrl+V; Ctrl+X; BackSpace
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if (nChar == VK_ESCAPE)
			mbESC = TRUE;
		GetParent()->SetFocus();
		return;
	}
	else
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

/*============================================================================*/
/*! �c���[���G�f�B�b�g�{�b�N�X

-# �E�B���h�E���쐬�����Ƃ��Ăяo�����

@param lpCreateStruct	�E�B���h�E���쐬���邽�߂̏��
@retval	int

*/
/*============================================================================*/
int CCustomTreeEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont* font = GetParent()->GetFont();
	SetFont(font);

	SetWindowText(msInitText);
	SetFocus();
	//	CalculateSize();
	SetSel(0, -1);

	CString	str;
	GetWindowText(str);
	int length = mnNumberLimit;
	SetLimitText(length);
	return 0;
}


//=============================================================================
// ��CCustomTreeListCtrl
//=============================================================================
CCustomTreeListCtrl::CCustomTreeListCtrl()
{
	myOffset = 0;

	mpEdit = NULL;
	mbInplace = FALSE;

	mbDragDragging = false;
	mhDragItemDrag = NULL;
	mhDragItemDrop = NULL;
	mpDragImagelist = NULL;
	mDragNode = NULL;

	// ����p�t�H���g�̍쐬
	mControlFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (mControlFont.GetLogFont(&lf)){
		lf.lfHeight = mTreeFontHeight;
		lf.lfWeight = FW_BOLD;
		lf.lfUnderline = 1;
		mControlFont.DeleteObject();
		mControlFont.CreateFontIndirect(&lf);
	}

	mDefaultFont.CreateStockObject(DEFAULT_GUI_FONT);
	if (mDefaultFont.GetLogFont(&lf)){
		lf.lfHeight = -mTreeFontHeight;
		lf.lfWeight = FW_NORMAL;
		mDefaultFont.DeleteObject();
		mDefaultFont.CreateFontIndirect(&lf);
	}

	mNodeTitleFont.CreateStockObject(DEFAULT_GUI_FONT);
	if (mNodeTitleFont.GetLogFont(&lf)) {
		lf.lfHeight = -mTreeFontHeight;
		lf.lfWeight = FW_NORMAL;
		mNodeTitleFont.DeleteObject();
		mNodeTitleFont.CreateFontIndirect(&lf);
	}
	mNodeMainFont.CreateStockObject(DEFAULT_GUI_FONT);
	if (mNodeMainFont.GetLogFont(&lf)) {
		lf.lfHeight = -mTreeFontHeight;
		lf.lfWeight = FW_NORMAL;
		mNodeMainFont.DeleteObject();
		mNodeMainFont.CreateFontIndirect(&lf);
	}
	mNodeSubFont.CreateStockObject(DEFAULT_GUI_FONT);
	if (mNodeSubFont.GetLogFont(&lf)) {
		lf.lfHeight = -mTreeFontHeight;
		lf.lfWeight = FW_NORMAL;
		mNodeSubFont.DeleteObject();
		mNodeSubFont.CreateFontIndirect(&lf);
	}
	mNodeLeafFont.CreateStockObject(DEFAULT_GUI_FONT);
	if (mNodeLeafFont.GetLogFont(&lf)) {
		lf.lfHeight = -mTreeFontHeight;
		lf.lfWeight = FW_NORMAL;
		mNodeLeafFont.DeleteObject();
		mNodeLeafFont.CreateFontIndirect(&lf);
	}

	mLastSelectItem = NULL;
}


CCustomTreeListCtrl::~CCustomTreeListCtrl()
{
	if (mpEdit != NULL)
		delete mpEdit;

	mControlFont.DeleteObject();

	mDefaultFont.DeleteObject();
	mNodeTitleFont.DeleteObject();
	mNodeMainFont.DeleteObject();
	mNodeSubFont.DeleteObject();
	mNodeLeafFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CCustomTreeListCtrl, CTreeCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CCustomTreeListCtrl::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CCustomTreeListCtrl::OnTvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, &CCustomTreeListCtrl::OnTvnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CCustomTreeListCtrl::OnTvnEndlabeledit)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CCustomTreeListCtrl::OnNMDblclk)
END_MESSAGE_MAP()


/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �R���g���[���̍쐬

@param	parent	�e�E�B���h�E

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::Create(CWnd* parent)
{
	mTreeParent = parent;

	mHeaderCtrl.Create(WS_CHILD | WS_VISIBLE | HDS_FULLDRAG, CRect(), mTreeParent, eHeaderID);

	CFont temp;
	temp.CreateStockObject(DEFAULT_GUI_FONT);
	mHeaderCtrl.SetFont(&temp);
	temp.DeleteObject();

	// �w�b�_�[�R���g���[���̍��������߂�
	CDC* pDC = GetDC();
	pDC->SelectObject(&mDefaultFont);
	CSize szExt = pDC->GetTextExtent(_T("W"));
	mcyHeader = szExt.cy + 7;// +(IsVersion6 ? 10 : 7);

	ReleaseDC(pDC);

	SetImageList(NULL, TVSIL_NORMAL);

	// �J�����̊J�n�ʒu���當���`��ʒu�̃I�t�Z�b�g�����߂�
	mxOffset = 6;
	mxPos = 0;
	mSelectItems.clear();

	// �w�b�_�[�A�C�e���̍쐬
	for (int i = 0; i < sizeof(mTreeItemHeader) / sizeof(mTreeItemHeader[0]); i++) {
		if (i == 0)
			SetHeaderItem(i, mTreeItemHeader[i], mTreeHeaderItemSize);
		else
			SetHeaderItem(i, mTreeItemHeader[i], mTreeHeaderSize);
	}

	// �A�C�e���C���f���g�̐ݒ�
	SetIndent(5);

	// �J�������̍X�V
	UpdateColumns();
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �E�B���h�E���T�u�N���X������O�ɃT�u�N���X����������ɂ́A�t���[�����[�N�ɂ���ČĂяo����܂�

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::PreSubclassWindow()
{
	CTreeCtrl::PreSubclassWindow();
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# ���O���b�Z�[�W����

@param

@retval
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::PreTranslateMessage(MSG* pMsg)
{
	return CTreeCtrl::PreTranslateMessage(pMsg);
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# ���b�Z�[�W����

@param

@retval
*/
/*============================================================================*/
LRESULT CCustomTreeListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �`��C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnPaint()
{
	CPaintDC dc(this);

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	CBitmap memBitmap;

	// ��Ɨp�r�b�g�}�b�v�̍쐬�i������h�~�j
	memDC.CreateCompatibleDC(&dc);
	if (memBitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height())) {
		CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

		// �������r�b�g�}�b�v�̕`��
		CWnd::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

		// DC�̈�̃R�s�[
		dc.BitBlt(0, 0, rcClient.right, rcClient.bottom, &memDC, 0, 0, SRCCOPY);

		// ��n��
		memDC.SelectObject(pOldBitmap);
		memBitmap.DeleteObject();
	}
	// ��n��
	memDC.DeleteDC();
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �w�i�`��

@param

@retval
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::OnEraseBkgnd(CDC* pDC)
{
	//CRect	rClient;
	//GetClientRect(&rClient);
	//pDC->FillSolidRect(rClient, RGB(255,0,255));
	return TRUE;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �����X�N���[���o�[�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this) {
		SetFocus();
	}
	if (mbInplace) {
		Invalidate(FALSE);
		mbInplace = FALSE;
	}

	CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �}�E�X���{�^���_�E���C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	{
		// �l�Z�����������ꂽ�ꍇ�͑I����Ԃɂ��ĉ������Ȃ�
		HTREEITEM hItem;
		UINT col = 0;
		hItem = HitTestEx(point, col);
		if (hItem == NULL) {
			SetFocus();
			return;
		}
		if (col == eTreeItemSubType_Value) {
			SelectItem(hItem);
			SetFocus();
			return;
		}
	}

#ifdef _ORG
	// Control�L�[���������Ă���ꍇ�́A�����I��
	// Control�L�[���������Ă��Ȃ��ꍇ�́A�P���I�����邢�͖��̕ύX
	if (!(MK_CONTROL & nFlags)/* && !(MK_SHIFT & nFlags)*/) {
		// Control�L�[�͉�����Ԃł͂Ȃ�

		TRACE("*** OnLButtonDown\n");

		CTreeCtrl::OnLButtonDown(nFlags, point);

		HTREEITEM hItem;
		UINT col = 0;
		hItem = HitTestEx(point, col);
		if (hItem == NULL) {
			SetFocus();
			return;
		}
		if (col == eTreeItemSubType_Value) {
			SetFocus();
			return;
		}

		// �擪�J�����ȊO�̃��x���ҏW
		if (hItem != NULL && col != 0) {
			//CString text = GetSubItemText(hItem, col);
			if (cellClick(hItem, col, point) == TRUE) {
				// �ҏW���[�h�֐؂�ւ�
				SelectItem(hItem);
				//SwitchEditMode(hItem, col, point);
				//return;
			}
		}
		//if (hItem != NULL && col == 0) {
		//	SelectItem(hItem);
		//	return;
		//}

		clearSelection();
		SelectItem(hItem);
		SetFocus();
	}
	else {
		SetFocus();
		do
		{
			HTREEITEM hItem;
			UINT col = 0;
			hItem = HitTestEx(point, col);
			if (hItem == NULL) {
				break;
			}
			if (ItemHasChildren(hItem)) {
				break;
			}
			if (GetParentItem(hItem) == NULL) {
				break;
			}
			unsigned short shKeyState = GetKeyState(VK_CONTROL);
			shKeyState >>= 15;
			if (shKeyState == 1) {
				procControlKeyPress(hItem);
				HTREEITEM hSelectedItem = GetSelectedItem();
				if (ItemHasChildren(hSelectedItem)) {
					SelectItem(hItem);
				}
				break;
			}
			else {
				if (mSelectItems.size() == 0) {
					SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
					mSelectItems.push_back(hItem);
					break;
				}
			}
			mLastSelectItem = hItem;
			if (mSelectItems.size() == 1)
			{
				clearSelection();
				SetItemState(mLastSelectItem, TVIS_SELECTED, TVIS_SELECTED);
				mSelectItems.push_back(mLastSelectItem);
			}
		} while (false);
	}
#else
	TRACE("*** OnLButtonDown\n");

	CTreeCtrl::OnLButtonDown(nFlags, point);

	HTREEITEM hItem;
	UINT col = 0;
	hItem = HitTestEx(point, col);
	if (hItem == NULL) {
		SetFocus();
		return;
	}
	if (col == eTreeItemSubType_Value) {
		SetFocus();
		return;
	}

	// �擪�J�����ȊO�̃��x���ҏW
	if (hItem != NULL) {
		// ����C�x���g�����`�F�b�N
		if (cellClick(hItem, col, point) == TRUE) {
		}
		SelectItem(hItem);
		SetFocus();
	}
#endif
}
/*============================================================================*/
/*! �c���[�R���g���[���g���@�\

-# �擪�Z���ȊO�̃Z���N���b�N���̏���

@param		hItem		�c���[�A�C�e��
@param		nSubItem	�c���[�T�u�A�C�e��
@param		point		�}�E�X�J�[�\���ʒu
@retval

*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::cellClick(HTREEITEM hItem, UINT nSubItem, CPoint point)
{
#if _DEMO_PHASE < 100
	return FALSE;
#endif
	CCustomDetail* p = (CCustomDetail*)mTreeParent;
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(p);

	if (pnode->GetWindowInfo().mode != eTreeItemMode_Edit) {
		return FALSE;
	}

	UINT mask = 1 << eTreeItemSubType_Item | 1 << eTreeItemSubType_Unit;
	if ((1 << nSubItem) & mask)
		return TRUE;

	// ����Z�����������ꂽ���`�F�b�N
	bool bControl = IsControl(point);
	if (bControl == false) {
		return FALSE;
	}

	CString strText = GetSubItemText(hItem, eTreeItemSubType_Control);
	// ���䕶����̏ꍇ�͐���R�}���h���s
	if (strText.IsEmpty() == false && strText == CString(mCOntrolSignString)) {
		// ����R�}���h�𑗐M
		pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(p, hItem);
		CString strCntl = pnode->GetMonCtrl().cname;
		p->MessageBox(_T("�i���j����R�}���h�𑗐M���܂�\n") + strCntl);
		return FALSE;
	}
	return TRUE;
}
/*============================================================================*/
/*! �c���[�R���g���[���g���@�\

-# �}�E�X���{�^���_�u���N���b�N�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CTreeCtrl::OnLButtonDblClk(nFlags, point);

	TRACE("*** OnLButtonDblClk\n");
	if (!(MK_CONTROL & nFlags)/* && !(MK_SHIFT & nFlags)*/) {
		// Control�L�[�͉�����Ԃł͂Ȃ�

		HTREEITEM hItem;
		UINT col = 0;
		hItem = HitTestEx(point, col);
		if (hItem == NULL) {
			SetFocus();
			return;
		}
		UINT mask = 1 << eTreeItemSubType_Item | 1 << eTreeItemSubType_Unit;
		if ((1 << col) & mask) {
			if (hItem != NULL /*&& col != 0*/) {
				//CString text = GetSubItemText(hItem, col);
				//if (cellClick(hItem, col, point) == TRUE)
				{
					// �ҏW���[�h�֐؂�ւ�
					SelectItem(hItem);
					SwitchEditMode(hItem, col, point);
					return;
				}
			}
		}
		SelectItem(hItem);
		SetFocus();
	}
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �}�E�X�ړ��C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	HTREEITEM	hItem = 0;
	UINT        flags = 0;
	CPoint		pt = point;
	ClientToScreen(&pt);

#if _DEMO_PHASE < 100
#endif

	if (mbDragDragging){
		// �h���b�O��
		// �h���b�O �A���h �h���b�v���쒆�Ƀh���b�O����Ă���C���[�W���ړ�
		mpDragImagelist->DragMove(pt);

		// �h���b�v�E�B���h�E�̊m�F
		CWnd* pDropWnd = WindowFromPoint(pt);
		BOOL bTarget = FALSE;

		if (pDropWnd == this) {
			// �������玩��
			TRACE("Drag From me to me\n");
		}
		else {
			// �������瑼�l
			TRACE("Drag From me to you\n");
		}

		// �h���b�v�悪�L�������m�F����
		TRACE("=== Drag Target Display : %s\n", CStringA(mDragNode->GetMonCtrl().display));
		vector<CTreeNode*>::iterator itr;
		for (itr = theApp.GetCustomControl().GetDataManager().GetTreeNode().begin(); itr != theApp.GetCustomControl().GetDataManager().GetTreeNode().end(); itr++) {
			if ((*itr)->GetWindowInfo().tree == pDropWnd) {
				if ((*itr)->GetWindowInfo().wnd == NULL)
					break;
				bTarget = ((CCustomDetail*)((*itr)->GetWindowInfo().wnd))->DragDrop_SetSelectTarget(eFromType_Custom, (LPARAM)mDragNode);
				break;
			}
		}

		// �h���b�v��̗L�������ɂ���ă}�E�X�J�[�\����ύX����
		if (bTarget == TRUE) {
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		}
		else {
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
		}

	}
	else{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		UINT col = 0;
		hItem = hitControl(point);
		if (hItem != NULL){
			if (ptInRectPointCell(point) == TRUE){
				SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
			}
		}
	}
	CTreeCtrl::OnMouseMove(nFlags, point);
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �}�E�X���{�^���A�b�v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
#if _DEMO_PHASE < 100
#endif

	if (!(MK_CONTROL & nFlags)/* && !(MK_SHIFT & nFlags)*/){
		if (mSelectItems.size() > 1){
			clearSelection();
			if (mLastSelectItem != NULL) {
				SetItemState(mLastSelectItem, TVIS_SELECTED, TVIS_SELECTED);
				mSelectItems.push_back(mLastSelectItem);
			}
		}
	}

	if (mbDragDragging){
		// �h���b�O��

		// �E�B���h�E�̃��b�N���������A�E�B���h�E���X�V�ł���悤�Ƀh���b�O�C���[�W���\��
		mpDragImagelist->DragLeave(NULL);
		// �h���b�O������I��
		mpDragImagelist->EndDrag();
		// �C���[�W���X�g�̍폜
		delete mpDragImagelist;
		mpDragImagelist = NULL;

		// �h���b�v��̊m�F
		CPoint pt = point;
		ClientToScreen(&pt);
		// �h���b�v�E�B���h�E�̊m�F
		CWnd* pDropWnd = WindowFromPoint(pt);

		bool bMove = false;
		if (pDropWnd == this) {
			// �������玩���Ȃ̂ňړ�����
			bMove = true;
		}
		else {
			// �������瑼�l�Ȃ̂ŃR�s�[����
			bMove = false;
		}

		BOOL bTarget = FALSE;
		vector<CTreeNode*>::iterator itr;
		for (itr = theApp.GetCustomControl().GetDataManager().GetTreeNode().begin(); itr != theApp.GetCustomControl().GetDataManager().GetTreeNode().end(); itr++) {
			if ((*itr)->GetWindowInfo().tree == pDropWnd) {
				if ((*itr)->GetWindowInfo().wnd == NULL)
					break;
				bTarget = ((CCustomDetail*)((*itr)->GetWindowInfo().wnd))->DragDrop_SetSelectTarget(eFromType_Custom, (LPARAM)mDragNode);
				break;
			}
		}
		
		if (bTarget == TRUE) {
			// �h���b�v��̓h���b�v�\�Ȃ̂Ńh���b�v�������s��
			vector<CTreeNode*>::iterator itr;
			for (itr = theApp.GetCustomControl().GetDataManager().GetTreeNode().begin(); itr != theApp.GetCustomControl().GetDataManager().GetTreeNode().end(); itr++) {
				if ((*itr)->GetWindowInfo().tree == pDropWnd) {
					if ((*itr)->GetWindowInfo().wnd == NULL)
						break;
					((CCustomDetail*)((*itr)->GetWindowInfo().wnd))->DragDrop_SetDropTarget(eFromType_Custom, (LPARAM)mDragNode, bMove);
					break;
				}
			}
		}
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		ReleaseCapture();
		mbDragDragging = false;
		SelectDropTarget(NULL);
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �J�X�^���`��

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMCUSTOMDRAW* pNMCustomDraw = (NMCUSTOMDRAW*)pNMHDR;
	NMTVCUSTOMDRAW* pNMTVCustomDraw = (NMTVCUSTOMDRAW*)pNMHDR;

	switch (pNMCustomDraw->dwDrawStage)
	{
	case CDDS_PREPAINT: // �`��O
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT: // ���ڂ̕`��O
	{
		//*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
		HTREEITEM hItem = (HTREEITEM)pNMCustomDraw->dwItemSpec;
		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hItem);
		if (pnode == NULL) {
			// �f�t�H���g�`��
			*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
			break;
		}
		switch (pnode->GetWindowInfo().type) {
		case	eTreeItemType_Window:
		case	eTreeItemType_Title:
			SelectObject(pNMCustomDraw->hdc, mNodeTitleFont);
			*pResult = CDRF_NEWFONT;
			break;
		case	eTreeItemType_Main:
			SelectObject(pNMCustomDraw->hdc, mNodeMainFont);
			*pResult = CDRF_NEWFONT;
			break;
		case	eTreeItemType_Sub:
			SelectObject(pNMCustomDraw->hdc, mNodeSubFont);
			*pResult = CDRF_NEWFONT;
			break;
		default:
			SelectObject(pNMCustomDraw->hdc, mNodeLeafFont);
			*pResult = CDRF_NEWFONT;
			break;
		}
		*pResult = *pResult | CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;

		break;
	}

	case CDDS_ITEMPOSTPAINT:    // ���ڂ̕`���
	{
		HTREEITEM hItem = (HTREEITEM)pNMCustomDraw->dwItemSpec;
		CRect rcItem = pNMCustomDraw->rc;

		CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hItem);
		if (pnode == NULL){
			// �f�t�H���g�`��
			*pResult = CDRF_DODEFAULT;
			break;
		}

		//UINT type = pnode->getWindowInfo().type;

		if (rcItem.IsRectEmpty()){
			// �f�t�H���g�`��
			*pResult = CDRF_DODEFAULT;
			break;
		}

		CDC dc;
		dc.Attach(pNMCustomDraw->hdc);

		switch (pnode->GetWindowInfo().type) {
		case	eTreeItemType_Window:
		case	eTreeItemType_Title:
			dc.SelectObject(mNodeTitleFont);
			break;
		case	eTreeItemType_Main:
			dc.SelectObject(mNodeMainFont);
			break;
		case	eTreeItemType_Sub:
			dc.SelectObject(mNodeSubFont);
			break;
		default:
			dc.SelectObject(mNodeLeafFont);
			break;
		}

		CRect rcLabel;
		GetItemRect(hItem, &rcLabel, TRUE);

		// ���x�����ڂ̓h��Ԃ��i�R���g���[���̔w�i�F�œh��Ԃ��j
		dc.FillSolidRect(&rcItem, GetBkColor());

		int nColsCnt = mHeaderCtrl.GetItemCount();

		//// �����̕`��
		int xOffset = 0;
		//for (int i = 0; i<nColsCnt; i++){
		//	xOffset += mColWidths[i];
		//	rcItem.right = xOffset - 1;
		//	dc.DrawEdge(&rcItem, BDR_SUNKENINNER, BF_RIGHT);
		//}
		//// �c���`��
		//dc.DrawEdge(&rcItem, BDR_SUNKENINNER, BF_BOTTOM);

		CString strText = GetItemText(hItem);
		CString strSub;
		AfxExtractSubString(strSub, strText, eDetailItem, '\t');

		// ���x�����ڂ̃T�C�Y�擾
		CRect rcText(0, 0, 0, 0);
		dc.DrawText(strSub, &rcText, DT_NOPREFIX | DT_CALCRECT);
		rcLabel.right = min(rcLabel.left + rcText.right + 4, mColWidths[0] - 4);

		COLORREF backcolor = pnode->GetColor().textback;
		if (pNMCustomDraw->uItemState & CDIS_SELECTED || GetDropHilightItem() == hItem){
			// �I�����̔w�i�F���n�C���C�g�ɐݒ�
			dc.FillSolidRect(&rcItem, GetSysColor(COLOR_HIGHLIGHT));
			backcolor = GetSysColor(COLOR_HIGHLIGHT);
		}
		else{
			// �e�m�[�h���̔w�i�F�ɐݒ�
			//dc.FillSolidRect(&rcItem, pnode->getColor().textback);
			rcText = rcItem;
			rcText.left = 0;
			rcText.right = mColWidths[0];
			dc.FillSolidRect(&rcText, backcolor);
		}

		// �t�H�[�J�X���̕`��
		//if (pNMCustomDraw->uItemState & CDIS_FOCUS){
		//	dc.DrawFocusRect(&rcLabel);
		//}

		// ���x���̕`��
		dc.SetBkMode(TRANSPARENT);
		rcText = rcLabel;
		rcText.DeflateRect(2, 1);
		dc.SetTextColor(pnode->GetColor().text);
		dc.DrawText(strSub, &rcText, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);

		xOffset = mColWidths[0];

		// ���̑��̃J���������̕`��
		for (int i = 1; i<nColsCnt; i++){
			switch (i){
			case	eTreeItemSubType_Value:
				dc.SetTextColor(pnode->GetColor().value);
				break;
			case	eTreeItemSubType_Unit:
				dc.SetTextColor(pnode->GetColor().unit);
				break;
			default:
				dc.SetTextColor(pnode->GetColor().text);
				break;
			}
			rcText = rcLabel;
			rcText.left = xOffset;
			rcText.right = xOffset + mColWidths[i];
			dc.FillSolidRect(&rcText, backcolor);
			if (AfxExtractSubString(strSub, strText, i, '\t')){
				rcText.DeflateRect(mxOffset, 1, 2, 1);
				if (strSub == CString(mCOntrolSignString)){
					CFont* p = (CFont*)dc.SelectObject(&mControlFont);
					//dc.SetTextColor(RGB(0, 0, 255));
					dc.SetTextColor(calcContrastColor(backcolor));
					dc.DrawText(mCOntrolSignStringDisplay, &rcText, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS);
					dc.SelectObject(p);
				}
				else{
					dc.DrawText(strSub, &rcText, (i == 1) ? DT_SINGLELINE | DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS : DT_SINGLELINE | DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_END_ELLIPSIS);
				}
			}
			xOffset += mColWidths[i];
		}

		// �A�C�e���̈��h��Ԃ��Ă���̂ōŌ�Ɍr����`�悷��i�r���Ȃ��̏ꍇ�͈ȉ����폜����j
		const bool bGrid = true;
		if (bGrid == true){
			// �����̕`��
			rcItem = pNMCustomDraw->rc;
			xOffset = 0;
			for (int i = 0; i<nColsCnt; i++){
				xOffset += mColWidths[i];
				rcItem.right = xOffset - 1;
				dc.DrawEdge(&rcItem, BDR_SUNKENINNER, BF_RIGHT);
			}
			// �c���`��
			dc.DrawEdge(&rcItem, BDR_SUNKENINNER, BF_BOTTOM);
		}

		dc.Detach();

		*pResult = CDRF_DODEFAULT;
		break;
	}

	default:
		*pResult = CDRF_DODEFAULT;
	}
	//*pResult = 0;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# ���x���ҏW�J�n

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
#if _DEMO_PHASE < 100
	*pResult = 1;
	return;
#endif

	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, pTVDispInfo->item.hItem);
	if (pnode != NULL){
		UINT type = pnode->GetWindowInfo().type;
		if (type == eTreeItemType_Item){
			// �}�E�X�ʒu����Z���ꏊ���擾����
			HTREEITEM hItem;
			UINT col = 0;
			CPoint point;
			GetCursorPos(&point);
			ScreenToClient(&point);
			hItem = HitTestEx(point, col);
			// �擪�Z���̏ꍇ�͕ҏW�������s��
			if (hItem != NULL){
				if (col != 0) {
					*pResult = 0;
					return;
				}
				// �擪�Z���̏ꍇ�̓��x���ҏW���s��
				SwitchEditMode(hItem, col, point);
				*pResult = 1;
				return;
			}
		}
	}
	*pResult = 0;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# ���x���ҏW�I��

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, pTVDispInfo->item.hItem);
	if (pnode == NULL){
		if (mpEdit != NULL) {
			delete mpEdit;
			mpEdit = NULL;
		}
		*pResult = 0;
		return;
	}

	UINT type = pnode->GetWindowInfo().type;

	if (!(pTVDispInfo->item.mask & TVIF_TEXT)) {
		if (mpEdit != NULL) {
			delete mpEdit;
			mpEdit = NULL;
		}
		*pResult = 0;
		return;
	}
	if (pTVDispInfo->item.pszText == NULL) {
		if (mpEdit != NULL) {
			delete mpEdit;
			mpEdit = NULL;
		}
		SelectItem(pTVDispInfo->item.hItem);
		SetFocus();
		*pResult = 0;
		return;
	}

	// �擪�A�C�e���̏ꍇ
	if (type == eTreeItemType_Item)
	{
		CString str = pTVDispInfo->item.pszText;
		SetSubItemText(pTVDispInfo->item.hItem, ((CCustomTreeEdit*)mpEdit)->GetSubItem(), str);
		switch (((CCustomTreeEdit*)mpEdit)->GetSubItem()){
		case	eDetailItem:
			swprintf_s(pnode->GetMonCtrl().display, mNameSize, _T("%s"), (LPCTSTR)GetSubItemText(pTVDispInfo->item.hItem, eDetailItem));
			break;
		case	eDetailUnit:
			swprintf_s(pnode->GetMonCtrl().unit, mUnitSize, _T("%s"), (LPCTSTR)GetSubItemText(pTVDispInfo->item.hItem, eDetailUnit));
			break;
		}
		if (mpEdit != NULL){
			delete mpEdit;
			mpEdit = NULL;
		}
		SelectItem(pTVDispInfo->item.hItem);
		SetFocus();
		*pResult = 0;
		return;
	}

	// �擪�A�C�e���ȊO�̏ꍇ
	swprintf_s(pnode->GetMonCtrl().display, mNameSize, _T("%s"), pTVDispInfo->item.pszText);
	SetItemText(pTVDispInfo->item.hItem, pTVDispInfo->item.pszText);
	Invalidate(FALSE);
	if (type == eTreeItemType_Title)
	{
		if (mTreeParent != NULL) {
			mTreeParent->SendMessage(eUserMessage_Manager_Update, 0, (LPARAM)pTVDispInfo->item.hItem);
		}
	}
	if (mpEdit != NULL) {
		delete mpEdit;
		mpEdit = NULL;
	}
	SelectItem(pTVDispInfo->item.hItem);
	SetFocus();
	*pResult = 0;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �h���b�O�����̊J�n

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;

#if _DEMO_PHASE < 100
#endif

	CPoint      ptAction;
	UINT        nFlags;

	// �h���b�O�A�C�e���̎擾
	GetCursorPos(&ptAction);
	ScreenToClient(&ptAction);
	mhDragItemDrag = HitTest(ptAction, &nFlags);
	mhDragItemDrop = NULL;

	// �h���b�O�L���`�F�b�N
	if (enableDragItem(mhDragItemDrag) == false) {
		return;
	}
	mbDragDragging = true;

	CPoint point;
	// �h���b�O�C���[�W�̍쐬
	mpDragImagelist = createDragImageEx(mhDragItemDrag);
	//mpImagelist = CreateDragImage(mhItemDrag);
	// �E�B���h�E�����b�N�����ɁA�h���b�O���쒆�Ƀh���b�O�C���[�W��\��
	mpDragImagelist->DragShowNolock(TRUE);
	// �V�����h���b�O �C���[�W���쐬
	mpDragImagelist->SetDragCursorImage(0, CPoint(0, 0));
	// �C���[�W�̃h���b�O���J�n
	mpDragImagelist->BeginDrag(0, CPoint(0, 0));
	// �h���b�O �A���h �h���b�v���쒆�Ƀh���b�O����Ă���C���[�W���ړ�
	//mpImagelist->DragMove(ptAction);
	// �h���b�O���쒆�ɍX�V�����b�N���A�w�肵���ʒu�Ƀh���b�O �C���[�W��\��
	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen(&pt);
	mpDragImagelist->DragEnter(NULL, pt);
	// �h���b�O���ꂽ�m�[�h���擾
	mDragNode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, mhDragItemDrag);

	SetCapture();
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �h���b�O�̗L�����m�F

@param		hItem		�c���[�A�C�e��
@retval

*/
/*============================================================================*/
bool CCustomTreeListCtrl::enableDragItem(HTREEITEM hItem)
{
	CTreeNode* pnode;
	BOOL bDropExecute = FALSE;

	pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hItem);
	if (pnode != NULL && pnode->GetWindowInfo().type == eTreeItemType_Item)
		return TRUE;

#if _DEMO_PHASE < 100
	return FALSE;
#endif

	if (pnode != NULL && pnode->GetWindowInfo().type == eTreeItemType_Main)
		return TRUE;
	if (pnode != NULL && pnode->GetWindowInfo().type == eTreeItemType_Sub)
		return TRUE;

	return FALSE;
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �c���[���ڂ̊J���̃C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	TRACE("*** OnNMDblclk\n");
	// �C�x���g�𖳌��ɂ���
	*pResult = 1; // �c���[�̊J��L���ɂ���̂ɂ͂O��ݒ肷��
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �T�u�A�C�e���e�L�X�g�̎擾

@param	hItem	�A�C�e���̃n���h��
@param	col		�T�u�A�C�e��

@retval	int	�A�C�e��
*/
/*============================================================================*/
CString CCustomTreeListCtrl::GetSubItemText(HTREEITEM hItem, int col)
{
	CString str;
	str = GetItemText(hItem);
	vector<CString>	list;
	vector<CString>::iterator itr;

	extractSubString(str, list);

	str = _T("");
	if (list.size() > (UINT)col) {
		str = list[col];
	}
	return str;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �T�u�A�C�e���e�L�X�g�̐ݒ�

@param	hItem	�A�C�e���̃n���h��
@param	col		�T�u�A�C�e��

@retval	int	�A�C�e��
*/
/*============================================================================*/
void CCustomTreeListCtrl::SetSubItemText(HTREEITEM hItem, int col, CString strText)
{
	CString str;
	str = GetItemText(hItem);
	vector<CString>	list;
	vector<CString>::iterator itr;

	extractSubString(str, list);

	if (list.size() <= (UINT)col)
		return;

	list[col] = strText;

	str.Empty();
	for (itr = list.begin(); itr != list.end(); itr++){
		if (str.IsEmpty() == false){
			str += '\t';
		}
		str += (*itr);
	}

	SetItemText(hItem, str);
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �}�E�X�ʒu����A�C�e���A�T�u�C�e���ʒu�����߂�

@param	point	�}�E�X�ʒu
@param	col		�T�u�A�C�e��

@retval	int	�A�C�e��
*/
/*============================================================================*/
HTREEITEM CCustomTreeListCtrl::HitTestEx(CPoint &point, UINT& col)
{
	int colnum = 0;
	HTREEITEM hItem = HitTest(point, NULL);
	col = 0;

	// �w�b�_�[�J�������̎擾
	int nColumnCount = mHeaderCtrl.GetItemCount();

	CRect rectClient, rect;
	GetClientRect(rectClient);
	GetItemRect(hItem, &rect, FALSE);
	rect.right = rectClient.right;
	if (rect.PtInRect(point) == FALSE) {
		return NULL;
	}

	// �}�E�X�ʒu�̃J�����ʒu���擾����
	for (colnum = 0; colnum < nColumnCount; colnum++) {
		int colwidth = mColWidths[colnum];
		if (point.x >= rect.left && point.x <= (rect.left + colwidth)) {
			col = colnum;
			return hItem;
		}
		rect.left += colwidth;
	}
	return NULL;
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �}�E�X�|�C���^�����䕶����ɂ��邩�̃`�F�b�N

@param  point   �}�E�X�ʒu

@retval ����I�����Ƀc���[�A�C�e����Ԃ�
*/
/*============================================================================*/
bool CCustomTreeListCtrl::IsControl(CPoint point)
{
	HTREEITEM hItem = hitControl(point);
	if (hItem != NULL){
		if (ptInRectPointCell(point) == TRUE){
			return true;
		}
	}
	return false;
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �R���g���[���̃��T�C�Y

@param	parent	�e�E�B���h�E

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::ResizeControl(int dx, int dy)
{
	CRect rect;

	mHeaderCtrl.GetWindowRect(rect);
	mTreeParent->ScreenToClient(rect);
	rect.right += dx;
	mHeaderCtrl.MoveWindow(rect);
	mHeaderCtrl.Invalidate();

	GetWindowRect(rect);
	mTreeParent->ScreenToClient(rect);
	rect.right += dx;
	rect.bottom += dy;
	MoveWindow(rect);
	Invalidate();

	UpdateScroller();
	RepositionControls();
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �w�b�_�[�A�C�e���Ԃ̃}�E�X�_�u���N���b�N

@param	item	�A�C�e���ԍ�

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::DividerDblClick(int item)
{
	AdjustColumnWidth(item, FALSE);
	Invalidate();
	UpdateWindow();
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �w�b�_�[���̓o�^

@param	parent	�e�E�B���h�E

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::SetHeaderItem(int item, CString title, int width)
{
	HDITEM hditem;
	hditem.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
	hditem.fmt = HDF_CENTER | HDF_STRING/* | HDF_OWNERDRAW*/;
	hditem.cxy = width;
	hditem.pszText = (LPWSTR)(LPCTSTR)title;
	mHeaderCtrl.InsertItem(item, &hditem);
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �J�������̍X�V

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomTreeListCtrl::UpdateColumns()
{
	mcxTotal = 0;

	HDITEM hditem;
	hditem.mask = HDI_WIDTH;
	int nCnt = mHeaderCtrl.GetItemCount();

	// �w�b�_�[�J���������i�[����̈�̊m��
	mColWidths.clear();
	mColWidths.resize(nCnt);

	// �w�b�_�[�R���g���[���̃J�������̎擾
	for (int i = 0; i<nCnt; i++) {
		if (mHeaderCtrl.GetItem(i, &hditem)) {
			mColWidths[i] = hditem.cxy;
			mcxTotal += hditem.cxy;
			if (i == 0)
				mcxFirstCol = hditem.cxy;
		}
	}

	UpdateScroller();
	RepositionControls();
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��
-# �e�R���g���[���̍Ĕz�u

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomTreeListCtrl::RepositionControls()
{
	// �e�q�R���g���[���̍Ĕz�u���s��
	if (m_hWnd) {
		CRect rcClient;
		mTreeParent->GetClientRect(&rcClient);
		int cx = rcClient.Width();
		int cy = rcClient.Height();

		// ���X�N���[���ʒu����̔z�u
		int x = 0;
		if (cx < mcxTotal) {
			x = mTreeParent->GetScrollPos(SB_HORZ);
			cx += x;
		}
		mHeaderCtrl.MoveWindow(-x, 0 + myOffset, cx, mcyHeader);
		MoveWindow(-x, mcyHeader + myOffset, cx, cy - (mcyHeader + myOffset));
	}
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �J�������̒���

@param	nColumn				�������s���J�����ԍ�
@param	bIgnoreCollapsed	���Ă���A�C�e���𖳎����邩�̃t���O

@retval void
*/
/*============================================================================*/
void CCustomTreeListCtrl::AdjustColumnWidth(int nColumn, BOOL bIgnoreCollapsed)
{
	int nMaxWidth = getMaxColumnWidth(GetRootItem(), nColumn, 0, bIgnoreCollapsed);

	HDITEM hditem;
	hditem.mask = HDI_WIDTH;
	mHeaderCtrl.GetItem(nColumn, &hditem);
	hditem.cxy = nMaxWidth + 25;
	mHeaderCtrl.SetItem(nColumn, &hditem);
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �w�b�_�[���̐����T�C�Y�擾

@param	col		�w�b�_�[�ʒu�i-1�̏ꍇ�S�āj

@retval	int		�T�C�Y
*/
/*============================================================================*/
int CCustomTreeListCtrl::GetHeaderWidth(int col/*=-1*/)
{
	HDITEM hditem;
	int size = 0;
	// �w�b�_�[�R���g���[���̃J�������̎擾
	hditem.mask = HDI_WIDTH;
	int nCnt = mHeaderCtrl.GetItemCount();
	for (int i = 0; i<nCnt; i++) {
		if (mHeaderCtrl.GetItem(i, &hditem)) {
			mColWidths[i] = hditem.cxy;
			if (col == i || col < 0){
				size += hditem.cxy;
			}
		}
	}

	return size;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �c���[�̃A�C�e����ҏW���[�h�֐؂�ւ���

@param	hItem		�c���[�A�C�e���n���h��
@param	col			�J�����ԍ�
@param	place		�N���b�N�ꏊ

@retval	BOOL		TRUE�F�ҏW���[�h�AFALSE�F�ҏW���[�h��
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::SwitchEditMode(HTREEITEM hItem, UINT col, CPoint point)
{
#if _DEMO_PHASE < 100
	SetFocus();
	return FALSE;
#endif
	if (hItem == NULL)
		return FALSE;

	CEdit* pedit = editSubLabel(hItem, col);
	if (pedit == NULL){
		SetFocus();
		return FALSE;
	}

	return TRUE;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �X�N���[���o�[�̍X�V

@param  �Ȃ�

@retval �Ȃ�
*/
/*============================================================================*/
void CCustomTreeListCtrl::UpdateScroller()
{
	CRect rcClient;
	mTreeParent->GetClientRect(&rcClient);
	int cx = rcClient.Width();

	int lx = mxPos;

	if (mxPos > mcxTotal - cx)
		mxPos = mcxTotal - cx;
	if (mxPos < 0)
		mxPos = 0;

	SCROLLINFO scrinfo;
	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
	scrinfo.nPage = cx;
	scrinfo.nMin = 0;
	scrinfo.nMax = mcxTotal;
	scrinfo.nPos = mxPos;
	mTreeParent->SetScrollInfo(SB_HORZ, &scrinfo);
}

/*============================================================================*/
/*! �c���[�R���g���[���g���@�\

-# Ctrl�L�[�������̑I������

@param	hItem	�c���[�I���A�C�e��

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::procControlKeyPress(HTREEITEM hCurItem)
{
	if (mSelectItems.size() > 0)
	{
		if (!isSameLevel(hCurItem)){
			SelectItem(hCurItem);
			clearSelection();
			return;
		}
	}

	int nState = (TVIS_SELECTED == GetItemState(hCurItem, TVIS_SELECTED)) ? 0 : TVIS_SELECTED;
	SetItemState(hCurItem, nState, TVIS_SELECTED);
	if (0 == nState)
	{
		removeFromSelectList(hCurItem);
	}
	else
	{
		mSelectItems.push_back(hCurItem);
	}
}

/*============================================================================*/
/*! �c���[�R���g���[���g���@�\

-# Shift�L�[�������̑I������

@param	hItem	�c���[�I���A�C�e��

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::procShiftKeyPress(HTREEITEM hCurItem)
{
	if (mSelectItems.size() > 0)
	{
		if (!isSameLevel(hCurItem)){
			return;
		}
	}
	HTREEITEM hItemFrom = mSelectItems[0];
	SetItemState(hCurItem, TVIS_SELECTED, TVIS_SELECTED);
	//SelectItems(hItemFrom, hCurItem);
}

/*============================================================================*/
/*! �c���[�R���g���[���g���@�\

-# Ctrl�L�[�������̑I������

@param	hItem	�c���[�I���A�C�e��

@retval
*/
/*============================================================================*/
bool CCustomTreeListCtrl::isSameLevel(HTREEITEM hItem)
{
	bool bSameLevel = true;
	vector<HTREEITEM>::iterator itr;
	for (itr = mSelectItems.begin(); itr != mSelectItems.end(); ++itr)
	{
		if (GetParentItem(hItem) != GetParentItem(*itr))
		{
			bSameLevel = false;
		}
	}
	return bSameLevel;
}

/*============================================================================*/
/*! �c���[�R���g���[���g���@�\

-# Ctrl�L�[�������̑I������

@param	hItem	�c���[�I���A�C�e��

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::removeFromSelectList(HTREEITEM hItem)
{
	vector<HTREEITEM>::iterator itr;
	for (itr = mSelectItems.begin(); mSelectItems.end() != itr; ++itr)
	{
		if ((*itr) == hItem){
			mSelectItems.erase(itr);
			//mSelectItems.clear();
			break;
		}
	}
}

/*============================================================================*/
/*! �c���[�R���g���[���g���@�\

-# �I�����X�g�̃N���A

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::clearSelection()
{
	int nSelItemCount = (int)mSelectItems.size();
	for (int nIdx = 0; nIdx < nSelItemCount; ++nIdx)
	{
		SetItemState(mSelectItems[nIdx], 0, TVIS_SELECTED);
	}
	mSelectItems.clear();
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# ���[�t�̍X�V

@param	hTargetItem			�h���b�v�A�C�e��
@param	pDataObject			�h���b�O���

@retval BOOL
*/
/*============================================================================*/
CString CCustomTreeListCtrl::createDragString(HTREEITEM hDragItem)
{
	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchItemNode(mTreeParent, hDragItem);
	if (pnode == NULL)
		return _T("");

	CString str;
	str.Format(_T("%s\t%s\t%s\t%s"), pnode->GetMonCtrl().display, pnode->GetMonCtrl().unit, pnode->GetMonCtrl().mname, pnode->GetMonCtrl().cname);

	return str;
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �����I��

@param

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::selectMultiItem(HTREEITEM hClickedItem, UINT nFlags)
{
	if(nFlags & MK_CONTROL){
		// ���݂̑I���A�C�e�����擾
		HTREEITEM hSelectedItem = GetSelectedItem();

		// �I���A�C�e���A�N���b�N�A�C�e���̏�Ԏ擾
		BOOL bIsClickedItemSelected = GetItemState(hClickedItem, TVIS_SELECTED) & TVIS_SELECTED;
		BOOL bIsSelectedItemSelected = GetItemState(hSelectedItem, TVIS_SELECTED) & TVIS_SELECTED;

		// �N���b�N�A�C�e����I������
		SelectItem(hClickedItem);

		// �I������Ă���A�C�e���̏�Ԃ𕜌�����
		if (bIsSelectedItemSelected)
			SetItemState(hSelectedItem, TVIS_SELECTED, TVIS_SELECTED);

		// �N���b�N�A�C�e������ԂőI���A��I����ݒ肷��
		if (bIsClickedItemSelected)
			SetItemState(hClickedItem, 0, TVIS_SELECTED);
		else
			SetItemState(hClickedItem, TVIS_SELECTED, TVIS_SELECTED);
	}
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �T�u�A�C�e���ʒu��CMFCMaskedEdit��\������

@param	hItem		�A�C�e���̃n���h��
@param	col			�T�u�A�C�e��
@param	val			�l
@param	type		���ʎq

@retval	CComboBox
*/
/*============================================================================*/
CEdit* CCustomTreeListCtrl::editSubLabel(HTREEITEM hItem, int col)
{
	CString text = GetSubItemText(hItem, col);

	//if (text.IsEmpty() == true)
	//	return NULL;

	// �w�b�_�[�J�������̎擾
	int nColumnCount = mHeaderCtrl.GetItemCount();
	if (col >= nColumnCount)
		return NULL;

	int offset = 0;
	for (int i = 0; i < col; i++)
		offset += mColWidths[i];

	CRect rect;
	GetItemRect(hItem, &rect, FALSE);

	CRect rcClient;
	GetClientRect(&rcClient);
	if (offset + rect.left < 0 || offset + rect.left > rcClient.right){
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		rect.left -= size.cx;
	}

	DWORD dwStyle;
	dwStyle = ES_LEFT;

	rect.left += offset + 4;
	rect.right = rect.left + mColWidths[col] - 3;
	if (rect.right > rcClient.right)
		rect.right = rcClient.right;

	dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL/*|ES_RIGHT*/;
	if (mpEdit != NULL){
		mpEdit->SendMessage(WM_KILLFOCUS);
		delete mpEdit;
		mpEdit = NULL;
	}

#define IDC_EDITCTRL 0x1234
	mpEdit = new CCustomTreeEdit(hItem, col, text);
	mpEdit->Create(dwStyle, rect, this, IDC_EDITCTRL);
	SelectItem(NULL); // ��������Ȃ���KillFocus�ŕҏW���I������

	return mpEdit;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �h���b�O�C���[�W�̍쐬

@param

@retval
*/
/*============================================================================*/
CImageList* CCustomTreeListCtrl::createDragImageEx(HTREEITEM hItem)
{
	if (GetImageList(TVSIL_NORMAL) != NULL)
		return CreateDragImage(hItem);

	CRect rect, rc;
	GetItemRect(hItem, rect, TRUE);
	rc = rect;
	rect.top = rect.left = 0;
	rect.right = GetHeaderWidth();// rc.Width();
	rect.bottom = rc.Height();

	// �r�b�g�}�b�v�̍쐬
	CClientDC	dc(this);
	CDC 		memDC;

	if (!memDC.CreateCompatibleDC(&dc))
		return NULL;

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	CFont* pOldFont = memDC.SelectObject(GetFont());

	memDC.FillSolidRect(&rect, mDragBackColor);
	//memDC.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	//memDC.TextOut(rect.left, rect.top, GetItemText(hItem));
	memDC.SetTextColor(mDragTextColor);
	CString strSub;
	AfxExtractSubString(strSub, GetItemText(hItem), eDetailItem, '\t');
	memDC.DrawText(strSub, rect, DT_LEFT | DT_TOP);

	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldMemDCBitmap);

	// �C���[�W���X�g�̍쐬
	CImageList* pImageList = new CImageList;
	pImageList->Create(rect.Width(), rect.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pImageList->Add(&bitmap, RGB(0, 255, 0));

	return pImageList;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �}�E�X�|�C���^�����䕶����ɂ��邩�̃`�F�b�N

@param  point   �}�E�X�ʒu

@retval ����I�����Ƀc���[�A�C�e����Ԃ�
*/
/*============================================================================*/
HTREEITEM CCustomTreeListCtrl::hitControl(CPoint point)
{
	int colnum = 0;
	UINT col = 0;
	HTREEITEM hItem = HitTestEx(point, col);
	if (hItem == NULL)
		return NULL;

	CString strText = GetItemText(hItem);
	int nColsCnt = mHeaderCtrl.GetItemCount();
	for (int i = 1; i < nColsCnt; i++){
		CString strSub;
		if (AfxExtractSubString(strSub, strText, i, '\t')){
			if (strSub == CString(mCOntrolSignString) && col == i){
				return hItem;
			}
		}
	}
	return NULL;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �}�E�X�|�C���^���Z�����̕������ɂ��邩�̃`�F�b�N

@param  point   �}�E�X�ʒu

@retval bool
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::ptInRectPointCell(CPoint point)
{
	int colnum = 0;
	UINT col = 0;
	HTREEITEM hItem = HitTestEx(point, col);
	if (hItem == NULL)
		return FALSE;

	CString strText = GetItemText(hItem);
	int nColsCnt = mHeaderCtrl.GetItemCount();
	CString cellText = _T("");
	for (int i = 1; i < nColsCnt; i++){
		CString strSub;
		if (AfxExtractSubString(strSub, strText, i, '\t')){
			if (i == col){
				cellText = strSub;
				if (strSub == CString(mCOntrolSignString)){
					cellText = mCOntrolSignStringDisplay;
				}
				break;
			}
		}
	}
	if (cellText.IsEmpty())
		return FALSE;

	CDC* pDC = GetDC();
	CFont* pOldFont = pDC->SelectObject(GetFont());
	CSize size = pDC->GetTextExtent(cellText);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	CRect rect;
	if (getColumnsRect(hItem, col, rect) == false)
		return FALSE;

	HDITEM hditem = { 0 };
	hditem.mask = HDI_FORMAT;
	mHeaderCtrl.GetItem(col, &hditem);

	if (hditem.fmt & HDF_CENTER)
		rect.DeflateRect((rect.Width() - size.cx) / 2, 0);
	else if (hditem.fmt & HDF_RIGHT)
		rect.left = rect.right - size.cx;
	else
		rect.right = rect.left + size.cx;

	return rect.PtInRect(point);
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �w��J������`�̎擾

@param	hItem	�A�C�e��
@param	col		�T�u�A�C�e��
@param	rect	�T�u�A�C�e����`

@retval	bool
*/
/*============================================================================*/
bool CCustomTreeListCtrl::getColumnsRect(HTREEITEM hItem, UINT col, CRect& rect)
{
	if (hItem == NULL){
		return false;
	}

	// �w�b�_�[�J�������̎擾
	int nColumnCount = mHeaderCtrl.GetItemCount();

	CRect rectClient;
	GetClientRect(rectClient);
	GetItemRect(hItem, &rect, FALSE);
	rect.right = rectClient.right;
	for (int colnum = 0; colnum < nColumnCount; colnum++) {
		int colwidth = mColWidths[colnum];
		if (colnum == col) {
			rect.right = rect.left + colwidth;
			return true;
		}
		rect.left += colwidth;
	}
	return false;
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �J�������̂̎擾

@param	hItem				�c���[�A�C�e��
@param	nColumn				�������s���J�����ԍ�
@param	nDepth				�c���[���x���̐[��
@param	bIgnoreCollapsed	���Ă���A�C�e���𖳎����邩�̃t���O

@retval void
*/
/*============================================================================*/
int CCustomTreeListCtrl::getMaxColumnWidth(HTREEITEM hItem, int nColumn, int nDepth, BOOL bIgnoreCollapsed)
{
	int nMaxWidth = 0;

	CString strText = GetItemText(hItem);
	CString strSub;
	if (AfxExtractSubString(strSub, strText, nColumn, '\t')){
		CDC dc;
		dc.CreateCompatibleDC(NULL);
		CFont* pOldFont = dc.SelectObject(GetFont());
		// �����񕝂��t�H���g�A����������v�Z
		nMaxWidth = dc.GetTextExtent(strSub, strSub.GetLength()).cx;
		dc.SelectObject(pOldFont);
		dc.DeleteDC();
	}

	// �C���f���g���l������
	if (nColumn == 0){
		int nIndent = nDepth;
		if (GetWindowLong(m_hWnd, GWL_STYLE) & TVS_LINESATROOT)
			nIndent++;

		int nImage, nSelImage;
		GetItemImage(hItem, nImage, nSelImage);
		if (nImage >= 0)
			nIndent++;

		nMaxWidth += nIndent * GetIndent();
	}

	if (!bIgnoreCollapsed || (GetItemState(hItem, TVIS_EXPANDED) & TVIS_EXPANDED)){
		// �q���x���������Z
		HTREEITEM hSubItem = GetChildItem(hItem);
		while (hSubItem){
			int nSubWidth = getMaxColumnWidth(hSubItem, nColumn, nDepth + 1, bIgnoreCollapsed);
			if (nSubWidth > nMaxWidth)
				nMaxWidth = nSubWidth;
			hSubItem = GetNextSiblingItem(hSubItem);
		}
	}

	return nMaxWidth;
}

#if _DEMO_PHASE >= 100
/*============================================================================*/
/*! �ݔ��ڍ�

-# �R�[���o�b�N�֐�

@param
@retval

*/
/*============================================================================*/
DROPEFFECT CALLBACK CCustomTreeListCtrl::Callback_DragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return DROPEFFECT_COPY;
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �R�[���o�b�N�֐�

@param
@retval

*/
/*============================================================================*/
DROPEFFECT CALLBACK CCustomTreeListCtrl::Callback_DragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CCustomTreeListCtrl* ptree = (CCustomTreeListCtrl*)pWnd;

	CPoint pt = CPoint(point);
	// �h���b�v�ʒu��񂩂�Ώۂ̃A�C�e�����擾����
	HTREEITEM hItem = ptree->HitTest(pt);
	if (hItem == NULL){
		ptree->SelectDropTarget(NULL);
	}
	else{
		CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(ptree->GetParent(), hItem);
		if (pnode == NULL){
			ptree->SelectDropTarget(NULL);
		}
		else{
			ptree->Expand(hItem, TVE_EXPAND);
			UINT type = pnode->getWindowInfo().type;
			switch (type){
			case	eTreeItemType_Sub:
				ptree->SelectDropTarget(hItem);
				break;
			case	eTreeItemType_Item:
				ptree->SelectDropTarget(hItem);
				break;
			default:
				ptree->SelectDropTarget(NULL);
			}
		}
	}

	return DROPEFFECT_COPY;
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �R�[���o�b�N�֐�

@param
@retval

*/
/*============================================================================*/
void CALLBACK CCustomTreeListCtrl::Callback_DragLeave(CWnd* pWnd)
{
	CCustomTreeListCtrl* ptree = (CCustomTreeListCtrl*)pWnd;
	ptree->SendMessage(TVM_SELECTITEM, TVGN_DROPHILITE, 0);
}
/*============================================================================*/
/*! �ݔ��ڍ�

-# �R�[���o�b�N�֐��i�h���b�v�C�x���g�j

@param
@retval

*/
/*============================================================================*/
BOOL CALLBACK CCustomTreeListCtrl::Callback_DragDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	CCustomTreeListCtrl* ptree = (CCustomTreeListCtrl*)pWnd;

	CPoint pt = CPoint(point);
	HTREEITEM hItem = ptree->HitTest(pt);
	if (hItem == NULL){
		return FALSE;
	}
	CTreeNode* pnode = theApp.GetDataManager().SearchItemNode(ptree->GetParent(), hItem);
	if (pnode == NULL){
		return FALSE;
	}

	// �h���b�v��ɂ���ď����𕪂���
	BOOL ret = FALSE;
	UINT type = pnode->getWindowInfo().type;
	if (type == eTreeItemType_Sub){
		ret = ptree->AddLeaf(hItem, pnode, point, pDataObject);
	}
	if (type == eTreeItemType_Item){
		ret = ptree->UpdateLeaf(hItem, pnode, point, pDataObject);
	}
	ptree->SelectDropTarget(NULL);

	return ret;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# ���[�t�̓o�^

@param	hTargetItem			�h���b�v�A�C�e��
@param	pDataObject			�h���b�O���

@retval BOOL
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::AddLeaf(HTREEITEM hTargetItem, CTreeNode* pnode, CPoint point, COleDataObject* pDataObject)
{
	stDragData data;
	BOOL ret = CreateDragData(hTargetItem, pnode, point, pDataObject, data);
	if (ret == FALSE)
		return FALSE;

	((CCustomDetail*)mTreeParent)->DropAddLeaf(hTargetItem, pnode, &data);

	return TRUE;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# ���[�t�̍X�V

@param	hTargetItem			�h���b�v�A�C�e��
@param	pDataObject			�h���b�O���

@retval BOOL
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::UpdateLeaf(HTREEITEM hTargetItem, CTreeNode* pnode, CPoint point, COleDataObject* pDataObject)
{
	stDragData data;
	BOOL ret = CreateDragData(hTargetItem, pnode, point, pDataObject, data);
	if (ret == FALSE)
		return FALSE;

	if (data.indexes.size() != 1)
		return FALSE;

	((CCustomDetail*)mTreeParent)->DropUpdateLeaf(hTargetItem, pnode, &data);

	return TRUE;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# ���[�t�̍X�V

@param	hTargetItem			�h���b�v�A�C�e��
@param	pDataObject			�h���b�O���

@retval BOOL
*/
/*============================================================================*/
BOOL CCustomTreeListCtrl::CreateDragData(HTREEITEM hTargetItem, CTreeNode* pnode, CPoint point, COleDataObject* pDataObject, stDragData& data)
{
	data.point.x = point.x;
	data.point.y = point.y;

	std::vector<CString>::iterator itr;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CF_DSPTEXT);
	CString dragText = static_cast<LPCTSTR>(::GlobalLock(hGlobal));

	CString str;
	int index = 0;
	// �h���b�O��ʎ擾
	AfxExtractSubString(str, dragText, index++, '\t');
	BYTE type = *((BYTE*)str.GetBuffer());
	data.type = (UINT)type;

	if (type != eFromType_Mon && type != eFromType_Cntl)
		return FALSE;

	vector<CString>& list = (type == eFromType_Mon) ? theApp.GetDataManager().GetDataMonitor().GetEqList() : theApp.GetDataManager().GetDataControl().GetEqList();

	while (AfxExtractSubString(str, dragText, index++, '\t')) {
		if (str.IsEmpty())
			continue;
		itr = std::find(list.begin(), list.end(), str);
		int item = (int)std::distance(list.begin(), itr);
		data.indexes.push_back(item);
	}

	return TRUE;
}
#endif

/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �t�H���g�̎擾

@param	type	�m�[�h���

@retval CFont
*/
/*============================================================================*/
CFont& CCustomTreeListCtrl::GetFontEx(UINT type)
{
	switch (type) {
	case	eTreeItemType_Title:
		return mNodeTitleFont;
		break;
	case	eTreeItemType_Main:
		return mNodeMainFont;
		break;
	case	eTreeItemType_Sub:
		return mNodeSubFont;
		break;
	case	eTreeItemType_Item:
		return mNodeLeafFont;
		break;
	}
	return mDefaultFont;
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

-# �t�H���g�̐ݒ�

@param	type	�m�[�h���
@param	lf		�_���t�H���g

@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::SetFontEx(UINT type, LOGFONT& lf)
{
	switch (type) {
	case	eTreeItemType_Window:
		mDefaultFont.DeleteObject();
		mDefaultFont.CreateFontIndirect(&lf);
		SetFont(&mDefaultFont);
		break;
	case	eTreeItemType_Title:
		mNodeTitleFont.DeleteObject();
		mNodeTitleFont.CreateFontIndirect(&lf);
		break;
	case	eTreeItemType_Main:
		mNodeMainFont.DeleteObject();
		mNodeMainFont.CreateFontIndirect(&lf);
		break;
	case	eTreeItemType_Sub:
		mNodeSubFont.DeleteObject();
		mNodeSubFont.CreateFontIndirect(&lf);
		break;
	case	eTreeItemType_Item:
		mNodeLeafFont.DeleteObject();
		mNodeLeafFont.CreateFontIndirect(&lf);
		break;
	}
}
/*============================================================================*/
/*! �c���[���X�g�R���g���[��

	-# �c���[ �r���[ �A�C�e���̑S�J

	@param	hItem		�c���[�A�C�e���n���h��

	@retval
*/
/*============================================================================*/
void CCustomTreeListCtrl::ExpandAll(HTREEITEM hItem/*=NULL*/)
{
	if (hItem == NULL){
		hItem = GetRootItem();
	}
	Expand(hItem, TVE_EXPAND);
	HTREEITEM hNextItem = GetChildItem(hItem);

	while (hNextItem != NULL) {
		Expand(hNextItem, TVE_EXPAND);
		ExpandAll(hNextItem);
		hNextItem = GetNextItem(hNextItem, TVGN_NEXT);
	}
}
