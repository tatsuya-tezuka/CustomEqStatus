
// CustomCsvToXml.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CCustomCsvToXmlApp:
// このクラスの実装については、CustomCsvToXml.cpp を参照してください。
//

class CCustomCsvToXmlApp : public CWinApp
{
public:
	CCustomCsvToXmlApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CCustomCsvToXmlApp theApp;