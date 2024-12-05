/******************************************************************************
* @file    CustomDataManager.h
* @brief   �y�J�X�^�}�C�Y�@�\�z�f�[�^�Ǘ��N���X
* @auther  SCC
* @date    2024/08/26 �V�K�쐬
* @par
******************************************************************************/
#pragma once

/* ------------------------------------------------------------------------------------ */
/* �Ď��E����f�[�^�x�[�X�A�N�Z�X                                                       */
/* ------------------------------------------------------------------------------------ */
#include "CustomDBAccess.h"
/* ------------------------------------------------------------------------------------ */
/* XML�t�@�C���A�N�Z�X                                                                  */
/* ------------------------------------------------------------------------------------ */
#include "Markup.h"

//------------------------------------------------------------------------------------ 
//	�O���[�o����`
//------------------------------------------------------------------------------------
static const CString m_strAppName = _T("StationControl LayoutFile");		// �[���A�v�����̐ݒ�
static const double m_nVersionCustom = 2.1;									// ���C�A�E�g�Ή��A�v���o�[�W�����̐ݒ�
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

/// �t�@�C���֘A
static const TCHAR* mAppDataSystem = { _T("NEC") };
static const TCHAR* mAppDataDataPath = { _T("DATA") };
static const TCHAR* mAppDataLogPath = { _T("LOG") };
static const TCHAR* mAppDataMasterPath = { _T("MASTER") };
static const TCHAR* mAppDataUserPath = { _T("USER") };
static const TCHAR* mAppSaveEquipmentFile = { _T("AppEquipment.xml") };

/// �J�X�^����ʊ֘A
static const COLORREF mDefaultTitleTextBackColor = RGB(147, 144, 192);
static const COLORREF mDefaultTextBackColor = RGB(14, 46, 65);
static const COLORREF mDefaultMainTextColor = RGB(242, 170, 132);
static const COLORREF mDefaultSubTextColor = RGB(193, 229, 245);
static const COLORREF mDefaultLeafTextColor = RGB(255, 255, 255);
static const COLORREF mDefaultUnitTextColor = RGB(255, 255, 255);
static const COLORREF mDefaultValueTextColor = RGB(145, 207, 80);
static const COLORREF mDefaultWarningValueTextColor = RGB(255, 255, 0);
static const COLORREF mDefaultErrorValueTextColor = RGB(255, 0, 0);
static const TCHAR* mDefaultCustomFontName = _T("BIZ UD�S�V�b�N");
static const TCHAR* mDefaultMonitorFontName = _T("Consolas");

static const TCHAR* mNoGroupText = { _T("No Group") };
static const TCHAR* mDefaultCustomTitle = { _T("�`�^�C�g��") };
static const TCHAR* mDefaultCustomRoot = { _T("���[�g") };
static const TCHAR* mDefaultCustomMainText = { _T("���C���m�[�h") };
static const TCHAR* mDefaultCustomSubText = { _T("�T�u�m�[�h") };
static const TCHAR* mDefaultCustomItemText = { _T("���[�t") };
static const int mTreeHeaderSize = 100;
static const int mTreeHeaderItemSize = 200;
static const int mTreeFontHeight = 16;
static const int mListFontHeight = 16;
static const COLORREF mRootNodeBackColor = RGB(180, 180, 180);
static const COLORREF mDragBackColor = RGB(250, 250, 250);
static const COLORREF mDragTextColor = RGB(0, 0, 0);
static const COLORREF mManagerHideColor = RGB(200, 200, 200);
static const TCHAR* mCOntrolSignString = { _T("#CNTL#") };
static const TCHAR* mCOntrolSignStringDisplay = { _T("����") };
static const TCHAR* mEditModeString = { _T("�i�ҏW���j") };
static const UINT mSortRange = 20;
static const UINT mGroupRange = 20;
static const UINT mMaxEqSyncNum = 5;

/// �^�C�g��
static const TCHAR* mMessage_Title_CustomManager = { _T("�J�X�^�}�C�Y�Ǘ����") };
static const TCHAR* mMessage_Title_CustomDetail = { _T("�J�X�^�����") };

/// ���b�Z�[�W
static const TCHAR* mMessage_ManagerDelete = { _T("�I�����ꂽ�ݔ��ڍ׏����폜���܂��B\n��낵���ł����H") };
static const TCHAR* mMessage_DetailSaveDifferentData = { _T("�ݔ��ڍׂ̕ύX���e��ۑ����܂����H") };
static const TCHAR* mMessage_DetailDelete = { _T("%s���폜���܂��B\n��낵���ł����H") };
static const TCHAR* mMessage_LoadLayout = { _T("�\������Ă���S�Ă̐ݔ��ڍ׉�ʂ̕ύX���e��j�����܂��B\n��낵���ł����H") };
static const TCHAR* mMessage_SaveLayout = { _T("�ҏW���̏��͔j������܂��B\n��낵���ł����H") };
static const TCHAR* mMessage_SameGroupName = { _T("�����̃O���[�v�������݂��܂��B�ēx���͂��Ă��������B") };
static const TCHAR* mMessage_EmptyGroupName = { _T("�O���[�v���̎w�肪����܂���B�ēx���͂��Ă��������B") };

static const int mMonMax = 5000;			// �Ď��̍ő吔
static const int mCtrlMax = 5000;			// ����̍ő吔

// �ݔ��ڍ׉�ʂ̃w�b�_�[����
enum { eDetailItem, eDetailValue, eDetailUnit, eDetailControl, eDetailMax };
static const TCHAR* mTreeItemHeader[eDetailMax] = {
	_T("����"),
	_T("�l"),
	_T("�P��"),
	_T("����"),
};

// �ݔ��Ǘ���ʂ̃w�b�_�[����
enum { eManagerTitle, eManagerNote, eManagerGroup, eManagerMax };
static const TCHAR* mGroupListHeader[] = {
	_T("�^�C�g��"),
	_T("���l"),
	_T("�O���[�v"),
};

/// �e�탁�b�Z�[�WID
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

/// ���C�A�E�g�t�@�C�����
enum eLayoutFileType {
	eLayoutFileType_SCL = 0,
	eLayoutFileType_XML,
};

/// �c���[�f�[�^
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
	eColorType_Window = 0,
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

typedef struct {
	CWnd*			wnd;				// �ݔ��ڍ׃E�B���h�E�n���h��
	CWnd*			tree;				// �ݔ��ڍ׃c���[�n���h���h��
	CWnd*			manager;			// �Ǘ��E�B���h�E�n���h���iCCustomManager�ŃZ�b�g�j
	UINT			mode;				// ���[�h�ύX�FeTreeItemMode
	UINT			kind;				// �\����ʁFeTreeItemKind
	UINT			type;				// �\������ʁFeTreeItemType
	TCHAR			title[mTitleSize];	// �E�B���h�E�^�C�g��
	WINDOWPLACEMENT	placement;			// �E�B���h�E�ʒu���
	UINT			hwidth[mHeaderSize]; // �ڍ׉�ʃw�b�_�[��
	UINT			zorder;				// Z�I�[�_�[
	UINT			monitor;			// ���j�^����
	UINT			sortno;				// �\�[�g�ԍ�
} stEquipmentInfo;

typedef struct {
	TCHAR			memo[mTitleSize];	// �E�B���h����
	TCHAR			groupname[mNameSize];	// �O���[�v
	UINT			groupno;			// �O���[�v�ԍ�(���ʃ��[�h�F�O���[�v���ԍ��@��ʃ��[�h�F�O���[�v�ԍ�)
} stManagerInfo;

typedef struct{
	TCHAR			display[mNameSize];	// �\����
	TCHAR			mname[mNameSize];	// �Ď���
	TCHAR			cname[mNameSize];	// ���䖼
	TCHAR			unit[mUnitSize];	// �P��
	UINT			formattype;			// �t�H�[�}�b�g��ʁFeControl
	TCHAR			format[mFormatSize];// �t�H�[�}�b�g����������
} stMonCtrlData;

typedef struct {
	COLORREF		back;				// �w�i�F
	COLORREF		textback;			// �e�L�X�g�w�i�F
	COLORREF		text;				// �e�L�X�g�F
	COLORREF		value;				// �l�e�L�X�g�F
	COLORREF		unit;				// �P�ʃe�L�X�g�F
	LOGFONT			font;				// �ڍ׉�ʃt�H���g
} stColorData;

/* ------------------------------------------------------------------------------------ */
/* �h���b�O�f�[�^                                                                       */
/* ------------------------------------------------------------------------------------ */
typedef struct {
	UINT		type;				// �Ď��A����AWFD
	POINT		point;				// �}�E�X�ʒu���
	vector<int>	indexes;			// �h���b�O�A�C�e��
	vector<TCHAR*> nodes;			// �h���b�O�A�C�e��
} stDragData;
enum { eFromType_None, eFromType_Mon, eFromType_Cntl, eFromType_Custom };


//------------------------------------------------------------------------------------ 
//	�c���[�^�f�[�^�N���X
//------------------------------------------------------------------------------------
class CTreeNode {
public:
	// �w�肳�ꂽ id �� Node �����B
	CTreeNode(HTREEITEM id, CWnd* pwnd, CWnd* ptree, UINT type);
	virtual ~CTreeNode();

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
	HTREEITEM			treeitem;			// �m�[�h�A�C�e��
	CTreeNode*			parent;				// �e�m�[�h

	TCHAR				xmlfile[_MAX_PATH];	// XML�t�@�C����
	stEquipmentInfo		equipment;			// �ݔ��ڍ׃E�B���h�E���
	stManagerInfo		manager;			// �ݔ��ڍ׃E�B���h�E���
	stMonCtrlData		monctrl;			// �Ď�����f�[�^
	stColorData			color;				// �F�E�t�H���g���
	vector<CTreeNode*>	children;			// �q�A�C�e�����X�g

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	HTREEITEM			GetTreeItem()	{ return treeitem; }
	void				SetTreeItem(HTREEITEM val)	{ treeitem = val; }
	TCHAR*				GetXmlFileName() { return xmlfile; }
	void				SetXmlFileName(TCHAR* xml)
	{
		swprintf_s(xmlfile, _MAX_PATH, _T("%s"), (LPCTSTR)xml);
	}
	stEquipmentInfo&	GetEquipment() { return equipment; }
	stManagerInfo&		GetManager() { return manager; }
	stMonCtrlData&		GetMonCtrl() { return monctrl; }
	stColorData&		GetColor() { return color; }
	CTreeNode*			GetParentNode() { return parent; }
	void				SetParentNode(CTreeNode* val)	{ parent = val; }
	vector<CTreeNode*>&	GetChildren() { return children; }

	// �w�肵�� childID �̎qNode�����݂��Ȃ���΁A�qNode���쐬���qNode��Ԃ�
	// �w�肵�� childID �̎qNode�����݂��Ă���΁A���ɑ��݂���qNode��Ԃ�
	CTreeNode*	CreateTreeNode(UINT type, HTREEITEM parent, HTREEITEM child, HTREEITEM hInsertAfter = TVI_FIRST);

	void	SortTreeNode(HTREEITEM parent);

	// �c���[�m�[�h�̍폜
	bool DeleteTreeNode(HTREEITEM target);

	// �w��m�[�h�̍폜
	void DeleteItemNode(CTreeNode* pnode);

	// �c���[�m�[�h�̌���
	CTreeNode* SearchTreeNode(HTREEITEM target);
	CTreeNode* SearchTreeNodeType(UINT target);

	// �m�[�h�R�s�[
	void	CopyTreeNode(CTreeNode* copyNode);
	void	CopyItem(CTreeNode* copyNode, bool bColorOnly = false)
	{
		if (bColorOnly == false) {
			memcpy(&equipment, &(copyNode->equipment), sizeof(stEquipmentInfo));
			memcpy(&manager, &(copyNode->manager), sizeof(stManagerInfo));
			memcpy(&monctrl, &(copyNode->monctrl), sizeof(stMonCtrlData));
		}
		memcpy(&color, &(copyNode->color), sizeof(stColorData));
	}
	void	DropCopyItem(CTreeNode* copyNode)
	{
		memcpy(&equipment, &(copyNode->equipment), sizeof(stEquipmentInfo));
		memcpy(&manager, &(copyNode->manager), sizeof(stManagerInfo));
		memcpy(&monctrl, &(copyNode->monctrl), sizeof(stMonCtrlData));
	}

	/// ���ݔ�����ڍ׉�ʃ��C�A�E�g
	bool	SaveTreeNode(CArchive& ar);
	bool	LoadTreeNode(CArchive& ar);
	bool	SaveTreeNodeXml(CMarkup& xml);
	bool	LoadTreeNodeXml(CMarkup& xml);

	bool	SaveCustomLayout();
	bool	LoadCustomLayout();

	//bool operator==(CTreeNode* data);
	bool Equal(CTreeNode* data, bool bChild);
	bool Equal(stEquipmentInfo& data, bool bChild);
	bool Equal(stManagerInfo& data, bool bChild);
	bool Equal(stMonCtrlData& data, bool bChild);
	bool Equal(stColorData& data, bool bChild);
	bool Equal(LOGFONT& data, bool bChild);

protected:
	void	deleteNode(CTreeNode* pnode);

	/// ���ݔ�����ڍ׉�ʃ��C�A�E�g
	//bool	saveChildNodeXml(CMarkup& ar, CTreeNode* pnode);
	//bool	loadChildNodeXml(CMarkup& ar, CTreeNode* pnode);

	/// CArchive
	// ��`�̓Ǎ�
	void loadRect(CArchive& ar, RECT& rect)
	{
		ar >> rect.left;
		ar >> rect.top;
		ar >> rect.right;
		ar >> rect.bottom;
	}
	// �|�C���g�̓Ǎ�
	void loadPoint(CArchive& ar, POINT& point) const
	{
		ar >> point.x;
		ar >> point.y;
	}

	// ��`�̕ۑ�
	void saveRect(CArchive& ar, RECT& rect) const
	{
		ar << rect.left;
		ar << rect.top;
		ar << rect.right;
		ar << rect.bottom;
	}
	// �|�C���g�̕ۑ�
	void savePoint(CArchive& ar, POINT& point) const
	{
		ar << point.x;
		ar << point.y;
	}

	/// XML
	// ��`�̓Ǎ�
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
	// �|�C���g�̓Ǎ�
	void loadPointXml(CMarkup& xml, POINT& point) const
	{
		xml.FindElem(_T("X"));
		point.x = _wtoi(xml.GetData());
		xml.FindElem(_T("Y"));
		point.y = _wtoi(xml.GetData());
	}

	// �F�R�[�h�̓Ǎ�
	void loadColorXml(CMarkup& xml, CString tagname, COLORREF& color) const
	{
		BYTE r, g, b;
		xml.FindElem(tagname);
		xml.IntoElem();
		xml.FindElem(_T("R"));
		r = (BYTE)_wtoi(xml.GetData());
		xml.FindElem(_T("G"));
		g = (BYTE)_wtoi(xml.GetData());
		xml.FindElem(_T("B"));
		b = (BYTE)_wtoi(xml.GetData());
		xml.OutOfElem();
		color = RGB(r, g, b);
	}

	// ��`�̕ۑ�
	void saveRectXml(CMarkup& xml, RECT& rect) const
	{
		xml.AddElem(_T("LEFT"), rect.left);
		xml.AddElem(_T("TOP"), rect.top);
		xml.AddElem(_T("RIGHT"), rect.right);
		xml.AddElem(_T("BOTTOM"), rect.bottom);
	}
	// �|�C���g�̕ۑ�
	void savePointXml(CMarkup& xml, POINT& point) const
	{
		xml.AddElem(_T("X"), point.x);
		xml.AddElem(_T("Y"), point.y);
	}

	// �F�R�[�h�̕ۑ�
	void saveColorXml(CMarkup& xml, CString tagname, COLORREF color) const
	{
		xml.AddElem(tagname);
		xml.IntoElem();
		xml.AddElem(_T("R"), GetRValue(color));
		xml.AddElem(_T("G"), GetGValue(color));
		xml.AddElem(_T("B"), GetBValue(color));
		xml.OutOfElem();
	}

	// ��`�̐ݒ�
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
	// �|�C���g�̐ݒ�
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
//	�J�X�^�}�C�Y�@�\�̃f�[�^�}�l�[�W���[�N���X
//------------------------------------------------------------------------------------
class CCustomDataManager
{
public:
	CCustomDataManager();
	~CCustomDataManager();

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
	/// �c���[�m�[�h
	vector<CTreeNode*>		mTreeNode;
	map<CWnd*, CTreeNode*>	mEditTreeNode;

	/// �Ď��E����f�[�^�x�[�X�f�[�^�Ǘ��֘A
	CCustomDBAccess			mDataMonitor;
	CCustomDBAccess			mDataControl;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	/// �c���[�m�[�h�擾
	vector<CTreeNode*>& GetTreeNode() { return mTreeNode; }
	/// �ҏW�p�c���[�m�[�h�擾
	map<CWnd*, CTreeNode*>& GetEditTreeNode() { return mEditTreeNode; }

	// �c���[�m�[�h�̓o�^
	void	AddTreeNode(CTreeNode* val)
	{
		mTreeNode.push_back(val);
	}


	//========================================================================================
	// �m�[�h�̌���
	//========================================================================================
	/// �c���[�m�[�h���̐ݔ��ڍ׃E�B���h�E����
	CTreeNode* SearchWndNode(CWnd* pTarget, bool bEdit=true)
	{
		// �ҏW�p���X�g���Ɏw��E�B���h�E�����݂���ꍇ�͕ҏW�p���g�p����
		if (bEdit == true) {
			map<CWnd*, CTreeNode*>::iterator itrwnd;
			itrwnd = mEditTreeNode.find(pTarget);
			if (itrwnd != mEditTreeNode.end()) {
				return (*itrwnd).second;
			}
		}

		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr)->GetEquipment().wnd == pTarget) {
				return (*itr);
			}
		}

		return NULL;
	}

	/// �c���[�m�[�h���̃A�C�e���̌���
	CTreeNode* SearchItemNode(CWnd* pTarget, HTREEITEM target)
	{
		// �ҏW�p���X�g���Ɏw��E�B���h�E�����݂���ꍇ�͕ҏW�p���g�p����
		map<CWnd*, CTreeNode*>::iterator itrwnd;
		itrwnd = mEditTreeNode.find(pTarget);
		if (itrwnd != mEditTreeNode.end()) {
			return (*itrwnd).second->SearchTreeNode(target);
		}

		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
			if ((*itr)->GetEquipment().wnd == pTarget){
				return (*itr)->SearchTreeNode(target);
			}
		}
		return NULL;
	}
	/// �c���[�m�[�h���̃A�C�e����ʂ̌���
	CTreeNode* SearchItemNodeType(CWnd* pTarget, UINT target)
	{
		// �ҏW�p���X�g���Ɏw��E�B���h�E�����݂���ꍇ�͕ҏW�p���g�p����
		map<CWnd*, CTreeNode*>::iterator itrwnd;
		itrwnd = mEditTreeNode.find(pTarget);
		if (itrwnd != mEditTreeNode.end()) {
			return (*itrwnd).second->SearchTreeNodeType(target);
		}

		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
			if ((*itr)->GetEquipment().wnd == pTarget){
				return (*itr)->SearchTreeNodeType(target);
			}
		}
		return NULL;
	}
	/// �S�ẴE�B���h�E�n���h���A�m�[�h�̍폜
	void DeleteAll()
	{
		DeleteAllWnd();
		DeleteAllNode();
	}
	/// �S�ẴE�B���h�E�n���h���̍폜
	void DeleteAllWnd()
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr)->GetEquipment().wnd != NULL) {
				delete (*itr)->GetEquipment().wnd;
				(*itr)->GetEquipment().wnd = NULL;
			}
		}
	}
	/// ��ʖ��̃E�B���h�E�n���h���̍폜
	void DeleteKindWnd(UINT kind)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr)->GetEquipment().wnd != NULL && (*itr)->GetEquipment().kind == kind) {
				delete (*itr)->GetEquipment().wnd;
				(*itr)->GetEquipment().wnd = NULL;
			}
		}
	}
	/// �ʃE�B���h�E�n���h���̍폜
	void DeleteItemWnd(CWnd* p)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++){
			if ((*itr)->GetEquipment().wnd == p){
				if ((*itr)->GetEquipment().wnd != NULL){
					delete (*itr)->GetEquipment().wnd;
					(*itr)->GetEquipment().wnd = NULL;
				}
				break;
			}
		}
	}

	// �S�Ẵm�[�h�̍폜
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
	// ��ʖ��̃m�[�h�̍폜
	void DeleteKindNode(UINT kind)
	{
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr) != NULL && (*itr)->GetEquipment().wnd != NULL && (*itr)->GetEquipment().kind == kind) {
				delete (*itr)->GetEquipment().wnd;
				(*itr)->GetEquipment().wnd = NULL;
				delete (*itr);
			}
		}
	}
	// �ʃm�[�h�̍폜
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
	// �ʃm�[�h�̃N���A
	void ClearItemNode(CTreeNode* p)
	{
		if (p != NULL) {
			vector<CTreeNode*>::iterator itrch;
			for (itrch = p->GetChildren().begin(); itrch != p->GetChildren().end(); itrch++) {
				if ((*itrch) != NULL) {
					delete (*itrch);
				}
			}
			p->GetChildren().clear();
		}
	}
	// �m�[�h�̃N���[���쐬
	CTreeNode* CloneItemNode(CTreeNode* source, CTreeNode* dest)
	{
		if (dest == NULL) {
			dest = new CTreeNode(0, NULL, NULL, eTreeItemType_Title);
		}
		ClearItemNode(dest);

		dest->CopyTreeNode(source);
		return dest;
	}

	/// �m�[�h���̓o�^�����擾����
	UINT GetNodeKindCount(UINT kind)
	{
		UINT count = 0;
		vector<CTreeNode*>::iterator itr;
		for (itr = mTreeNode.begin(); itr != mTreeNode.end(); itr++) {
			if ((*itr)->GetEquipment().wnd != NULL && (*itr)->GetEquipment().kind == kind) {
				count++;
			}
		}
		return count;
	}

	// �m�[�h�̌���
	void SwapItemNode(UINT pos1, UINT pos2)
	{
		swap(mTreeNode[pos1], mTreeNode[pos2]);
	}
	void SwapItemNode(vector<CTreeNode*>::iterator itr1, vector<CTreeNode*>::iterator itr2)
	{
		iter_swap(itr1, itr2);
	}

	/// ���J���[���
	bool	GetNodeColor(CWnd* pwnd, UINT type, stColorData& color);
	bool	SetNodeColor(CWnd* pwnd, UINT type, stColorData& color);

	/// ���ݔ�����ڍ׉�ʃ��C�A�E�g
	bool	SaveTreeData(CString strFile, CWnd* pTargetWnd = NULL);
	bool	LoadTreeData(CString strFile, bool bClear);
	bool	SaveTreeDataXml(CString strFile, CWnd* pTargetWnd = NULL);
	bool	SaveasTreeDataXml(CString strFile, CWnd* pTargetWnd);
	CTreeNode* LoadTreeDataXml(CString strFile, UINT kind=UINT_MAX);

	/// Z�I�[�_�[�ݒ�
	void	SetTreeZorder();
	void	ResetTreeZorder();

	/// �t�@�C���Ǘ�
	void	LoadEquipmentData(UINT typeLayout, CString strfile, bool bClear = true);
	void	SaveEquipmentData(UINT typeLayout, CString strfile, CWnd* pTargetWnd = NULL);

	bool	SaveCustomLayout(CArchive& ar);
	bool	LoadCustomLayout(CArchive& ar);

	/// ��ʘA���֘A
	UINT	GetMaxInnerNo(UINT group);

	/// �Ď��E����f�[�^�x�[�X�f�[�^�Ǘ��֘A
	CCustomDBAccess& GetDataMonitor() { return mDataMonitor; }
	CCustomDBAccess& GetDataControl() { return mDataControl; }

	/// �m�[�h���X�g����ҏW�p�m�[�h���쐬����
	void BackupEditNode(CWnd* pWnd)
	{
		CTreeNode* pnode = SearchWndNode(pWnd);
		map<CWnd*, CTreeNode*>::iterator itrwnd;
		itrwnd = mEditTreeNode.find(pWnd);
		if (itrwnd == mEditTreeNode.end()) {
			CTreeNode* dest = NULL;
			dest = CloneItemNode(pnode, dest);
			mEditTreeNode.insert(map<CWnd*, CTreeNode*>::value_type(pWnd, dest));
		}
		else {
			(*itrwnd).second = CloneItemNode(pnode, (*itrwnd).second);
		}
	}

	/// �ҏW�p�m�[�h���m�[�h���X�g�Ƀ��X�g�A����
	void RestoreEditNode(CWnd* pWnd)
	{
		CTreeNode* pnode = SearchWndNode(pWnd, false);
		map<CWnd*, CTreeNode*>::iterator itrwnd;
		itrwnd = mEditTreeNode.find(pWnd);
		if (itrwnd == mEditTreeNode.end()) {
			// �ҏW�m�[�h�ɂȂ��ꍇ�͉������Ȃ�
			return;
		}
		// �ҏW�p�m�[�h�ɑ��݂���ꍇ�͕ҏW�p�m�[�h����m�[�h���X�g�փR�s�[����
		CloneItemNode((*itrwnd).second, pnode);
	}

	/// �ҏW�p�m�[�h�𖼑O��t���ĕۑ�����
	void SaveasEditNode(CWnd* pWnd, CString xmlfile)
	{
		map<CWnd*, CTreeNode*>::iterator itrwnd;
		itrwnd = mEditTreeNode.find(pWnd);
		if (itrwnd == mEditTreeNode.end()) {
			// �ҏW�m�[�h�ɂȂ��ꍇ�͉������Ȃ�
			return;
		}
		// �ҏW�p�m�[�h��ۑ�����
		SaveasTreeDataXml(xmlfile, pWnd);
		// ���m�[�h���擾
		CTreeNode* dest = SearchWndNode(pWnd, false);
		// �ҏW�m�[�h�����m�[�h�ɃR�s�[����
		dest = CloneItemNode((*itrwnd).second, dest);
		//AddTreeNode(dest);
		//CTreeNode* pnode = SearchWndNode(pWnd, false);
		//pnode->GetEquipment().wnd = pWnd;
	}

	/// �ҏW�m�[�h�ƃm�[�h���X�g���̃m�[�h���r����
	bool CompareEditNode(CWnd* pWnd)
	{
		CTreeNode* pnode = SearchWndNode(pWnd, false);
		map<CWnd*, CTreeNode*>::iterator itrwnd;
		itrwnd = mEditTreeNode.find(pWnd);
		if (itrwnd == mEditTreeNode.end()) {
			return false;
		}
		return (*itrwnd).second->Equal(pnode, false);
	}

	/// �ҏW�p�m�[�h�̍폜
	void DeleteEditNode(CWnd* pWnd)
	{
		map<CWnd*, CTreeNode*>::iterator itrwnd;
		itrwnd = mEditTreeNode.find(pWnd);
		if (itrwnd == mEditTreeNode.end()) {
			return;
		}
		delete (*itrwnd).second;
		mEditTreeNode.erase(itrwnd);
	}

	bool	IsVisibleEditMode();

protected:
	/// �J���[���
	bool	getNodeTypeColor(CTreeNode* pnode, UINT type, stColorData& color);
	bool	setNodeTypeColor(CTreeNode* pnode, UINT type, stColorData& color);

	/* ------------------------------------------------------------------------------------ */
};

