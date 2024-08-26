
// CustomEqStatus.h : CustomEqStatus アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CCustomEqStatusApp:
// このクラスの実装については、CustomEqStatus.cpp を参照してください。
//

class CCustomEqStatusApp : public CWinApp
{
public:
	CCustomEqStatusApp();


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCustomEqStatusApp theApp;
