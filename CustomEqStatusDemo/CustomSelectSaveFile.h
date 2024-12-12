#pragma once
#include "afxdialogex.h"


// CCustomSelectSaveFile ダイアログ

class CCustomSelectSaveFile : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomSelectSaveFile)

public:
	CCustomSelectSaveFile(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CCustomSelectSaveFile();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELECTSAVEFILE };
#endif

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
	CString		mSavePathName;
	CString		mSaveFileName;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CString		GetSavePathName() { return mSavePathName; }
	CString		GetSaveFileName() { return mSaveFileName; }
	void		SetSavePathName(CString name) { mSavePathName = name; }
	void		SetSaveFileName(CString name) { mSaveFileName = name; }

protected:

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CEdit mSaveFileCntl;
	afx_msg void OnBnClickedNo();
	afx_msg void OnBnClickedCancel();
	CStatic mIcon;
};
