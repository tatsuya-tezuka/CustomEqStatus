
// CustomEqStatusView.cpp : CCustomEqStatusView クラスの実装
//

#include "stdafx.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "CustomEqStatus.h"
#endif

#include "CustomEqStatusDoc.h"
#include "CustomEqStatusView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCustomEqStatusView

IMPLEMENT_DYNCREATE(CCustomEqStatusView, CView)

BEGIN_MESSAGE_MAP(CCustomEqStatusView, CView)
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CCustomEqStatusView コンストラクション/デストラクション

CCustomEqStatusView::CCustomEqStatusView()
{
	// TODO: 構築コードをここに追加します。

}

CCustomEqStatusView::~CCustomEqStatusView()
{
}

BOOL CCustomEqStatusView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CCustomEqStatusView 描画

void CCustomEqStatusView::OnDraw(CDC* /*pDC*/)
{
	CCustomEqStatusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
}


// CCustomEqStatusView 印刷

BOOL CCustomEqStatusView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 既定の印刷準備
	return DoPreparePrinting(pInfo);
}

void CCustomEqStatusView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
}

void CCustomEqStatusView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
}


// CCustomEqStatusView 診断

#ifdef _DEBUG
void CCustomEqStatusView::AssertValid() const
{
	CView::AssertValid();
}

void CCustomEqStatusView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCustomEqStatusDoc* CCustomEqStatusView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCustomEqStatusDoc)));
	return (CCustomEqStatusDoc*)m_pDocument;
}
#endif //_DEBUG


// CCustomEqStatusView メッセージ ハンドラー
