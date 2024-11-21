#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomMonCntlListCtrl.h"

CCustomMonCntlListCtrl::CCustomMonCntlListCtrl()
{
	mcDragBackColor = GetSysColor(COLOR_WINDOW);
	mcDragTextColor = GetSysColor(COLOR_WINDOWTEXT);
}

CCustomMonCntlListCtrl::~CCustomMonCntlListCtrl()
{
}

BEGIN_MESSAGE_MAP(CCustomMonCntlListCtrl, CListCtrl)
#if _DEMO_PHASE >= 50
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CCustomMonCntlListCtrl::OnLvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
#endif
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

#if _DEMO_PHASE >= 50
/*============================================================================*/
/*! ���X�g�R���g���[��

-# �R�[���o�b�N�֐��F�h���b�O�I�[�o�[����

@param

@retval
*/
/*============================================================================*/
DROPEFFECT CALLBACK CCustomMonCntlListCtrl::Callback_MonCntl_DragOver(CWnd* pWnd, void* pDataObject, UINT dwKeyState, CPoint point)
{
	CCustomMonCntlListCtrl* plist = (CCustomMonCntlListCtrl*)pWnd;
	CCustomDropObject* pdata = (CCustomDropObject*)pDataObject;

	if (!(pdata->mFormat & plist->GetDropFormat())) {
		return DROPEFFECT_NONE;
	}

	TRACE("DragOver\n");

	plist->SetDropTarget(point);

	DROPEFFECT de = DROPEFFECT_NONE;

	if (dwKeyState & MK_SHIFT) {
		de = DROPEFFECT_MOVE;
	}
	else {
		de = DROPEFFECT_COPY;
	}

	return de;
}

/*============================================================================*/
/*! ���X�g�R���g���[��

-# �R�[���o�b�N�֐��F�h���b�O�h���b�v����

@param

@retval
*/
/*============================================================================*/
BOOL CALLBACK CCustomMonCntlListCtrl::Callback_MonCntl_DragDrop(CWnd* pWnd, void* pDataObject, UINT dropEffect, CPoint point)
{
	CCustomMonCntlListCtrl* plist = (CCustomMonCntlListCtrl*)pWnd;
	CCustomDropObject* pdata = (CCustomDropObject*)pDataObject;

	if (!(pdata->mFormat & plist->GetDropFormat())) {
		return DROPEFFECT_NONE;
	}

	TRACE("DragDrop\n");

	BOOL bRet = plist->DataObjectToList(pdata);
	if (dropEffect == CCustomDropObject::DE_MOVE) {
		// �ړ����̍폜�̓h���b�O���ōs��
	}
	plist->ClearDropTarget();
	return dropEffect;
}
/*============================================================================*/
/*! ���X�g�R���g���[��

-# �R�[���o�b�N�֐��F�h���b�O���[�u����

@param

@retval
*/
/*============================================================================*/
void CALLBACK CCustomMonCntlListCtrl::Callback_MonCntl_DragLeave(CWnd* pWnd)
{
	CCustomMonCntlListCtrl* plist = (CCustomMonCntlListCtrl*)pWnd;
	// �h���b�O �A���h �h���b�v �^�[�Q�b�g�Ƃ��ċ����\���N���A
	plist->ClearDropTarget();
}

/*============================================================================*/
/*! ���X�g�R���g���[��

-# �h���b�v��Ԃ̃N���A

@param

@retval
*/
/*============================================================================*/
void CCustomMonCntlListCtrl::ClearDropTarget()
{
	int count = GetItemCount();
	// �h���b�O �A���h �h���b�v �^�[�Q�b�g�Ƃ��ċ����\���N���A
	for (int i = 0; i < count; i++) {
		SetItemState(i, 0, LVIS_DROPHILITED);
	}
	RedrawItems(0, count);
}

/*============================================================================*/
/*! ���X�g�R���g���[��

-# �h���b�v��Ԃ̐ݒ�

@param

@retval
*/
/*============================================================================*/
void CCustomMonCntlListCtrl::SetDropTarget(CPoint point)
{
	int count = GetItemCount();

	// �h���b�O �A���h �h���b�v �^�[�Q�b�g�Ƃ��ċ����\���N���A
	ClearDropTarget();

	LVHITTESTINFO Info;
	ScreenToClient(&point);
	Info.pt = point;
	Info.flags = LVHT_ONITEMLABEL;
	if (-1 != SubItemHitTest(&Info))
	{
		SetItemState(Info.iItem, LVIS_DROPHILITED, LVIS_DROPHILITED);
		RedrawItems(Info.iItem, Info.iItem);
	}
}
/*============================================================================*/
/*! ���X�g�R���g���[��

-# �h���b�v���ꂽ�f�[�^�����X�g�֓o�^

@param

@retval
*/
/*============================================================================*/
BOOL CCustomMonCntlListCtrl::DataObjectToList(CCustomDropObject* pDataObject)
{
	int count = GetItemCount();
	int targetItem = -1;
	for (int i = 0; i < count; i++) {
		if (GetItemState(i, LVIS_DROPHILITED) == LVIS_DROPHILITED) {
			targetItem = i + 1;
			break;
		}
	}

	if (targetItem < 0)
		return FALSE;

	TCHAR* pbuf = (TCHAR*)pDataObject->GetBuffer();
	if (pbuf == NULL)
		return FALSE;

	CString str, sItem, strItems = CString(pbuf);
	UINT index = 0;
	while (AfxExtractSubString(sItem, strItems, index++, ';')) {
		if (sItem.IsEmpty())
			continue;
		UINT item = 0;
		while (AfxExtractSubString(str, sItem, item, '\t')) {
			if (str.IsEmpty())
				continue;
			AddItem(targetItem, item, str);
			item++;
		}
		targetItem++;
	}

	return TRUE;
}

/*============================================================================*/
/*! ���X�g�R���g���[��

-# �h���b�O�̊J�n

@param		pNMHDR	NMHDR �\����
@param		pResult	��������

@retval
*/
/*============================================================================*/
void CCustomMonCntlListCtrl::OnLvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV == NULL) {
		*pResult = 0;
		return;
	}
	if (GetSelectedCount() == 0) {
		*pResult = 0;
		return;
	}

	theApp.GetCustomControl().GetCustomDragTarget().Register(this, mDragFormat);
	theApp.GetCustomControl().GetCustomDragTarget().SetCallbackDragOver(this, Callback_MonCntl_DragOver);
	theApp.GetCustomControl().GetCustomDragTarget().SetCallbackDragDrop(this, Callback_MonCntl_DragDrop);
	theApp.GetCustomControl().GetCustomDragTarget().SetCallbackDragLeave(this, Callback_MonCntl_DragLeave);

	if (mpDragImage != NULL)
		delete mpDragImage;
	CPoint ptDragItem;
	mpDragImage = CreateDragImageEx(&ptDragItem);
	if (mpDragImage) {
		CStringArray list;
		if (PrepareItemBuff(ptDragItem, list) == FALSE) {
			ASSERT(0);
			*pResult = 0;
			return;
		}
		theApp.GetCustomControl().GetCustomDragTarget().OnDragBegin(list, mDragFormat, CCustomDropObject::DT_TCHAR);
		mpDragImage->BeginDrag(0, ptDragItem);
		mpDragImage->DragEnter(CWnd::GetDesktopWindow(), pNMLV->ptAction);
		SetCapture();
	}

	*pResult = 0;
}
/*============================================================================*/
/*! ���X�g�R���g���[��

-# �h���b�O�A�C�e���̍쐬

@param

@retval
*/
/*============================================================================*/
BOOL CCustomMonCntlListCtrl::PrepareItemBuff(POINT point, CStringArray& list)
{
	POSITION pos = GetFirstSelectedItemPosition();

	if (pos == NULL) {
		return FALSE;
	}

	list.RemoveAll();
	CString str;
	while (pos) {
		int index = GetNextSelectedItem(pos);
		// �Ď��E����ꗗ�͐擪�J�����݂̂Ƃ���
		// [;]�͕����A�C�e�����̃Z�p���[�^
		str.Format(_T("%s;"), (LPCTSTR)GetItemText(index, 0));
		list.Add(str);
	}

	return TRUE;
}

/*============================================================================*/
/*! ���X�g�R���g���[��

-# �}�E�X�ړ��C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomMonCntlListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mpDragImage) {
		CPoint ptDragImage(point);
		ClientToScreen(&ptDragImage);
		mpDragImage->DragMove(ptDragImage);

		// �h���b�v�E�B���h�E�̊m�F
		CWnd* pDropWnd = WindowFromPoint(ptDragImage);

		BOOL bTarget = FALSE;
		UINT ret = theApp.GetCustomControl().GetCustomDragTarget().OnDragMove(this, nFlags, ptDragImage);
		// �Ď��E����ꗗ�̏ꍇ�̓R�s�[�����Ȃ�
		if (ret == CCustomDropObject::DE_COPY) {
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DRAGCOPY));
		}
		else if (ret == CCustomDropObject::DE_MOVE) {
			//SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DRAGMOVE));
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DRAGCOPY));
		}
		else {
			SetCursor(theApp.LoadCursor(IDC_CURSOR_DRAGERROR));
		}
	}

	CListCtrl::OnMouseMove(nFlags, point);
}

/*============================================================================*/
/*! ���X�g�R���g���[��

-# �}�E�X���{�^���A�b�v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomMonCntlListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (mpDragImage) {
		::ReleaseCapture();
		mpDragImage->DragLeave(NULL/*CWnd::GetDesktopWindow()*/);
		mpDragImage->EndDrag();
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

		delete mpDragImage;
		mpDragImage = NULL;

		CPoint ptDragImage(point);
		ClientToScreen(&ptDragImage);
		// �Ď��E����ꗗ�̏ꍇ�̓R�s�[�����Ȃ�
		//UINT ret = theApp.GetCustomControl().GetCustomDragTarget().OnDragDrop(this, (nFlags & MK_SHIFT) ? CCustomDropObject::DE_MOVE : CCustomDropObject::DE_COPY, ptDragImage);
		UINT ret = theApp.GetCustomControl().GetCustomDragTarget().OnDragDrop(this, CCustomDropObject::DE_COPY, ptDragImage);
		TRACE("Drop Result(%d)\n", ret);
		if (ret == CCustomDropObject::DE_COPY) {
			// �R�s�[�i�{CTRL�j�̏ꍇ;
		}
		else if (ret == CCustomDropObject::DE_MOVE) {
			// �ړ��̏ꍇ
		}
		else {
			TRACE("DROPEFFECT_NONE\n");
		}
	}

	CListCtrl::OnLButtonUp(nFlags, point);
}

/*============================================================================*/
/*! ���X�g�R���g���[��

-# �}�E�X�E�{�^���_�E���C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomMonCntlListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (mpDragImage) {
		::ReleaseCapture();
		mpDragImage->DragLeave(CWnd::GetDesktopWindow());
		mpDragImage->EndDrag();

		delete mpDragImage;
		mpDragImage = NULL;
	}

	CListCtrl::OnRButtonDown(nFlags, point);
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �h���b�O�C���[�W�̍쐬

@param		lpPoint	�ʒu���

@retval
*/
/*============================================================================*/
CImageList* CCustomMonCntlListCtrl::CreateDragImageEx(LPPOINT lpPoint)
{
	CRect rectSingle;
	CRect rectComplete(0, 0, 0, 0);
	int	nIndex = -1;
	BOOL bFirst = TRUE;

	// �����őI������Ă���A�C�e���̋�`�ƕ�������擾����

	// �r�b�g�}�b�v���쐬���邽�߂̋�`���擾����
	POSITION pos = GetFirstSelectedItemPosition();
	while (pos) {
		nIndex = GetNextSelectedItem(pos);
		GetItemRect(nIndex, rectSingle, LVIR_BOUNDS);
		if (rectSingle.bottom < 0)
			continue;
		if (bFirst) {
			GetItemRect(nIndex, rectComplete, LVIR_BOUNDS);
			bFirst = FALSE;
		}
		rectComplete.UnionRect(rectComplete, rectSingle);
	}

	CClientDC dcClient(this);
	CDC dcMem;
	CBitmap Bitmap;
	// ���X�g�R���g���[���̃t�H���g���擾
	CFont* oldfont, * pfont = GetFont();

	// ������DC�ƃr�b�g�}�b�v�̍쐬
	if (!dcMem.CreateCompatibleDC(&dcClient)) {
		return NULL;
	}

	if (!Bitmap.CreateCompatibleBitmap(&dcClient, rectComplete.Width(), rectComplete.Height())) {
		return NULL;
	}

	CBitmap* pOldMemDCBitmap = dcMem.SelectObject(&Bitmap);
	// �w�i�h��Ԃ�
	dcMem.FillSolidRect(0, 0, rectComplete.Width(), rectComplete.Height(), mDragImageMaskColor);

	oldfont = dcMem.SelectObject(pfont);
	COLORREF oldcolor = dcMem.SetTextColor(mcDragTextColor);

	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	CString str;
	CRect rectItem, rectBase;

	// �擪�̑I���A�C�e����`���擾�i���`�j
	rectBase = CRect(0, 0, 0, 0);
	pos = GetFirstSelectedItemPosition();
	while (pos) {
		nIndex = GetNextSelectedItem(pos);
		if (nIndex >= 0) {
			GetItemRect(nIndex, rectBase, LVIR_BOUNDS);
			if (rectBase.bottom < 0)
				continue;
			break;
		}
	}

	// �I������Ă���A�C�e���̃e�L�X�g���擾���ă�����DC�ɕ`�悷��
	pos = GetFirstSelectedItemPosition();
	while (pos) {
		nIndex = GetNextSelectedItem(pos);
		GetItemRect(nIndex, rectItem, LVIR_BOUNDS);
		if (rectItem.bottom < 0)
			continue;
		//TRACE("Drag Item(%d (%d,%d,%d,%d))\n", nIndex, rectItem.left, rectItem.top, rectItem.right, rectItem.bottom);
		CRect rectDraw = CRect(0, 0, 0, 0);
		for (int col = 0; col < nColumnCount; col++) {
			int colwidth = GetColumnWidth(col);
			str = GetItemText(nIndex, col);
			rectDraw.left = rectDraw.right;
			rectDraw.right = rectDraw.left + colwidth;
			rectDraw.top = rectItem.top - rectBase.top;
			rectDraw.bottom = rectDraw.top + rectItem.Height();
			dcMem.FillSolidRect(rectDraw, mcDragBackColor);
			dcMem.DrawText(str, rectDraw, DT_LEFT | DT_TOP);
			break;
		}
	}

	dcMem.SetTextColor(oldcolor);
	dcMem.SelectObject(oldfont);
	dcMem.SelectObject(pOldMemDCBitmap);

	// �h���b�O�C���[�W�̍쐬
	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(rectComplete.Width(), rectComplete.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pCompleteImageList->Add(&Bitmap, mDragImageMaskColor);

	Bitmap.DeleteObject();
	if (lpPoint) {
		CPoint ptCursor;
		GetCursorPos(&ptCursor);
		ScreenToClient(&ptCursor);
		lpPoint->x = ptCursor.x - rectComplete.left;
		lpPoint->y = ptCursor.y - rectComplete.top;
	}

	return pCompleteImageList;
}

BOOL CCustomMonCntlListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->wParam == VK_ESCAPE)
	{
		if (mpDragImage) {
			::ReleaseCapture();
			mpDragImage->DragLeave(CWnd::GetDesktopWindow());
			mpDragImage->EndDrag();

			delete mpDragImage;
			mpDragImage = NULL;
		}
		return TRUE;
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
#endif
