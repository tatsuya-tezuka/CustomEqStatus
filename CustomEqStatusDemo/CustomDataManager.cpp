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

CTreeNode::CTreeNode(HTREEITEM id, CWnd* pwnd, CWnd* ptree)
: treeitem(id)
{
	memset(&wininfo, 0, sizeof(stWindowInfo));
	memset(&monctrl, 0, sizeof(stMonCtrlData));
	memset(&color, 0, sizeof(stColorData));
	color.back = GetSysColor(COLOR_WINDOW);
	color.textback = GetSysColor(COLOR_WINDOW);
	color.text = GetSysColor(COLOR_BTNTEXT);
	color.value = GetSysColor(COLOR_BTNTEXT);
	color.unit = GetSysColor(COLOR_BTNTEXT);
	CFont font;
	font.CreateStockObject(DEFAULT_GUI_FONT);
	if (font.GetLogFont(&color.font)){
		color.font.lfHeight = -mTreeFontHeight;
		color.font.lfWeight = FW_BOLD;
		font.DeleteObject();
	}

	wininfo.wnd = pwnd;
	wininfo.tree = ptree;
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

bool CTreeNode::Equal(CTreeNode* data)
{
	if (Equal(data->GetWindowInfo()) == false)
		return false;
	if (Equal(data->GetMonCtrl()) == false)
		return false;
	if (Equal(data->GetColor()) == false)
		return false;

	if (children.size() != data->GetChildren().size())
		return false;

	UINT size = (UINT)children.size();
	for (UINT i = 0; i < size; i++) {
		if (children[i]->Equal(data->GetChildren()[i]) == false)
			return false;
	}

	return true;
}
bool CTreeNode::Equal(stWindowInfo& data)
{
	if (CString(wininfo.title) != CString(data.title))
		return false;

	return true;
}
bool CTreeNode::Equal(stMonCtrlData& data)
{
	if (CString(monctrl.display) != CString(data.display))
		return false;
	if (CString(monctrl.mname) != CString(data.mname))
		return false;
	if (CString(monctrl.cname) != CString(data.cname))
		return false;
	if (CString(monctrl.unit) != CString(data.unit))
		return false;
	if (CString(monctrl.format) != CString(data.format))
		return false;
	if (monctrl.formattype != data.formattype)
		return false;

	return true;
}
bool CTreeNode::Equal(stColorData& data)
{
	if (color.back != data.back)
		return false;
	if (color.textback != data.textback)
		return false;
	if (color.text != data.text)
		return false;
	if (color.value != data.value)
		return false;
	if (color.unit != data.unit)
		return false;
	if (Equal(data.font) == false)
		return false;

	return true;
}
bool CTreeNode::Equal(LOGFONT& data)
{
	if (color.font.lfWeight != data.lfWeight)
		return false;
	if (CString(color.font.lfFaceName) != CString(data.lfFaceName))
		return false;
	if (color.font.lfItalic != data.lfItalic)
		return false;
	if (color.font.lfHeight != data.lfHeight)
		return false;

	return true;
}

/*============================================================================*/
/*! ツリーノード

-# ツリーノードの作成

@param  parent			親ノード
@param  child			作成する子ノード
@param  hInsertAfter	登録時の位置情報

@retval CTreeNode	ノードクラスポインタ
*/
/*============================================================================*/
CTreeNode* CTreeNode::CreateTreeNode(HTREEITEM parent, HTREEITEM child, HTREEITEM hInsertAfter/* = TVI_FIRST*/)
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
	CTreeNode *childitem = new CTreeNode(child, wininfo.wnd, wininfo.tree);
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
	return left->GetWindowInfo().sortno < right->GetWindowInfo().sortno;
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
	CLogTraceEx::Write(_T("***"), _T("CTreeNode"), _T("DeleteTreeNode"), pnode->wininfo.title, _T(""), nLogEx::debug);
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
	memcpy(&wininfo, &(copyNode->wininfo), sizeof(stWindowInfo));
	memcpy(&monctrl, &(copyNode->monctrl), sizeof(stMonCtrlData));
	memcpy(&color, &(copyNode->color), sizeof(stColorData));

	// 子リストをコピーする
	vector<CTreeNode*>::iterator itr;
	for (itr = copyNode->GetChildren().begin(); itr != copyNode->GetChildren().end(); itr++) {
		CTreeNode* child = new CTreeNode((HTREEITEM)NULL, NULL, NULL);
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
	if (wininfo.type == target)
		return this;

	// 子アイテムリストに存在するか確認する
	vector<CTreeNode *>::iterator itr;
	for (itr = children.begin(); itr != children.end(); itr++) {
		if ((*itr)->GetWindowInfo().type == target) {
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
	if (pnode->GetWindowInfo().type == type){
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
	if (pnode->GetWindowInfo().type == type){
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
		if (pTargetWnd != NULL && pTargetWnd != (*itr)->GetWindowInfo().wnd) {
			continue;
		}
		if ((*itr)->GetWindowInfo().kind != eTreeItemKind_User)
			continue;

		(*itr)->SaveTreeNode(mArc);
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
	if (wininfo.wnd != NULL) {
		memset(&wininfo.placement, 0, sizeof(WINDOWPLACEMENT));
		wininfo.placement.length = sizeof(WINDOWPLACEMENT);
		wininfo.wnd->GetWindowPlacement(&wininfo.placement);
		//★((CCustomDetail*)wininfo.wnd)->GetHeaderWidth(wininfo.hwidth, mHeaderSize);
	}

	// ウィンドウ情報
	ar << wininfo.mode;
	ar << wininfo.kind;
	ar << wininfo.type;
	if (wininfo.type == eTreeItemType_Title) {
		ar << CString(wininfo.title);
		ar << CString(wininfo.memo);
		ar << CString(wininfo.groupname);
		ar << wininfo.groupno;
		ar << wininfo.monitor;
		ar << wininfo.placement.flags;
		if (wininfo.wnd == NULL)
			ar << FALSE;
		else
			ar << wininfo.wnd->IsWindowVisible();
		savePoint(ar, wininfo.placement.ptMinPosition);
		savePoint(ar, wininfo.placement.ptMaxPosition);
		saveRect(ar, wininfo.placement.rcNormalPosition);
		ar << (UINT)mHeaderSize;
		for (int i = 0; i < mHeaderSize; i++) {
			ar << wininfo.hwidth[i];
		}
		ar << wininfo.zorder;
	}
	//★wininfo.treeopen = ((CCustomDetail*)wininfo.wnd)->GetTreeExpandState(treeitem);
	ar << wininfo.treeopen;

	// 監視制御情報
	ar << CString(monctrl.display);
	if (wininfo.type == eTreeItemType_Item) {
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
		CTreeNode* pnode = new CTreeNode((HTREEITEM)NULL, NULL, NULL);
		if (pnode->LoadTreeNode(mArc) == false) {
			delete pnode;
			break;
		}
		mTreeNode.push_back(pnode);
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
	ar >> wininfo.mode;
	wininfo.mode = eTreeItemMode_Monitor; // ロード時は常に監視モード
	ar >> wininfo.kind;
	ar >> wininfo.type;
	if (wininfo.type == eTreeItemType_Title) {
		ar >> str;
		swprintf_s(wininfo.title, mTitleSize, _T("%s"), (LPCTSTR)str);
		ar >> str;
		swprintf_s(wininfo.memo, mTitleSize, _T("%s"), (LPCTSTR)str);
		ar >> str;
		swprintf_s(wininfo.groupname, mNameSize, _T("%s"), (LPCTSTR)str);
		ar >> wininfo.groupno;
		ar >> wininfo.monitor;
		wininfo.placement.length = sizeof(WINDOWPLACEMENT);
		ar >> wininfo.placement.flags;
		ar >> wininfo.placement.showCmd;
		loadPoint(ar, wininfo.placement.ptMinPosition);
		loadPoint(ar, wininfo.placement.ptMaxPosition);
		loadRect(ar, wininfo.placement.rcNormalPosition);
		UINT hsize;
		ar >> hsize;
		for (int i = 0; i < (int)hsize; i++) {
			ar >> wininfo.hwidth[i];
		}
		ar >> wininfo.zorder;
	}
	ar >> wininfo.treeopen;;

	// 監視制御情報
	ar >> str;
	swprintf_s(monctrl.display, mNameSize, _T("%s"), (LPCTSTR)str);
	if (wininfo.type == eTreeItemType_Item) {
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
	ar >> str;
	swprintf_s(color.font.lfFaceName, LF_FACESIZE, _T("%s"), (LPCTSTR)str);

	// 子ノードの取得
	UINT size;
	ar >> size;
	children.reserve(size);
	for (UINT i = 0; i < size; i++) {
		//CTreeNode* child = new CTreeNode((HTREEITEM)i, NULL, NULL);
		CTreeNode* child = new CTreeNode((HTREEITEM)NULL, NULL, NULL);
		child->LoadTreeNode(ar);
		children.push_back(child);
	}

	return true;
}

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
		if ((*itr)->GetWindowInfo().wnd == NULL) {
			continue;
		}
		if ((*itr)->GetWindowInfo().wnd->IsWindowVisible() == FALSE) {
			continue;
		}

		if ((*itr)->SaveCustomLayout() == false)
			continue;

		// ここまできたらXMLファイル名を保存する
		count++;
	}
	// 保存データ数の設定
	ar << count;

	// 個々のデータを保存
	for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
		if ((*itr)->GetWindowInfo().wnd == NULL) {
			continue;
		}
		if ((*itr)->GetWindowInfo().wnd->IsWindowVisible() == FALSE) {
			continue;
		}

		if ((*itr)->SaveCustomLayout() == false)
			continue;

		// ここまできたらXMLファイル名を保存する
		ar << CString((*itr)->GetXmlFileName());
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

@param

@retval
*/
/*============================================================================*/
bool CTreeNode::SaveCustomLayout()
{
	if (CString(xmlfile).IsEmpty() == true)
		return false;

	// ウィンドウ位置情報の取得
	memset(&wininfo.placement, 0, sizeof(WINDOWPLACEMENT));
	wininfo.placement.length = sizeof(WINDOWPLACEMENT);
	wininfo.wnd->GetWindowPlacement(&wininfo.placement);

	CMarkup xml;
	xml.Load(xmlfile);
	xml.FindElem(_T("ROOT"));
	xml.IntoElem();
	xml.FindElem(_T("EQUIPMENT"));
	xml.IntoElem();
	xml.FindElem(_T("WINDOWINFO"));
	xml.IntoElem();

	xml.FindElem(_T("FLAGS"));
	xml.SetData(wininfo.placement.flags);
	xml.FindElem(_T("SHOWCMD"));
	xml.SetData(wininfo.wnd->IsWindowVisible());
	setPointXml(xml, wininfo.placement.ptMinPosition);
	setPointXml(xml, wininfo.placement.ptMaxPosition);
	setRectXml(xml, wininfo.placement.rcNormalPosition);

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

	// 現行のデータを削除する
	//DeleteKindWnd(eTreeItemKind_User);
	//DeleteKindNode(eTreeItemKind_User);
	//DeleteAll();

	CString strAppName;
	double nVersion;
	UINT count = 0;

	// 端末アプリ名の設定
	ar >> strAppName;
	// レイアウト対応アプリバージョンの設定
	ar >> nVersion;

	// 保存データ数の設定
	ar >> count;

	// 個々のデータを保存
	for (UINT i = 0; i < count; i++) {
		CString xmlfile;
		ar >> xmlfile;

		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if (CString((*itr)->GetXmlFileName()).MakeLower() == xmlfile.MakeLower()) {
				if ((*itr)->GetWindowInfo().wnd == NULL) {
					CCustomDetail* pitem = theApp.GetCustomControl().CreateEquipment((*itr));
					if (pitem == NULL)
						return false;

					// ウィンドウハンドルの設定
					(*itr)->GetWindowInfo().wnd = pitem;
				}
				(*itr)->GetWindowInfo().wnd->ShowWindow(SW_SHOWNA);
				(*itr)->GetWindowInfo().wnd->SetActiveWindow();
				// 常に監視モードとする
				(*itr)->GetWindowInfo().mode = eTreeItemMode_Monitor;
				break;
			}
		}

		//CTreeNode* pnode = LoadTreeDataXml(xmlfile);
		//// 設備詳細画面を復元する
		//CCustomDetail* pitem = theApp.GetCustomControl().CreateEquipment(pnode);
		//pitem->ShowWindow(SW_SHOW);
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

@param

@retval
*/
/*============================================================================*/
bool CTreeNode::LoadCustomLayout()
{
	if (CString(xmlfile).IsEmpty() == true)
		return false;

	// ウィンドウ位置情報の取得
	memset(&wininfo.placement, 0, sizeof(WINDOWPLACEMENT));
	wininfo.placement.length = sizeof(WINDOWPLACEMENT);
	wininfo.wnd->GetWindowPlacement(&wininfo.placement);

	CMarkup xml;
	xml.Load(xmlfile);
	xml.FindElem(_T("ROOT"));
	xml.IntoElem();
	xml.FindElem(_T("EQUIPMENT"));
	xml.IntoElem();
	xml.FindElem(_T("WINDOWINFO"));
	xml.IntoElem();

	xml.FindElem(_T("FLAGS"));
	xml.SetData(wininfo.placement.flags);
	xml.FindElem(_T("SHOWCMD"));
	xml.SetData(wininfo.wnd->IsWindowVisible());
	setPointXml(xml, wininfo.placement.ptMinPosition);
	setPointXml(xml, wininfo.placement.ptMaxPosition);
	setRectXml(xml, wininfo.placement.rcNormalPosition);

	xml.Save(xmlfile);

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
	xml.SetEOLLEN(sizeof(MCD_T("\n")) / sizeof(MCD_CHAR)-1);

	// BOMを未出力
	xml.SetBOM(false);

	xml.SetDocFlags(CMarkup::MDF_UTF8PREAMBLE);

	// <ROOT>を出力
	xml.AddElem(_T("ROOT"));
	xml.IntoElem();
	xml.AddElem(_T("VERSION"), EN_FILE_VERSION_MAJOR);

	if (pTargetWnd == NULL){
		xml.AddElem(_T("SIZE"), (UINT)mTreeNode.size());
	}
	else{
		xml.AddElem(_T("SIZE"), (UINT)1);
	}

	vector<CTreeNode*>::iterator itr;
	for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
		if (pTargetWnd != NULL && pTargetWnd != (*itr)->GetWindowInfo().wnd){
			continue;
		}
		xml.AddElem(_T("EQUIPMENT"));
		xml.IntoElem();
		(*itr)->SaveTreeNodeXml(xml);
		xml.OutOfElem();
	}
	xml.OutOfElem();
	xml.Save(strFile);
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CLogTraceEx::Write(_T("***"), _T("CCustomDataManager"), _T("SaveTreeDataXml"), _T("Stop"), _T(""), nLogEx::debug);
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
	// ウィンドウ位置情報取得
	if (wininfo.wnd != NULL){
		memset(&wininfo.placement, 0, sizeof(WINDOWPLACEMENT));
		wininfo.placement.length = sizeof(WINDOWPLACEMENT);
		wininfo.wnd->GetWindowPlacement(&wininfo.placement);
		//★((CCustomDetail*)wininfo.wnd)->GetHeaderWidth(wininfo.hwidth, mHeaderSize);
	}

	// ウィンドウ情報
	xml.AddElem(_T("WINDOWINFO"));
	xml.IntoElem();
	xml.AddElem(_T("MODE"), wininfo.mode);
	xml.AddElem(_T("KIND"), wininfo.kind);
	xml.AddElem(_T("TYPE"), wininfo.type);
	if (wininfo.type == eTreeItemType_Title){
		xml.AddElem(_T("TITLE"), wininfo.title);
		xml.AddElem(_T("MEMO"), wininfo.memo);
		xml.AddElem(_T("GROUP"), wininfo.groupname);
		xml.AddElem(_T("GROUPNO"), wininfo.groupno);
		xml.AddElem(_T("MONITOR"), wininfo.monitor);
		xml.AddElem(_T("FLAGS"), wininfo.placement.flags);
		xml.AddElem(_T("SHOWCMD"), (wininfo.wnd == NULL) ? 0 : wininfo.wnd->IsWindowVisible()/*wininfo.placement.showCmd*/);
		savePointXml(xml, wininfo.placement.ptMinPosition);
		savePointXml(xml, wininfo.placement.ptMaxPosition);
		saveRectXml(xml, wininfo.placement.rcNormalPosition);
		for (int i = 0; i < mHeaderSize; i++){
			CString str;
			str.Format(_T("HWIDTH%d"), i + 1);
			xml.AddElem(str, wininfo.hwidth[i]);
		}
		xml.AddElem(_T("ZORDER"), wininfo.zorder);
	}
	//★wininfo.treeopen = ((CCustomDetail*)wininfo.wnd)->GetTreeExpandState(treeitem);
	xml.AddElem(_T("TREEOPEN"), wininfo.treeopen);
	xml.OutOfElem();

	// 監視制御情報
	xml.AddElem(_T("MONCTRLINFO"));
	xml.IntoElem();
	xml.AddElem(_T("DISPLAY"), monctrl.display);
	if (wininfo.type == eTreeItemType_Item){
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
	xml.AddElem(_T("BACK"), color.back);
	xml.AddElem(_T("TEXTBACK"), color.textback);
	xml.AddElem(_T("TEXT"), color.text);
	xml.AddElem(_T("VALUE"), color.value);
	xml.AddElem(_T("UNIT"), color.unit);
	// フォント
	xml.AddElem(_T("LFHEIGHT"), color.font.lfHeight);
	xml.AddElem(_T("LFWIDTH"), color.font.lfWidth);
	xml.AddElem(_T("LFWEIGHT"), color.font.lfWeight);
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
		pnode = new CTreeNode((HTREEITEM)NULL, NULL, NULL);
		if (pnode->LoadTreeNodeXml(xml) == false){
			delete pnode;
			pnode = NULL;
			xml.OutOfElem();
			break;
		}

		swprintf_s(pnode->GetXmlFileName(), _MAX_PATH, _T("%s"), (LPCTSTR)strFile);
		if (kind != UINT_MAX) {
			pnode->GetWindowInfo().kind = kind;
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
	xml.FindElem(_T("WINDOWINFO"));
	xml.IntoElem();
	xml.FindElem(_T("MODE"));
	wininfo.mode = _wtoi(xml.GetData());
	wininfo.mode = eTreeItemMode_Monitor; // ロード時は常に監視モード
	xml.FindElem(_T("KIND"));
	wininfo.kind = _wtoi(xml.GetData());
	xml.FindElem(_T("TYPE"));
	wininfo.type = _wtoi(xml.GetData());
	if (wininfo.type == eTreeItemType_Title){
		xml.FindElem(_T("TITLE"));
		swprintf_s(wininfo.title, mTitleSize, _T("%s"), (LPCTSTR)xml.GetData());
		xml.FindElem(_T("MEMO"));
		swprintf_s(wininfo.memo, mTitleSize, _T("%s"), (LPCTSTR)xml.GetData());
		xml.FindElem(_T("GROUP"));
		swprintf_s(wininfo.groupname, mNameSize, _T("%s"), (LPCTSTR)xml.GetData());
		xml.FindElem(_T("GROUPNO"));
		wininfo.groupno = _wtoi(xml.GetData());
		xml.FindElem(_T("MONITOR"));
		wininfo.monitor = _wtoi(xml.GetData());
		wininfo.placement.length = sizeof(WINDOWPLACEMENT);
		xml.FindElem(_T("FLAGS"));
		wininfo.placement.flags = _wtoi(xml.GetData());
		xml.FindElem(_T("SHOWCMD"));
		wininfo.placement.showCmd = _wtoi(xml.GetData());
		loadPointXml(xml, wininfo.placement.ptMinPosition);
		loadPointXml(xml, wininfo.placement.ptMaxPosition);
		loadRectXml(xml, wininfo.placement.rcNormalPosition);
		for (int i = 0; i < mHeaderSize; i++){
			CString str;
			str.Format(_T("HWIDTH%d"), i + 1);
			xml.FindElem(str);
			wininfo.hwidth[i] = _wtoi(xml.GetData());
		}
		xml.FindElem(_T("ZORDER"));
		wininfo.zorder = _wtoi(xml.GetData());
	}
	xml.FindElem(_T("TREEOPEN"));
	wininfo.treeopen = _wtoi(xml.GetData());
	xml.OutOfElem();

	// 監視制御情報
	xml.FindElem(_T("MONCTRLINFO"));
	xml.IntoElem();
	xml.FindElem(_T("DISPLAY"));
	swprintf_s(monctrl.display, mNameSize, _T("%s"), (LPCTSTR)xml.GetData());
	if (wininfo.type == eTreeItemType_Item){
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
	xml.FindElem(_T("BACK"));
	color.back = _wtoi(xml.GetData());
	xml.FindElem(_T("TEXTBACK"));
	color.textback = _wtoi(xml.GetData());
	xml.FindElem(_T("TEXT"));
	color.text = _wtoi(xml.GetData());
	xml.FindElem(_T("VALUE"));
	color.value = _wtoi(xml.GetData());
	xml.FindElem(_T("UNIT"));
	color.unit = _wtoi(xml.GetData());
	// フォント
	xml.FindElem(_T("LFHEIGHT"));
	color.font.lfHeight = _wtoi(xml.GetData());
	xml.FindElem(_T("LFWIDTH"));
	color.font.lfWidth = _wtoi(xml.GetData());
	xml.FindElem(_T("LFWEIGHT"));
	color.font.lfWeight = _wtoi(xml.GetData());
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
		CTreeNode* child = new CTreeNode((HTREEITEM)NULL, NULL, NULL);
		child->LoadTreeNodeXml(xml);
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
			if (pWnd == (*itr)->GetWindowInfo().wnd){
				(*itr)->GetWindowInfo().zorder = pos++;
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
		winmap.insert(map<UINT, CWnd*>::value_type((*itr)->GetWindowInfo().zorder, (*itr)->GetWindowInfo().wnd));
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

	// 設備詳細画面を復元する
	vector<CTreeNode*>& treedata = GetTreeNode();
	vector<CTreeNode*>::iterator itr;
	for (itr = treedata.begin(); itr != treedata.end(); itr++) {
		if (bClear == false && (*itr)->GetWindowInfo().wnd != NULL)
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
	msg.Format(_T("TreeNode Count : %d"), GetTreeNode().size());
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

	DWORD	end_time = timeGetTime();
	theApp.GetCustomControl().PrintMemoryInfo();
	//=====================================================//
	//↓↓↓↓↓↓↓↓↓↓↓↓ Log ↓↓↓↓↓↓↓↓↓↓↓↓//
	CString msg;
	msg.Format(_T("TreeNode Count : %d"), GetTreeNode().size());
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
		if (HIWORD((*itr)->GetWindowInfo().groupno) == HIWORD(group)) {
			max = __max(max, LOWORD((*itr)->GetWindowInfo().groupno));
		}
	}
	return max;
}
