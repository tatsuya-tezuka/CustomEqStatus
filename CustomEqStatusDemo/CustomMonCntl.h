#pragma once
#include "afxdialogex.h"
#include "CustomDialogBase.h"
#include "CustomMonCntlListCtrl.h"


// CCustomMonCntl ダイアログ

class CCustomMonCntl : public CCustomDialogBase
{
	DECLARE_DYNAMIC(CCustomMonCntl)

public:
	CCustomMonCntl(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CCustomMonCntl();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MONCNTL };
#endif
	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	enum { eType_Mon=1, eType_Cntl };

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	vector<CString>	mDbData;

	// コントロールのリサイズ
	vector<DWORD>		mControl;
	int					mSaveCx, mSaveCy;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	void	createControl();
	void	createTreeData();
	void	createTreeMon(HTREEITEM hMon);
	void	createTreeCntl(HTREEITEM hCntl);

	void	_AddData();
	void	_SetControlInfo(WORD id, WORD anchore);

	/// ドラッグ＆ドロップ
	static BOOL CALLBACK messageDragDrop(UINT message, LPARAM lParam1, LPARAM lParam2, LPARAM lParam3);
	BOOL	_IsDragDropSelect(LPARAM lParam1, LPARAM lParam2, LPARAM lParam3);
	void	_DropItem(LPARAM lParam1, LPARAM lParam2, LPARAM lParam3);
	int		_GetIndex(LPARAM lParam1, LPARAM lParam2, LPARAM lParam3);

	/// カラムの自動サイズ調整
	void _AutoSizeColumns(CListCtrl& list, int col = -1)
	{
		list.SetRedraw(FALSE);
		CHeaderCtrl* pHeader = (CHeaderCtrl*)list.GetDlgItem(0);
		int nColumnCount = pHeader->GetItemCount();

		for (int i = 0; i < nColumnCount; i++)
		{
			list.SetColumnWidth(i, LVSCW_AUTOSIZE);
			int nColumnWidth = list.GetColumnWidth(i);
			list.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
			int nHeaderWidth = list.GetColumnWidth(i);
			list.SetColumnWidth(i, max(nColumnWidth, nHeaderWidth));
		}
		list.SetRedraw(TRUE);
	}
	/// リスト項目の登録
	int _AddItem(CListCtrl& list, int nItem, int nSubItem, CString strItem, DWORD dwParam = 0, int nImage = -1)
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
			return list.InsertItem(&lv);
		return list.SetItem(&lv);
	}

	bool WildcardMatch(const TCHAR* pszString, const TCHAR* pszMatch)
	{
		while (*pszMatch)
		{
			if (*pszMatch == _T('?'))
			{
				if (!*pszString)
					return false;

				++pszString;
				++pszMatch;
			}
			else if (*pszMatch == _T('*'))
			{
				if (WildcardMatch(pszString, pszMatch + 1))
					return true;

				if (*pszString && WildcardMatch(pszString + 1, pszMatch))
					return true;

				return false;
			}
			else
			{
				if (::CharUpper(MAKEINTRESOURCE(MAKELONG(*pszString++, 0))) != ::CharUpper(MAKEINTRESOURCE(MAKELONG(*pszMatch++, 0))))
					return false;
			}
		}

		return !*pszString && !*pszMatch;
	}

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl mTreeData;
	CCustomMonCntlListCtrl mListData;
	CEdit mEditSearch;
	CString mSearchText;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTreeData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnGetdispinfoListData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditSearch();
};
