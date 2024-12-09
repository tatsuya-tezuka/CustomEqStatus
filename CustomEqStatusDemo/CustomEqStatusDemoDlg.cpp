
// CustomEqStatusDemoDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomEqStatusDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
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


// CCustomEqStatusDemoDlg �_�C�A���O



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


// CCustomEqStatusDemoDlg ���b�Z�[�W �n���h���[

LRESULT CCustomEqStatusDemoDlg::OnNcHitTest(CPoint point)
{
	LRESULT nHitTest = CDialog::OnNcHitTest(point);
	if (nHitTest == HTCLIENT)    // �����N���C�A���g�̈悾������A
		return HTCAPTION;         // �L���v�V�����ł��A�ƕԂ�
	else
		return nHitTest;
	//return CDialogEx::OnNcHitTest(point);
}

BOOL CCustomEqStatusDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// �J�X�^�}�C�Y�@�\��ʂ̍쐬
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

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
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

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CCustomEqStatusDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CCustomEqStatusDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*============================================================================*/
/*! ���C�����

-# ���C�A�E�g�ďo

@param

@retval
*/
/*============================================================================*/
void CCustomEqStatusDemoDlg::OnBnClickedMfcbuttonLoad()
{
	// �m�F���b�Z�[�W
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
/*! ���C�����

-# ���C�A�E�g�ۑ�

@param

@retval
*/
/*============================================================================*/
void CCustomEqStatusDemoDlg::OnBnClickedMfcbuttonSave()
{
	// �m�F���b�Z�[�W
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
/*! ���C�����

-# �J�X�^���Ǘ���ʂ̌ďo

@param

@retval
*/
/*============================================================================*/
void CCustomEqStatusDemoDlg::OnBnClickedMfcbutton()
{
	createCustomControl();
}

/*============================================================================*/
/*! ���C�����

-# �J�X�^�}�C�Y�@�\��ʂ̍쐬

@param

@retval
*/
/*============================================================================*/
void CCustomEqStatusDemoDlg::createCustomControl()
{
	// �J�X�^���Ǘ���ʂ̍쐬
	theApp.GetCustomControl().GetCustomManager().ShowWindow(SW_SHOW);
}


void CCustomEqStatusDemoDlg::OnBnClickedButtonNodeoutput()
{
	CString msg;
	vector<CTreeNode*>::iterator itr;
	for (itr = theApp.GetCustomControl().GetDataManager().GetTreeNode().begin(); itr != theApp.GetCustomControl().GetDataManager().GetTreeNode().end(); itr++) {
		//=====================================================//
		//������������������������ Log ������������������������//
		msg.Format(_T("%s"), (*itr)->GetEquipment().title);
		CLogTraceEx::Write(_T("***"), _T("***"), _T("***"), _T("*"), msg, nLogEx::debug);
		//������������������������ Log ������������������������//
		//=====================================================//
		PrintChild((*itr));
	}

	// �ҏW�p���X�g���Ɏw��E�B���h�E�����݂���ꍇ�͕ҏW�p���g�p����
	map<CWnd*, CTreeNode*>::iterator itre;
	for (itre = theApp.GetCustomControl().GetDataManager().GetEditTreeNode().begin(); itre != theApp.GetCustomControl().GetDataManager().GetEditTreeNode().end(); itre++) {
		//=====================================================//
		//������������������������ Log ������������������������//
		msg.Format(_T("%s"), (*itre).second->GetEquipment().title);
		CLogTraceEx::Write(_T("###"), _T("###"), _T("###"), _T("#"), msg, nLogEx::debug);
		//������������������������ Log ������������������������//
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
		//������������������������ Log ������������������������//
		msg.Format(_T("%s\t%s\t%s\t%s (%d)"), (*itr)->GetMonCtrl().display, (*itr)->GetMonCtrl().mname, (*itr)->GetMonCtrl().cname, (*itr)->GetMonCtrl().unit, (*itr)->GetEquipment().sortno);
		CString sep = _T("*");
		switch ((*itr)->GetEquipment().type) {
		case	eTreeItemType_Main:	sep = _T("**"); break;
		case	eTreeItemType_Sub:	sep = _T("***"); break;
		case	eTreeItemType_Item:	sep = _T("****"); break;
		}
		CLogTraceEx::Write(_T("***"), _T("***"), _T("***"), sep, msg, nLogEx::debug);
		//������������������������ Log ������������������������//
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
		//������������������������ Log ������������������������//
		msg.Format(_T("%s\t%s\t%s\t%s (%d)"), (*itr)->GetMonCtrl().display, (*itr)->GetMonCtrl().mname, (*itr)->GetMonCtrl().cname, (*itr)->GetMonCtrl().unit, (*itr)->GetEquipment().sortno);
		CString sep = _T("*");
		switch ((*itr)->GetEquipment().type) {
		case	eTreeItemType_Main:	sep = _T("##"); break;
		case	eTreeItemType_Sub:	sep = _T("###"); break;
		case	eTreeItemType_Item:	sep = _T("####"); break;
		}
		CLogTraceEx::Write(_T("###"), _T("###"), _T("###"), sep, msg, nLogEx::debug);
		//������������������������ Log ������������������������//
		//=====================================================//
		PrintChildEdit((*itr));
	}
}
