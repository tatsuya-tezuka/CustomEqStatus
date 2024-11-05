/******************************************************************************
* @file    CustomDataManager.h
* @brief   【カスタマイズ機能】データ管理クラス
* @auther  SCC
* @date    2024/08/26 新規作成
* @par
******************************************************************************/
#pragma once

/* ------------------------------------------------------------------------------------ */
/* 監視・制御データベースアクセス                                                       */
/* ------------------------------------------------------------------------------------ */
#include "CustomDBAccess.h"
/* ------------------------------------------------------------------------------------ */
/* XMLファイルアクセス                                                                  */
/* ------------------------------------------------------------------------------------ */
#include "Markup.h"

//------------------------------------------------------------------------------------ 
//	グローバル定義
//------------------------------------------------------------------------------------
static const CString m_strAppName = _T("StationControl LayoutFile");		// 端末アプリ名の設定
static const double m_nVersionCustom = 2.1;										// レイアウト対応アプリバージョンの設定
enum eFileVersionMajor
{
	EN_FILE_VERSION_MAJOR = 100, /*1.00*/
};

static const int mTitleSize = 128;
static const int mNameSize = 64;
static const int mUnitSize = 10;
static const int mFormatSize = 10;
static const int mHeaderSize = 5;
static const int mEditLimitTextSize = 80;

/// ファイル関連
static const TCHAR* mAppDataSystem = { _T("NEC") };
static const TCHAR* mAppDataDataPath = { _T("DATA") };
static const TCHAR* mAppDataLogPath = { _T("LOG") };
static const TCHAR* mAppDataMasterPath = { _T("MASTER") };
static const TCHAR* mAppDataUserPath = { _T("USER") };
static const TCHAR* mAppSaveEquipmentFile = { _T("AppEquipment.xml") };

/// カスタム画面関連
static const TCHAR* mDefaultCustomTitle = { _T("～タイトル") };
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
static const COLORREF mManagerHideColor = RGB(200, 200, 200);
static const TCHAR* mCOntrolSignString = { _T("#CNTL#") };
static const TCHAR* mCOntrolSignStringDisplay = { _T("制御") };
static const TCHAR* mEditModeString = { _T("（編集中）") };
static const UINT mSortRange = 20;
static const UINT mGroupRange = 10;

/// タイトル
static const TCHAR* mMessage_Title_CustomManager = { _T("カスタム管理画面") };
static const TCHAR* mMessage_Title_CustomDetail = { _T("カスタム画面") };

/// メッセージ
static const TCHAR* mMessage_ManagerDelete = { _T("選択されたカスタム情報を削除します。\nよろしいですか？") };
static const TCHAR* mMessage_DetailSaveDifferentData = { _T("設備詳細の変更内容を保存しますか？") };
static const TCHAR* mMessage_DetailDelete = { _T("%sを削除します。\nよろしいですか？") };

static const int mMonMax = 5000;			// 監視の最大数
static const int mCtrlMax = 5000;			// 制御の最大数

// 設備詳細画面のヘッダー項目
enum { eDetailItem, eDetailValue, eDetailUnit, eDetailControl, eDetailMax };
static const TCHAR* mTreeItemHeader[eDetailMax] = {
	_T("項目"),
	_T("値"),
	_T("単位"),
	_T("制御"),
};

// 設備管理画面のヘッダー項目
enum { eManagerTitle, eManagerNote, eManagerGroup, eManagerMax };
static const TCHAR* mGroupListHeader[] = {
	_T("タイトル"),
	_T("備考"),
	_T("グループ"),
};

/// 各種メッセージID
enum eUserMessage{
	eUserMessage_Manager_Update = (WM_USER + 1),
	eUserMessage_Manager_Reset,
	eUserMessage_Manager_Delete,

	eUserMessage_Detail_Mode,

	eUserMessage_Drag_GetIndex,

	eUserMessage_TreeMonEventFirst = (WM_USER + 100),
	eUserMessage_TreeMonEventLast = (eUserMessage_TreeMonEventFirst + mMonMax),
	eUserMessage_TreeCtrlEventFirst = (eUserMessage_TreeMonEventLast + 1),
	eUserMessage_TreeCtrlEventLast = (eUserMessage_TreeCtrlEventFirst + mCtrlMax),
};

/// レイアウトファイル種別
enum eLayoutFileType {
	eLayoutFileType_SCL = 0,
	eLayoutFileType_XML,
};

/// ツリーデータ
enum eTreeItemMode{
	eTreeItemMode_Monitor = 0,
	eTreeItemMode_Edit,
	eTreeItemMode_Mofify,
};
enum eTreeItemKind{
	eTreeItemKind_User = 0,
	eTreeItemKind_Master,
};

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
	UINT			mode;				// モード変更：eTreeItemMode
	UINT			kind;				// 表示種別：eTreeItemKind
	UINT			type;				// 表示名種別：eTreeItemType
	TCHAR			title[mTitleSize];	// ウィンドウタイトル
	TCHAR			memo[mTitleSize];	// ウィンドメモ
	TCHAR			groupname[mNameSize];	// グループ
	UINT			groupno;			// グループ番号(下位ワード：グループ内番号　上位ワード：グループ番号)
	WINDOWPLACEMENT	placement;			// ウィンドウ位置情報
	UINT			treeopen;			// ツリーアイテムの開閉状態
	UINT			hwidth[mHeaderSize]; // 詳細画面ヘッダー幅
	UINT			zorder;				// Zオーダー
	UINT			monitor;			// モニタ識別
	UINT			group;				// グループ番号
	UINT			sortno;				// ソート番号
} stWindowInfo;

typedef struct{
	TCHAR			display[mNameSize];	// 表示名
	TCHAR			mname[mNameSize];	// 監視名
	TCHAR			cname[mNameSize];	// 制御名
	TCHAR			unit[mUnitSize];	// 単位
	UINT			formattype;			// フォーマット種別：eControl
	TCHAR			format[mFormatSize];// フォーマット書式文字列
} stMonCtrlData;

typedef struct {
	COLORREF		back;				// 背景色
	COLORREF		textback;			// テキスト背景色
	COLORREF		text;				// テキスト色
	COLORREF		value;				// 値テキスト色
	COLORREF		unit;				// 単位テキスト色
	LOGFONT			font;				// 詳細画面フォント
} stColorData;

/* ------------------------------------------------------------------------------------ */
/* ドラッグデータ                                                                       */
/* ------------------------------------------------------------------------------------ */
typedef struct {
	UINT		type;				// 監視、制御、WFD
	POINT		point;				// マウス位置情報
	vector<int>	indexes;			// ドラッグアイテム
	vector<TCHAR*> nodes;			// ドラッグアイテム
} stDragData;
enum { eFromType_None, eFromType_Mon, eFromType_Cntl, eFromType_Custom };


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

	TCHAR				xmlfile[_MAX_PATH];	// XMLファイル名
	stWindowInfo		wininfo;			// 設備詳細ウィンドウ情報
	stMonCtrlData		monctrl;			// 監視制御データ
	stColorData			color;				// 色・フォント情報
	vector<CTreeNode*>	children;			// 子アイテムリスト

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	HTREEITEM			GetTreeItem()	{ return treeitem; }
	void				SetTreeItem(HTREEITEM val)	{ treeitem = val; }
	TCHAR*				GetXmlFileName() { return xmlfile; }
	void				SetXmlFileName(TCHAR* xml)
	{
		swprintf_s(xmlfile, _MAX_PATH, _T("%s"), (LPCTSTR)xml);
	}
	stWindowInfo&		GetWindowInfo() { return wininfo; }
	stMonCtrlData&		GetMonCtrl() { return monctrl; }
	stColorData&		GetColor() { return color; }
	CTreeNode*			GetParentNode() { return parent; }
	void				SetParentNode(CTreeNode* val)	{ parent = val; }
	vector<CTreeNode*>&	GetChildren() { return children; }

	// 指定した childID の子Nodeが存在しなければ、子Nodeを作成し子Nodeを返す
	// 指定した childID の子Nodeが存在していれば、既に存在する子Nodeを返す
	CTreeNode*	CreateTreeNode(HTREEITEM parent, HTREEITEM child, HTREEITEM hInsertAfter = TVI_FIRST);

	void	SortTreeNode(HTREEITEM parent);

	// ツリーノードの削除
	bool DeleteTreeNode(HTREEITEM target);

	// 指定ノードの削除
	void DeleteItemNode(CTreeNode* pnode);

	// ツリーノードの検索
	CTreeNode* SearchTreeNode(HTREEITEM target);
	CTreeNode* SearchTreeNodeType(UINT target);

	// ノードコピー
	void	CopyTreeNode(CTreeNode* copyNode);
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

	bool	SaveCustomLayout();
	bool	LoadCustomLayout();

	//bool operator==(CTreeNode* data);
	bool Equal(CTreeNode* data);
	bool Equal(stWindowInfo& data);
	bool Equal(stMonCtrlData& data);
	bool Equal(stColorData& data);
	bool Equal(LOGFONT& data);

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

	// 矩形の設定
	void setRectXml(CMarkup& xml, RECT& rect) const
	{
		xml.FindElem(_T("LEFT"));
		xml.SetData(rect.left);
		xml.FindElem(_T("TOP"));
		xml.SetData(rect.top);
		xml.FindElem(_T("RIGHT"));
		xml.SetData(rect.right);
		xml.FindElem(_T("BOTTOM"));
		xml.SetData(rect.bottom);
	}
	// ポイントの設定
	void setPointXml(CMarkup& xml, POINT& point) const
	{
		xml.FindElem(_T("X"));
		xml.SetData(point.x);
		xml.FindElem(_T("Y"));
		xml.SetData(point.y);
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

	/// 監視・制御データベースデータ管理関連
	CCustomDBAccess			mDataMonitor;
	CCustomDBAccess			mDataControl;

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
	/// 全てのウィンドウハンドル、ノードの削除
	void DeleteAll()
	{
		DeleteAllWnd();
		DeleteAllNode();
	}
	/// 全てのウィンドウハンドルの削除
	void DeleteAllWnd()
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr)->GetWindowInfo().wnd != NULL) {
				delete (*itr)->GetWindowInfo().wnd;
				(*itr)->GetWindowInfo().wnd = NULL;
			}
		}
	}
	/// 種別毎のウィンドウハンドルの削除
	void DeleteKindWnd(UINT kind)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr)->GetWindowInfo().wnd != NULL && (*itr)->GetWindowInfo().kind == kind) {
				delete (*itr)->GetWindowInfo().wnd;
				(*itr)->GetWindowInfo().wnd = NULL;
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
					(*itr)->GetWindowInfo().wnd = NULL;
				}
				break;
			}
		}
	}

	// 全てのノードの削除
	void DeleteAllNode()
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr) != NULL) {
				delete (*itr);
			}
		}
		mTreeNode.clear();
	}
	// 種別毎のノードの削除
	void DeleteKindNode(UINT kind)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr) != NULL && (*itr)->GetWindowInfo().wnd != NULL && (*itr)->GetWindowInfo().kind == kind) {
				delete (*itr)->GetWindowInfo().wnd;
				(*itr)->GetWindowInfo().wnd = NULL;
				delete (*itr);
			}
		}
	}
	// 個別ノードの削除
	void DeleteItemNode(CTreeNode* p)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr) == p) {
				if ((*itr) != NULL) {
					delete (*itr);
				}
				mTreeNode.erase(itr);
				break;
			}
		}
	}
	// 個別ノードのクリア
	void ClearItemNode(CTreeNode* p)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr) == p) {
				if ((*itr) != NULL) {
					vector<CTreeNode*>::iterator itrch;
					for (itrch = (*itr)->GetChildren().begin(); itrch != (*itr)->GetChildren().end(); itrch++) {
						if ((*itrch) != NULL) {
							delete (*itrch);
						}
					}
					(*itr)->GetChildren().clear();
				}
				break;
			}
		}
	}
	// ノードのクローン作成
	CTreeNode* CloneItemNode(CTreeNode* source, CTreeNode* dest)
	{
		if (dest == NULL) {
			dest = new CTreeNode(0, NULL, NULL);
		}
		ClearItemNode(dest);

		dest->CopyTreeNode(source);
		return dest;
	}

	/// ノード毎の登録数を取得する
	UINT GetNodeKindCount(UINT kind)
	{
		UINT count = 0;
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr)->GetWindowInfo().wnd != NULL && (*itr)->GetWindowInfo().kind == kind) {
				count++;
			}
		}
		return count;
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
	bool	GetNodeColor(CWnd* pwnd, UINT type, stColorData& color);
	bool	SetNodeColor(CWnd* pwnd, UINT type, stColorData& color);

	/// ◆設備制御詳細画面レイアウト
	bool	SaveTreeData(CString strFile, CWnd* pTargetWnd = NULL);
	bool	LoadTreeData(CString strFile, bool bClear);
	bool	SaveTreeDataXml(CString strFile, CWnd* pTargetWnd = NULL);
	CTreeNode* LoadTreeDataXml(CString strFile, UINT kind=UINT_MAX);

	/// Zオーダー設定
	void	SetTreeZorder();
	void	ResetTreeZorder();

	/// ファイル管理
	void	LoadEquipmentData(UINT typeLayout, CString strfile, bool bClear = true);
	void	SaveEquipmentData(UINT typeLayout, CString strfile, CWnd* pTargetWnd = NULL);

	bool	SaveCustomLayout(CArchive& ar);
	bool	LoadCustomLayout(CArchive& ar);

	/// 画面連結関連
	UINT	GetMaxInnerNo(UINT group);

	/// 監視・制御データベースデータ管理関連
	CCustomDBAccess& GetDataMonitor() { return mDataMonitor; }
	CCustomDBAccess& GetDataControl() { return mDataControl; }

protected:
	/// カラー情報
	bool	getNodeTypeColor(CTreeNode* pnode, UINT type, stColorData& color);
	bool	setNodeTypeColor(CTreeNode* pnode, UINT type, stColorData& color);

	/* ------------------------------------------------------------------------------------ */
};

