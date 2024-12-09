
// CustomEqStatusDemoDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomEqStatusDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCustomEqStatusDemoDlg ダイアログ



CCustomEqStatusDemoDlg::CCustomEqStatusDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCustomEqStatusDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCustomEqStatusDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCustomEqStatusDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON_LOAD, &CCustomEqStatusDemoDlg::OnBnClickedMfcbuttonLoad)
	ON_BN_CLICKED(IDC_MFCBUTTON__SAVE, &CCustomEqStatusDemoDlg::OnBnClickedMfcbuttonSave)
	ON_BN_CLICKED(IDC_MFCBUTTON__MANAGER, &CCustomEqStatusDemoDlg::OnBnClickedMfcbutton)
	ON_BN_CLICKED(IDC_BUTTON_NODEOUTPUT, &CCustomEqStatusDemoDlg::OnBnClickedButtonNodeoutput)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CCustomEqStatusDemoDlg メッセージ ハンドラー

LRESULT CCustomEqStatusDemoDlg::OnNcHitTest(CPoint point)
{
	LRESULT nHitTest = CDialog::OnNcHitTest(point);
	if (nHitTest == HTCLIENT)    // もしクライアント領域だったら、
		return HTCAPTION;         // キャプションです、と返す
	else
		return nHitTest;
	//return CDialogEx::OnNcHitTest(point);
}

BOOL CCustomEqStatusDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// カスタマイズ機能画面の作成
	createCustomControl();
	CenterWindow();
	CRect rect;
	GetWindowRect(rect);

	RECT mrect;
	MONITORINFOEX info;
	int height = rect.Height();
	rect.top = 0;
	if (theApp.GetCustomControl().GetCustomMonitor().GetMonitor(0, &mrect, &info) == true) {
		rect.top = info.rcWork.bottom - height;
	}
	::SetWindowPos(m_hWnd, HWND_TOP, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);

	GetDlgItem(IDC_MFCBUTTON__MANAGER)->SetWindowText(mMessage_Title_CustomManager);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CCustomEqStatusDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CCustomEqStatusDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CCustomEqStatusDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*============================================================================*/
/*! メイン画面

-# レイアウト呼出

@param

@retval
*/
/*============================================================================*/
void CCustomEqStatusDemoDlg::OnBnClickedMfcbuttonLoad()
{
	// 確認メッセージ
	if (MessageBox(mMessage_LoadLayout, mMessage_Title_CustomDetail, MB_YESNO | MB_ICONQUESTION) == IDNO) {
		return;
	}

	const TCHAR BASED_CODE szFilter[] = _T("Station Control Layout(*.scl)|*.scl|");
	CFileDialog dlg(TRUE, _T("xml"), NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() != IDOK)
		return;

#if _DEMO_PHASE < 100
	return;
#endif

	CFile file;
	if (file.Open(dlg.GetPathName(), CFile::modeRead | CFile::typeBinary) == NULL) {
		return;
	}
	CArchive mArc(&file, CArchive::load);

	bool ret;

	try
	{
		theApp.GetCustomControl().GetDataManager().LoadCustomLayout(mArc);
		ret = true;
	}
	catch (CArchiveException* e)
	{
		e->Delete();
		ret = false;
	}
	catch (CFileException* e)
	{
		e->Delete();
		ret = false;
	}
	catch (...) {
		ret = false;
	}

	mArc.Flush();
	file.Close();
}

/*============================================================================*/
/*! メイン画面

-# レイアウト保存

@param

@retval
*/
/*============================================================================*/
void CCustomEqStatusDemoDlg::OnBnClickedMfcbuttonSave()
{
	// 確認メッセージ
	bool isedit = theApp.GetCustomControl().GetDataManager().IsVisibleEditMode();
	if (isedit == true) {
		if (MessageBox(mMessage_SaveLayout, mMessage_Title_CustomDetail, MB_YESNO | MB_ICONQUESTION) == IDNO) {
			return;
		}
	}

	const TCHAR BASED_CODE szFilter[] = _T("Station Control Layout(*.scl)|*.scl|");
	CFileDialog dlg(FALSE, _T("xml"), NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() != IDOK)
		return;

#if _DEMO_PHASE < 100
	return;
#endif

	CFile file;
	if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) == NULL) {
		return;
	}
	CArchive mArc(&file, CArchive::store);

	bool ret = false;

	try
	{
		theApp.GetCustomControl().GetDataManager().SaveCustomLayout(mArc);
		ret = true;
	}
	catch (CArchiveException* e)
	{
		e->Delete();
		ret = false;
	}
	catch (CFileException* e)
	{
		e->Delete();
		ret = false;
	}
	catch (...) {
		ret = false;
	}

	mArc.Flush();
	file.Close();

	if (ret == false) {
		DeleteFile(dlg.GetPathName());
	}
}

/*============================================================================*/
/*! メイン画面

-# カスタム管理画面の呼出

@param

@retval
*/
/*============================================================================*/
void CCustomEqStatusDemoDlg::OnBnClickedMfcbutton()
{
	createCustomControl();
}

/*============================================================================*/
/*! メイン画面

-# カスタマイズ機能画面の作成

@param

@retval
*/
/*============================================================================*/
void CCustomEqStatusDemoDlg::createCustomControl()
{
	// カスタム管理画面の作成
	theApp.GetCustomControl().GetCustomManager().ShowWindow(SW_SHOW);
}


void CCustomEqStatusDemoDlg::OnBnClickedButtonNodeoutput()
{
	CString msg;
	vector<CTreeNode*>::iterator itr;
	for (itr = theApp.GetCustomControl().GetDataManager().GetTreeNode().begin(); itr != theApp.GetCustomControl().GetDataManager().GetTreeNode().end(); itr++) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		msg.Format(_T("%s"), (*itr)->GetEquipment().title);
		CLogTraceEx::Write(_T("***"), _T("***"), _T("***"), _T("*"), msg, nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		PrintChild((*itr));
	}

	// 編集用リスト内に指定ウィンドウが存在する場合は編集用を使用する
	map<CWnd*, CTreeNode*>::iterator itre;
	for (itre = theApp.GetCustomControl().GetDataManager().GetEditTreeNode().begin(); itre != theApp.GetCustomControl().GetDataManager().GetEditTreeNode().end(); itre++) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		msg.Format(_T("%s"), (*itre).second->GetEquipment().title);
		CLogTraceEx::Write(_T("###"), _T("###"), _T("###"), _T("#"), msg, nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		PrintChildEdit((*itre).second);
	}
}

void CCustomEqStatusDemoDlg::PrintChild(CTreeNode* pnode)
{
	CString msg;
	vector<CTreeNode*>::iterator itr;
	for (itr = pnode->GetChildren().begin(); itr != pnode->GetChildren().end(); itr++) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		msg.Format(_T("%s\t%s\t%s\t%s (%d)"), (*itr)->GetMonCtrl().display, (*itr)->GetMonCtrl().mname, (*itr)->GetMonCtrl().cname, (*itr)->GetMonCtrl().unit, (*itr)->GetEquipment().sortno);
		CString sep = _T("*");
		switch ((*itr)->GetEquipment().type) {
		case	eTreeItemType_Main:	sep = _T("**"); break;
		case	eTreeItemType_Sub:	sep = _T("***"); break;
		case	eTreeItemType_Item:	sep = _T("****"); break;
		}
		CLogTraceEx::Write(_T("***"), _T("***"), _T("***"), sep, msg, nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		PrintChild((*itr));
	}
}

void CCustomEqStatusDemoDlg::PrintChildEdit(CTreeNode* pnode)
{
	CString msg;
	vector<CTreeNode*>::iterator itr;
	for (itr = pnode->GetChildren().begin(); itr != pnode->GetChildren().end(); itr++) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		msg.Format(_T("%s\t%s\t%s\t%s (%d)"), (*itr)->GetMonCtrl().display, (*itr)->GetMonCtrl().mname, (*itr)->GetMonCtrl().cname, (*itr)->GetMonCtrl().unit, (*itr)->GetEquipment().sortno);
		CString sep = _T("*");
		switch ((*itr)->GetEquipment().type) {
		case	eTreeItemType_Main:	sep = _T("##"); break;
		case	eTreeItemType_Sub:	sep = _T("###"); break;
		case	eTreeItemType_Item:	sep = _T("####"); break;
		}
		CLogTraceEx::Write(_T("###"), _T("###"), _T("###"), sep, msg, nLogEx::debug);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		PrintChildEdit((*itr));
	}
}
