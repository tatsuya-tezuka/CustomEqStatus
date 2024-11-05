
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
END_MESSAGE_MAP()


// CCustomEqStatusDemoDlg ���b�Z�[�W �n���h���[

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

#if _DEMO_PHASE < 10
	CenterWindow();
	CRect rect;
	GetWindowRect(rect);
	::SetWindowPos(m_hWnd, HWND_TOP, rect.left, 0, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
#else
	// �J�X�^�}�C�Y�@�\��ʂ̍쐬
	createCustomControl();
	CenterWindow();
	CRect rect;
	GetWindowRect(rect);
	::SetWindowPos(m_hWnd, HWND_TOP, rect.left, 0, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
#endif

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
	const TCHAR BASED_CODE szFilter[] = _T("Station Control Layout(*.scl)|*.scl|");
	CFileDialog dlg(TRUE, _T("xml"), NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() != IDOK)
		return;

#if _DEMO_PHASE < 10
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
	const TCHAR BASED_CODE szFilter[] = _T("Station Control Layout(*.scl)|*.scl|");
	CFileDialog dlg(FALSE, _T("xml"), NULL, OFN_OVERWRITEPROMPT | OFN_LONGNAMES | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() != IDOK)
		return;

#if _DEMO_PHASE < 10
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
#if _DEMO_PHASE < 10
	return;
#endif
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
