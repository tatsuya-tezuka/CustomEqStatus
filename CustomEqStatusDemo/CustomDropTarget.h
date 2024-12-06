#pragma once

// ドラッグ＆ドロップ用コールバック関数定義
typedef DROPEFFECT(CALLBACK* DROPTARGET_DRAGENTER)(CWnd* pWnd, void* pDataObject, UINT dwKeyState, CPoint point);
typedef DROPEFFECT(CALLBACK* DROPTARGET_DRAGOVER)(CWnd* pWnd, void* pDataObject, UINT dwKeyState, CPoint point);
typedef void(CALLBACK* DROPTARGET_DRAGLEAVE)(CWnd* pWnd);
typedef BOOL(CALLBACK* DROPTARGET_DRAGDROP)(CWnd* pWnd, void* pDataObject, UINT dwKeyState, CPoint point);

template<class T>
class CDropData
{
public:
	CDropData();
	~CDropData();

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
	T		mpData;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	set(T data);

protected:

	/* ------------------------------------------------------------------------------------ */

};
template<class T>
CDropData<T>::CDropData()
{
}
template<class T>
CDropData<T>::~CDropData()
{
}
template<class T>
void CDropData<T>::set(T data)
{
	mpData = data;
	TRACE("set[%s:%d]\n", typeid(T).name(), sizeof(T));
}


class CNode {
public:
	/// 指定された id の Node を作る
	CNode(const CString id=_T(""));

	virtual ~CNode();

	/// Node の id を返す
	CString getID() { return fID; }

	/// ドラッグウィンドウの設定
	void setWnd(CWnd* p) { pDragWnd = p; }
	/// ドラッグウィンドウを返す
	CWnd* getWnd() { return pDragWnd; }

	/// 指定した childID の子Nodeが存在しなければ、子Nodeを作成し子Nodeを返す。
	/// 指定した childID の子Nodeが存在していれば、既に存在する子Nodeを返す。
	CNode* createChildIfNotExist(const CString childID, UINT type);

	/// 子Node の一覧を返す
	const ::std::vector<CNode*>& getChildren() { return fChildren; }

	/// CTreeNodeの設定
	void setNodeData(CTreeNode* p) { pdata = p; }

	/// CTreeNodeを返す
	CTreeNode* getNodeData() { return pdata; }

private:
	CString fID;
	CWnd* pDragWnd;
	CTreeNode *pdata;
	::std::vector<CNode*> fChildren;
};


class CCustomDropObject
{
public:
	CCustomDropObject()
	{
		mCallbackDragEnter = NULL;
		mCallbackDragOver = NULL;
		mCallbackDragLeave = NULL;
		mCallbackDragDrop = NULL;
		mpBuff = NULL;
		mpNode = NULL;
		mBuffSize = 0;
	}
	virtual ~CCustomDropObject()
	{
		if (mpBuff != NULL)
			delete mpBuff;
		if (mpNode != NULL)
			delete mpNode;
	}

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// mFormat
	enum {
		DF_NONE = 0x00,
		DF_MANAGER_MASTER = 0x01,	// マスタカスタム管理
		DF_MASTER = 0x02,			// マスタカスタム
		DF_MANAGER_USER = 0x04,		// ユーザカスタム管理
		DF_USER = 0x08,				// ユーザカスタム
		DF_MONITOR = 0x10,			// 監視
		DF_CONTROL = 0x20			// 制御
	};
	//// mKind(eTreeItemTypeと同じ並び)
	//enum {
	//	DK_NONE,					// 
	//	DK_TITLE,					// タイトル（ツリー）
	//	DK_MAINNODE,				// メインノード（ツリー）
	//	DK_SUBNODE,					// サブノード（ツリー）
	//	DK_LEAF,					// リーフ（ツリー）
	//	DK_GROUP,					// グループ（グループリスト）
	//	DK_ITEM,					// アイテム（グループリスト）
	//};

	enum {
		DE_NONE,
		DE_COPY,
		DE_MOVE
	};

	enum {
		DT_TCHAR,
		DT_NODE,
	};


	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	UINT			mFormat;		// フォーマット
	UINT			mKind;			// 種別
	UINT			mDataType;		// データ種別
	void*			mItems;			// ドラッグアイテム

	DROPTARGET_DRAGENTER	mCallbackDragEnter;
	DROPTARGET_DRAGOVER		mCallbackDragOver;
	DROPTARGET_DRAGLEAVE	mCallbackDragLeave;
	DROPTARGET_DRAGDROP		mCallbackDragDrop;

	CNode*			mpNode;
	TCHAR*			mpBuff;
	UINT			mBuffSize;

	UINT					datatype;
	CDropData<TCHAR*>		mpchar;
	CDropData<CTreeNode*>	mpnode;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
	void	CreateBuffer(CStringArray& items);
	void	CreateBuffer(CNode* items);
	void	CreateNode(CNode* item, CNode* node);
	void*	GetBuffer() const;
	UINT	GetBuffSize() const;

	/* ------------------------------------------------------------------------------------ */
};

class CCustomDropTarget
{
public:
	CCustomDropTarget()
	{
		mObjectList.clear();
		mTargetWnd = NULL;
	}
	virtual ~CCustomDropTarget()
	{
	}

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
	map<CWnd*, CCustomDropObject>	mObjectList;
	CWnd* mTargetWnd;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	void	Register(CWnd* pDragWnd, UINT format);
	void	OnDragBegin(CStringArray& items, UINT format, UINT dtype);
	void	OnDragBegin(CNode* items, UINT format, UINT dtype);
	UINT	OnDragMove(CWnd* pDragWnd, UINT dwKeyState, POINT point);
	UINT	OnDragDrop(CWnd* pDragWnd, UINT dropEffect, POINT point);
	void	OnDragLeave(CWnd* pDragWnd);

	void	SetCallbackDragEnter(CWnd* pDragWnd, DROPTARGET_DRAGENTER callback);
	void	SetCallbackDragOver(CWnd* pDragWnd, DROPTARGET_DRAGOVER callback);
	void	SetCallbackDragLeave(CWnd* pDragWnd, DROPTARGET_DRAGLEAVE callback);
	void	SetCallbackDragDrop(CWnd* pDragWnd, DROPTARGET_DRAGDROP callback);

	void	SetDragKind(CWnd* pDragWnd, UINT kind)
	{
		map<CWnd*, CCustomDropObject>::iterator itr;
		for (itr = mObjectList.begin(); itr != mObjectList.end(); itr++) {
			if ((*itr).first == pDragWnd) {
				(*itr).second.mKind = kind;
				break;
			}
		}
	}

	UINT	GetDragKind(CWnd* pDragWnd)
	{
		map<CWnd*, CCustomDropObject>::iterator itr;
		for (itr = mObjectList.begin(); itr != mObjectList.end(); itr++) {
			if ((*itr).first == pDragWnd) {
				return (*itr).second.mKind;
			}
		}
		return eTreeItemType_Window;
	}

protected:
	void	AllDragLeave(CWnd* pDropWnd=NULL);

	/* ------------------------------------------------------------------------------------ */
};

