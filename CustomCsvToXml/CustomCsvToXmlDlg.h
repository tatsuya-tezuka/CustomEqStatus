
// CustomCsvToXmlDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxeditbrowsectrl.h"
#include "Markup.h"

class CNode {
public:
	CNode(UINT level, const CString name = _T(""));
	virtual ~CNode();

	CString getName() { return mName; }
	UINT getLevel() { return mLevel; }
	void setLevel(UINT level) { mLevel = level; }

	CNode* createChildIfNotExist(const CString childName);
	vector<CNode*>& getChildren() { return mChildren; }

	UINT			mLevel;
	CString			mName;
	CString			mMon, mCntl, mUnit;
	vector<CNode*>	mChildren;
};

// CCustomCsvToXmlDlg ダイアログ
class CCustomCsvToXmlDlg : public CDialogEx
{
// コンストラクション
public:
	CCustomCsvToXmlDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_CUSTOMCSVTOXML_DIALOG };

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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
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
};
