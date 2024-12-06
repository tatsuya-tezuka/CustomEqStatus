
// CustomCsvToXmlDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxeditbrowsectrl.h"
#include "Markup.h"
#include "afxwin.h"

class CNode {
public:
	CNode(UINT level, const CString name = _T(""));
	virtual ~CNode();

	CString getName() { return mName; }
	UINT getLevel() { return mLevel; }
	void setLevel(UINT level) { mLevel = level; }

	CNode* createChildIfNotExist(const CString childName, bool bSame=true);
	vector<CNode*>& getChildren() { return mChildren; }

	UINT			mLevel;
	CString			mName;
	CString			mMon, mCntl, mUnit;
	vector<CNode*>	mChildren;
};

// CCustomCsvToXmlDlg �_�C�A���O
class CCustomCsvToXmlDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CCustomCsvToXmlDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CUSTOMCSVTOXML_DIALOG };

	void	CreateSCL(CString sclpath, CNode* root);
	bool	SaveNodeScl(CArchive& ar, CNode* cur);

	void _SavePointScl(CArchive& ar, POINT& point) const
	{
		ar << point.x;
		ar << point.y;
	}
	void _SaveRectScl(CArchive& ar, RECT& rect) const
	{
		ar << rect.left;
		ar << rect.top;
		ar << rect.right;
		ar << rect.bottom;
	}

	void	CreateXML(CString xmlpath, CNode* root);
	bool	SaveNodeXml(CMarkup& xml, CNode* cur);

	void _SavePointXml(CMarkup& xml, POINT& point) const
	{
		xml.AddElem(_T("X"), point.x);
		xml.AddElem(_T("Y"), point.y);
	}
	void _SaveRectXml(CMarkup& xml, RECT& rect) const
	{
		xml.AddElem(_T("LEFT"), rect.left);
		xml.AddElem(_T("TOP"), rect.top);
		xml.AddElem(_T("RIGHT"), rect.right);
		xml.AddElem(_T("BOTTOM"), rect.bottom);
	}

	// �F�R�[�h�̓Ǎ�
	void _LoadColorXml(CMarkup& xml, CString tagname, COLORREF& color) const
	{
		BYTE r, g, b;
		xml.FindElem(tagname);
		xml.IntoElem();
		xml.FindElem(_T("R"));
		r = (BYTE)_wtoi(xml.GetData());
		xml.FindElem(_T("G"));
		g = (BYTE)_wtoi(xml.GetData());
		xml.FindElem(_T("B"));
		b = (BYTE)_wtoi(xml.GetData());
		xml.OutOfElem();
		color = RGB(r, g, b);
	}

	// �F�R�[�h�̕ۑ�
	void _SaveColorXml(CMarkup& xml, CString tagname, COLORREF color) const
	{
		xml.AddElem(tagname);
		xml.IntoElem();
		xml.AddElem(_T("R"), GetRValue(color));
		xml.AddElem(_T("G"), GetGValue(color));
		xml.AddElem(_T("B"), GetBValue(color));
		xml.OutOfElem();
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCEditBrowseCtrl mCSV;
	CString mCsvFileName;
	CString mSavePath;
	afx_msg void OnBnClickedButtonConv();
	int mFileType;
	afx_msg void OnBnClickedRadioScl();
	afx_msg void OnBnClickedRadioXml();
	CButton mConvCtrl;
};
