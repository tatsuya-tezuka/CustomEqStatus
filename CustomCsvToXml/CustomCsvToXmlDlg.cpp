
// CustomCsvToXmlDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CustomCsvToXml.h"
#include "CustomCsvToXmlDlg.h"
#include "afxdialogex.h"
#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNode::CNode(UINT level, const CString name)
: mLevel(level)
, mName(name)
, mChildren()
{
}

CNode::~CNode()
{
	for (vector<CNode*>::iterator i = mChildren.begin(); i != mChildren.end(); i++) {
		delete* i;
	}
}

CNode* CNode::createChildIfNotExist(const CString childName)
{
	for (vector<CNode*>::const_iterator i = mChildren.begin(); i != mChildren.end(); i++) {
		CNode* child = *i;
		if (child->getName() == childName) {
			return child;
		}
	}
	CNode* child = new CNode(mLevel + 1, childName);
	mChildren.push_back(child);
	return child;
}

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


// CCustomCsvToXmlDlg ダイアログ



CCustomCsvToXmlDlg::CCustomCsvToXmlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCustomCsvToXmlDlg::IDD, pParent)
	, mCsvFileName(_T(""))
	, mSavePath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCustomCsvToXmlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_CSV, mCSV);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_CSV, mCsvFileName);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_PATH, mSavePath);
}

BEGIN_MESSAGE_MAP(CCustomCsvToXmlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONV, &CCustomCsvToXmlDlg::OnBnClickedButtonConv)
END_MESSAGE_MAP()


// CCustomCsvToXmlDlg メッセージ ハンドラー

BOOL CCustomCsvToXmlDlg::OnInitDialog()
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

	static LPCTSTR fileFilter = _T("タブ区切りテキスト (*.txt)|*.txt||");
	DWORD flag = 6UL | OFN_OVERWRITEPROMPT;
	mCSV.EnableFileBrowseButton(NULL, fileFilter);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CCustomCsvToXmlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCustomCsvToXmlDlg::OnPaint()
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
HCURSOR CCustomCsvToXmlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCustomCsvToXmlDlg::OnBnClickedButtonConv()
{
	CWaitCursor wait;

	UpdateData(TRUE);

	TCHAR* ploc = _wsetlocale(LC_ALL, _T("japanese"));

	CStdioFile	file(mCsvFileName, CFile::modeRead | CFile::typeText);

	CString	cbuf, str;
	CString title, mainnode, subnode, leaf, mon, unit, cntl;
	BOOL bRead = file.ReadString(cbuf);
	CNode* root = new CNode(0);
	CNode* cur[6];
	title.Empty();
	while (bRead) {
		UINT pos = 0;
		while (AfxExtractSubString(str, cbuf, pos++, '\t')) {
			switch (pos) {
			case	1:
				if (title.IsEmpty() == false && title != str) {
					CreateXML(mSavePath, root);
					delete root;
					root = new CNode(0);
				}
				title = str;
				cur[0] = root->createChildIfNotExist(title);
				break;
			case	2:
				mainnode = str;
				cur[1] = cur[0]->createChildIfNotExist(mainnode);
				break;
			case	3:
				subnode = str;
				cur[2] = cur[1]->createChildIfNotExist(subnode);
				break;
			case	4:
				leaf = str;
				cur[3] = cur[2]->createChildIfNotExist(leaf);
				break;
			case	5:
				mon = str;
				cur[3]->mMon = mon;
				break;
			case	6:
				unit = str;
				cur[3]->mUnit = unit;
				break;
			case	7:
				cntl = str;
				cur[3]->mCntl = cntl;
				break;
			}
		}
		bRead = file.ReadString(cbuf);
	}
	file.Close();

	CreateXML(mSavePath, root);
}

void CCustomCsvToXmlDlg::CreateXML(CString xmlpath, CNode* root)
{
	CString	decl = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	CMarkup xml(decl);

	// 改行コードを設定
	xml.SetEOL(MCD_T("\n"));
	xml.SetEOLLEN(sizeof(MCD_T("\n")) / sizeof(MCD_CHAR)-1);

	// BOMを未出力
	xml.SetBOM(false);

	xml.SetDocFlags(CMarkup::MDF_UTF8PREAMBLE);

	// <ROOT>を出力
	xml.AddElem(_T("ROOT"));
	xml.IntoElem();
	xml.AddElem(_T("VERSION"), 100);
	xml.AddElem(_T("SIZE"), (UINT)1);

	vector<CNode*>::iterator itr;
	CString strTitle = _T("");
	for (itr = root->getChildren().begin(); itr != root->getChildren().end(); itr++) {
		if (strTitle.IsEmpty()) {
			strTitle = (*itr)->getName();
		}
		xml.AddElem(_T("EQUIPMENT"));
		xml.IntoElem();
		SaveNodeXml(xml, (*itr));
		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.Save(xmlpath + _T("\\") + strTitle + _T(".xml"));
}

bool CCustomCsvToXmlDlg::SaveNodeXml(CMarkup& xml, CNode* cur)
{
	// ウィンドウ位置情報取得
	// ウィンドウ情報
	xml.AddElem(_T("WINDOWINFO"));
	xml.IntoElem();
	xml.AddElem(_T("MODE"), 0);
	xml.AddElem(_T("KIND"), 0);
	xml.AddElem(_T("TYPE"), cur->getLevel());
	if (cur->getLevel() == 1) {
		xml.AddElem(_T("TITLE"), cur->getName());
		xml.AddElem(_T("GROUP"), _T("グループなし"));
		xml.AddElem(_T("MONITOR"), 0);
		xml.AddElem(_T("FLAGS"), 0);
		xml.AddElem(_T("SHOWCMD"), 1);
		_SavePointXml(xml, CPoint(-1, -1));
		_SavePointXml(xml, CPoint(-1, -1));
		_SaveRectXml(xml, CRect(321, 374, 873, 1000));
		for (int i = 0; i < 5; i++) {
			CString str;
			str.Format(_T("HWIDTH%d"), i + 1);
			xml.AddElem(str, 100);
		}
		xml.AddElem(_T("ZORDER"), 1);
	}
	xml.AddElem(_T("TREEOPEN"), 32);
	xml.OutOfElem();

	// 監視制御情報
	xml.AddElem(_T("MONCTRLINFO"));
	xml.IntoElem();
	xml.AddElem(_T("DISPLAY"), cur->getName());
	if (cur->getLevel() == 4) {
		xml.AddElem(_T("MONNAME"), cur->mMon);
		xml.AddElem(_T("CTRLNAME"), cur->mCntl);
		xml.AddElem(_T("UNIT"), cur->mUnit);
		xml.AddElem(_T("FORMATTYPE"), 0);
		xml.AddElem(_T("FORMAT"), 0);
	}
	xml.OutOfElem();

	// 色情報
	xml.AddElem(_T("COLORINFO"));
	xml.IntoElem();
	xml.AddElem(_T("BACK"), 16777215);
	xml.AddElem(_T("TEXTBACK"), 16777215);
	xml.AddElem(_T("TEXT"), 0);
	xml.AddElem(_T("VALUE"), 0);
	xml.AddElem(_T("UNIT"), 0);
	// フォント
	xml.AddElem(_T("LFHEIGHT"), -16);
	xml.AddElem(_T("LFWIDTH"), 0);
	xml.AddElem(_T("LFWEIGHT"), 700);
	xml.AddElem(_T("LFFACENAME"), _T("MS UI Gothic"));
	xml.OutOfElem();

	// 子ノードの保存
	xml.AddElem(_T("SIZE"), (UINT)cur->getChildren().size());
	vector<CNode*>::iterator itr;
	for (itr = cur->getChildren().begin(); itr != cur->getChildren().end(); itr++) {
		xml.AddElem(_T("NODE"));
		xml.IntoElem();
		SaveNodeXml(xml, (*itr));
		xml.OutOfElem();
	}

	return true;
}
