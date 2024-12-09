/******************************************************************************
* @file    CustomDataManager.cpp
* @brief   【カスタマイズ機能】データ管理クラス
* @auther  SCC
* @date    2024/08/26 新規作成
* @par
******************************************************************************/
#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomDataManager.h"

//------------------------------------------------------------------------------------ 
//	概要：ツリー型データクラス
//------------------------------------------------------------------------------------

CTreeNode::CTreeNode(HTREEITEM id, CWnd* pwnd, CWnd* ptree, UINT type)
: treeitem(id)
{
	memset(&equipment, 0, sizeof(stEquipmentInfo));
	memset(&manager, 0, sizeof(stManagerInfo));
	memset(&monctrl, 0, sizeof(stMonCtrlData));
	memset(&color, 0, sizeof(stColorData));
	switch (type) {
	case	eTreeItemType_Window:
	case	eTreeItemType_Title:
		color.back = GetSysColor(COLOR_WINDOW);
		color.textback = mDefaultTitleTextBackColor;
		color.text = GetSysColor(COLOR_BTNTEXT);
		color.value = GetSysColor(COLOR_BTNTEXT);
		color.unit = GetSysColor(COLOR_BTNTEXT);
		break;
	case	eTreeItemType_Main:
		color.back = GetSysColor(COLOR_WINDOW);
		color.textback = mDefaultTextBackColor;
		color.text = mDefaultMainTextColor;
		color.value = GetSysColor(COLOR_BTNTEXT);
		color.unit = GetSysColor(COLOR_BTNTEXT);
		break;
	case	eTreeItemType_Sub:
		color.back = GetSysColor(COLOR_WINDOW);
		color.textback = mDefaultTextBackColor;
		color.text = mDefaultSubTextColor;
		color.value = GetSysColor(COLOR_BTNTEXT);
		color.unit = GetSysColor(COLOR_BTNTEXT);
		break;
	case	eTreeItemType_Item:
		color.back = GetSysColor(COLOR_WINDOW);
		color.textback = mDefaultTextBackColor;
		color.text = mDefaultLeafTextColor;
		color.value = mDefaultValueTextColor;
		color.unit = mDefaultUnitTextColor;
		break;
	default:
		color.back = GetSysColor(COLOR_WINDOW);
		color.textback = GetSysColor(COLOR_WINDOW);
		color.text = GetSysColor(COLOR_BTNTEXT);
		color.value = GetSysColor(COLOR_BTNTEXT);
		color.unit = GetSysColor(COLOR_BTNTEXT);
	}
	CFont font;
	font.CreateStockObject(DEFAULT_GUI_FONT);
	if (font.GetLogFont(&color.font)){
		color.font.lfHeight = -mTreeFontHeight;
		color.font.lfWeight = FW_BOLD;
		color.font.lfCharSet = DEFAULT_CHARSET;
		color.font.lfUnderline = 0;
		color.font.lfStrikeOut = 0;
		font.DeleteObject();
	}

	equipment.wnd = pwnd;
	equipment.tree = ptree;
	parent = NULL;
	children.clear();
}

CTreeNode::~CTreeNode()
{
	// すべての子アイテムリストを削除する
	vector<CTreeNode *>::iterator itr;
	for (itr = children.begin(); itr != children.end(); itr++) {
		delete (*itr);
	}
	parent = NULL;
	children.clear();
}

//bool CTreeNode::operator==(CTreeNode* data)
//{
//	if (Equal(data->GetWindowInfo()) == false)
//		return false;
//	if (Equal(data->GetMonCtrl()) == false)
//		return false;
//	if (Equal(data->GetColor()) == false)
//		return false;
//
//	if (children.size() != data->GetChildren().size())
//		return false;
//
//	UINT size = (UINT)children.size();
//	for (UINT i = 0; i < size; i++) {
//		if (!(*children[i] == data->GetChildren()[i]))
//			return false;
//	}
//
//	return true;
//}

bool CTreeNode::Equal(CTreeNode* data, bool bChild)
{
	if (Equal(data->GetEquipment(), bChild) == false)
		return false;
	if (Equal(data->GetManager(), bChild) == false)
		return false;
	if (Equal(data->GetMonCtrl(), bChild) == false)
		return false;
	if (Equal(data->GetColor(), bChild) == false)
		return false;

	if (children.size() != data->GetChildren().size())
		return false;

	UINT size = (UINT)children.size();
	for (UINT i = 0; i < size; i++) {
		if (children[i]->Equal(data->GetChildren()[i], true) == false)
			return false;
	}

	return true;
}
bool CTreeNode::Equal(stEquipmentInfo& data, bool bChild)
{
	if (bChild == true) {
		return true;
	}

	if (CString(equipment.title) != CString(data.title)) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("TITLE"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return false;
	}

	CRect rectBase = CRect(equipment.placement.rcNormalPosition);
	CRect rectEdit = CRect(data.placement.rcNormalPosition);
	if (rectBase.Width() != rectEdit.Width()) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("WIDTH"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return false;
	}
	if (rectBase.Height() != rectEdit.Height()) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("HEIGHT"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return false;
	}

	for (int i = 0; i < mHeaderSize; i++) {
		if (equipment.hwidth[i] != data.hwidth[i]) {
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("HWIDTH"), _T("NG"), nLogEx::info);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return false;
		}
	}

	return true;
}
bool CTreeNode::Equal(stManagerInfo& data, bool bChild)
{
	return true;
}
bool CTreeNode::Equal(stMonCtrlData& data, bool bChild)
{
	if (CString(monctrl.display) != CString(data.display)) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("DISPLAY"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return false;
	}
	if (CString(monctrl.mname) != CString(data.mname)) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("MON NAME"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return false;
	}
	if (CString(monctrl.cname) != CString(data.cname)) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("CNTL NAME"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return false;
	}
	if (CString(monctrl.unit) != CString(data.unit)) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("UNIT"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return false;
	}
	if (CString(monctrl.format) != CString(data.format)) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("FORMAT"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return false;
	}
	if (monctrl.formattype != data.formattype) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("FORMAT TYPE"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return false;
	}

	return true;
}
bool CTreeNode::Equal(stColorData& data, bool bChild)
{
	if (color.back != data.back) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("BACK"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================]//
		return false;
	}
	if (color.textback != data.textback) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("TEXT BACK"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================]//
		return false;
	}
	if (color.text != data.text) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("TEXT"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================]//
		return false;
	}
	if (color.value != data.value) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("VALUE"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================]//
		return false;
	}
	if (color.unit != data.unit) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("UNIT"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================]//
		return false;
	}
	if (Equal(data.font, bChild) == false) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("FONT"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================]//
		return false;
	}

	return true;
}
bool CTreeNode::Equal(LOGFONT& data, bool bChild)
{
	if (color.font.lfWeight != data.lfWeight) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("LFWEIGHT"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================]//
		return false;
	}
	if (CString(color.font.lfFaceName) != CString(data.lfFaceName)) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("LFFACE NAME"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================]//
		return false;
	}
	if (color.font.lfItalic != data.lfItalic) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("LFITALIC"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================]//
		return false;
	}
	if (color.font.lfHeight != data.lfHeight) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("Equal"), _T("LFHEIGHT"), _T("NG"), nLogEx::info);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================]//
		return false;
	}

	return true;
}

/*============================================================================*/
/*! ツリーノード

-# ツリーノードの作成

@param  type			作成ノードタイプ
@param  parent			親ノード
@param  child			作成する子ノード
@param  hInsertAfter	登録時の位置情報

@retval CTreeNode	ノードクラスポインタ
*/
/*============================================================================*/
CTreeNode* CTreeNode::CreateTreeNode(UINT type, HTREEITEM parent, HTREEITEM child, HTREEITEM hInsertAfter/* = TVI_FIRST*/)
{
	if (treeitem != parent){
		return NULL;
	}

	// 子アイテムリストに存在するか確認する
	vector<CTreeNode *>::iterator itr;
	for (itr = children.begin(); itr != children.end(); itr++) {
		if ((*itr)->treeitem == child) {
			// 既に子アイテムリストに存在する
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("CreateTreeNode"), _T("Already Target"), _T(""), nLogEx::warning);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return (*itr);
		}
	}

	// 子アイテムリストに存在しないので新たに作成する
	CTreeNode *childitem = new CTreeNode(child, equipment.wnd, equipment.tree, type);
	childitem->parent = this;
	if (hInsertAfter == TVI_LAST){
		children.push_back(childitem);
	}
	else{
		children.insert(children.begin(), childitem);
	}

	return childitem;
}

#include <algorithm>
static bool procSort(CTreeNode* left, CTreeNode* right)
{
	return left->GetEquipment().sortno < right->GetEquipment().sortno;
}
/*============================================================================*/
/*! ツリーノード

-# ツリーノードのソート

@param  parent			親ノード

@retval
*/
/*============================================================================*/
void CTreeNode::SortTreeNode(HTREEITEM parent)
{
	CTreeNode* pnode = SearchTreeNode(parent);

	std::sort(pnode->GetChildren().begin(), pnode->GetChildren().end(), procSort);

}

/*============================================================================*/
/*! ツリーノード

-# ツリーノードの削除

@param  target	削除対象ノード

@retval	bool
*/
/*============================================================================*/
bool CTreeNode::DeleteTreeNode(HTREEITEM target)
{
	if (treeitem == target){
		// 先頭ノードなので何もしない
		return false;
	}

	CTreeNode* pnode = SearchTreeNode(target);
	if (pnode == NULL)
		return false;

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("DeleteTreeNode"), pnode->equipment.title, _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	deleteNode(pnode);

	// 子リストから削除する
	vector<CTreeNode *>::iterator itr;
	for (itr = children.begin(); itr != children.end(); itr++) {

		if ((*itr) == pnode){
			children.erase(itr);
			break;
		}
	}
	delete pnode;

	return true;
}

/*============================================================================*/
/*! ツリーノード

-# 指定ノードの削除

@param  target	削除対象ノード

@retval
*/
/*============================================================================*/
void CTreeNode::DeleteItemNode(CTreeNode* pnode)
{
	// 子ノードが存在するので削除する
	vector<CTreeNode*>::iterator itr;
	for (itr = children.begin(); itr != children.end(); itr++) {
		if ((*itr) == pnode) {
			deleteNode((*itr));
			delete (*itr);
			children.erase(itr);
			break;
		}
		else {
			(*itr)->DeleteItemNode(pnode);
		}
	}
}

/*============================================================================*/
/*! ツリーノード

-# ツリーノードの削除

@param  target	削除対象ノード

@retval
*/
/*============================================================================*/
void CTreeNode::deleteNode(CTreeNode* pnode)
{
	// 子ノードが存在するので削除する
	vector<CTreeNode *>::iterator itr;
	for (itr = pnode->children.begin(); itr != pnode->children.end(); itr++) {
		deleteNode((*itr));
		delete (*itr);
	}
	pnode->children.clear();
}

/*============================================================================*/
/*! ツリーノード

-# ツリーノードのコピー

@param  copyNode	コピー対象ノード

@retval	bool
*/
/*============================================================================*/
void CTreeNode::CopyTreeNode(CTreeNode* copyNode)
{
	memcpy(&equipment, &(copyNode->equipment), sizeof(stEquipmentInfo));
	memcpy(&manager, &(copyNode->manager), sizeof(stManagerInfo));
	memcpy(&monctrl, &(copyNode->monctrl), sizeof(stMonCtrlData));
	memcpy(&color, &(copyNode->color), sizeof(stColorData));
	swprintf_s(xmlfile, _MAX_PATH, _T("%s"), (LPCTSTR)copyNode->xmlfile);

	// 子リストをコピーする
	vector<CTreeNode*>::iterator itr;
	for (itr = copyNode->GetChildren().begin(); itr != copyNode->GetChildren().end(); itr++) {
		CTreeNode* child = new CTreeNode((HTREEITEM)NULL, NULL, NULL, eTreeItemType_Title);
		child->CopyTreeNode((*itr));
		children.push_back(child);
	}
}

/*============================================================================*/
/*! ツリーノード

-# ツリーノードの検索

@param  target	検索対象ノード

@retval CTreeNode	ノードクラスポインタ
*/
/*============================================================================*/
CTreeNode* CTreeNode::SearchTreeNode(HTREEITEM target)
{
	if (treeitem == target)
		return this;

	// 子アイテムリストに存在するか確認する
	vector<CTreeNode *>::iterator itr;
	for (itr = children.begin(); itr != children.end(); itr++) {
		if ((*itr)->treeitem == target) {
			// 既に子アイテムリストに存在する
			return (*itr);
		}
		CTreeNode* pnode = (*itr)->SearchTreeNode(target);
		if (pnode != NULL)
			return pnode;
	}
	return NULL;
}

/*============================================================================*/
/*! ツリーノード

-# ツリーノードの検索

@param  target	検索対象ノード種別

@retval CTreeNode	ノードクラスポインタ
*/
/*============================================================================*/
CTreeNode* CTreeNode::SearchTreeNodeType(UINT target)
{
	if (equipment.type == target)
		return this;

	// 子アイテムリストに存在するか確認する
	vector<CTreeNode *>::iterator itr;
	for (itr = children.begin(); itr != children.end(); itr++) {
		if ((*itr)->GetEquipment().type == target) {
			// 既に子アイテムリストに存在する
			return (*itr);
		}
		CTreeNode* pnode = (*itr)->SearchTreeNodeType(target);
		if (pnode != NULL)
			return pnode;
	}
	return NULL;
}



//------------------------------------------------------------------------------------ 
//	カスタマイズ機能のデータマネージャークラス
//------------------------------------------------------------------------------------

CCustomDataManager::CCustomDataManager()
{
}

CCustomDataManager::~CCustomDataManager()
{
	// ウィンドウ、ノードの削除
	DeleteAllWnd();
	DeleteAllNode();

	map<CWnd*, CTreeNode*>::iterator itr;
	for (itr = mEditTreeNode.begin(); itr != mEditTreeNode.end(); itr++) {
		delete (*itr).second;
	}
	mEditTreeNode.clear();
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# 各種色情報の取得

@param

@retval
*/
/*============================================================================*/
bool CCustomDataManager::GetNodeColor(CWnd* pwnd, UINT type, stColorData& color)
{
	CTreeNode* pnode = SearchWndNode(pwnd);
	if (pnode == NULL)
		return false;

	switch (type){
	case	eTreeItemType_Window:
	case	eTreeItemType_Title:
		color = pnode->GetColor();
		return true;
		break;
	}

	bool ret = getNodeTypeColor(pnode, type, color);

	return ret;
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# 各種色情報の設定

@param

@retval
*/
/*============================================================================*/
bool CCustomDataManager::SetNodeColor(CWnd* pwnd, UINT type, stColorData& color)
{
	CTreeNode* pnode = SearchWndNode(pwnd);
	if (pnode == NULL)
		return false;

	switch (type){
	case	eTreeItemType_Window:
	case	eTreeItemType_Title:
		pnode->GetColor() = color;
		return true;
		break;
	}

	bool ret = setNodeTypeColor(pnode, type, color);

	return ret;
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# 各種色情報の取得

@param	pnode		対象ノード情報
@param	type		対象ノード種別
@param	subtype		対象ノードセル種別
@param	color		色情報

@retval
*/
/*============================================================================*/
bool CCustomDataManager::getNodeTypeColor(CTreeNode* pnode, UINT type, stColorData& color)
{
	if (pnode->GetEquipment().type == type){
		color = pnode->GetColor();
		return true;
	}

	bool ret;
	vector<CTreeNode*>::iterator itr;
	for (itr = pnode->GetChildren().begin(); itr != pnode->GetChildren().end(); itr++) {
		ret = getNodeTypeColor((*itr), type, color);
		if (ret == true)
			return true;
	}
	return false;
}
/*============================================================================*/
/*! カスタムデータ管理クラス

-# 各種色情報の設定

@param	pnode		対象ノード情報
@param	type		対象ノード種別
@param	subtype		対象ノードセル種別
@param	color		色情報

@retval
*/
/*============================================================================*/
bool CCustomDataManager::setNodeTypeColor(CTreeNode* pnode, UINT type, stColorData& color)
{
	if (pnode->GetEquipment().type == type){
		pnode->GetColor() = color;
		return true;
	}

	bool ret;
	vector<CTreeNode*>::iterator itr;
	for (itr = pnode->GetChildren().begin(); itr != pnode->GetChildren().end(); itr++) {
		ret = setNodeTypeColor((*itr), type, color);
	}
	return true;
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# ツリーデータ情報の保存

@param		strFile	保存ファイル

@retval
*/
/*============================================================================*/
bool CCustomDataManager::SaveTreeData(CString strFile, CWnd* pTargetWnd/* = NULL*/)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveTreeData"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) == NULL) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveTreeData"), _T("Open Error"), _T(""), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return false;
	}

	CArchive mArc(&file, CArchive::store);

	try {
		// バージョン保存
		mArc << (UINT)EN_FILE_VERSION_MAJOR;

		if (pTargetWnd == NULL) {
			mArc << (UINT)GetNodeKindCount(eTreeItemKind_User);
		}
		else {
			mArc << (UINT)1;
		}

		// 個々のデータを保存
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if (pTargetWnd != NULL && pTargetWnd != (*itr)->GetEquipment().wnd) {
				continue;
			}
			if ((*itr)->GetEquipment().kind != eTreeItemKind_User)
				continue;

			(*itr)->SaveTreeNode(mArc);
		}
	}
	catch (...) {
	}

	mArc.Flush();
	mArc.Close();
	file.Close();
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveTreeData"), _T("Stop"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	return true;
}

/*============================================================================*/
/*! ツリーノード

-# ノード情報の保存

@param		ar	CArchiveクラス

@retval
*/
/*============================================================================*/
bool CTreeNode::SaveTreeNode(CArchive& ar)
{
	// ウィンドウ位置情報取得
	if (equipment.wnd != NULL) {
		memset(&equipment.placement, 0, sizeof(WINDOWPLACEMENT));
		equipment.placement.length = sizeof(WINDOWPLACEMENT);
		equipment.wnd->GetWindowPlacement(&equipment.placement);
		//★((CCustomDetail*)wininfo.wnd)->GetHeaderWidth(wininfo.hwidth, mHeaderSize);
	}

	// ウィンドウ情報
	ar << equipment.mode;
	ar << equipment.kind;
	ar << equipment.type;
	if (equipment.type == eTreeItemType_Title) {
		ar << CString(equipment.title);
		ar << equipment.monitor;
		ar << equipment.placement.flags;
		if (equipment.wnd == NULL)
			ar << FALSE;
		else
			ar << equipment.wnd->IsWindowVisible();
		savePoint(ar, equipment.placement.ptMinPosition);
		savePoint(ar, equipment.placement.ptMaxPosition);
		saveRect(ar, equipment.placement.rcNormalPosition);
		ar << (UINT)mHeaderSize;
		for (int i = 0; i < mHeaderSize; i++) {
			ar << equipment.hwidth[i];
		}
		ar << equipment.zorder;
		// Manager
		ar << CString(manager.memo);
		ar << CString(manager.groupname);
		ar << manager.groupno;
	}
	//★wininfo.treeopen = ((CCustomDetail*)wininfo.wnd)->GetTreeExpandState(treeitem);
	ar << 0/*wininfo.treeopen*/;

	// 監視制御情報
	ar << CString(monctrl.display);
	if (equipment.type == eTreeItemType_Item) {
		ar << CString(monctrl.mname);
		ar << CString(monctrl.cname);
		ar << CString(monctrl.unit);
		ar << monctrl.formattype;
		ar << CString(monctrl.format);
	}

	// 色情報
	ar << color.back;
	ar << color.textback;
	ar << color.text;
	ar << color.value;
	ar << color.unit;
	// フォント
	ar << color.font.lfHeight;
	ar << color.font.lfWidth;
	ar << color.font.lfWeight;
	ar << color.font.lfCharSet;
	ar << CString(color.font.lfFaceName);

	// 子ノードの保存
	ar << (UINT)children.size();
	vector<CTreeNode*>::iterator itr;
	for (itr = children.begin(); itr != children.end(); itr++) {
		(*itr)->SaveTreeNode(ar);
	}

	return true;
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# ツリーデータ情報の読込

@param		strFile	読込ファイル

@retval
*/
/*============================================================================*/
bool CCustomDataManager::LoadTreeData(CString strFile, bool bClear)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("LoadTreeData"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	CFile file;
	if (file.Open(strFile, CFile::modeRead | CFile::typeBinary) == NULL) {
		//=====================================================//
		//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
		CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("LoadTreeData"), _T("Open Error"), _T(""), nLogEx::error);
		//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
		//=====================================================//
		return false;
	}

	CArchive mArc(&file, CArchive::load);

	try {
		// バージョン
		UINT version;
		mArc >> version;
		if (version != EN_FILE_VERSION_MAJOR) {
			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("LoadTreeData"), _T("Version Error"), _T(""), nLogEx::error);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//
			return false;
		}

		if (bClear == true) {
			// 現行のデータを削除する
			//DeleteKindWnd(eTreeItemKind_User);
			DeleteKindNode(eTreeItemKind_User);
		}

		UINT size;
		mArc >> size;
		mTreeNode.reserve(size);
		for (UINT i = 0; i < size; i++) {
			//CTreeNode* pnode = new CTreeNode((HTREEITEM)i, NULL, NULL);
			CTreeNode* pnode = new CTreeNode((HTREEITEM)NULL, NULL, NULL, eTreeItemType_Title);
			if (pnode->LoadTreeNode(mArc) == false) {
				delete pnode;
				break;
			}
			mTreeNode.push_back(pnode);
		}
	}
	catch (...) {
	}
	mArc.Close();
	file.Close();
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("LoadTreeData"), _T("Stop"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	return true;
}
/*============================================================================*/
/*! ツリーノード

-# ノード情報の読込

@param		ar	CArchiveクラス

@retval
*/
/*============================================================================*/
bool CTreeNode::LoadTreeNode(CArchive& ar)
{
	CString str;
	// ウィンドウ情報
	ar >> equipment.mode;
	equipment.mode = eTreeItemMode_Monitor; // ロード時は常に監視モード
	ar >> equipment.kind;
	ar >> equipment.type;
	if (equipment.type == eTreeItemType_Title) {
		ar >> str;
		swprintf_s(equipment.title, mTitleSize, _T("%s"), (LPCTSTR)str);
		ar >> equipment.monitor;
		equipment.placement.length = sizeof(WINDOWPLACEMENT);
		ar >> equipment.placement.flags;
		ar >> equipment.placement.showCmd;
		loadPoint(ar, equipment.placement.ptMinPosition);
		loadPoint(ar, equipment.placement.ptMaxPosition);
		loadRect(ar, equipment.placement.rcNormalPosition);
		UINT hsize;
		ar >> hsize;
		for (int i = 0; i < (int)hsize; i++) {
			ar >> equipment.hwidth[i];
		}
		ar >> equipment.zorder;
		// Manager
		ar >> str;
		swprintf_s(manager.memo, mTitleSize, _T("%s"), (LPCTSTR)str);
		ar >> str;
		swprintf_s(manager.groupname, mNameSize, _T("%s"), (LPCTSTR)str);
		ar >> manager.groupno;
	}
	UINT temp;
	ar >> temp;

	// 監視制御情報
	ar >> str;
	swprintf_s(monctrl.display, mNameSize, _T("%s"), (LPCTSTR)str);
	if (equipment.type == eTreeItemType_Item) {
		ar >> str;
		swprintf_s(monctrl.mname, mNameSize, _T("%s"), (LPCTSTR)str);
		ar >> str;
		swprintf_s(monctrl.cname, mNameSize, _T("%s"), (LPCTSTR)str);
		ar >> str;
		swprintf_s(monctrl.unit, mUnitSize, _T("%s"), (LPCTSTR)str);
		ar >> monctrl.formattype;
		ar >> str;
		swprintf_s(monctrl.format, mFormatSize, _T("%s"), (LPCTSTR)str);
	}

	// 色情報
	ar >> color.back;
	ar >> color.textback;
	ar >> color.text;
	ar >> color.value;
	ar >> color.unit;
	// フォント
	ar >> color.font.lfHeight;
	ar >> color.font.lfWidth;
	ar >> color.font.lfWeight;
	ar >> color.font.lfCharSet;
	ar >> str;
	swprintf_s(color.font.lfFaceName, LF_FACESIZE, _T("%s"), (LPCTSTR)str);

	// 子ノードの取得
	UINT size;
	ar >> size;
	children.reserve(size);
	for (UINT i = 0; i < size; i++) {
		//CTreeNode* child = new CTreeNode((HTREEITEM)i, NULL, NULL);
		CTreeNode* child = new CTreeNode((HTREEITEM)NULL, NULL, NULL, eTreeItemType_Title);
		child->LoadTreeNode(ar);
		children.push_back(child);
	}

	return true;
}

static CWnd* compWnd = NULL;
/*============================================================================*/
/*! ツリーノード

-# カスタマイズ画面のレイアウト保存

@param		ar				CArchiveクラス

@retval
*/
/*============================================================================*/
bool CCustomDataManager::SaveCustomLayout(CArchive& ar)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveCustomLayout"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 端末アプリ名の設定
	ar << m_strAppName;
	// レイアウト対応アプリバージョンの設定
	ar << m_nVersionCustom;

	// 保存データ数の取得
	UINT count = 0;
	vector<CTreeNode*>::iterator itr;
	for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
		if ((*itr)->GetEquipment().wnd == NULL) {
			continue;
		}
		if ((*itr)->GetEquipment().wnd->IsWindowVisible() == FALSE) {
			continue;
		}

		if ((*itr)->SaveCustomLayout() == false)
			continue;

		count++;
	}
	// 保存データ数の設定
	ar << count;

	// 個々のデータを保存(Zオーダーを考慮)
	CWnd* pWnd = theApp.GetMainWnd()->GetWindow(GW_ENABLEDPOPUP);
	UINT pos = 0;
	while (pWnd) {
		compWnd = pWnd;
		//itr = std::find_if(mTreeNode.begin(), mTreeNode.end(), [](CTreeNode* p) { return (p->GetWindowInfo().wnd == compWnd); });
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr)->GetEquipment().wnd == NULL) {
				continue;
			}
			if ((*itr)->GetEquipment().wnd->IsWindowVisible() == FALSE) {
				continue;
			}

			if (pWnd != (*itr)->GetEquipment().wnd) {
				continue;
			}

			//=====================================================//
			//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
			TRACE("Save Title:%s\n", CStringA((*itr)->GetEquipment().title));
			CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveCustomLayout"), (*itr)->GetEquipment().title, _T(""), nLogEx::debug);
			//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
			//=====================================================//

			// ここまできたらXMLファイル名を保存する
			ar << CString((*itr)->GetXmlFileName());
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveCustomLayout"), _T("Stop"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	return true;
}

/*============================================================================*/
/*! ツリーノード

-# カスタマイズ画面のレイアウト保存

@param	pplacement	位置情報

@retval
*/
/*============================================================================*/
bool CTreeNode::SaveCustomLayout(WINDOWPLACEMENT* pplacement/* = NULL*/)
{
	if (CString(xmlfile).IsEmpty() == true)
		return false;

	// ウィンドウ位置情報の取得
	if (pplacement == NULL) {
		memset(&equipment.placement, 0, sizeof(WINDOWPLACEMENT));
		equipment.placement.length = sizeof(WINDOWPLACEMENT);
		equipment.wnd->GetWindowPlacement(&equipment.placement);
		pplacement = &equipment.placement;
	}

	CMarkup xml;
	xml.Load(xmlfile);
	xml.FindElem(_T("ROOT"));
	xml.IntoElem();
	xml.FindElem(_T("EQUIPMENT"));
	xml.IntoElem();

	// LAYOUT
	if (xml.FindElem(_T("LAYOUTINFO")) == true) {
		xml.IntoElem();
		xml.FindElem(_T("FLAGS"));
		xml.SetData(pplacement->flags);
		xml.FindElem(_T("SHOWCMD"));
		xml.SetData(equipment.wnd->IsWindowVisible());
		setPointXml(xml, pplacement->ptMinPosition);
		setPointXml(xml, pplacement->ptMaxPosition);
		setRectXml(xml, pplacement->rcNormalPosition);
	}
	else {
		xml.AddElem(_T("LAYOUTINFO"));
		xml.IntoElem();
		xml.AddElem(_T("FLAGS"), pplacement->flags);
		xml.AddElem(_T("SHOWCMD"), (equipment.wnd == NULL) ? 0 : equipment.wnd->IsWindowVisible()/*wininfo.placement.showCmd*/);
		savePointXml(xml, pplacement->ptMinPosition);
		savePointXml(xml, pplacement->ptMaxPosition);
		saveRectXml(xml, pplacement->rcNormalPosition);
	}

	xml.OutOfElem(); // LAYOUTINFO
	xml.OutOfElem(); // EQUIPMENT
	xml.OutOfElem(); // ROOT

	xml.Save(xmlfile);

	return true;
}

/*============================================================================*/
/*! ツリーノード

-# カスタマイズ画面のレイアウト復元

@param		ar				CArchiveクラス

@retval
*/
/*============================================================================*/
bool CCustomDataManager::LoadCustomLayout(CArchive& ar)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("LoadCustomLayout"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	// 現在憑依されている設備詳細画面を閉じる
	DeleteAllWnd();
	// 編集用のウィンドウ情報も削除する
	map<CWnd*, CTreeNode*>::iterator itredit;
	for (itredit = mEditTreeNode.begin(); itredit != mEditTreeNode.end(); itredit++) {
		delete (*itredit).second;
	}
	mEditTreeNode.clear();

	CString strAppName;
	double nVersion;
	UINT count = 0;

	// 端末アプリ名の設定
	ar >> strAppName;
	// レイアウト対応アプリバージョンの設定
	ar >> nVersion;

	// 保存データ数の設定
	ar >> count;
	vector<CString> loadlist;

	// Zオーダー順で保存されているため、一度リストに登録する
	for (UINT i = 0; i < count; i++) {
		CString xmlfile;
		ar >> xmlfile;
		loadlist.push_back(xmlfile);
	}

	// 個々のデータを取得
	// Zオーダーを考慮して、逆引きで情報を取得する
	vector<CString>::reverse_iterator itrr;
	for (itrr = loadlist.rbegin(); itrr != loadlist.rend(); itrr++) {
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if (CString((*itr)->GetXmlFileName()).MakeLower() == (*itrr).MakeLower()) {
				(*itr)->LoadCustomLayout();
				if ((*itr)->GetEquipment().wnd == NULL) {
					CCustomDetail* pitem = theApp.GetCustomControl().CreateEquipment((*itr));
					if (pitem == NULL)
						return false;

					// ウィンドウハンドルの設定
					(*itr)->GetEquipment().wnd = pitem;
				}
				(*itr)->GetEquipment().wnd->ShowWindow(SW_SHOWNA);
				(*itr)->GetEquipment().wnd->SetActiveWindow();
				// 常に監視モードとする
				(*itr)->GetEquipment().mode = eTreeItemMode_Monitor;
				//=====================================================//
				//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
				TRACE("Load Title:%s\n", CStringA((*itr)->GetEquipment().title));
				CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("LoadCustomLayout"), (*itr)->GetEquipment().title, _T(""), nLogEx::debug);
				//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
				//=====================================================//
				break;
			}
		}
	}

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("LoadCustomLayout"), _T("Stop"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//

	return true;
}

/*============================================================================*/
/*! ツリーノード

-# カスタマイズ画面のレイアウト保存

@param	pplacement	位置情報

@retval
*/
/*============================================================================*/
bool CTreeNode::LoadCustomLayout(WINDOWPLACEMENT* pplacement/* = NULL*/)
{
	if (CString(xmlfile).IsEmpty() == true)
		return false;

	// ウィンドウ位置情報の取得
	if(pplacement == NULL){
		pplacement = &equipment.placement;
	}
	memset(pplacement, 0, sizeof(WINDOWPLACEMENT));
	pplacement->length = sizeof(WINDOWPLACEMENT);

	CMarkup xml;
	xml.Load(xmlfile);
	xml.FindElem(_T("ROOT"));
	xml.IntoElem();
	xml.FindElem(_T("EQUIPMENT"));
	xml.IntoElem();

	// LAYOUT
	if (xml.FindElem(_T("LAYOUTINFO")) == true){
		xml.IntoElem();
	}
	else {
		xml.FindElem(_T("EQUIPMENTINFO"));
		xml.IntoElem();
	}

	xml.FindElem(_T("FLAGS"));
	pplacement->flags = _wtoi(xml.GetData());
	xml.FindElem(_T("SHOWCMD"));
	pplacement->showCmd = _wtoi(xml.GetData());
	loadPointXml(xml, pplacement->ptMinPosition);
	loadPointXml(xml, pplacement->ptMaxPosition);
	loadRectXml(xml, pplacement->rcNormalPosition);

	xml.OutOfElem(); // LAYOUTINFO or EQUIPMENTINFO
	xml.OutOfElem(); // EQUIPMENT
	xml.OutOfElem(); // ROOT

	return true;
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# ツリーデータ情報の保存

@param		strFile		保存ファイル
@param		pTargetWnd	カスタム画面ハンドル

@retval
*/
/*============================================================================*/
bool CCustomDataManager::SaveTreeDataXml(CString strFile, CWnd* pTargetWnd/* = NULL*/)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveTreeDataXml"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	CString	decl = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	CMarkup xml(decl);

	SetTreeZorder();

	// 改行コードを設定
	xml.SetEOL(MCD_T("\n"));
	xml.SetEOLLEN(sizeof(MCD_T("\n")) / sizeof(MCD_CHAR) - 1);

	// BOMを未出力
	xml.SetBOM(false);

	xml.SetDocFlags(CMarkup::MDF_UTF8PREAMBLE);

	// <ROOT>を出力
	xml.AddElem(_T("ROOT"));
	xml.IntoElem();
	xml.AddElem(_T("VERSION"), EN_FILE_VERSION_MAJOR);

	if (pTargetWnd == NULL) {
		xml.AddElem(_T("SIZE"), (UINT)mTreeNode.size());
	}
	else {
		xml.AddElem(_T("SIZE"), (UINT)1);
	}

	WINDOWPLACEMENT layout;
	vector<CTreeNode*>::iterator itr;
	for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
		if (pTargetWnd != NULL && pTargetWnd != (*itr)->GetEquipment().wnd) {
			continue;
		}
		xml.AddElem(_T("EQUIPMENT"));
		xml.IntoElem();
		// LAYOUTINFOが削除されるので、ここでバックアップ
		(*itr)->LoadCustomLayout(&layout);
		
		(*itr)->SaveTreeNodeXml(xml);

		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.Save(strFile);

	// LAYOUTINFOが削除されるので、ここでリストア
	for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
		if (pTargetWnd != NULL && pTargetWnd != (*itr)->GetEquipment().wnd) {
			continue;
		}
		(*itr)->SaveCustomLayout(&layout);
	}

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveTreeDataXml"), _T("Stop"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	return true;
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# ツリーデータ情報の名前を付けて保存

@param		strFile		保存ファイル
@param		pTargetWnd	編集用カスタム画面ハンドル

@retval
*/
/*============================================================================*/
bool CCustomDataManager::SaveasTreeDataXml(CString strFile, CWnd* pTargetWnd)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveasTreeDataXml"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	CString	decl = _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	CMarkup xml(decl);

	SetTreeZorder();

	// 改行コードを設定
	xml.SetEOL(MCD_T("\n"));
	xml.SetEOLLEN(sizeof(MCD_T("\n")) / sizeof(MCD_CHAR) - 1);

	// BOMを未出力
	xml.SetBOM(false);

	xml.SetDocFlags(CMarkup::MDF_UTF8PREAMBLE);

	// <ROOT>を出力
	xml.AddElem(_T("ROOT"));
	xml.IntoElem();
	xml.AddElem(_T("VERSION"), EN_FILE_VERSION_MAJOR);

	xml.AddElem(_T("SIZE"), (UINT)1);

	map<CWnd*, CTreeNode*>::iterator itrwnd;
	itrwnd = mEditTreeNode.find(pTargetWnd);
	if (itrwnd == mEditTreeNode.end()) {
		return false;
	}

	xml.AddElem(_T("EQUIPMENT"));
	xml.IntoElem();
	(*itrwnd).second->SaveTreeNodeXml(xml);
	xml.OutOfElem();

	xml.OutOfElem();
	xml.Save(strFile);
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveasTreeDataXml"), _T("Stop"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	return true;
}

/*============================================================================*/
/*! ツリーノード

-# ノード情報の保存

@param		xml	XMLクラス

@retval
*/
/*============================================================================*/
bool CTreeNode::SaveTreeNodeXml(CMarkup& xml)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveTreeNodeXml"), _T("Start"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	/*
		ウィンドウ位置の保存はレイアウト保存時だけ
	*/
	// ウィンドウ位置情報取得
	if (equipment.wnd != NULL){
		memset(&equipment.placement, 0, sizeof(WINDOWPLACEMENT));
		equipment.placement.length = sizeof(WINDOWPLACEMENT);
		equipment.wnd->GetWindowPlacement(&equipment.placement);
		//★((CCustomDetail*)wininfo.wnd)->GetHeaderWidth(wininfo.hwidth, mHeaderSize);
	}

	// 設備詳細画面情報
	xml.AddElem(_T("EQUIPMENTINFO"));
	xml.IntoElem();
	xml.AddElem(_T("MODE"), equipment.mode);
	xml.AddElem(_T("KIND"), equipment.kind);
	xml.AddElem(_T("TYPE"), equipment.type);
	if (equipment.type == eTreeItemType_Title){
		xml.AddElem(_T("TITLE"), equipment.title);
		xml.AddElem(_T("MONITOR"), equipment.monitor);
		xml.AddElem(_T("FLAGS"), equipment.placement.flags);
		xml.AddElem(_T("SHOWCMD"), (equipment.wnd == NULL) ? 0 : equipment.wnd->IsWindowVisible()/*wininfo.placement.showCmd*/);
		savePointXml(xml, equipment.placement.ptMinPosition);
		savePointXml(xml, equipment.placement.ptMaxPosition);
		saveRectXml(xml, equipment.placement.rcNormalPosition);
		for (int i = 0; i < mHeaderSize; i++){
			CString str;
			str.Format(_T("HWIDTH%d"), i + 1);
			xml.AddElem(str, equipment.hwidth[i]);
		}
		xml.AddElem(_T("ZORDER"), equipment.zorder);
		// 設備詳細管理画面情報
		xml.AddElem(_T("MEMO"), manager.memo);
		xml.AddElem(_T("GROUPNO"), manager.groupno);
		xml.AddElem(_T("GROUPNAME"), manager.groupname);
	}
	//★wininfo.treeopen = ((CCustomDetail*)wininfo.wnd)->GetTreeExpandState(treeitem);
	xml.AddElem(_T("TREEOPEN"), 0/*wininfo.treeopen*/);
	xml.OutOfElem();

	// 監視制御情報
	xml.AddElem(_T("MONCTRLINFO"));
	xml.IntoElem();
	xml.AddElem(_T("DISPLAY"), monctrl.display);
	if (equipment.type == eTreeItemType_Item){
		xml.AddElem(_T("MONNAME"), monctrl.mname);
		xml.AddElem(_T("CTRLNAME"), monctrl.cname);
		xml.AddElem(_T("UNIT"), monctrl.unit);
		xml.AddElem(_T("FORMATTYPE"), monctrl.formattype);
		xml.AddElem(_T("FORMAT"), monctrl.format);
	}
	xml.OutOfElem();

	// 色情報
	xml.AddElem(_T("COLORINFO"));
	xml.IntoElem();
	saveColorXml(xml, _T("BACK"), color.back);
	saveColorXml(xml, _T("TEXTBACK"), color.textback);
	saveColorXml(xml, _T("TEXT"), color.text);
	saveColorXml(xml, _T("VALUE"), color.value);
	saveColorXml(xml, _T("UNIT"), color.unit);
	xml.OutOfElem();
	// フォント
	xml.AddElem(_T("FONTINFO"));
	xml.IntoElem();
	xml.AddElem(_T("LFHEIGHT"), color.font.lfHeight);
	xml.AddElem(_T("LFWIDTH"), color.font.lfWidth);
	xml.AddElem(_T("LFWEIGHT"), color.font.lfWeight);
	xml.AddElem(_T("LFWEIGHT"), color.font.lfWeight);
	xml.AddElem(_T("LFCHARSET"), color.font.lfCharSet);
	xml.AddElem(_T("LFFACENAME"), color.font.lfFaceName);
	xml.OutOfElem();

	// 子ノードの保存
	xml.AddElem(_T("SIZE"), (UINT)children.size());
	vector<CTreeNode*>::iterator itr;
	for (itr = children.begin(); itr != children.end(); itr++){
		xml.AddElem(_T("NODE"));
		xml.IntoElem();
		(*itr)->SaveTreeNodeXml(xml);
		xml.OutOfElem();
	}

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveTreeNodeXml"), _T("Stop"), _T(""), nLogEx::debug);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	return true;
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# ツリーデータ情報の読込

@param		strFile	読込ファイル
@param		kind	ノード種別
@param		pnode	交換ノード

@retval
*/
/*============================================================================*/
CTreeNode* CCustomDataManager::LoadTreeDataXml(CString strFile, UINT kind/* = UINT_MAX*/)
{
	CMarkup xml;
	CString str;
	CTreeNode* pnode = NULL;

	// XMLファイルを読み込む
	xml.Load(strFile);
	xml.FindElem(_T("ROOT"));
	xml.IntoElem();
	xml.FindElem(_T("VERSION"));
	UINT version = _wtoi(xml.GetData());
	if (version != EN_FILE_VERSION_MAJOR){
		return pnode;
	}

	xml.FindElem(_T("SIZE"));
	UINT size = _wtoi(xml.GetData());
	for (UINT i = 0; i < size; i++){
		xml.FindElem(_T("EQUIPMENT"));
		xml.IntoElem();
		//CTreeNode* pnode = new CTreeNode((HTREEITEM)i, NULL, NULL);
		pnode = new CTreeNode((HTREEITEM)NULL, NULL, NULL, eTreeItemType_Title);
		if (pnode->LoadTreeNodeXml(xml) == false){
			delete pnode;
			pnode = NULL;
			xml.OutOfElem();
			break;
		}

		swprintf_s(pnode->GetXmlFileName(), _MAX_PATH, _T("%s"), (LPCTSTR)strFile);
		if (kind != UINT_MAX) {
			pnode->GetEquipment().kind = kind;
		}
		mTreeNode.push_back(pnode);
		xml.OutOfElem();
	}

	return pnode;
}
/*============================================================================*/
/*! ツリーノード

-# ノード情報の読込

@param		xml	XMLクラス

@retval
*/
/*============================================================================*/
bool CTreeNode::LoadTreeNodeXml(CMarkup& xml)
{
	// ウィンドウ情報
	xml.FindElem(_T("EQUIPMENTINFO"));
	xml.IntoElem();
	xml.FindElem(_T("MODE"));
	equipment.mode = _wtoi(xml.GetData());
	equipment.mode = eTreeItemMode_Monitor; // ロード時は常に監視モード
	xml.FindElem(_T("KIND"));
	equipment.kind = _wtoi(xml.GetData());
	xml.FindElem(_T("TYPE"));
	equipment.type = _wtoi(xml.GetData());
	if (equipment.type == eTreeItemType_Title){
		xml.FindElem(_T("TITLE"));
		swprintf_s(equipment.title, mTitleSize, _T("%s"), (LPCTSTR)xml.GetData());
		//if (HIWORD(wininfo.groupno) == 0) {
		//	swprintf_s(wininfo.groupname, mNameSize, mNoGroupText);
		//}
		xml.FindElem(_T("MONITOR"));
		equipment.monitor = _wtoi(xml.GetData());
		equipment.placement.length = sizeof(WINDOWPLACEMENT);
		xml.FindElem(_T("FLAGS"));
		equipment.placement.flags = _wtoi(xml.GetData());
		xml.FindElem(_T("SHOWCMD"));
		equipment.placement.showCmd = _wtoi(xml.GetData());
		loadPointXml(xml, equipment.placement.ptMinPosition);
		loadPointXml(xml, equipment.placement.ptMaxPosition);
		loadRectXml(xml, equipment.placement.rcNormalPosition);
		for (int i = 0; i < mHeaderSize; i++){
			CString str;
			str.Format(_T("HWIDTH%d"), i + 1);
			xml.FindElem(str);
			equipment.hwidth[i] = _wtoi(xml.GetData());
		}
		xml.FindElem(_T("ZORDER"));
		equipment.zorder = _wtoi(xml.GetData());
		// Manager
		xml.FindElem(_T("MEMO"));
		swprintf_s(manager.memo, mTitleSize, _T("%s"), (LPCTSTR)xml.GetData());
		xml.FindElem(_T("GROUPNO"));
		CString str = xml.GetData();
		manager.groupno = _wtoi(xml.GetData());
		xml.FindElem(_T("GROUPNAME"));
		swprintf_s(manager.groupname, mNameSize, _T("%s"), (LPCTSTR)xml.GetData());
	}
	//xml.FindElem(_T("TREEOPEN"));
	//wininfo.treeopen = 0;// _wtoi(xml.GetData());
	xml.OutOfElem();

	// 監視制御情報
	xml.FindElem(_T("MONCTRLINFO"));
	xml.IntoElem();
	xml.FindElem(_T("DISPLAY"));
	swprintf_s(monctrl.display, mNameSize, _T("%s"), (LPCTSTR)xml.GetData());
	if (equipment.type == eTreeItemType_Item){
		xml.FindElem(_T("MONNAME"));
		swprintf_s(monctrl.mname, mNameSize, _T("%s"), (LPCTSTR)xml.GetData());
		xml.FindElem(_T("CTRLNAME"));
		swprintf_s(monctrl.cname, mNameSize, _T("%s"), (LPCTSTR)xml.GetData());
		xml.FindElem(_T("UNIT"));
		swprintf_s(monctrl.unit, mUnitSize, _T("%s"), (LPCTSTR)xml.GetData());
		xml.FindElem(_T("FORMATTYPE"));
		monctrl.formattype = _wtoi(xml.GetData());
		xml.FindElem(_T("FORMAT"));
		swprintf_s(monctrl.format, mFormatSize, _T("%s"), (LPCTSTR)xml.GetData());
	}
	xml.OutOfElem();

	// 色情報
	xml.FindElem(_T("COLORINFO"));
	xml.IntoElem();
	loadColorXml(xml, _T("BACK"), color.back);
	loadColorXml(xml, _T("TEXTBACK"), color.textback);
	loadColorXml(xml, _T("TEXT"), color.text);
	loadColorXml(xml, _T("VALUE"), color.value);
	loadColorXml(xml, _T("UNIT"), color.unit);
	xml.OutOfElem();
	// フォント
	xml.FindElem(_T("FONTINFO"));
	xml.IntoElem();
	xml.FindElem(_T("LFHEIGHT"));
	color.font.lfHeight = _wtoi(xml.GetData());
	xml.FindElem(_T("LFWIDTH"));
	color.font.lfWidth = _wtoi(xml.GetData());
	xml.FindElem(_T("LFWEIGHT"));
	color.font.lfWeight = _wtoi(xml.GetData());
	xml.FindElem(_T("LFCHARSET"));
	color.font.lfCharSet = _wtoi(xml.GetData());
	xml.FindElem(_T("LFFACENAME"));
	swprintf_s(color.font.lfFaceName, LF_FACESIZE, _T("%s"), (LPCTSTR)xml.GetData());
	xml.OutOfElem();

	// 子ノードの取得
	UINT size;
	xml.FindElem(_T("SIZE"));
	size = _wtoi(xml.GetData());
	for (UINT i = 0; i < size; i++) {
		xml.FindElem(_T("NODE"));
		xml.IntoElem();
		//CTreeNode* child = new CTreeNode((HTREEITEM)i, NULL, NULL);
		CTreeNode* child = new CTreeNode((HTREEITEM)NULL, NULL, NULL, eTreeItemType_Title);
		child->LoadTreeNodeXml(xml);
		child->GetEquipment().sortno = (i+1) * mSortRange;
		children.push_back(child);
		xml.OutOfElem();
	}

	return true;
}
/*============================================================================*/
/*! カスタムデータ管理クラス

-# ノードウィンドウのZオーダーの設定

@param

@retval
*/
/*============================================================================*/
void CCustomDataManager::SetTreeZorder()
{
	CWnd* pWnd = theApp.GetMainWnd()->GetWindow(GW_ENABLEDPOPUP);
	UINT pos = 0;
	while (pWnd){
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
			if (pWnd == (*itr)->GetEquipment().wnd){
				(*itr)->GetEquipment().zorder = pos++;
				break;
			}
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
}
/*============================================================================*/
/*! カスタムデータ管理クラス

-# ノードウィンドウのZオーダー更新

@param

@retval
*/
/*============================================================================*/
void CCustomDataManager::ResetTreeZorder()
{
	map<UINT, CWnd*> winmap;
	vector<CTreeNode*>::iterator itr;
	for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
		winmap.insert(map<UINT, CWnd*>::value_type((*itr)->GetEquipment().zorder, (*itr)->GetEquipment().wnd));
	}

	map<UINT, CWnd*>::reverse_iterator ritr;
	for (ritr = winmap.rbegin(); ritr != winmap.rend(); ++ritr){
		SetWindowPos((*ritr).second->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# 設備詳細設定の復元

@param		typeLayout	レイアウトファイル種別
@param		strfile		ノードデータのファイル名
@param		bClear		trueの場合、保存されているノードデータを削除
@retval

*/
/*============================================================================*/
void CCustomDataManager::LoadEquipmentData(UINT typeLayout, CString strfile, bool bClear/* = true*/)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("LoadEquipmentData"), _T("Start"), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	CWaitCursor wait;
	timeBeginPeriod(1);
	DWORD	start_time = timeGetTime();

	theApp.GetCustomControl().PrintMemoryInfo();

	try {
		// アプリケーション終了時に保存された設備詳細データの復元
		switch (typeLayout) {
		case	eLayoutFileType_SCL:
			LoadTreeData(strfile, bClear);
			break;
		case	eLayoutFileType_XML:
			AfxMessageBox(_T("LoadEquipmentData Error"));
			return;
			//LoadTreeDataXml(strfile, bClear);
			break;
		default:
			return;
		}
	}
	catch (...) {
	}

	// 設備詳細画面を復元する
	vector<CTreeNode*>& treedata = GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		if (bClear == false && (*itr)->GetEquipment().wnd != NULL)
			continue;

		// 設備詳細画面の作成
		// ※作成時は非表示とする
		CCustomDetail* pitem = theApp.GetCustomControl().CreateEquipment((*itr));
	}

	//ResetTreeZorder();

	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	//CLogTraceEx::Write(_T("***"), _T("CCustomEquipmentApp"), _T("RestoreEquipmentDataArchive"), _T("Show Window"), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	// 記憶している状態で再表示
	//for (itr = treedata.begin(); itr != treedata.end(); itr++) {
	//	(*itr)->getWindowInfo().wnd->ShowWindow((*itr)->getWindowInfo().placement.showCmd);
	//}

	DWORD	end_time = timeGetTime();
	theApp.GetCustomControl().PrintMemoryInfo();
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CString msg;
	msg.Format(_T("TreeNode Count : %d"), (UINT)GetTreeNode().size());
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("LoadEquipmentData"), msg, _T(""), nLogEx::info);
	msg.Format(_T("Elaps Time : %dms"), end_time - start_time);
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("LoadEquipmentData"), msg, _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# 設備詳細設定の復元

@param		typeLayout	レイアウトファイル種別
@param		strfile		ノードデータのファイル名
@retval

*/
/*============================================================================*/
void CCustomDataManager::SaveEquipmentData(UINT typeLayout, CString strfile, CWnd* pTargetWnd/* = NULL*/)
{
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveEquipmentData"), _T("Start"), _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
	CWaitCursor wait;
	timeBeginPeriod(1);
	DWORD	start_time = timeGetTime();

	theApp.GetCustomControl().PrintMemoryInfo();

	try {
		// アプリケーション終了時に保存された設備詳細データの復元
		switch (typeLayout) {
		case	eLayoutFileType_SCL:
			SaveTreeData(strfile, pTargetWnd);
			break;
		case	eLayoutFileType_XML:
			SaveTreeDataXml(strfile, pTargetWnd);
			break;
		default:
			return;
		}
	}
	catch (...) {
	}

	DWORD	end_time = timeGetTime();
	theApp.GetCustomControl().PrintMemoryInfo();
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CString msg;
	msg.Format(_T("TreeNode Count : %d"), (UINT)GetTreeNode().size());
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveEquipmentData"), msg, _T(""), nLogEx::info);
	msg.Format(_T("Elaps Time : %dms"), end_time - start_time);
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveEquipmentData"), msg, _T(""), nLogEx::info);
	//↑↑↑↑↑↑↑↑↑↑↑↑ Log ↑↑↑↑↑↑↑↑↑↑↑↑//
	//=====================================================//
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# グループ番号の最大内部番号の取得

@param		group	グループ番号
@retval		最大内部番号

*/
/*============================================================================*/
UINT CCustomDataManager::GetMaxInnerNo(UINT group)
{
	vector<CTreeNode*>::iterator itr;
	UINT max = 0;
	for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
		if (HIWORD((*itr)->GetManager().groupno) == HIWORD(group)) {
			max = __max(max, LOWORD((*itr)->GetManager().groupno));
		}
	}
	return max;
}

/*============================================================================*/
/*! カスタムデータ管理クラス

-# 表示されている設備詳細画面が編集中かを取得する

@param
@retval		true 表示されている設備詳細画面が編集中

*/
/*============================================================================*/
bool CCustomDataManager::IsVisibleEditMode()
{
	vector<CTreeNode*>::iterator itr;
	for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
		if ((*itr)->GetEquipment().wnd != NULL && (*itr)->GetEquipment().mode == eTreeItemMode_Edit)
			return true;
	}

	map<CWnd*, CTreeNode*>::iterator itrwnd;
	for (itrwnd = mEditTreeNode.begin(); itrwnd != mEditTreeNode.end(); itrwnd++) {
		if ((*itrwnd).second->GetEquipment().wnd != NULL && (*itrwnd).second->GetEquipment().mode == eTreeItemMode_Edit)
			return true;
	}

	return false;
}
