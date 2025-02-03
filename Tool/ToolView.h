
// ToolView.h: CToolView 클래스의 인터페이스
//

#pragma once

#include "CDevice.h"
#include "CSingleTexture.h"

class CTerrain;
class CToolDoc;
class CObj;

class CToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CToolView() noexcept;
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();


public:
	CDevice*	m_pDevice;
	CTerrain*	m_pTerrain;
	CObj*		m_pObj;
	UINT_PTR m_nTimer;
	float m_fSrollSpeed;
	float m_fAlpha; 
	DWORD m_dwDisplayTime;
	bool m_bIsObj;
	bool m_bIsTileMode;
	D3DXVECTOR2 m_vFirstMousePos;
	D3DXVECTOR2 m_vSecondMousePos;
	bool m_bDragging;
	LPD3DXLINE m_DragLine;
	DWORD m_dragStartTime;
	DWORD m_dwDragEndTime;



public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	void Draw_Drag_Line();
	void Select_In_Line_Obj();

};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

// 1. 쿼터 뷰 형식으로 타일 깔기
// 2. 마우스 피킹을 이용하여 원하는 타일 이미지 변경하기

// 직선의 방정식을 사용할 것

// y = ax + b (a : 기울기 b : y 절편)
