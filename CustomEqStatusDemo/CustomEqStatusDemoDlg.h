
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

	void	GetDemoFiles(vector<CString>& list);

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
	afx_msg void OnBnClickedMfcbuttonManager();
	afx_msg void OnBnClickedMfcbuttonDetail();
	CMFCButton mManager;
	CMFCButton mDetail;
	UINT mLoop;
};
