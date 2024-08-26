
// CustomEqStatusView.h : CCustomEqStatusView クラスのインターフェイス
//

#pragma once


class CCustomEqStatusView : public CView
{
protected: // シリアル化からのみ作成します。
	CCustomEqStatusView();
	DECLARE_DYNCREATE(CCustomEqStatusView)

// 属性
public:
	CCustomEqStatusDoc* GetDocument() const;

// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装
public:
	virtual ~CCustomEqStatusView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CustomEqStatusView.cpp のデバッグ バージョン
inline CCustomEqStatusDoc* CCustomEqStatusView::GetDocument() const
   { return reinterpret_cast<CCustomEqStatusDoc*>(m_pDocument); }
#endif

