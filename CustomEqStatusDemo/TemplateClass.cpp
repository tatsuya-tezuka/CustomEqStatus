// TemplateClass.cpp : 実装ファイル
//

#include "stdafx.h"
#include "CustomEqStatusDemoDlg.h"
#include "TemplateClass.h"

//------------------------------------------------------------------------------------ 
//	概要：
//------------------------------------------------------------------------------------
CTemplateClass::CTemplateClass()
{
}

CTemplateClass::~CTemplateClass()
{
}

/*============================================================================*/
/*! テンプレートクラス

-# 作成

@param	val		作成時の識別子

@retval	成功した場合はtrue、それ以外はfalseを返す
*/
/*============================================================================*/
bool CTemplateClass::Create(UINT val)
{
#ifdef _DEBUG
	// デバッグ用コード
	createItem(10);
#else
	createItem(val);
#endif
	return true;
}

/*============================================================================*/
/*! テンプレートクラス

-# 作成

@param	val		作成時の識別子

@retval	成功した場合はtrue、それ以外はfalseを返す
*/
/*============================================================================*/
bool CTemplateClass::createItem(UINT val)
{
	return true;
}
