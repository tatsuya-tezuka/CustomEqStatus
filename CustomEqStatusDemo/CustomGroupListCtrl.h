/******************************************************************************
* @file    CustomGroupListCtrl.h
* @brief   �y�J�X�^�}�C�Y�@�\�z�ݔ��󋵊Ǘ���ʗp�O���[�v���X�g�R���g���[��
* @auther  SCC
* @date    2024/08/26 �V�K�쐬
* @par
******************************************************************************/
#pragma once
#include "afxcmn.h"

class CCustomGroupHeaderCtrl : public CHeaderCtrl
{
public:
	CCustomGroupHeaderCtrl();
	virtual ~CCustomGroupHeaderCtrl();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */

	DECLARE_MESSAGE_MAP()

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

class CCustomGroupListCtrl : public CListCtrl
{
public:
	CCustomGroupListCtrl();
	virtual ~CCustomGroupListCtrl();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	CFont	mListFont;

	/// �h���b�O���h���b�v�֘A

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	CreateGroupControl();
	BOOL	GroupByColumn(int nCol, BOOL bEnableGroup=TRUE);

	/// �J�����̎����T�C�Y����
	void AutoSizeColumns(int col = -1)
	{
		SetRedraw(FALSE);
		CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
		int mincol = col < 0 ? 0 : col;
		int maxcol = col < 0 ? pHeader->GetItemCount() : col;

		for (col = mincol; col <= maxcol; col++){
			SetColumnWidth(col, LVSCW_AUTOSIZE);
			int nColumnWidth = GetColumnWidth(col);
			SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
			int nHeaderWidth = GetColumnWidth(col);
			SetColumnWidth(col, max(nColumnWidth, nHeaderWidth));
		}
		SetRedraw(TRUE);
	}
	/// ���X�g���ڂ̓o�^
	int AddItem(int nItem, int nSubItem, CString strItem, DWORD dwParam = 0, int nImage = -1)
	{
		LV_ITEM lv;
		memset(&lv, 0, sizeof(lv));
		lv.mask = LVIF_TEXT;
		lv.iItem = nItem;
		lv.iSubItem = nSubItem;
		if (nSubItem == 0) {
			lv.mask |= LVIF_PARAM;
			lv.lParam = dwParam;
		}
		if (nImage >= 0) {
			lv.mask |= LVIF_IMAGE;
			lv.iImage = nImage;
		}
		lv.pszText = (LPTSTR)(LPCTSTR)strItem;
		lv.cchTextMax = strItem.GetLength();

		if (nSubItem == 0)
			return InsertItem(&lv);
		return SetItem(&lv);
	}

protected:
	LRESULT	insertGroupHeader(int nIndex, int nGroupId, const CString& strHeader, DWORD dwState = LVGS_NORMAL, DWORD dwAlign = LVGA_HEADER_LEFT);
	BOOL	setGroupTask(int nGroupID, const CString& task);
	BOOL	setGroupSubtitle(int nGroupID, const CString& subtitle);
	BOOL	setGroupFooter(int nGroupID, const CString& footer, DWORD dwAlign = LVGA_FOOTER_CENTER);
	BOOL	setRowGroupId(int nRow, int nGroupId);

	/* ------------------------------------------------------------------------------------ */

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult);
};
