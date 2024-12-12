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
	, mSaveFileName(_T(""))
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
	DDX_Control(pDX, IDC_STATIC_ICON, mIcon);
}


BEGIN_MESSAGE_MAP(CCustomSelectSaveFile, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCustomSelectSaveFile::OnBnClickedOk)
	ON_BN_CLICKED(IDNO, &CCustomSelectSaveFile::OnBnClickedNo)
	ON_BN_CLICKED(IDCANCEL, &CCustomSelectSaveFile::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCustomSelectSaveFile メッセージ ハンドラー


BOOL CCustomSelectSaveFile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	HICON hIcon = LoadIcon(NULL, IDI_QUESTION);
	mIcon.SetIcon(hIcon);

	TCHAR xmlfile[_MAX_PATH];
	swprintf_s(xmlfile, _MAX_PATH, _T("%s"), (LPCTSTR)mSaveFileName);
	PathRemoveExtension(xmlfile);
	PathStripPath(xmlfile);
	mSaveFileName = CString(xmlfile);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CCustomSelectSaveFile::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (mSaveFileName.IsEmpty() == true) {
		MessageBox(mMessage_EmptyXmlFile, mMessage_Title_CustomDetail, MB_OK | MB_ICONERROR);
		return;
	}

	CString xmlfile = mSaveFileName + _T(".xml");

	CFileFind find;
	if (find.FindFile(mSavePathName + _T("\\") + xmlfile)){
		// 同名ファイルが存在する
		if (MessageBox(mMessage_SameXmlFile, mMessage_Title_CustomDetail, MB_YESNO | MB_ICONQUESTION) == IDNO) {
			return;
		}
	}

	CDialogEx::OnOK();
}


void CCustomSelectSaveFile::OnBnClickedNo()
{
	EndDialog(IDNO);
}


void CCustomSelectSaveFile::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}
