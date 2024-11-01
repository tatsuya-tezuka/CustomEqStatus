#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomDropTarget.h"


CNode::CNode(const CString id)
    : fID(id)
    , fChildren()
{
    // Nothig to do.
}

CNode::~CNode()
{
    for (::std::vector<CNode*>::iterator i = fChildren.begin(); i != fChildren.end(); i++) {
        delete* i;
    }
}

CString CNode::getID()
{
    return fID;
}

CNode* CNode::createChildIfNotExist(const CString childID)
{
    for (::std::vector<CNode*>::const_iterator i = fChildren.begin(); i != fChildren.end(); i++) {
        CNode* child = *i;
        if (child->getID() == childID) {
            return child;
        }
    }
    CNode* child = new CNode(childID);
    fChildren.push_back(child);
    return child;
}

const ::std::vector<CNode*>& CNode::getChildren()
{
    return fChildren;
}




/*============================================================================*/
/*! ドラッグ＆ドロップアイテム

/*! カスタマイズ機能ドラッグ＆ドロップ

@param

@retval
*/
/*============================================================================*/
void CCustomDropObject::CreateBuffer(CStringArray& items)
{
    if (mpBuff != NULL) {
        delete mpBuff;
        mpBuff = NULL;
        mBuffSize = 0;
    }
    ASSERT(mpBuff == NULL);
    ASSERT(mBuffSize == 0);

    for (int i = 0; i < items.GetSize(); i++) {
        mBuffSize += items[i].GetLength() * sizeof(TCHAR) + 1;
    }
    mBuffSize = (mBuffSize / 32 + 1) * 32;

    int nBuffSize = 0;

    mpBuff = new TCHAR[mBuffSize];

    ZeroMemory(mpBuff, mBuffSize);

    TCHAR* pCurrent = mpBuff;

    for (int i = 0; i < items.GetSize(); i++) {
        wmemcpy_s(pCurrent, mBuffSize, (TCHAR*)(LPCTSTR)items[i], items[i].GetLength());
        pCurrent += items[i].GetLength();
    }

    ASSERT(pCurrent - mBuffSize - 1 <= mpBuff);
}

void CCustomDropObject::CreateBuffer(CNode* items)
{
    if (mpNode != NULL) {
        delete mpNode;
        mpNode = NULL;
    }
    ASSERT(mpNode == NULL);

    mpNode = new CNode();

    for (::std::vector<CNode*>::const_iterator itr = items->getChildren().begin(); itr != items->getChildren().end(); itr++) {
        //TRACE("#%s\n", CStringA((*itr)->getID()));
        CNode* cur = mpNode->createChildIfNotExist((*itr)->getID());
        CreateNode((*itr), cur);
    }
}

void CCustomDropObject::CreateNode(CNode* item, CNode* node)
{
    const ::std::vector<CNode*> children = item->getChildren();
    for (::std::vector<CNode*>::const_iterator itr = children.begin(); itr != children.end(); itr++) {
        //TRACE("@%s\n", CStringA((*itr)->getID()));
        CNode* cur = node->createChildIfNotExist((*itr)->getID());
        CreateNode((*itr), cur);
    }
}
/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# ドラッグアイテムバッファの取得

@param

@retval
*/
/*============================================================================*/
void* CCustomDropObject::GetBuffer() const
{
    ASSERT(mpBuff);
    return (void*)mpBuff;
}
/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# ドラッグアイテムバッファサイズの取得

@param

@retval
*/
/*============================================================================*/
UINT CCustomDropObject::GetBuffSize() const
{
    ASSERT(mBuffSize);
    return mBuffSize;
}

/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# ドラッグハンドルの登録

@param

@retval
*/
/*============================================================================*/
void CCustomDropTarget::Register(CWnd* pDragWnd, UINT format)
{
    map<CWnd*, CCustomDropObject>::iterator itr;
    itr = mObjectList.find(pDragWnd);
    if (itr == mObjectList.end()) {
        CCustomDropObject obj;
        obj.mFormat = format;
        mObjectList.insert(map<CWnd*, CCustomDropObject>::value_type(pDragWnd, obj));
    }
}

/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# ＤｎＤ中にマウスカーソルがターゲットウィンドウに入ったときにコールするコールバック関数の登録

@param

@retval
*/
/*============================================================================*/
void CCustomDropTarget::SetCallbackDragEnter(CWnd* pDragWnd, DROPTARGET_DRAGENTER callback)
{
    map<CWnd*, CCustomDropObject>::iterator itr;
    itr = mObjectList.find(pDragWnd);
    if (itr != mObjectList.end()) {
        (*itr).second.mCallbackDragEnter = callback;
    }
}

/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# ウィンドウ内でのマウスカーソルが移動したときにコールするコールバック関数の登録

@param

@retval
*/
/*============================================================================*/
void CCustomDropTarget::SetCallbackDragOver(CWnd* pDragWnd, DROPTARGET_DRAGOVER callback)
{
    map<CWnd*, CCustomDropObject>::iterator itr;
    itr = mObjectList.find(pDragWnd);
    if (itr != mObjectList.end()) {
        (*itr).second.mCallbackDragOver = callback;
    }
}

/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# ウィンドウから出ていったときにコールするコールバック関数の登録

@param

@retval
*/
/*============================================================================*/
void CCustomDropTarget::SetCallbackDragLeave(CWnd* pDragWnd, DROPTARGET_DRAGLEAVE callback)
{
    map<CWnd*, CCustomDropObject>::iterator itr;
    itr = mObjectList.find(pDragWnd);
    if (itr != mObjectList.end()) {
        (*itr).second.mCallbackDragLeave = callback;
    }
}

/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# 実際にドロップ処理時にコールするコールバック関数の登録

@param

@retval
*/
/*============================================================================*/
void CCustomDropTarget::SetCallbackDragDrop(CWnd* pDragWnd, DROPTARGET_DRAGDROP callback)
{
    map<CWnd*, CCustomDropObject>::iterator itr;
    itr = mObjectList.find(pDragWnd);
    if (itr != mObjectList.end()) {
        (*itr).second.mCallbackDragDrop = callback;
    }
}

/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# ドラッグ＆ドロップ開始時の文字列データ、マウス位置情報の設定

@param  items      ドラッグアイテム
@param  format     ドラッグフォーマット
@param  dtype      データ種別

@retval
*/
/*============================================================================*/
void CCustomDropTarget::OnDragBegin(CStringArray& items, UINT format, UINT dtype)
{
    map<CWnd*, CCustomDropObject>::iterator itr;
    for (itr = mObjectList.begin(); itr != mObjectList.end(); itr++) {
        (*itr).second.mFormat = format;
        (*itr).second.mDataType = dtype;
        (*itr).second.CreateBuffer(items);
    }
}
void CCustomDropTarget::OnDragBegin(CNode* items, UINT format, UINT dtype)
{
    map<CWnd*, CCustomDropObject>::iterator itr;
    for (itr = mObjectList.begin(); itr != mObjectList.end(); itr++) {
        (*itr).second.mFormat = format;
        (*itr).second.mDataType = dtype;
        (*itr).second.CreateBuffer(items);
    }
}

/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# ドラッグ＆ドロップ移動

@param

@retval UINT    DE_NONE,DE_COPY,DE_MOVE
*/
/*============================================================================*/
UINT CCustomDropTarget::OnDragMove(CWnd* pDragWnd, UINT dwKeyState, POINT point)
{
    //AllDragLeave();

    map<CWnd*, CCustomDropObject>::iterator itrdrag;
    itrdrag = mObjectList.find(pDragWnd);

    CWnd* pDropWnd = CWnd::WindowFromPoint(point);
    if (mTargetWnd != pDropWnd) {
        TRACE("CCustomDropTarget::OnDragMove(%08X : %08X\n", pDropWnd, mTargetWnd);
    }
    if (mTargetWnd != NULL && mTargetWnd != pDropWnd) {
        TRACE("DragLeave(%08X)\n", mTargetWnd);
        AllDragLeave(mTargetWnd);
    }
    if (mTargetWnd != pDropWnd) {
        mTargetWnd = pDropWnd;
    }

    map<CWnd*, CCustomDropObject>::iterator itr;
    for (itr = mObjectList.begin(); itr != mObjectList.end(); itr++) {
        if ((*itr).first == pDropWnd) {
            if ((*itr).second.mCallbackDragOver != NULL) {
                //return (*itr).second.mCallbackDragOver(pDropWnd, (void*)&(*itr).second, dwKeyState, CPoint(point));
                return (*itr).second.mCallbackDragOver(pDropWnd, (void*)&(*itrdrag).second, dwKeyState, CPoint(point));
            }
        }
    }
    return CCustomDropObject::DE_NONE;
}

/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# ドラッグ＆ドロップ実行

@param

@retval UINT    DE_NONE,DE_COPY,DE_MOVE
*/
/*============================================================================*/
UINT CCustomDropTarget::OnDragDrop(CWnd* pDragWnd, UINT dropEffect, POINT point)
{
    map<CWnd*, CCustomDropObject>::iterator itrdrag;
    itrdrag = mObjectList.find(pDragWnd);

    CWnd* pDropWnd = CWnd::WindowFromPoint(point);

    map<CWnd*, CCustomDropObject>::iterator itr;
    for (itr = mObjectList.begin(); itr != mObjectList.end(); itr++) {
        if ((*itr).first == pDropWnd) {
            if ((*itr).second.mCallbackDragDrop != NULL) {
                //return (*itr).second.mCallbackDragDrop(pDropWnd, (void*)&(*itr).second, dropEffect, CPoint(point));
                return (*itr).second.mCallbackDragDrop(pDropWnd, (void*)&(*itrdrag).second, dropEffect, CPoint(point));
            }
        }
    }
    return CCustomDropObject::DE_NONE;
}

/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# ドラッグ＆ドロップ実行

@param

@retval UINT    DE_NONE,DE_COPY,DE_MOVE
*/
/*============================================================================*/
void CCustomDropTarget::OnDragLeave(CWnd* pDragWnd)
{
    AllDragLeave();
}

/*============================================================================*/
/*! カスタマイズ機能ドラッグ＆ドロップ

-# 全ての対象ウィンドウにDragLeaveを送る

@param

@retval
*/
/*============================================================================*/
void CCustomDropTarget::AllDragLeave(CWnd* pDropWnd)
{
    map<CWnd*, CCustomDropObject>::iterator itr;
    for (itr = mObjectList.begin(); itr != mObjectList.end(); itr++) {
        if (pDropWnd != NULL && pDropWnd != (*itr).first)
            continue;
        if ((*itr).second.mCallbackDragLeave != NULL) {
            (*itr).second.mCallbackDragLeave((*itr).first);
        }
    }
}
