
// CustomEqStatusDemoDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxbutton.h"
#include "resource.h"		// ���C�� �V���{��

// CCustomEqStatusDemoDlg �_�C�A���O
class CCustomEqStatusDemoDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CCustomEqStatusDemoDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CUSTOMDEMO_DIALOG };

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
	void	createCustomControl();

	/* ------------------------------------------------------------------------------------ */


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMfcbuttonLoad();
	afx_msg void OnBnClickedMfcbuttonSave();
	afx_msg void OnBnClickedMfcbutton();
	afx_msg void OnBnClickedButtonMoncntl();
	afx_msg void OnBnClickedButtonCustom();
};
