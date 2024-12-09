
// CustomEqStatusDemoDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxbutton.h"
#include "resource.h"		// メイン シンボル
#include "CustomDataManager.h"

// CCustomEqStatusDemoDlg ダイアログ
class CCustomEqStatusDemoDlg : public CDialogEx
{
// コンストラクション
public:
	CCustomEqStatusDemoDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_CUSTOMDEMO_DIALOG };

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	void	createCustomControl();

	void	PrintChild(CTreeNode* pnode);
	void	PrintChildEdit(CTreeNode* pnode);

	/* ------------------------------------------------------------------------------------ */


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
	afx_msg void OnBnClickedMfcbuttonLoad();
	afx_msg void OnBnClickedMfcbuttonSave();
	afx_msg void OnBnClickedMfcbutton();
	afx_msg void OnBnClickedButtonNodeoutput();
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
