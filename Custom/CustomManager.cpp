// CustomManager.cpp : 実装ファイル
//

#include "stdafx.h"
#include "EquipmentStatusDemo2013.h"
#include "CustomManager.h"
#include "afxdialogex.h"


// CCustomManager ダイアログ

IMPLEMENT_DYNAMIC(CCustomManager, CCustomDialogBase)

CCustomManager::CCustomManager(CWnd* pParent /*=NULL*/)
: CCustomDialogBase(CCustomManager::IDD, pParent)
{

}

CCustomManager::~CCustomManager()
{
}

void CCustomManager::DoDataExchange(CDataExchange* pDX)
{
	CCustomDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCustomManager, CCustomDialogBase)
END_MESSAGE_MAP()


// CCustomManager メッセージ ハンドラー
