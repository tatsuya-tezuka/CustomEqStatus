#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomGroupListCtrl.h"


//=============================================================================
// CCustomGroupEdit
//=============================================================================
/*============================================================================*/
/*! �c���[���G�f�B�b�g�{�b�N�X

-# �R���X�g���N�^�[

@param
@retval

*/
/*============================================================================*/
CCustomGroupEdit::CCustomGroupEdit(int iItem, int iSubItem, CString sInitText)
:m_sInitText(sInitText)
{
	m_iItem = iItem;
	m_iSubItem = iSubItem;
	m_bESC = FALSE;
	m_bKeyReturn = FALSE;
	m_bKeyShift = FALSE;
	m_bNotify = FALSE;
	m_nNumberLimit = 100;
}

/*============================================================================*/
/*! �c���[���G�f�B�b�g�{�b�N�X

-# �f�X�g���N�^�[

@param
@retval

*/
/*============================================================================*/
CCustomGroupEdit::~CCustomGroupEdit()
{
}

/*============================================================================*/
/*! �c���[���G�f�B�b�g�{�b�N�X

-# ���b�Z�[�W �}�b�v�̒�`

@param
@retval

*/
/*============================================================================*/
BEGIN_MESSAGE_MAP(CCustomGroupEdit, CEdit)
	//{{AFX_MSG_MAP(CTreeEdit)
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
BOOL CCustomGroupEdit::PreTranslateMessage(MSG* pMsg)
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
				m_bKeyReturn = TRUE;
			if (GetKeyState(VK_SHIFT) & 0xff00)
				m_bKeyShift = TRUE;
			else
				m_bKeyShift = FALSE;

			::TranslateMessage(pMsg);
			if (!(GetStyle() & ES_MULTILINE) || pMsg->wParam != VK_ESCAPE){
				::DispatchMessage(pMsg);
			}
			if (pMsg->wParam == VK_ESCAPE){
				m_bESC = TRUE;
				GetParent()->SetFocus();
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
void CCustomGroupEdit::SetNumberLimit(UINT len)
{
	m_nNumberLimit = len;
}

/*============================================================================*/
/*! �c���[���G�f�B�b�g�{�b�N�X

-# ���̓t�H�[�J�X���������O�ɌĂяo�����

@param pNewWnd	���̓t�H�[�J�X���󂯎��E�B���h�E�ւ̃|�C���^�[
@retval

*/
/*============================================================================*/
void CCustomGroupEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CString str;
	GetWindowText(str);

	if (m_bNotify == TRUE)
	{
		return;
	}
	m_bNotify = TRUE;

	// Send Notification to parent of ListView ctrl
	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_iItem;
	dispinfo.item.iSubItem = m_iSubItem;
	dispinfo.item.pszText = m_bESC ? NULL : LPTSTR((LPCTSTR)str);
	dispinfo.item.cchTextMax = m_bESC ? 0 : str.GetLength();

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
void CCustomGroupEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 0x03 || nChar == 0x16 || nChar == 0x18 || nChar == 0x08)//Ctrl+C; Ctrl+V; Ctrl+X; BackSpace
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if (nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if (nChar == VK_ESCAPE)
			m_bESC = TRUE;
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
int CCustomGroupEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Set the proper font
	CFont* font = GetParent()->GetFont();
	SetFont(font);

	SetWindowText(m_sInitText);
	SetFocus();
	//	CalculateSize();
	SetSel(0, -1);

	CString	str;
	GetWindowText(str);
	int length = m_nNumberLimit;
	SetLimitText(length);
	return 0;
}

//=============================================================================
// CCustomGroupHeaderCtrl
//=============================================================================

CCustomGroupHeaderCtrl::CCustomGroupHeaderCtrl()
{
}

CCustomGroupHeaderCtrl::~CCustomGroupHeaderCtrl()
{
}


BEGIN_MESSAGE_MAP(CCustomGroupHeaderCtrl, CHeaderCtrl)
END_MESSAGE_MAP()

/*============================================================================*/
/*! �O���[�v���X�g�p�w�b�_�[�R���g���[��

-# �I�[�i�[�`��

@param
@retval

*/
/*============================================================================*/
void CCustomGroupHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_HEADER);

	HDITEM hdi;
	TCHAR  lpBuffer[256];

	hdi.mask = HDI_TEXT;
	hdi.pszText = lpBuffer;
	hdi.cchTextMax = 256;

	GetItem(lpDrawItemStruct->itemID, &hdi);

	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));

	// �w�b�_�[���ڂ̕`��
	//pDC->DrawFrameControl(&lpDrawItemStruct->rcItem, DFC_CAPTION, DFCS_FLAT | DFCS_TRANSPARENT);
	pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_WINDOW));
	pDC->DrawEdge(&lpDrawItemStruct->rcItem, BDR_SUNKENINNER, BF_RIGHT);
	pDC->DrawEdge(&lpDrawItemStruct->rcItem, BDR_SUNKENINNER, BF_BOTTOM);

	// ������`��
	UINT uFormat = DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | DT_CENTER;
	pDC->DrawText(lpBuffer, (int)wcslen(lpBuffer), &lpDrawItemStruct->rcItem, uFormat);

	pDC->SelectStockObject(SYSTEM_FONT);
}



CCustomGroupListCtrl::CCustomGroupListCtrl()
{
	mListFont.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT lf;
	if (mListFont.GetLogFont(&lf)) {
		lf.lfHeight = mListFontHeight;
		lf.lfWeight = FW_BOLD;
		mListFont.DeleteObject();
		mListFont.CreateFontIndirect(&lf);
	}

	mpEdit = NULL;

	mDragData.indexes.clear();
	mpDragImage = NULL;
	mcDragBackColor = GetSysColor(COLOR_WINDOW);
	mcDragTextColor = GetSysColor(COLOR_WINDOWTEXT);
}


CCustomGroupListCtrl::~CCustomGroupListCtrl()
{
	mListFont.DeleteObject();
}

BEGIN_MESSAGE_MAP(CCustomGroupListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CCustomGroupListCtrl::OnNMCustomdraw)
	ON_NOTIFY(HDN_BEGINTRACKA, 0, &CCustomGroupListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACKW, 0, &CCustomGroupListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKA, 0, &CCustomGroupListCtrl::OnHdnDividerdblclick)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKW, 0, &CCustomGroupListCtrl::OnHdnDividerdblclick)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, &CCustomGroupListCtrl::OnLvnEndlabeledit)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CCustomGroupListCtrl::OnLvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


namespace
{
	struct GROUPSORT
	{
		GROUPSORT(HWND hWnd, int nCol, bool bAscending)
			:m_hWnd(hWnd)
			, m_ColumnIndex(nCol)
			, m_Ascending(bAscending)
		{}

		HWND m_hWnd;
		int  m_ColumnIndex;
		bool m_Ascending;
		CSimpleMap<int, CString>	m_GroupNames;
		CSimpleMap<int, UINT>		m_GroupIndexs;
		CSimpleMap<int, CString>	m_ItemNames;

		const CString& LookupGroupName(int nGroupId)
		{
			int groupIdx = m_GroupNames.FindKey(nGroupId);
			if (groupIdx == -1)
			{
				static const CString emptyStr;
				return emptyStr;
			}
			return m_GroupNames.GetValueAt(groupIdx);
		}

		const UINT& LookupGroupIndex(int nGroupId)
		{
			int groupIdx = m_GroupIndexs.FindKey(nGroupId);
			if (groupIdx == -1)
			{
				static const UINT empty;
				return empty;
			}
			return m_GroupIndexs.GetValueAt(groupIdx);
		}

		const CString& LookupItemName(int nItemId)
		{
			int itemIdx = m_ItemNames.FindKey(nItemId);
			if (itemIdx == -1)
			{
				static const CString emptyStr;
				return emptyStr;
			}
			return m_ItemNames.GetValueAt(itemIdx);
		}
	};
	int CALLBACK SortFuncGroup(int nLeftId, int nRightId, void* lParamSort)
	{
		GROUPSORT& ps = *(GROUPSORT*)lParamSort;

		const CString& left = ps.LookupGroupName(nLeftId);
		const CString& right = ps.LookupGroupName(nRightId);

		if (left == mNoGroupText) {
			return -1;
		}
		if (right == mNoGroupText) {
			return 1;
		}

		TRACE("# SortFuncGroup : %s : %s (%d)\n", CStringA(ps.LookupGroupName(nLeftId)), CStringA(ps.LookupGroupName(nRightId)), _tcscmp(left, right));

		if (ps.m_Ascending)
			return _tcscmp(left, right);
		else
			return _tcscmp(right, left);
	}
	
	int CALLBACK SortFuncGroupEx(int nLeftId, int nRightId, void* lParamSort)
	{
		GROUPSORT& ps = *(GROUPSORT*)lParamSort;

		const UINT& left = ps.LookupGroupIndex(nLeftId);
		const UINT& right = ps.LookupGroupIndex(nRightId);

		if (ps.m_Ascending) {
			if (left < right)
				return -1;
			else if (left > right)
				return 1;
			return 0;
		}
		else {
			if (left > right)
				return -1;
			else if (left < right)
				return 1;
			return 0;
		}
	}

	int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		GROUPSORT& ps = *reinterpret_cast<GROUPSORT*>(lParamSort);

		TCHAR left[256] = _T(""), right[256] = _T("");
		ListView_GetItemText(ps.m_hWnd, lParam1, ps.m_ColumnIndex, left, sizeof(left));
		ListView_GetItemText(ps.m_hWnd, lParam2, ps.m_ColumnIndex, right, sizeof(right));

		if (ps.m_Ascending)
			return _tcscmp(left, right);
		else
			return _tcscmp(right, left);
	}

	int CALLBACK SortFuncEx(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
	{
		GROUPSORT& ps = *reinterpret_cast<GROUPSORT*>(lParamSort);

		int leftno = static_cast<int>(lParam1);
		int rightno = static_cast<int>(lParam2);
		LVITEM leftitem, rightitem;
		leftitem.iItem = leftno;
		leftitem.mask = LVIF_PARAM;
		leftitem.iSubItem = 0;
		ListView_GetItem(ps.m_hWnd, &leftitem);
		rightitem.iItem = rightno;
		rightitem.mask = LVIF_PARAM;
		rightitem.iSubItem = 0;
		ListView_GetItem(ps.m_hWnd, &rightitem);
		CTreeNode* pleft = (CTreeNode*)leftitem.lParam;
		CTreeNode* pright = (CTreeNode*)rightitem.lParam;

		const UINT& left = LOWORD(pleft->GetManager().groupno);
		const UINT& right = LOWORD(pright->GetManager().groupno);

		if (ps.m_Ascending) {
			if (left < right)
				return -1;
			else if (left > right)
				return 1;
			return 0;
		}
		else {
			if (left > right)
				return -1;
			else if (left < right)
				return 1;
			return 0;
		}
	}
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �\�[�g�֐�

@param

@retval
*/
/*============================================================================*/
void CCustomGroupListCtrl::SortItem()
{
	GROUPSORT groupsort(m_hWnd, 0, true);
	ListView_SortItemsEx(m_hWnd, SortFuncEx, &groupsort);
}

void CCustomGroupListCtrl::SortGroup()
{
	GROUPSORT groupsort(m_hWnd, 0, true);
	for (int nRow = 0; nRow < GetItemCount(); ++nRow){
		CTreeNode* pnode = (CTreeNode*)GetItemData(nRow);
		int nGroupId = pnode->GetManager().groupno;
		if (groupsort.m_GroupNames.FindKey(HIWORD(nGroupId)) == -1) {
			groupsort.m_GroupNames.Add(HIWORD(nGroupId), getGroupHeader(HIWORD(nGroupId)));
		}
		if (groupsort.m_GroupIndexs.FindKey(HIWORD(nGroupId)) == -1) {
			groupsort.m_GroupIndexs.Add(HIWORD(nGroupId), LOWORD(nGroupId));
		}
	}
	Invalidate(FALSE);
	ListView_SortGroups(m_hWnd, SortFuncGroup, &groupsort);
}


/*============================================================================*/
/*! �O���[�v���X�g

-# �E�B���h�E���b�Z�[�W����

@param

@retval
*/
/*============================================================================*/
BOOL CCustomGroupListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->wParam == VK_ESCAPE){
		if (mpDragImage) {
			::ReleaseCapture();
			mpDragImage->DragLeave(CWnd::GetDesktopWindow());
			mpDragImage->EndDrag();

			delete mpDragImage;
			mpDragImage = NULL;
			SetItemState(-1, 0, LVIS_DROPHILITED);
			return TRUE;
		}
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
/*============================================================================*/
/*! �O���[�v���X�g

-# �J�X�^���`��

@param

@retval
*/
/*============================================================================*/
void CCustomGroupListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	int iRow = (int)lplvcd->nmcd.dwItemSpec;
	CDC* pDC = CDC::FromHandle(lplvcd->nmcd.hdc);
	CHeaderCtrl* pHeader = NULL;
	pHeader = GetHeaderCtrl();

	CTreeNode* pnode = NULL;
	if (GetItemCount() != 0){
		pnode = (CTreeNode*)GetItemData(iRow);
	}

	switch (lplvcd->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		if (pnode != NULL){
			//lplvcd->clrText = pnode->GetColor().text;
			//if (pnode->GetWindowInfo().wnd == NULL || pnode->GetWindowInfo().wnd->IsWindowVisible() == FALSE)
			//	lplvcd->clrText = mManagerHideColor;// GetSysColor(COLOR_GRAYTEXT);
		}
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		*pResult = CDRF_NOTIFYPOSTPAINT + CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM:
		if (pnode != NULL){
			//lplvcd->clrTextBk = pnode->GetColor().textback;
		}
		*pResult = CDRF_DODEFAULT;
		return;
	case CDDS_ITEMPOSTPAINT:
		//if (GetItemState(iRow, LVIS_SELECTED) == LVIS_SELECTED)
		//{
		//	CRect rect;
		//	GetItemRect(iRow, rect, LVIR_BOUNDS);
		//	pDC->FillSolidRect(&rect, pnode->getColor().back);
		//	CString strTemp;
		//	int nColumns = pHeader->GetItemCount();
		//	HDITEM hditem;
		//	UINT nFormat;
		//	int nFmt;
		//	for (int i = 0; i < nColumns; i++)
		//	{
		//		strTemp = GetItemText(iRow, i);
		//		GetSubItemRect(iRow, i, LVIR_BOUNDS, rect);
		//		hditem.mask = HDI_FORMAT;
		//		pHeader->GetItem(i, &hditem);
		//		nFmt = hditem.fmt & HDF_JUSTIFYMASK;
		//		nFormat = DT_VCENTER | DT_SINGLELINE;
		//		if (nFmt == HDF_CENTER) {
		//			nFormat |= DT_CENTER;
		//		}
		//		else if (nFmt == HDF_LEFT) {
		//			nFormat |= DT_LEFT;
		//		}
		//		else {
		//			nFormat |= DT_RIGHT;
		//		}
		//		pDC->SelectStockObject(NULL_BRUSH);
		//		pDC->SetBkMode(TRANSPARENT);
		//		pDC->SetTextColor(RGB(0,0,255));
		//		GetSubItemRect(iRow, i, LVIR_LABEL, rect);
		//		int offset = pDC->GetTextExtent(_T("W"), 1).cx;
		//		rect.left += 6;
		//		pDC->DrawText(strTemp, &rect, nFormat);
		//	}
		//}
		*pResult = CDRF_DODEFAULT;
		break;

	default:
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
/*============================================================================*/
/*! �O���[�v���X�g

-# �w��J�����ԍ�����O���[�v�o�^���s��

@param
@retval

*/
/*============================================================================*/
void CCustomGroupListCtrl::CreateGroupControl(CWnd* parent)
{
	mParent = parent;
	// �g���X�^�C���̎擾
	DWORD  dwStyle = ListView_GetExtendedListViewStyle(m_hWnd);
	//  �g���X�^�C���̃Z�b�g
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER;
#ifdef _DEBUG
	dwStyle |= LVS_EX_INFOTIP;
#endif
	//  �V�����g���X�^�C����ݒ�
	ListView_SetExtendedListViewStyle(m_hWnd, dwStyle);

	SetFont(&mListFont);

	int width = 0;
	for (int i = 0; i < sizeof(mGroupListHeader) / sizeof(mGroupListHeader[0]); i++) {
		switch (i) {
		case	eManagerTitle:	width = 300; break;
		case	eManagerNote:	width = 200; break;
#ifdef _DEBUG
		case	eManagerGroup:	width = 50; break;
#else
		case	eManagerGroup:	width = 0; break;
#endif
		}
		InsertColumn(i, mGroupListHeader[i], LVCFMT_LEFT, width);
	}

	EnableToolTips(TRUE);
}
/*============================================================================*/
/*! �O���[�v���X�g

-# �w��J�����ԍ�����O���[�v�o�^���s��

@param
@retval

*/
/*============================================================================*/
BOOL CCustomGroupListCtrl::GroupByColumn(int nCol, BOOL bEnableGroup/* = TRUE*/)
{
	TRACE("##### GroupByColumn\n");

	CWaitCursor waitCursor;

	SetRedraw(FALSE);

	RemoveAllGroups();

	//EnableGroupView(bEnableGroup && GetItemCount() > 0);
	EnableGroupView(bEnableGroup);

	BOOL ret = FALSE;

	if (IsGroupViewEnabled()){
		// �O���[�v�\���̏ꍇ
		// �O���[�v�ԍ��̒����A�O���[�v�w�b�_�[�A�t�b�^�[�̐ݒ���s��

		CSimpleMap<CString, CSimpleArray<int> > groups;
		vector<CString> footers;
		footers.clear();

		//========================================================================================================
		// �O���[�v�ԍ��Ȃ��̏ꍇ
		for (int nRow = 0; nRow < GetItemCount(); ++nRow) {
			// �f�[�^�̃O���[�v�ԍ�����ΏۃJ�����̃e�L�X�g��ݒ肷��
			CTreeNode* pnode = (CTreeNode*)GetItemData(nRow);
			if (HIWORD(pnode->GetManager().groupno) != 0) {
				continue;
			}
			SetItemText(nRow, nCol, mNoGroupText);

			// �A�C�e�������񂩂�O���[�v���쐬����
			CString cellText = GetItemText(nRow, nCol);

			int nGroupId = groups.FindKey(cellText);
			if (nGroupId == -1) {
				// �O���[�v�����݂��Ȃ��̂ŃO���[�v�Ƃ��Đݒ肷��
				CSimpleArray<int> rows;
				groups.Add(cellText, rows);
				nGroupId = groups.FindKey(cellText);
			}
			groups.GetValueAt(nGroupId).Add(nRow);
			pnode->GetManager().groupno = MAKELONG(LOWORD(pnode->GetManager().groupno), nGroupId);
			swprintf_s(pnode->GetManager().groupname, mNameSize, _T("%s"), (LPCTSTR)cellText);

			CString strFooter = cellText;
			if (footers.size() < (UINT)(nGroupId + 1)) {
				// �O���[�v�����݂��Ȃ��̂ŃO���[�v�Ƃ��Đݒ肷��
				footers.push_back(strFooter);
			}
		}

		//========================================================================================================
		// �O���[�v�ԍ�����̏ꍇ
		for (int nRow = 0; nRow < GetItemCount(); ++nRow) {
			// �f�[�^�̃O���[�v�ԍ�����ΏۃJ�����̃e�L�X�g��ݒ肷��
			CTreeNode* pnode = (CTreeNode*)GetItemData(nRow);
			if (HIWORD(pnode->GetManager().groupno) == 0) {
				continue;
			}
			else {
				CString str;
				str.Format(_T("Group_%d"), HIWORD(pnode->GetManager().groupno));
				if (wcslen(pnode->GetManager().groupname) != 0 && CString(pnode->GetManager().groupname).IsEmpty() == false) {
					str = CString(pnode->GetManager().groupname);
				}
				SetItemText(nRow, nCol, str);
			}
			// �A�C�e�������񂩂�O���[�v���쐬����
			CString cellText = GetItemText(nRow, nCol);

			int nGroupId = groups.FindKey(cellText);
			if (nGroupId == -1) {
				// �O���[�v�����݂��Ȃ��̂ŃO���[�v�Ƃ��Đݒ肷��
				CSimpleArray<int> rows;
				groups.Add(cellText, rows);
				nGroupId = groups.FindKey(cellText);
			}
			groups.GetValueAt(nGroupId).Add(nRow);
			pnode->GetManager().groupno = MAKELONG(LOWORD(pnode->GetManager().groupno), nGroupId);
			swprintf_s(pnode->GetManager().groupname, mNameSize, _T("%s"), (LPCTSTR)cellText);
			TRACE("# GroupByColumn : GroupNo=%d, GroupName=%s\n", nGroupId, CStringA(cellText));

			CString strFooter = cellText;
			if (footers.size() < (UINT)(nGroupId + 1)) {
				// �O���[�v�����݂��Ȃ��̂ŃO���[�v�Ƃ��Đݒ肷��
				footers.push_back(strFooter);
			}
		}

		// �O���[�v���̐ݒ�
		for (int nGroupId = 0; nGroupId < groups.GetSize(); ++nGroupId){
			// �O���[�v�ԍ�����O���[�v�����擾����
			const CSimpleArray<int>& groupRows = groups.GetValueAt(nGroupId);
			DWORD dwState = LVGS_NORMAL;

			// �O���[�v�J�t���O�ݒ�
			dwState = LVGS_COLLAPSIBLE;

			// �O���[�v�w�b�_�[�̐ݒ�
			VERIFY(insertGroupHeader(nGroupId, nGroupId, footers[nGroupId], dwState) != -1);
			//setGroupTask(nGroupId, _T("Task: Check Group"));
			CString subtitle;
			subtitle = groups.GetKeyAt(nGroupId);
			//setGroupSubtitle(nGroupId, subtitle);
			//setGroupFooter(nGroupId, footers[nGroupId] + _T(" end"), LVGA_FOOTER_CENTER);
			setGroupFooter(nGroupId, _T("----- ") + footers[nGroupId] + _T(" -----"), LVGA_FOOTER_CENTER);

			for (int groupRow = 0; groupRow < groupRows.GetSize(); ++groupRow){
				VERIFY(setRowGroupId(groupRows[groupRow], nGroupId));
			}
		}
		SetColumnWidth(nCol, 0);
		ret = TRUE;
	}

	SetRedraw(TRUE);
	Invalidate(FALSE);

	return ret;
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �}�E�X�ʒu��񂩂�A�C�e���A�T�u�A�C�e���ԍ����擾����

@param	point	�}�E�X�ʒu
@param	col		�T�u�A�C�e���ԍ��i�[
@retval

*/
/*============================================================================*/
int CCustomGroupListCtrl::HitTestEx(CPoint &point, int *col)
{
	int colnum = 0;
	int row = HitTest(point, NULL);
	if (col) *col = 0;

	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return row;

	// ���ݕ\������Ă���ŏ��̃A�C�e���C���f�b�N�X���擾����
	//row = GetTopIndex();

	// ���̃A�C�e�������擾����
	int bottom = row + GetCountPerPage();

	if (bottom > GetItemCount())
		bottom = GetItemCount();

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	//for (; row <= bottom; row++)
	{
		// �A�C�e���̈�̎擾
		CRect rect;
		GetItemRect(row, &rect, LVIR_BOUNDS);

		//if (rect.PtInRect(point))
		{
			// Now find the column
			for (colnum = 0; colnum < nColumnCount; colnum++){
				int colwidth = GetColumnWidth(colnum);
				if (point.x >= rect.left && point.x <= (rect.left + colwidth)){
					if (col) *col = colnum;
					TRACE("# HitTestEx : Select Item(%d) SubItem(%d)\n", row, *col);
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	TRACE("# HitTestEx : Not Select Item\n");
	return -1;
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �O���[�v�w�b�_�[�̐ݒ�

@param
@retval

*/
/*============================================================================*/
LRESULT CCustomGroupListCtrl::insertGroupHeader(int nIndex, int nGroupId, const CString& strHeader, DWORD dwState /* = LVGS_NORMAL */, DWORD dwAlign /*= LVGA_HEADER_LEFT*/)
{
	LVGROUP lg = { 0 };
	lg.cbSize = sizeof(lg);
	lg.iGroupId = nGroupId;
	lg.state = dwState;
	lg.mask = LVGF_GROUPID | LVGF_HEADER | LVGF_STATE | LVGF_ALIGN;
	lg.uAlign = dwAlign;

	// �w�b�_�[�^�C�g���̐ݒ�
	lg.pszHeader = (LPWSTR)(LPCTSTR)strHeader;
	lg.cchHeader = strHeader.GetLength();

	return InsertGroup(nIndex, (PLVGROUP)&lg);
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �O���[�v�w�b�_�[�̐ݒ�

@param
@retval

*/
/*============================================================================*/
BOOL CCustomGroupListCtrl::SetGroupHeader(int nGroupID, const CString& header)
{
	LVGROUP lg = { 0 };
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_HEADER;
	lg.pszHeader = (LPWSTR)(LPCTSTR)header;
	lg.cchHeader = header.GetLength();

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg) == -1)
		return FALSE;

	setGroupFooter(nGroupID, _T("----- ") + header + _T(" -----"), LVGA_FOOTER_CENTER);

	return TRUE;
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �O���[�v�^�X�N�̐ݒ�

@param
@retval

*/
/*============================================================================*/
BOOL CCustomGroupListCtrl::setGroupTask(int nGroupID, const CString& task)
{
	LVGROUP lg = { 0 };
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_TASK;
	lg.pszTask = (LPWSTR)(LPCTSTR)task;
	lg.cchTask = task.GetLength();

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg) == -1)
		return FALSE;

	return TRUE;
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �O���[�v�T�u�^�C�g���̐ݒ�

@param
@retval

*/
/*============================================================================*/
BOOL CCustomGroupListCtrl::setGroupSubtitle(int nGroupID, const CString& subtitle)
{
	LVGROUP lg = { 0 };
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_SUBTITLE;
	lg.pszSubtitle = (LPWSTR)(LPCTSTR)subtitle;
	lg.cchSubtitle = subtitle.GetLength();

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg) == -1)
		return FALSE;

	return TRUE;
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �O���[�v�t�b�^�[�̐ݒ�

@param
@retval

*/
/*============================================================================*/
BOOL CCustomGroupListCtrl::setGroupFooter(int nGroupID, const CString& footer, DWORD dwAlign /*= LVGA_FOOTER_LEFT*/)
{
	LVGROUP lg = { 0 };
	lg.cbSize = sizeof(lg);
	lg.mask = LVGF_FOOTER | LVGF_ALIGN;
	lg.uAlign = dwAlign;
	lg.pszFooter = (LPWSTR)(LPCTSTR)footer;
	lg.cchFooter = footer.GetLength();

	if (SetGroupInfo(nGroupID, (PLVGROUP)&lg) == -1)
		return FALSE;

	return TRUE;
}
/*============================================================================*/
/*! �O���[�v���X�g

-# �O���[�vID�̐ݒ�

@param
@retval

*/
/*============================================================================*/
BOOL CCustomGroupListCtrl::setRowGroupId(int nRow, int nGroupId)
{
	LVITEM lvItem = { 0 };
	lvItem.mask = LVIF_GROUPID;
	lvItem.iItem = nRow;
	lvItem.iSubItem = 0;
	lvItem.iGroupId = nGroupId;
	return SetItem(&lvItem);
}
/*============================================================================*/
/*! �O���[�v���X�g

-# �O���[�vID�̎擾

@param
@retval

*/
/*============================================================================*/
int CCustomGroupListCtrl::getRowGroupId(int nRow)
{
	LVITEM lvi = { 0 };
	lvi.mask = LVIF_GROUPID;
	lvi.iItem = nRow;
	VERIFY(GetItem(&lvi));
	return lvi.iGroupId;
}
/*============================================================================*/
/*! �O���[�v���X�g

-# �O���[�v�w�b�_�[���̂̎擾

@param
@retval

*/
/*============================================================================*/
CString CCustomGroupListCtrl::getGroupHeader(int nGroupId)
{
	LVGROUP lg = { 0 };
	lg.cbSize = sizeof(lg);
	lg.iGroupId = nGroupId;
	lg.mask = LVGF_HEADER | LVGF_GROUPID;
	VERIFY(GetGroupInfo(nGroupId, (PLVGROUP)&lg) != -1);

#ifdef UNICODE
	return lg.pszHeader;
#else
	CComBSTR header(lg.pszHeader);
	return (LPCTSTR)COLE2T(header);
#endif
}
/*============================================================================*/
/*! �O���[�v���X�g

-# �O���[�vID�̎擾

@param
@retval

*/
/*============================================================================*/
int CCustomGroupListCtrl::getHeaderGroupId(CString& strHeader)
{
	for (int i = 0; i < GetGroupCount(); i++) {
		CString str = getGroupHeader(i);
		if (str.MakeLower() == strHeader.MakeLower()) {
			return i;
		}
	}
	return 0;
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �O���[�v���A�C�e�����̎擾

@param		nGroupId	�O���[�v�ԍ�

@retval	�O���[�v���A�C�e����
*/
/*============================================================================*/
int CCustomGroupListCtrl::GetGroupItemCount(UINT nGroupId)
{
	int count = 0;
	for (int nRow = 0; nRow < GetItemCount(); ++nRow) {
		// �f�[�^�̃O���[�v�ԍ�����ΏۃJ�����̃e�L�X�g��ݒ肷��
		CTreeNode* pnode = (CTreeNode*)GetItemData(nRow);
		if (HIWORD(pnode->GetManager().groupno) != nGroupId) {
			continue;
		}
		count++;
	}
	return count;
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �}�E�X�ɂ��w�b�_�[���ύX

@param
@retval

*/
/*============================================================================*/
void CCustomGroupListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	int nCol = (int)phdr->iItem;
#ifdef _DEBUG
	SetFocus();
	*pResult = 0;
#else
	if (nCol == eManagerGroup) {
		*pResult = 1;
	}
	else {
		SetFocus();
		*pResult = 0;
	}
#endif
}
/*============================================================================*/
/*! �O���[�v���X�g

-# �}�E�X�ɂ��w�b�_�[�J�����ԃ_�u���N���b�N

@param
@retval

*/
/*============================================================================*/
void CCustomGroupListCtrl::OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	int nCol = (int)phdr->iItem;
#ifdef _DEBUG
	SetFocus();
	AutoSizeColumns(nCol);
	*pResult = 0;
#else
	if (nCol == eManagerGroup) {
		*pResult = 1;
	}
	else {
		SetFocus();
		AutoSizeColumns(nCol);
		*pResult = 0;
	}
#endif

	*pResult = 0;
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �}�E�X���{�^������

@param
@retval

*/
/*============================================================================*/
void CCustomGroupListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDown(nFlags, point);

	//int pos = 0, index;
	//int colnum, strpos = 0;
	//CString str;
	//if ((index = HitTestEx(point, &colnum)) != -1 && colnum != 0){
	//	UINT flag = LVIS_SELECTED;
	//	if ((GetItemState(index, flag) & flag) == flag){
	//		if (EditExecute(index, colnum) == false){
	//			SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	//		}
	//	}
	//	else{
	//		SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	//	}
	//}
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �}�E�X���{�^���_�u���N���b�N

@param
@retval

*/
/*============================================================================*/
void CCustomGroupListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int pos = 0, index;
	int colnum, strpos = 0;
	CString str;
	if ((index = HitTestEx(point, &colnum)) != -1 && colnum != 0) {
		UINT flag = LVIS_SELECTED;
		if ((GetItemState(index, flag) & flag) == flag) {
			if (EditExecute(index, colnum) == false) {
				SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
		else {
			SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}
		return;
	}

	CListCtrl::OnLButtonDblClk(nFlags, point);
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �J������Ԃɂ���ĕҏW���s��

@param	item			�I���A�C�e��
@param	colnum			�I���J����

@retval bool	�ҏW���s���ꍇ��true��Ԃ��B����ȊO��false��Ԃ�
*/
/*============================================================================*/
bool CCustomGroupListCtrl::EditExecute(int item, int colnum)
{
	// �I�𕶎���̎擾
	CString text = GetItemText(item, colnum);
	//DWORD data = GetItemData(item);

	// �I���J�����̕ҏW���@�𒲂ׂ�
	// �w��J�����̓R���{�{�b�N�X�ҏW���H
	// �G�f�B�b�g�ҏW�ł�
	ModifyStyle(0, LVS_EDITLABELS);
	editSubLabel(item, colnum);
	return true;
}
/*============================================================================*/
/*! �O���[�v���X�g

-# ���X�g�R���g���[����̃T�u���x���̕ҏW

@param	item			�I���A�C�e��
@param	colnum			�I���J����

@retval bool	�ҏW���s���ꍇ��true��Ԃ��B����ȊO��false��Ԃ�
*/
/*============================================================================*/
CEdit* CCustomGroupListCtrl::editSubLabel(int item, int colnum)
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if (colnum >= nColumnCount)
		return NULL;

	int offset = 0;
	for (int i = 0; i < colnum; i++)
		offset += GetColumnWidth(i);

	CRect rect;
	GetItemRect(item, &rect, LVIR_BOUNDS);

	CRect rcClient;
	GetClientRect(&rcClient);
	if (offset + rect.left < 0 || offset + rect.left > rcClient.right){
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}

	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn(colnum, &lvcol);
	DWORD dwStyle;
	if ((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if ((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else dwStyle = ES_CENTER;

	rect.left += offset + 4;
	rect.right = rect.left + GetColumnWidth(colnum) - 3;
	if (rect.right > rcClient.right) rect.right = rcClient.right;

	dwStyle |= WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL/*|ES_RIGHT*/;
	if (mpEdit != NULL)
		delete mpEdit;

	mpEdit = new CCustomGroupEdit(item, colnum, GetItemText(item, colnum));
	mpEdit->Create(dwStyle, rect, this, 1);
	((CCustomGroupEdit*)mpEdit)->SetNumberLimit(6);
	((CCustomGroupEdit*)mpEdit)->SetLimitText(mEditLimitTextSize);
	return mpEdit;
}
/*============================================================================*/
/*! �O���[�v���X�g

-# ���x���ҏW�̏I��

@param

@retval
*/
/*============================================================================*/
void CCustomGroupListCtrl::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	LV_ITEM *plvItem = &pDispInfo->item;

	if (plvItem->pszText != NULL){
		CString str = plvItem->pszText;
		SetItemText(plvItem->iItem, plvItem->iSubItem, str);
	}
	ModifyStyle(LVS_EDITLABELS, 0);
	if (mpEdit != NULL){
		delete mpEdit;
		mpEdit = NULL;
	}
	*pResult = 0;
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �h���b�O�̊J�n

@param		pNMHDR	NMHDR �\����
@param		pResult	��������

@retval
*/
/*============================================================================*/
void CCustomGroupListCtrl::OnLvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

#if _DEMO_PHASE < 100
	* pResult = 0;
	return;
#endif
	if (IsGroupViewEnabled() == FALSE) {
		*pResult = 0;
		return;
	}

	if (pNMLV) {
		mDragData.indexes.clear();
		// �擪�̃h���b�O�A�C�e�������擾����
		POSITION pos = GetFirstSelectedItemPosition();
		while (pos) {
			int item = GetNextSelectedItem(pos);
			CTreeNode* pnode = (CTreeNode*)GetItemData(item);
			mDragData.group = HIWORD(pnode->GetManager().groupno);
			break;
		}

		// �I������Ă��鍀�ڂ��S�ē����O���[�v���`�F�b�N����
		pos = GetFirstSelectedItemPosition();
		while (pos) {
			int item = (int)GetNextSelectedItem(pos);
			CTreeNode* pnode = (CTreeNode*)GetItemData(item);
			if (mDragData.group == HIWORD(pnode->GetManager().groupno)) {
				mDragData.indexes.push_back(item);
			}
			else {
				// �قȂ����O���[�v�̑I����ԂȂ̂ŉ������Ȃ�
				*pResult = 0;
				return;
			}
		}

		if (mDragData.indexes.size() > 0) {
			if (mpDragImage != NULL)
				delete mpDragImage;
			CPoint ptDragItem;
			mpDragImage = createDragImageEx(&ptDragItem);
			if (mpDragImage) {
				mpDragImage->BeginDrag(0, ptDragItem);
				mpDragImage->DragEnter(CWnd::GetDesktopWindow(), pNMLV->ptAction);
				SetCapture();
			}
		}
	}

	*pResult = 0;
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �}�E�X�ړ��C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomGroupListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (mpDragImage) {
		CPoint ptDragImage(point);
		ClientToScreen(&ptDragImage);
		mpDragImage->DragMove(ptDragImage);

		SetRedraw(FALSE);
		UINT flags = 0;
		int index = HitTest(point, &flags);
		LVHITTESTINFO hitTestInfo = { 0 };
		hitTestInfo.pt = point;
		int i = HitTest(&hitTestInfo);
		TRACE("MouseMove(%d:%d)\n", flags, hitTestInfo.flags);
		if (isDropEnable(point) == true) {
			if (index >= 0) {
				SetItemState(-1, 0, LVIS_DROPHILITED);
				SetItemState(index, LVIS_DROPHILITED, LVIS_DROPHILITED);
				EnsureVisibleEx(index);
			}
			//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_DRAGMOVE));
		}
		else {
			//SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
			SetCursor(theApp.LoadCursor(IDC_CURSOR_DRAGERROR));
		}

		SetRedraw(TRUE);
	}

	CListCtrl::OnMouseMove(nFlags, point);
}

/*============================================================================*/
/*! �c���[���X�g�R���g���[���i�h���b�O���h���b�v�֘A�j

-# �w��A�C�e������������

@param	hItem		�A�C�e��

@retval
*/
/*============================================================================*/
void CCustomGroupListCtrl::EnsureVisibleEx(int item)
{
	return;
	// �A�C�e���̑O����m�F����
	int nPrev = (item == 0) ? item : item - 1;
	int nNext = (item == GetItemCount() - 1) ? item : item + 1;
	EnsureVisible(nPrev, FALSE);
	EnsureVisible(nNext, FALSE);

}

/*============================================================================*/
/*! �O���[�v���X�g

-# �}�E�X���{�^���A�b�v�C�x���g

@param

@retval
*/
/*============================================================================*/
void CCustomGroupListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (mpDragImage) {
		::ReleaseCapture();
		mpDragImage->DragLeave(CWnd::GetDesktopWindow());
		mpDragImage->EndDrag();

		delete mpDragImage;
		mpDragImage = NULL;

		dropItem(point);

		SetItemState(-1, 0, LVIS_DROPHILITED);
	}

	CListCtrl::OnLButtonUp(nFlags, point);
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �h���b�O�C���[�W�̍쐬

@param		lpPoint	�ʒu���

@retval
*/
/*============================================================================*/
CImageList* CCustomGroupListCtrl::createDragImageEx(LPPOINT lpPoint)
{
	const COLORREF	mDragImageMaskColor = RGB(255, 0, 255);

	CRect rectSingle;
	CRect rectComplete(0, 0, 0, 0);
	int	nIndex = -1;
	BOOL bFirst = TRUE;

	// �����őI������Ă���A�C�e���̋�`�ƕ�������擾����

	// �r�b�g�}�b�v���쐬���邽�߂̋�`���擾����
	// �h���b�O�f�[�^�̍H���ŏc�T�C�Y�����肷��
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	GetItemRect(0, rectSingle, LVIR_BOUNDS);
	rectComplete = rectSingle;
	rectComplete.top = ptCursor.y;
	rectComplete.bottom = ptCursor.y + (rectSingle.Height() * (int)mDragData.indexes.size());

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
		dcMem.DeleteDC();
		return NULL;
	}

	CBitmap* pOldMemDCBitmap = dcMem.SelectObject(&Bitmap);
	// �w�i�h��Ԃ�
	dcMem.FillSolidRect(0, 0, rectComplete.Width(), rectComplete.Height(), mcDragBackColor);

	oldfont = dcMem.SelectObject(pfont);
	COLORREF oldcolor = dcMem.SetTextColor(mcDragTextColor);

	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	CString str;
	CRect rectItem, rectDraw;

	vector<int>::iterator itr;
	rectDraw = CRect(0,0,0,0);
	for(itr= mDragData.indexes.begin(); itr!= mDragData.indexes.end(); itr++){
		GetItemRect((*itr), rectItem, LVIR_BOUNDS);
		rectDraw.right = rectItem.Width();
		rectDraw.bottom += rectItem.Height();
		str = GetItemText((*itr), 0);
		dcMem.DrawText(str, rectDraw, DT_LEFT | DT_TOP);
		rectDraw.top = rectDraw.bottom;
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

/*============================================================================*/
/*! �O���[�v���X�g

-# �h���b�v�A�C�e��

@param		lpPoint	�ʒu���

@retval
*/
/*============================================================================*/
void CCustomGroupListCtrl::dropItem(CPoint point)
{
	if (isDropEnable(point) == false) {
		return;
	}

	SetRedraw(FALSE);

	// �悸�͑I�����ڂ̃m�[�h�����擾����
	vector<CTreeNode*> nodeList;
	vector<int>::iterator itr;
	for (itr = mDragData.indexes.begin(); itr != mDragData.indexes.end(); itr++) {
		CTreeNode* node = (CTreeNode*)GetItemData((*itr));
		nodeList.push_back(node);
	}

	// �h���b�v��A�C�e���̎擾
	int group = 0;
	UINT flags = 0;
	int index = HitTest(point, &flags);
	if (index >= 0) {
		// �h���b�v��̃m�[�h�����擾����
		CTreeNode* masternode = (CTreeNode*)GetItemData(index);
		group = HIWORD(masternode->GetManager().groupno);
		// �I�����ڂ��폜����
		POSITION pos;
		pos = GetFirstSelectedItemPosition();
		while (pos) {
			pos = GetFirstSelectedItemPosition();
			int item = GetNextSelectedItem(pos);
			DeleteItem(item);
		}

		// �h���b�v���ꂽ�A�C�e����ǉ�����
		vector<CTreeNode*>::iterator itrnode;
		UINT index = 1;
		for (itrnode = nodeList.begin(); itrnode != nodeList.end(); itrnode++) {
			(*itrnode)->GetManager().groupno = MAKELONG(LOWORD(masternode->GetManager().groupno) + index, HIWORD(masternode->GetManager().groupno));
			swprintf_s((*itrnode)->GetManager().groupname, mNameSize, _T("%s"), (LPCTSTR)masternode->GetManager().groupname);
			index++;
		}
	}
	else {
		// �O���[�v�w�b�_�[�Ƀh���b�v���ꂽ
		group = GroupHitTest(point);
		// �I�����ڂ��폜����
		POSITION pos;
		pos = GetFirstSelectedItemPosition();
		while (pos) {
			pos = GetFirstSelectedItemPosition();
			int item = GetNextSelectedItem(pos);
			DeleteItem(item);
		}
		// �h���b�v�悪�O���[�v�w�b�_�[
		UINT min;
		UINT max = GetGroupInner(group, min);
		vector<CTreeNode*>::iterator itrnode;
		UINT index = min - mGroupRange + 1;
		for (itrnode = nodeList.begin(); itrnode != nodeList.end(); itrnode++) {
			(*itrnode)->GetManager().groupno = MAKELONG(index, group);
			swprintf_s((*itrnode)->GetManager().groupname, mNameSize, _T("%s"), (LPCTSTR)getGroupHeader(group));
			index++;
		}
	}

	SetRedraw(TRUE);

	mParent->PostMessage(eUserMessage_Manager_Reset, (WPARAM)group, 1);
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �h���b�v�\���̃`�F�b�N

@param

@retval
*/
/*============================================================================*/
bool CCustomGroupListCtrl::isDropEnable(CPoint point)
{
	// �h���b�v��A�C�e���̎擾
	UINT flags = 0;
	int item = HitTest(point, &flags);

	int group;
	if (item >= 0) {
		// �h���b�v�悪���X�g����
		CTreeNode* pnode = (CTreeNode*)GetItemData(item);
		// �h���b�v��A�C�e���̃O���[�v�ԍ����擾����
		group = HIWORD(pnode->GetManager().groupno);
	}
	else {
		// �h���b�v�悪�O���[�v�w�b�_�[
		// �O���[�v�w�b�_�[�̃O���[�v�ԍ����擾����
		group = GroupHitTest(point);
	}

	if (group < 0)
		return false;

	// �h���b�O�A�C�e���̃O���[�v�ԍ��Ɣ�r���ď����𕪊򂷂�
	int count = GetGroupItemCount(group);

	// �h���b�O�F�O���[�v�Ȃ��A�h���b�v�F�O���[�v�Ȃ��@���@OK
	if (mDragData.group == 0 && group == 0)
		return true;

	// �h���b�v�悪�O���[�v����̏ꍇ�̓O���[�v���̃A�C�e�������m�F����
	if ((mDragData.indexes.size() + count) > mMaxEqSyncNum)
		return false;

	// �h���b�O�F�O���[�v�Ȃ��A�h���b�v�F�O���[�v����@���@OK
	if (mDragData.group == 0 && group != 0)
		return true;
	// �h���b�O�F�O���[�v����A�h���b�v�F�O���[�v����A�h���b�O�ƃh���b�v�̃O���[�v�������@���@OK
	if (mDragData.group != 0 && group != 0 && mDragData.group == group)
		return true;

	return false;
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �}�E�X�E�N���b�N

@param

@retval
*/
/*============================================================================*/
void CCustomGroupListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	UINT flags = 0;
	int item = HitTest(point, &flags);
	TRACE("# OnRButtonDown : RclickItem(%d)\n", item);
	if (item < 0) {
		for (int i = 0; i < GetGroupCount(); i++) {
			// �O���[�vREC�擾	
			CRect rect;
			if (GetGroupRect(i, rect, LVGGR_HEADER) == TRUE) {
				if (rect.PtInRect(ptCursor) == TRUE) {
					TRACE("# OnRButtonDown : GroupID(%d) OK\n", i);
				}
			}
		}
	}

	if (mpDragImage) {
		::ReleaseCapture();
		mpDragImage->DragLeave(CWnd::GetDesktopWindow());
		mpDragImage->EndDrag();

		delete mpDragImage;
		mpDragImage = NULL;
		SetItemState(-1, 0, LVIS_DROPHILITED);
	}

	CListCtrl::OnRButtonDown(nFlags, point);
}

/*============================================================================*/
/*! �O���[�v���X�g

-# �ʒu��񂩂�O���[�v���X�g�̏ꏊ���擾

@param

@retval
*/
/*============================================================================*/
int CCustomGroupListCtrl::GroupHitTest(const CPoint& point)
{
	if (!IsGroupViewEnabled())
		return -1;

	if (HitTest(point) != -1)
		return -1;

	if (IsGroupViewEnabled()){
		LRESULT groupCount = SNDMSG((m_hWnd), LVM_GETGROUPCOUNT, (WPARAM)0, (LPARAM)0);
		if (groupCount <= 0) {
			// �O���[�v���Ȃ�
			return -1;
		}

		for (int i = 0; i < groupCount; ++i){
			LVGROUP lg = { 0 };
			lg.cbSize = sizeof(lg);
			lg.mask = LVGF_GROUPID;

			VERIFY(SNDMSG((m_hWnd), LVM_GETGROUPINFOBYINDEX, (WPARAM)(i), (LPARAM)(&lg)));

			CRect rect(0, LVGGR_HEADER, 0, 0);
			VERIFY(SNDMSG((m_hWnd), LVM_GETGROUPRECT, (WPARAM)(lg.iGroupId), (LPARAM)(RECT*)(&rect)));

			if (rect.PtInRect(point)) {
				return lg.iGroupId;
			}
		}
		return -1;
	}

	if (GetItemCount() == 0) {
		return -1;
	}

	int nFirstRow = -1;
	CRect gridRect;
	GetWindowRect(&gridRect);
	for (CPoint pt = point; pt.y < gridRect.bottom; pt.y += 2){
		nFirstRow = HitTest(pt);
		if (nFirstRow != -1) {
			break;
		}
	}

	if (nFirstRow == -1) {
		return -1;
	}

	int nGroupId = getRowGroupId(nFirstRow);

	// Extra validation that the above row belongs to a different group
	int nAboveRow = GetNextItem(nFirstRow, LVNI_ABOVE);
	if (nAboveRow != -1 && nGroupId == getRowGroupId(nAboveRow)) {
		return -1;
	}

	return nGroupId;
}


void CCustomGroupListCtrl::PreSubclassWindow()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊��N���X���Ăяo���Ă��������B

	CListCtrl::PreSubclassWindow();

	//SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
	//SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	//SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);
	//SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES);
}

/*============================================================================*/
/*! �ݔ��ڍ׊Ǘ�

-# �O���[�v���ԍ��̍ő�l�A�ŏ��l���擾����

@param	nGroup		�O���[�v�ԍ�
@param	nMin		�O���[�v���ԍ��̍ŏ��l

@retval	�O���[�v���ԍ��̍ő�l
*/
/*============================================================================*/
UINT CCustomGroupListCtrl::GetGroupInner(UINT nGroup, UINT& min)
{
	UINT max = 0;
	min = 9999;
	for (UINT item = 0; item < (UINT)GetItemCount(); item++) {
		// �f�[�^�̃O���[�v�ԍ�����ΏۃJ�����̃e�L�X�g��ݒ肷��
		CTreeNode* pnode = (CTreeNode*)GetItemData(item);
		if (pnode->GetEquipment().kind == eTreeItemKind_User && HIWORD(pnode->GetManager().groupno) == nGroup) {
			min = __min(min, LOWORD(pnode->GetManager().groupno));
			max = __max(max, LOWORD(pnode->GetManager().groupno));
		}
	}
	return max;
}
