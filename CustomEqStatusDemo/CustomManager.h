/******************************************************************************
* @file    CustomManager.h
* @brief   【カスタマイズ機能】設備状況管理クラス
* @auther  SCC
* @date    2024/08/26 新規作成
* @par
******************************************************************************/
#pragma once

#include "CustomDialogBase.h"
#include "afxcmn.h"
#include "CustomGroupListCtrl.h"
#include "CustomSynchroWindow.h"
#include "CustomGroupName.h"

// CCustomManager ダイアログ

class CCustomManager : public CCustomDialogBase
{
	DECLARE_DYNAMIC(CCustomManager)

public:
	CCustomManager(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CCustomManager();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_MANAGER };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum { eSelectUser = 0, eSelectMaster };

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	CCustomSynchroWindow		mSyncWindow;

	/// ツールチップ
	CString		mToolText;

	/// メニューグループID
	int			mMenuSelectGroupID;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CCustomSynchroWindow& GetCustomSyncWindow() { return mSyncWindow; }
	void	UpdateGroup(UINT nGroupId=0, CPoint *point=NULL);
	void	ResetGroupInnerNo();
	void	ShowGroup();
	bool	IsGroupVisible(UINT nGroup);

	void LoadRect(CArchive& ar, RECT& rect)
	{
		ar >> rect.left;
		ar >> rect.top;
		ar >> rect.right;
		ar >> rect.bottom;
	}
	void LoadPoint(CArchive& ar, POINT& point) const
	{
		ar >> point.x;
		ar >> point.y;
	}
	bool Load(CArchive& ar)
	{
		WINDOWPLACEMENT wp;
		memset(&wp, 0, sizeof(WINDOWPLACEMENT));
		wp.length = sizeof(WINDOWPLACEMENT);

		try
		{
			ar >> wp.flags;
			ar >> wp.showCmd;
			LoadPoint(ar, wp.ptMinPosition);
			LoadPoint(ar, wp.ptMaxPosition);
			LoadRect(ar, wp.rcNormalPosition);
			SetWindowPlacement(&wp);
		}
		catch (CArchiveException* e)
		{
			e->Delete();
			return false;
		}
		return true;
	}

	void SaveRect(CArchive& ar, RECT& rect) const
	{
		ar << rect.left;
		ar << rect.top;
		ar << rect.right;
		ar << rect.bottom;
	}
	void SavePoint(CArchive& ar, POINT& point) const
	{
		ar << point.x;
		ar << point.y;
	}
	bool Save(CArchive& ar) const
	{
		WINDOWPLACEMENT wp;
		memset(&wp, 0, sizeof(WINDOWPLACEMENT));
		wp.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(&wp);

		try
		{
			ar << wp.flags;
			ar << wp.showCmd;
			SavePoint(ar, wp.ptMinPosition);
			SavePoint(ar, wp.ptMaxPosition);
			SaveRect(ar, wp.rcNormalPosition);
		}
		catch (CArchiveException* e)
		{
			e->Delete();
			return false;
		}
		return true;
	}

protected:
	void	createItem(UINT nSelect, bool bResetInner=false);
	void	createEquipment();
	void	createEqDetail(CTreeNode* node = NULL);
	void	updateMenuItemStatus(CMenu* pMenu, bool bUser, bool bGroup, bool bNoGroup);
	void	updateItemData(LPARAM lParam);
	void	updateXmlFile();
	void	showCustomDetail(int nItem, CPoint point);
	UINT	GetGroupMaxNo();
	UINT	GetGroupInnerMaxNo();
	bool	IsSameGroupName(CString groupName);
	void	SyncEditMode();

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	int mSelectType;
	CCustomGroupListCtrl mManagerList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioUser();
	afx_msg void OnBnClickedRadioMaster();
	afx_msg void OnNMRClickListManager(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListManager(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnManagerNew();
	afx_msg void OnManagerDelete();
	afx_msg void OnManagerShow();
	afx_msg void OnManagerCreate();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnClose();
	afx_msg void OnMenumanagerClose();
	afx_msg void OnLvnGetInfoTipListManager(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMangroupRename();
	afx_msg void OnMangroupReset();
	afx_msg void OnMangroupShow();
};
