/******************************************************************************
* @file    CustomDialogBase.cpp
* @brief   【カスタマイズ機能】ダイアログ基底クラス
* @auther  SCC
* @date    2024/08/26 新規作成
* @par
******************************************************************************/
#include "stdafx.h"
#include "CustomEqStatusDemo.h"
#include "CustomDialogBase.h"

/*
	◆使用方法
	�@通常通りダイログを作成する
	�A作成されたダイアログの基底クラスはCDialogExになっているので全てCCustomDialogBaseに変更する
	�Bダイアログ内でダイアログ枠のリサイズに同期させるにはOnInitDialog内で当クラスのSetControlInfo関数を使用する
*/

IMPLEMENT_DYNAMIC(CCustomDialogBase, CDialogEx)

CCustomDialogBase::CCustomDialogBase(UINT IDD, CWnd* pParent /*=NULL*/)
: CDialogEx(IDD, pParent)
{
	mIDD = IDD;
	mModal = false;
	mSaveCx = mSaveCy = 0;
}

CCustomDialogBase::~CCustomDialogBase()
{
}

void CCustomDialogBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCustomDialogBase, CDialogEx)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
	ON_WM_MOVING()
END_MESSAGE_MAP()

/*============================================================================*/
/*! CCustomDialogBase

-# サイズ変更処理

@breif	ダイアログのリサイズに同期して設定されたコントロールもリサイズを行う
@param
@retval
*/
/*============================================================================*/
void CCustomDialogBase::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType == SIZE_MINIMIZED)
		return;

	if (GetSafeHwnd() == NULL)
		return;

	if (mControl.size() == 0)
		return;

	if (mSaveCx == 0){
		mSaveCx = cx;
		mSaveCy = cy;
		return;
	}

	int dx = cx - mSaveCx;
	int dy = cy - mSaveCy;

	CRect rect, rectClient;
	GetClientRect(rectClient);

	CWnd *pWnd;
	DWORD info;
	WORD anchore;
	TCHAR buf[80];

	::GetClassNameW(m_hWnd, buf, 80);

	vector<DWORD>::iterator itr;
	for (itr = mControl.begin(); itr != mControl.end(); itr++){
		info = (*itr);
		pWnd = GetDlgItem(LOWORD(info));
		if (!pWnd){
			continue;
		}

		if (!HIWORD(info))
			continue;

		anchore = HIWORD(info);
		pWnd->GetWindowRect(rect);
		ScreenToClient(rect);

		if (anchore & RESIZE_HOR)
			rect.right += dx;
		else if (anchore & ANCHORE_RIGHT)
			rect.OffsetRect(dx, 0);

		if (anchore & RESIZE_VER)
			rect.bottom += dy;
		else if (anchore & ANCHORE_BOTTOM)
			rect.OffsetRect(0, dy);

		// 特定のコントロールを自前でリサイズする場合は以下のOnProcSizeをオーバーライドしてください。
		if (OnProcSize(pWnd, dx, dy) == FALSE){
			pWnd->MoveWindow(rect);
		}
	}

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	if (pnode != NULL && HIWORD(pnode->GetWindowInfo().groupno) != 0) {
		CRect rect;
		pnode->GetWindowInfo().wnd->GetWindowRect(rect);
		theApp.GetCustomControl().GetCustomManager().GetCustomSyncWindow().Move(pnode->GetWindowInfo().groupno, pnode->GetWindowInfo().wnd, &rect);
	}

	mSaveCx = cx;
	mSaveCy = cy;
}

/*============================================================================*/
/*! CCustomDialogBase

-# ウィンドウ移動

@breif	画面連動されている場合は同期して移動する
@param
@retval
*/
/*============================================================================*/
void CCustomDialogBase::OnMoving(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnMoving(fwSide, pRect);

	CTreeNode* pnode = theApp.GetCustomControl().GetDataManager().SearchWndNode(this);
	if (pnode == NULL || HIWORD(pnode->GetWindowInfo().groupno) == 0)
		return;

	CRect rect;
	pnode->GetWindowInfo().wnd->GetWindowRect(rect);
	theApp.GetCustomControl().GetCustomManager().GetCustomSyncWindow().Move(pnode->GetWindowInfo().groupno, pnode->GetWindowInfo().wnd, &rect);
}

/*============================================================================*/
/*! CCustomDialogBase

-# サイズ変更時の最大、最小サイズを決める

@breif	ダイアログのリサイズ時の最小サイズを決定する
@param
@retval
*/
/*============================================================================*/
void CCustomDialogBase::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 200;
	lpMMI->ptMinTrackSize.y = 200;

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

/*============================================================================*/
/*! CCustomDialogBase

-# メッセージ処理

@breif	Enterキー（Escキー）を押しても閉じないようにする。
@param  なし
@retval なし
*/
/*============================================================================*/
BOOL CCustomDialogBase::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			return FALSE;
		default:
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

/*============================================================================*/
/*! CCustomDialogBase

-# ダイアログを閉じるをカスタマイズ

@breif
@param
@retval
*/
/*============================================================================*/
void CCustomDialogBase::OnClose()
{
	CString title;
	GetWindowText(title);

	if (IDCANCEL == CustomMessageBoxHooked(this->m_hWnd, _T("終了します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
		return;
	//if (IDCANCEL == MessageBox(_T("終了します。\nよろしいですか？"), title, MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2))
	//	return;

	if (mModal)
	{
		CDialogEx::OnClose();
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

/*============================================================================*/
/*! CCustomDialogBase

-# ダイアログのモーダル表示をカスタマイズ

@breif
@param
@retval
*/
/*============================================================================*/
INT_PTR CCustomDialogBase::DoModal()
{
	mModal = true;
	return CDialogEx::DoModal();
}
