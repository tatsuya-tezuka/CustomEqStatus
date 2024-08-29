#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomGroupListCtrl.h"


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
END_MESSAGE_MAP()

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
		return TRUE;
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
		lplvcd->clrText = pnode->GetColor().text;
		if (pnode->GetWindowInfo().wnd == NULL || pnode->GetWindowInfo().wnd->IsWindowVisible() == FALSE)
			lplvcd->clrText = mManagerHideColor;// GetSysColor(COLOR_GRAYTEXT);
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		*pResult = CDRF_NOTIFYPOSTPAINT + CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM:
		lplvcd->clrTextBk = pnode->GetColor().textback;
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
void CCustomGroupListCtrl::CreateGroupControl()
{
	// �g���X�^�C���̎擾
	DWORD  dwStyle = ListView_GetExtendedListViewStyle(m_hWnd);
	//  �g���X�^�C���̃Z�b�g
	dwStyle &= ~LVS_TYPEMASK;
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_DOUBLEBUFFER;
	//  �V�����g���X�^�C����ݒ�
	ListView_SetExtendedListViewStyle(m_hWnd, dwStyle);

	SetFont(&mListFont);

	int width = 0;
	for (int i = 0; i < sizeof(mGroupListHeader) / sizeof(mGroupListHeader[0]); i++) {
		switch (i) {
		case	eManagerTitle:	width = 300; break;
		case	eManagerNote:	width = 200; break;
		case	eManagerGroup:	width = 0; break;
		}
		InsertColumn(i, mGroupListHeader[i], LVCFMT_LEFT, width);
	}
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
	CWaitCursor waitCursor;

	SetRedraw(FALSE);

	RemoveAllGroups();

	//EnableGroupView(GetItemCount() > 0);
	EnableGroupView(bEnableGroup);

	if (IsGroupViewEnabled()){
		// �O���[�v�\���̏ꍇ

		CSimpleMap<CString, CSimpleArray<int> > groups;
		vector<CString> footers;
		footers.clear();

		// �e�A�C�e���̃O���[�v�ݒ�
		for (int nRow = 0; nRow < GetItemCount(); ++nRow){
			// �f�[�^�̃O���[�v�ԍ�����ΏۃJ�����̃e�L�X�g��ݒ肷��
			CTreeNode* pnode = (CTreeNode*)GetItemData(nRow);
			if (pnode->GetWindowInfo().groupno == 0){
				SetItemText(nRow, nCol, _T("No Group"));
			}
			else{
				CString str;
				str.Format(_T("Group_%d"), pnode->GetWindowInfo().groupno);
				SetItemText(nRow, nCol, str);
			}
			// �A�C�e�������񂩂�O���[�v���쐬����
			CString cellText = GetItemText(nRow, nCol);

			int nGroupId = groups.FindKey(cellText);
			if (nGroupId == -1){
				// �O���[�v�����݂��Ȃ��̂ŃO���[�v�Ƃ��Đݒ肷��
				CSimpleArray<int> rows;
				groups.Add(cellText, rows);
				nGroupId = groups.FindKey(cellText);
			}
			groups.GetValueAt(nGroupId).Add(nRow);

			CString strFooter = cellText;
			if (footers.size() < (nGroupId + 1)){
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
			setGroupFooter(nGroupId, footers[nGroupId]);

			for (int groupRow = 0; groupRow < groupRows.GetSize(); ++groupRow){
				VERIFY(setRowGroupId(groupRows[groupRow], nGroupId));
			}
		}
		SetColumnWidth(nCol, 0);
		SetRedraw(TRUE);
		Invalidate(FALSE);
		return TRUE;
	}

	SetRedraw(TRUE);
	Invalidate(FALSE);
	return FALSE;
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
BOOL CCustomGroupListCtrl::setGroupFooter(int nGroupID, const CString& footer, DWORD dwAlign /*= LVGA_FOOTER_CENTER*/)
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

-# �}�E�X�ɂ��w�b�_�[���ύX

@param
@retval

*/
/*============================================================================*/
void CCustomGroupListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	int nCol = (int)phdr->iItem;
	if (nCol == eManagerGroup){
		*pResult = 1;
	}
	else{
		*pResult = 0;
	}
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
	if (nCol == eManagerGroup){
		*pResult = 1;
	}
	else{
		AutoSizeColumns(nCol);
		*pResult = 0;
	}

	*pResult = 0;
}
