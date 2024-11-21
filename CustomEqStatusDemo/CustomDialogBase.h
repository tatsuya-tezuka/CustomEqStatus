/******************************************************************************
* @file    CustomDialogBase.h
* @brief   �y�J�X�^�}�C�Y�@�\�z�_�C�A���O���N���X
* @auther  SCC
* @date    2024/08/26 �V�K�쐬
* @par
******************************************************************************/
#pragma once
#include "afxdialogex.h"

class CCustomDialogBase : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomDialogBase)
public:
	CCustomDialogBase(UINT IDD, CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CCustomDialogBase();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum{
		ANCHORE_LEFTTOP = 0x0000,	// ����͌Œ肳����
		ANCHORE_RIGHT = 0x0001,	// �E���͌Œ肳����
		ANCHORE_BOTTOM = 0x0002,	// �����͌Œ肳����
		RESIZE_HOR = 0x0004,	// ���̃��T�C�Y���\�ɂ���
		RESIZE_VER = 0x0008,	// �c�̃��T�C�Y���\�ɂ���
		RESIZE_BOTH = (RESIZE_HOR | RESIZE_VER),
	};

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	UINT	mIDD;
	bool	mModal;
	int		mMessageRet;

	// �R���g���[���̃��T�C�Y
	vector<DWORD>		mControl;
	int					mSaveCx, mSaveCy;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	/// �R���g���[���̓o�^
	void SetControlInfo(WORD id, WORD anchore)
	{
		if (anchore == ANCHORE_LEFTTOP)
			return;

		DWORD info = MAKELONG(id, anchore);
		mControl.push_back(info);
	}

	UINT	GetIDD() { return mIDD; }

	virtual BOOL OnProcSize(CWnd* pWnd, int dx, int dy) { return FALSE; }

protected:

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	virtual INT_PTR DoModal();
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
};

