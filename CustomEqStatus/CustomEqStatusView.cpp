
// CustomEqStatusView.cpp : CCustomEqStatusView �N���X�̎���
//

#include "stdafx.h"
// SHARED_HANDLERS �́A�v���r���[�A�k���ŁA����ь����t�B���^�[ �n���h���[���������Ă��� ATL �v���W�F�N�g�Œ�`�ł��A
// ���̃v���W�F�N�g�Ƃ̃h�L�������g �R�[�h�̋��L���\�ɂ��܂��B
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
	// �W������R�}���h
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CCustomEqStatusView �R���X�g���N�V����/�f�X�g���N�V����

CCustomEqStatusView::CCustomEqStatusView()
{
	// TODO: �\�z�R�[�h�������ɒǉ����܂��B

}

CCustomEqStatusView::~CCustomEqStatusView()
{
}

BOOL CCustomEqStatusView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return CView::PreCreateWindow(cs);
}

// CCustomEqStatusView �`��

void CCustomEqStatusView::OnDraw(CDC* /*pDC*/)
{
	CCustomEqStatusDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���̏ꏊ�Ƀl�C�e�B�u �f�[�^�p�̕`��R�[�h��ǉ����܂��B
}


// CCustomEqStatusView ���

BOOL CCustomEqStatusView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ����̈������
	return DoPreparePrinting(pInfo);
}

void CCustomEqStatusView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ����O�̓��ʂȏ�����������ǉ����Ă��������B
}

void CCustomEqStatusView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �����̌㏈����ǉ����Ă��������B
}


// CCustomEqStatusView �f�f

#ifdef _DEBUG
void CCustomEqStatusView::AssertValid() const
{
	CView::AssertValid();
}

void CCustomEqStatusView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCustomEqStatusDoc* CCustomEqStatusView::GetDocument() const // �f�o�b�O�ȊO�̃o�[�W�����̓C�����C���ł��B
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCustomEqStatusDoc)));
	return (CCustomEqStatusDoc*)m_pDocument;
}
#endif //_DEBUG


// CCustomEqStatusView ���b�Z�[�W �n���h���[
