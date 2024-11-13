// CustomGroupName.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "afxdialogex.h"
#include "CustomGroupName.h"


// CCustomGroupName ダイアログ

IMPLEMENT_DYNAMIC(CCustomGroupName, CDialogEx)

CCustomGroupName::CCustomGroupName(CString strName, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_GROUPNAME, pParent)
	, mGroupNameText(_T(""))
{
	mGroupName = strName;
}

CCustomGroupName::~CCustomGroupName()
{
}

void CCustomGroupName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_GROUPNAME, mGroupEdit);
	DDX_Text(pDX, IDC_EDIT_GROUPNAME, mGroupNameText);
	DDV_MaxChars(pDX, mGroupNameText, 64);
}


BEGIN_MESSAGE_MAP(CCustomGroupName, CDialogEx)
END_MESSAGE_MAP()


// CCustomGroupName メッセージ ハンドラー


BOOL CCustomGroupName::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	mGroupNameText = mGroupName;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CCustomGroupName::OnOK()
{
	UpdateData(TRUE);
	mGroupName = mGroupNameText;

	CDialogEx::OnOK();
}
