// CustomSelectSaveFile.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "afxdialogex.h"
#include "CustomSelectSaveFile.h"


// CCustomSelectSaveFile ダイアログ

IMPLEMENT_DYNAMIC(CCustomSelectSaveFile, CDialogEx)

CCustomSelectSaveFile::CCustomSelectSaveFile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SELECTSAVEFILE, pParent)
	, mSaveFileName(_T("default.xml"))
{

}

CCustomSelectSaveFile::~CCustomSelectSaveFile()
{
}

void CCustomSelectSaveFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SAVEFILE, mSaveFileName);
	DDV_MaxChars(pDX, mSaveFileName, 100);
	DDX_Control(pDX, IDC_EDIT_SAVEFILE, mSaveFileCntl);
}


BEGIN_MESSAGE_MAP(CCustomSelectSaveFile, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCustomSelectSaveFile::OnBnClickedOk)
END_MESSAGE_MAP()


// CCustomSelectSaveFile メッセージ ハンドラー


BOOL CCustomSelectSaveFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CCustomSelectSaveFile::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (mSaveFileName.Right(4).CompareNoCase(_T(".xml")) != 0) {
		mSaveFileName += _T(".xml");
	}

	CFileFind find;
	if (find.FindFile(mSavePathName + _T("\\") + mSaveFileName))
	{
		// 同名ファイルが存在する
		MessageBox(_T("ファイルが既に存在します。\n確認してください。"), 0, MB_OK | MB_ICONERROR);
		return;
	}

	CDialogEx::OnOK();
}
