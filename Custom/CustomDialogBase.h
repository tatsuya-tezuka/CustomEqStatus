#pragma once
#include "afxdialogex.h"

class CCustomDialogBase : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomDialogBase)
public:
	CCustomDialogBase(UINT IDD, CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CCustomDialogBase();

	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum{
		ANCHORE_LEFTTOP = 0x0000,	// 左上は固定させる
		ANCHORE_RIGHT = 0x0001,	// 右側は固定させる
		ANCHORE_BOTTOM = 0x0002,	// 下側は固定させる
		RESIZE_HOR = 0x0004,	// 横のリサイズを可能にする
		RESIZE_VER = 0x0008,	// 縦のリサイズを可能にする
		RESIZE_BOTH = (RESIZE_HOR | RESIZE_VER),
	};

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:

protected:
	UINT	mIDD;
	bool	mModal;

	// コントロールのリサイズ
	vector<DWORD>		mControl;
	int					mSaveCx, mSaveCy;

	/* ------------------------------------------------------------------------------------ */
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	/// コントロールの登録
	void SetControlInfo(WORD id, WORD anchore)
	{
		if (anchore == ANCHORE_LEFTTOP)
			return;

		DWORD info = MAKELONG(id, anchore);
		mControl.push_back(info);
	}

	UINT	GetIDD() { return mIDD; }

	virtual BOOL OnProcSize(CWnd* pWnd, int dx, int dy) { return FALSE; }

protected:

	/* ------------------------------------------------------------------------------------ */

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	virtual INT_PTR DoModal();
};

