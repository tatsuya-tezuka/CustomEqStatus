
// CustomEqStatusDemoDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxbutton.h"
#include "resource.h"		// メイン シンボル


// CCustomEqStatusDemoDlg ダイアログ
class CCustomEqStatusDemoDlg : public CDialogEx
{
// コンストラクション
public:
	CCustomEqStatusDemoDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_CUSTOMDEMO_DIALOG };

	void	GetDemoFiles(vector<CString>& list);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
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
