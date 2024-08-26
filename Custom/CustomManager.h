/******************************************************************************
* @file    CustomManager.h
* @brief   【カスタマイズ機能】設備状況管理クラス
* @auther  SCC
* @date    2024/08/26 新規作成
* @par
******************************************************************************/
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
