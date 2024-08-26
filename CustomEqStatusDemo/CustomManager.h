#pragma once

#include "CustomDialogBase.h"

// CCustomManager ダイアログ

class CCustomManager : public CCustomDialogBase
{
	DECLARE_DYNAMIC(CCustomManager)

public:
	CCustomManager(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CCustomManager();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_MANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
};
