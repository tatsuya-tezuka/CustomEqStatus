/******************************************************************************
* @file    CustomManager.h
* @brief   �y�J�X�^�}�C�Y�@�\�z�ݔ��󋵊Ǘ��N���X
* @auther  SCC
* @date    2024/08/26 �V�K�쐬
* @par
******************************************************************************/
#pragma once

#include "CustomDialogBase.h"
#include "afxcmn.h"
#include "CustomGroupListCtrl.h"
#include "CustomSynchroWindow.h"

// CCustomManager �_�C�A���O

class CCustomManager : public CCustomDialogBase
{
	DECLARE_DYNAMIC(CCustomManager)

public:
	CCustomManager(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CCustomManager();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_MANAGER };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum { eSelectUser = 0, eSelectMaster };

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	CCustomSynchroWindow		mSyncWindow;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CCustomSynchroWindow& GetCustomSyncWindow() { return mSyncWindow; }

protected:
	void	createItem(UINT nSelect);
	void	createEquipment();
	void	updateGroup();
	void	createEqDetail(CTreeNode* node = NULL);
	void	updateMenuItemStatus(CMenu* pMenu);

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
	afx_msg void OnManagerMonitor();
	afx_msg void OnManagerEdit();
	afx_msg void OnManagerShow();
	afx_msg void OnManagerHide();
	afx_msg void OnManagerCreate();
	afx_msg void OnManagerCancel();
	afx_msg void OnManagerLoad();
	afx_msg void OnManagerSave();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
