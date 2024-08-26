#pragma once
//------------------------------------------------------------------------------------ 
//	関連ヘッダーファイル
//------------------------------------------------------------------------------------
#include "Markup.h"

//------------------------------------------------------------------------------------ 
//	グローバル定義
//------------------------------------------------------------------------------------
enum eFileVersionMajor
{
	EN_FILE_VERSION_MAJOR = 100, /*1.00*/
};

static const int mTitleSize = 128;
static const int mNameSize = 64;
static const int mUnitSize = 10;
static const int mFormatSize = 10;
static const int mHeaderSize = 5;
/// ファイル関連
static const TCHAR* mAppDataSystem = { _T("NEC") };
static const TCHAR* mAppDataDataPath = { _T("DATA") };
static const TCHAR* mAppDataLogPath = { _T("LOG") };
static const TCHAR* mAppSaveEquipmentFile = { _T("AppEquipment.xml") };

/// カスタム画面関連
static const TCHAR* mDefaultCustomTitle = { _T("タイトル") };
static const TCHAR* mDefaultCustomRoot = { _T("ルート") };
static const TCHAR* mDefaultCustomMainText = { _T("メインノード") };
static const TCHAR* mDefaultCustomSubText = { _T("サブノード") };
static const TCHAR* mDefaultCustomItemText = { _T("リーフ") };
static const int mTreeHeaderSize = 100;
static const int mTreeHeaderItemSize = 200;
static const int mTreeFontHeight = 16;
static const int mListFontHeight = 16;
static const COLORREF mRootNodeBackColor = RGB(180, 180, 180);
static const COLORREF mDragBackColor = RGB(250, 250, 250);
static const COLORREF mDragTextColor = RGB(0, 0, 0);
static const TCHAR* mCOntrolSignString = { _T("#CNTL#") };
static const TCHAR* mCOntrolSignStringDisplay = { _T("制御") };

static const int mMonMax = 5000;			// 監視の最大数
static const int mCtrlMax = 5000;			// 制御の最大数

/// 各種メッセージID
enum eUserMessage{
	eUserMessage_Manger_New = (WM_USER + 1),
	eUserMessage_Manager_Update,
	eUserMessage_Manager_Delete,
	eUserMessage_Manager_ResizeFit,
	eUserMessage_Manager_Reset,
	eUserMessage_Manager_Grid,

	eUserMessage_Drag_Select,
	eUserMessage_Drag_DropTarget,
	eUserMessage_Drag_GetIndex,

	eUserMessage_TreeMonEventFirst = (WM_USER + 100),
	eUserMessage_TreeMonEventLast = (eUserMessage_TreeMonEventFirst + mMonMax),
	eUserMessage_TreeCtrlEventFirst = (eUserMessage_TreeMonEventLast + 1),
	eUserMessage_TreeCtrlEventLast = (eUserMessage_TreeCtrlEventFirst + mCtrlMax),
};

/// ツリーデータ
enum eTreeItemType{
	eTreeItemType_Window = 0x00000000,
	eTreeItemType_Title = 0x00000001,
	eTreeItemType_Main = 0x00000002,
	eTreeItemType_Sub = 0x00000003,
	eTreeItemType_Item = 0x00000004,
};

enum eTreeItemSubType{
	eTreeItemSubType_Item = 0x00000000,
	eTreeItemSubType_Value = 0x00000001,
	eTreeItemSubType_Unit = 0x00000002,
	eTreeItemSubType_Control = 0x00000003,
};

enum eColorType {
	eColorType_Window,
	eColorType_TitleBack,
	eColorType_TitleText,
	eColorType_MainBack,
	eColorType_MainText,
	eColorType_SubBack,
	eColorType_SubText,
	eColorType_ItemBack,
	eColorType_ItemText,
	eColorType_ValueText,
	eColorType_UnitText,

	eColorType_Max,
};

typedef struct{
	CWnd*			wnd;				// 設備詳細ウィンドウハンドル
	CWnd*			tree;				// 設備詳細ツリーハンドルドル
	CWnd*			manager;			// 管理ウィンドウハンドル（CCustomManagerでセット）
	UINT			type;				// 表示名種別：eTreeItemType
	TCHAR			title[mTitleSize];	// ウィンドウタイトル
	WINDOWPLACEMENT	placement;			// ウィンドウ位置情報
	UINT			treeopen;			// ツリーアイテムの開閉状態
	UINT			hwidth[mHeaderSize]; // 詳細画面ヘッダー幅
	UINT			zorder;				// Zオーダー
	UINT			monitor;			// モニタ識別
} stWindowInfo;

typedef struct{
	TCHAR			display[mNameSize];	// 表示名
	TCHAR			mname[mNameSize];	// 監視名
	TCHAR			cname[mNameSize];	// 制御名
	TCHAR			unit[mUnitSize];	// 単位
	UINT			formattype;			// フォーマット種別：eControl
	TCHAR			format[mFormatSize];// フォーマット書式文字列
	TCHAR			group[mNameSize];	// グループ
} stMonCtrlData;

typedef struct{
	COLORREF		back;				// 背景色
	COLORREF		textback;			// テキスト背景色
	COLORREF		text;				// テキスト色
	COLORREF		value;				// 値テキスト色
	COLORREF		unit;				// 単位テキスト色
	LOGFONT			font;				// 詳細画面フォント
} stColorData;

//------------------------------------------------------------------------------------ 
//	ツリー型データクラス
//------------------------------------------------------------------------------------
class CTreeNode {
public:
	// 指定された id の Node を作る。
	CTreeNode(HTREEITEM id, CWnd* pwnd, CWnd* ptree);
	virtual ~CTreeNode();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	HTREEITEM			treeitem;			// ノードアイテム
	CTreeNode*			parent;				// 親ノード
	vector<CTreeNode*>	children;			// 子アイテムリスト

	stWindowInfo		wininfo;			// 設備詳細ウィンドウ情報
	stMonCtrlData		monctrl;			// 監視制御データ
	stColorData			color;				// 色・フォント情報

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	HTREEITEM			GetTreeItem()	{ return treeitem; }
	void				SetTreeItem(HTREEITEM val)	{ treeitem = val; }
	stWindowInfo&		GetWindowInfo() { return wininfo; }
	stMonCtrlData&		GetMonCtrl() { return monctrl; }
	stColorData&		GetColor() { return color; }
	CTreeNode*			GetParentNode() { return parent; }
	void				SetParentNode(CTreeNode* val)	{ parent = val; }
	vector<CTreeNode*>&	GetChildren() { return children; }

	// 指定した childID の子Nodeが存在しなければ、子Nodeを作成し子Nodeを返す
	// 指定した childID の子Nodeが存在していれば、既に存在する子Nodeを返す
	CTreeNode*	CreateTreeNode(HTREEITEM parent, HTREEITEM child, HTREEITEM hInsertAfter = TVI_FIRST);

	// ツリーノードの削除
	bool DeleteTreeNode(HTREEITEM target);

	// ツリーノードの検索
	CTreeNode* SearchTreeNode(HTREEITEM target);
	CTreeNode* SearchTreeNodeType(UINT target);

	void	CopyItem(CTreeNode* copyNode, bool bColorOnly = false)
	{
		if (bColorOnly == false){
			memcpy(&wininfo, &(copyNode->wininfo), sizeof(stWindowInfo));
			memcpy(&monctrl, &(copyNode->monctrl), sizeof(stMonCtrlData));
		}
		memcpy(&color, &(copyNode->color), sizeof(stColorData));
	}

	/// ◆設備制御詳細画面レイアウト
	bool	SaveTreeNode(CArchive& ar);
	bool	LoadTreeNode(CArchive& ar);
	bool	SaveTreeNodeXml(CMarkup& xml);
	bool	LoadTreeNodeXml(CMarkup& xml);

protected:
	void	deleteNode(CTreeNode* pnode);

	/// ◆設備制御詳細画面レイアウト
	//bool	saveChildNodeXml(CMarkup& ar, CTreeNode* pnode);
	//bool	loadChildNodeXml(CMarkup& ar, CTreeNode* pnode);

	/// CArchive
	// 矩形の読込
	void loadRect(CArchive& ar, RECT& rect)
	{
		ar >> rect.left;
		ar >> rect.top;
		ar >> rect.right;
		ar >> rect.bottom;
	}
	// ポイントの読込
	void loadPoint(CArchive& ar, POINT& point) const
	{
		ar >> point.x;
		ar >> point.y;
	}

	// 矩形の保存
	void saveRect(CArchive& ar, RECT& rect) const
	{
		ar << rect.left;
		ar << rect.top;
		ar << rect.right;
		ar << rect.bottom;
	}
	// ポイントの保存
	void savePoint(CArchive& ar, POINT& point) const
	{
		ar << point.x;
		ar << point.y;
	}

	/// XML
	// 矩形の読込
	void loadRectXml(CMarkup& xml, RECT& rect)
	{
		xml.FindElem(_T("LEFT"));
		rect.left = _wtoi(xml.GetData());
		xml.FindElem(_T("TOP"));
		rect.top = _wtoi(xml.GetData());
		xml.FindElem(_T("RIGHT"));
		rect.right = _wtoi(xml.GetData());
		xml.FindElem(_T("BOTTOM"));
		rect.bottom = _wtoi(xml.GetData());
	}
	// ポイントの読込
	void loadPointXml(CMarkup& xml, POINT& point) const
	{
		xml.FindElem(_T("X"));
		point.x = _wtoi(xml.GetData());
		xml.FindElem(_T("Y"));
		point.y = _wtoi(xml.GetData());
	}

	// 矩形の保存
	void saveRectXml(CMarkup& xml, RECT& rect) const
	{
		xml.AddElem(_T("LEFT"), rect.left);
		xml.AddElem(_T("TOP"), rect.top);
		xml.AddElem(_T("RIGHT"), rect.right);
		xml.AddElem(_T("BOTTOM"), rect.bottom);
	}
	// ポイントの保存
	void savePointXml(CMarkup& xml, POINT& point) const
	{
		xml.AddElem(_T("X"), point.x);
		xml.AddElem(_T("Y"), point.y);
	}

	/* ------------------------------------------------------------------------------------ */
};

//------------------------------------------------------------------------------------ 
//	カスタマイズ機能のデータマネージャークラス
//------------------------------------------------------------------------------------
class CCustomDataManager
{
public:
	CCustomDataManager();
	~CCustomDataManager();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	/// ツリーノード
	vector<CTreeNode*>		mTreeNode;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	/// ツリーノード取得
	vector<CTreeNode*>&	GetTreeNode() { return mTreeNode; }
	// ツリーノード取得
	void	AddTreeNode(CTreeNode* val)
	{
		//val->getColor().textback = mRootNodeBackColor;
		mTreeNode.push_back(val);
	}
	/// ツリーノード内のアイテムの検索
	CTreeNode* SearchItemNode(CWnd* pTarget, HTREEITEM target)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
			if ((*itr)->GetWindowInfo().wnd == pTarget){
				return (*itr)->SearchTreeNode(target);
			}
		}
		return NULL;
	}
	/// ツリーノード内のアイテム種別の検索
	CTreeNode* SearchItemNodeType(CWnd* pTarget, UINT target)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
			if ((*itr)->GetWindowInfo().wnd == pTarget){
				return (*itr)->SearchTreeNodeType(target);
			}
		}
		return NULL;
	}
	/// ツリーノード内の設備詳細ウィンドウ検索
	CTreeNode* SearchWndNode(CWnd* pTarget)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
			if ((*itr)->GetWindowInfo().wnd == pTarget){
				return (*itr);
			}
		}
		return NULL;
	}
	/// 全てのウィンドウハンドルの削除
	void DeleteAllWnd()
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
			if ((*itr)->GetWindowInfo().wnd != NULL){
				delete (*itr)->GetWindowInfo().wnd;
			}
		}
	}
	/// 個別ウィンドウハンドルの削除
	void DeleteItemWnd(CWnd* p)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
			if ((*itr)->GetWindowInfo().wnd == p){
				if ((*itr)->GetWindowInfo().wnd != NULL){
					delete (*itr)->GetWindowInfo().wnd;
				}
				break;
			}
		}
	}

	// 全てのノードの削除
	void DeleteAllNode()
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
			if ((*itr) != NULL){
				delete (*itr);
			}
		}
		mTreeNode.clear();
	}
	// 個別ノードの削除
	void DeleteItemNode(CTreeNode* p)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
			if ((*itr) == p){
				if ((*itr) != NULL){
					delete (*itr);
				}
				mTreeNode.erase(itr);
				break;
			}
		}
	}
	// ノードの交換
	void SwapItemNode(UINT pos1, UINT pos2)
	{
		swap(mTreeNode[pos1], mTreeNode[pos2]);
	}
	void SwapItemNode(vector<CTreeNode*>::iterator itr1, vector<CTreeNode*>::iterator itr2)
	{
		iter_swap(itr1, itr2);
	}

	/// ◆カラー情報
	bool	GetNodeColor(CWnd* pwnd, UINT type, UINT subtype, stColorData& color);
	bool	SetNodeColor(CWnd* pwnd, UINT type, UINT subtype, stColorData& color);

	/// ◆設備制御詳細画面レイアウト
	bool	SaveTreeData(CString strFile, CWnd* pTargetWnd = NULL);
	bool	LoadTreeData(CString strFile, bool bClear);
	bool	SaveTreeDataXml(CString strFile, CWnd* pTargetWnd = NULL);
	bool	LoadTreeDataXml(CString strFile, bool bClear);

	/// Zオーダー設定
	void	SetTreeZorder();
	void	ResetTreeZorder();

protected:
	/// カラー情報
	bool	getNodeTypeColor(CTreeNode* pnode, UINT type, UINT subtype, stColorData& color);
	bool	setNodeTypeColor(CTreeNode* pnode, UINT type, UINT subtype, stColorData& color);

	/* ------------------------------------------------------------------------------------ */
};

