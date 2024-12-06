#pragma once

// �h���b�O���h���b�v�p�R�[���o�b�N�֐���`
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
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	T		mpData;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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
	/// �w�肳�ꂽ id �� Node �����
	CNode(const CString id=_T(""));

	virtual ~CNode();

	/// Node �� id ��Ԃ�
	CString getID() { return fID; }

	/// �h���b�O�E�B���h�E�̐ݒ�
	void setWnd(CWnd* p) { pDragWnd = p; }
	/// �h���b�O�E�B���h�E��Ԃ�
	CWnd* getWnd() { return pDragWnd; }

	/// �w�肵�� childID �̎qNode�����݂��Ȃ���΁A�qNode���쐬���qNode��Ԃ��B
	/// �w�肵�� childID �̎qNode�����݂��Ă���΁A���ɑ��݂���qNode��Ԃ��B
	CNode* createChildIfNotExist(const CString childID, UINT type);

	/// �qNode �̈ꗗ��Ԃ�
	const ::std::vector<CNode*>& getChildren() { return fChildren; }

	/// CTreeNode�̐ݒ�
	void setNodeData(CTreeNode* p) { pdata = p; }

	/// CTreeNode��Ԃ�
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
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// mFormat
	enum {
		DF_NONE = 0x00,
		DF_MANAGER_MASTER = 0x01,	// �}�X�^�J�X�^���Ǘ�
		DF_MASTER = 0x02,			// �}�X�^�J�X�^��
		DF_MANAGER_USER = 0x04,		// ���[�U�J�X�^���Ǘ�
		DF_USER = 0x08,				// ���[�U�J�X�^��
		DF_MONITOR = 0x10,			// �Ď�
		DF_CONTROL = 0x20			// ����
	};
	//// mKind(eTreeItemType�Ɠ�������)
	//enum {
	//	DK_NONE,					// 
	//	DK_TITLE,					// �^�C�g���i�c���[�j
	//	DK_MAINNODE,				// ���C���m�[�h�i�c���[�j
	//	DK_SUBNODE,					// �T�u�m�[�h�i�c���[�j
	//	DK_LEAF,					// ���[�t�i�c���[�j
	//	DK_GROUP,					// �O���[�v�i�O���[�v���X�g�j
	//	DK_ITEM,					// �A�C�e���i�O���[�v���X�g�j
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
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	UINT			mFormat;		// �t�H�[�}�b�g
	UINT			mKind;			// ���
	UINT			mDataType;		// �f�[�^���
	void*			mItems;			// �h���b�O�A�C�e��

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
	/* �����o�֐�                                                                           */
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
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	map<CWnd*, CCustomDropObject>	mObjectList;
	CWnd* mTargetWnd;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
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

