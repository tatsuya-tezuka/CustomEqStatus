#pragma once
#include "afxdialogex.h"


// CCustomGroupName ダイアログ

class CCustomGroupName : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomGroupName)

public:
	CCustomGroupName(CString strName, CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CCustomGroupName();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GROUPNAME };
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
	CString			mGroupName;

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CEdit mGroupEdit;
	CString mGroupNameText;
};
