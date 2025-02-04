
// ToolView.cpp: CToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "CDevice.h"
#include "CTextureMgr.h"
#include "CTerrain.h"
#include "CObjManager.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CMiniView.h"
#include "CKeyManager.h"
#include "CUndoManager.h"
#include "CMyForm.h"
#include "ConsoleWindow.h"
#include "CTransformInfo.h"

HWND	g_hWnd;


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CToolView 생성/소멸
static int iCount = 0;
CToolView::CToolView() noexcept
	: m_pDevice(CDevice::Get_Instance()), m_bDragging(false),
	m_pTerrain(nullptr), m_nTimer(0), m_fSrollSpeed(0.f), m_DragLine(nullptr), m_dwDragEndTime(0),
	m_fAlpha(0.f), m_dwDisplayTime(0), m_pObj(nullptr), m_bIsObj(false), m_bIsTileMode(false), m_dragStartTime(0)

{
	// TODO: 여기에 생성 코드를 추가합니다.
	ZeroMemory(&m_vFirstMousePos, sizeof(D3DXVECTOR2));
	ZeroMemory(&m_vSecondMousePos, sizeof(D3DXVECTOR2));
}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	//ConsoleWindow::Create();
	srand((unsigned int)time(NULL));
	m_nTimer = SetTimer(1, 16, NULL);
	// AfxGetMainWnd : 현재 메인 윈도우의 값을 반환하는 전역함수

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT rcWnd{};

	// GetWindowRect : 현재 윈도우(창)의 rect 정보를 얻어오는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : 지정한 매개 변수 대로 Rect 정보를 기록하는 함수

	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	// rcWnd.right : 창의 가로 사이즈
	// rcWnd.bottom : 창의 세로 사이즈

	RECT rcMainView{};

	// GetClientRect : 현재 View창의 정보를 Rect 형식으로 얻어오는 함수
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	// SetWindowPos : 매개 변수대로 윈도우 위치와 크기를 조정하는 함수
	// SetWindowPos(배치할 윈도우의 Z순서에 대한 포인터, X좌표, Y좌표, 가로 크기, 세로 크기, 배치할 윈도우의 크기 및 위치지정 옵션)

	pMainFrm->SetWindowPos(nullptr,		// 순서 변경하지 않음
		0,
		0,
		int(WINCX + fRowFrm),
		int(WINCY + fColFrm),
		SWP_NOZORDER); // 현재 순서를 유지함


	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Init_Device()))
	{
		AfxMessageBox(L"Init_Device Create Failed");
		return;
	}



	if (!m_pTerrain)
	{
		m_pTerrain = new CTerrain;
		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
		m_pTerrain->Set_MainView(this);
		m_pTerrain->Set_MiniView(pMiniView);
		m_pTerrain->Initialize();
		m_DragLine = m_pTerrain->m_pLine;
	}

	m_fSrollSpeed = 5.f;
	m_fAlpha = 255.f;

	if (!m_pObj)
	{
		m_pObj = new CObj;
		m_pObj->Initialize();
	}

}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	m_vFirstMousePos = D3DXVECTOR2((float)point.x, (float)point.y);

	m_dragStartTime = GetTickCount();

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	if (nullptr != pMyForm)
	{
		if (0 == pMyForm->m_TextureListBox.GetCount() && CObjManager::Get_Instance()->m_vecObj.empty())
			return;
	}


	m_pTerrain->Picking_Tile(D3DXVECTOR3((float)point.x, (float)point.y, 0.f), m_bIsObj, m_bIsTileMode);

	//	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	//if (!m_bIsTileMode)
	//{
	//	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	//	CTransformInfo* pTransformInfo = dynamic_cast<CTransformInfo*>(pMainFrm->m_ThirdSplitter.GetPane(1, 0));

	//	if (m_pObj && m_bIsObj)
	//	{
	//		CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	//		m_pObj->Place_OnTile();
	//		CObjManager::Get_Instance()->Add_Obj(m_pObj);
	//		
	//		pTransformInfo->Set_SelectedObj(m_pObj);
	//		pTransformInfo->Set_TransformSpin(m_pObj->m_tInfo.vPos, m_pObj->m_tInfo.vRot, m_pObj->m_tInfo.vSize);

	//		m_pObj = new CObj;
	//		m_pObj->Initialize();
	//		iCount++;
	//		m_bIsObj = false;
	//	}
	//	else // 기존 오브젝트 피킹
	//	{
	//		if (GetTickCount() - m_dwDragEndTime < 300)
	//		{
	//			pMiniView->Invalidate(FALSE);
	//			return;
	//		}

	//		if (CObjManager::Get_Instance()->m_bIsPicking)
	//		{
	//			CObjManager::Get_Instance()->m_bIsPicking = false;
	//			for (auto& pObj : CObjManager::Get_Instance()->m_vecObj)
	//			{
	//				if (!pObj->m_bIsSet)
	//				{
	//					pObj->Place_OnTile();
	//					CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	//					pTransformInfo->Set_SelectedObj(pObj);
	//					pTransformInfo->Set_TransformSpin(pObj->m_tInfo.vPos, pObj->m_tInfo.vRot, pObj->m_tInfo.vSize);

	//					break;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			if (!m_bDragging)
	//			{
	//				if (!pTransformInfo->m_vecSelectedObj.empty())
	//				{
	//					pTransformInfo->m_vecSelectedObj.clear();
	//				}

	//			for (auto& pObj : CObjManager::Get_Instance()->m_vecObj)
	//			{
	//				if (pObj->Picking_Obj(D3DXVECTOR3((float)point.x, (float)point.y, 0.f)))
	//				{
	//					CObjManager::Get_Instance()->m_bIsPicking = true;

	//					pTransformInfo->Set_SelectedObj(pObj);
	//					//pTransformInfo->Set_TransformSpin(pObj->m_tInfo.vPos, pObj->m_tInfo.vRot, pObj->m_tInfo.vSize);
	//					
	//					break;
	//				}
	//			}
	//			}
	//		}
	//	}
	//}
	pMiniView->Invalidate(FALSE);
}


void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pDevice->Render_Begin();

	m_pTerrain->Render();
	m_pObj->Render();
	CObjManager::Get_Instance()->Render();
	TCHAR szSpeed[32];
	float fDeltaTime = (GetTickCount64() - m_dwDisplayTime) / 1000.0f;
	m_fAlpha = max(0.0f, 255.0f - (fDeltaTime * 85.0f)); // 3초에 걸쳐 사라짐

	// alpha값 적용

	RECT rc;
	GetClientRect(&rc);
	RECT textRect = { rc.right / 2 - 100, rc.bottom / 2 - 15, rc.right / 2 + 100, rc.bottom / 2 + 15 };
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 10.f, 10.f, 5.f); // 폰트 크기 2배
	D3DXMatrixTranslation(&matTrans, rc.right / 2.0f, rc.bottom / 2.0f, 0.f);
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	swprintf_s(szSpeed, L"Scroll Speed: %.1f", m_fSrollSpeed);
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szSpeed,
		lstrlen(szSpeed),
		nullptr,
		DT_CENTER,
		D3DCOLOR_ARGB((BYTE)m_fAlpha, 255, 255, 255));
	Draw_Drag_Line();
	m_pDevice->Render_End();
}

void CToolView::OnDestroy()
{
	KillTimer(m_nTimer);
	CView::OnDestroy();
	//ConsoleWindow::Destroy();
	Safe_Delete(m_pObj);
	CObjManager::Destroy_Instance();
	CKeyManager::Destroy_Instance();
	Safe_Delete(m_pTerrain);
	CTextureMgr::Destroy_Instance();
	CUndoManager::Destroy_Instance();
	m_pDevice->Destroy_Instance();
}


#pragma region 생략
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

#pragma endregion


BOOL CToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (zDelta > 0)
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_RBUTTON))
		{
			m_fSrollSpeed = min(15.f, m_fSrollSpeed + 0.5f);
			m_dwDisplayTime = GetTickCount();
		}
		else
		{
			m_pTerrain->Set_CameraZoom(true);
			if (m_pObj)
			{
				m_pObj->Set_CameraZoom(true);
			}
			CObjManager::Get_Instance()->Set_CameraZoom(true);
		}
	}
	else
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_RBUTTON))
		{
			m_fSrollSpeed = max(2.f, m_fSrollSpeed - 0.5f);
			m_dwDisplayTime = GetTickCount();
		}
		else
		{
			m_pTerrain->Set_CameraZoom(false);
			if (m_pObj)
			{
				m_pObj->Set_CameraZoom(false);
			}

			CObjManager::Get_Instance()->Set_CameraZoom(false);
		}
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_nTimer)
	{
		// 게임 로직 업데이트
		if (m_pDevice)
		{
			m_pTerrain->Update();
	
			if (CKeyManager::Get_Instance()->Key_Pressing('A'))
			{
				m_pTerrain->Set_CameraOffsetX(-m_fSrollSpeed);
				if (m_pObj)
				{
					m_pObj->Set_CameraOffsetX(-m_fSrollSpeed);
				}
				CObjManager::Get_Instance()->Set_CameraOffsetX(-m_fSrollSpeed);
			}
			if (CKeyManager::Get_Instance()->Key_Pressing('D'))
			{
				m_pTerrain->Set_CameraOffsetX(m_fSrollSpeed);
				if (m_pObj)
				{
					m_pObj->Set_CameraOffsetX(m_fSrollSpeed);
				}
				CObjManager::Get_Instance()->Set_CameraOffsetX(m_fSrollSpeed);
			}

			if (CKeyManager::Get_Instance()->Key_Pressing('W'))
			{
				m_pTerrain->Set_CameraOffsetY(-m_fSrollSpeed);
				if (m_pObj)
				{
					m_pObj->Set_CameraOffsetY(-m_fSrollSpeed);
				}
				CObjManager::Get_Instance()->Set_CameraOffsetY(-m_fSrollSpeed);
			}
			if (CKeyManager::Get_Instance()->Key_Pressing('S'))
			{
				m_pTerrain->Set_CameraOffsetY(m_fSrollSpeed);
				if (m_pObj)
				{
					m_pObj->Set_CameraOffsetY(m_fSrollSpeed);
				}
				CObjManager::Get_Instance()->Set_CameraOffsetY(m_fSrollSpeed);
			}

			if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('Z') & 0x0001))
			{
				CUndoManager::Get_Instance()->Undo();
				CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
				CTransformInfo* pTransformInfo = dynamic_cast<CTransformInfo*>(pMainFrm->m_ThirdSplitter.GetPane(1, 0));

				if (pTransformInfo)
				{
					pTransformInfo->Set_TransformSpin(D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f), D3DXVECTOR3(0.f, 0.f, 0.f));
				}
					

			}
			CObjManager::Get_Instance()->Update();
			if (m_pObj)
			{
				m_pObj->Update();
			}
			// 명시적 렌더링
			Invalidate(FALSE);
			CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
			CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

			pMiniView->Invalidate(FALSE);
			//CKeyManager::Get_Instance()->Update();
		}
	}

	CView::OnTimer(nIDEvent);
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{

	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		m_vSecondMousePos = D3DXVECTOR2((float)point.x, (float)point.y);
		DWORD currentTime = GetTickCount();
		if (currentTime - m_dragStartTime > 200
			&& D3DXVec2Length(&(m_vSecondMousePos - m_vFirstMousePos)) >= 70.f
			&&!m_bIsTileMode)
		{
			m_bDragging = true;
		}
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
		if (nullptr != pMyForm)
		{
			if (0 == pMyForm->m_TextureListBox.GetCount())
				return;
		}

		m_pTerrain->Picking_Tile(D3DXVECTOR3((float)point.x, (float)point.y, 0.f), m_bIsObj, m_bIsTileMode);
		//	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

		pMiniView->Invalidate(FALSE);
	}

	if (m_bIsObj)
	{
		m_pObj->Set_Position(D3DXVECTOR3((float)point.x, (float)point.y, 0.f));
	}
	else
	{
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CTransformInfo* pTransformInfo = dynamic_cast<CTransformInfo*>(pMainFrm->m_ThirdSplitter.GetPane(1, 0));


		if (!m_bDragging)
		{

		for (auto& pObj : CObjManager::Get_Instance()->m_vecObj)
		{
			if (!pObj->m_bIsSet)
			{
				pObj->Set_Position(D3DXVECTOR3((float)point.x, (float)point.y, 0.f));
				break;
			}
		}
		}
	}
	CView::OnMouseMove(nFlags, point);
}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pTerrain->OnLButtonUp();
	if (m_bDragging)  // 드래깅 중이었다면
	{
		Select_In_Line_Obj();
		m_dwDragEndTime = GetTickCount(); // 드래그 끝난 시간 저장
	}
	else
	{
		if (!m_bIsTileMode)
		{
			CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
			CTransformInfo* pTransformInfo = dynamic_cast<CTransformInfo*>(pMainFrm->m_ThirdSplitter.GetPane(1, 0));

			if (m_pObj && m_bIsObj)
			{
				CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
				m_pObj->Place_OnTile();
				CObjManager::Get_Instance()->Add_Obj(m_pObj);

				pTransformInfo->Set_SelectedObj(m_pObj);
				pTransformInfo->Set_TransformSpin(m_pObj->m_tInfo.vPos, m_pObj->m_tInfo.vRot, m_pObj->m_tInfo.vSize);

				m_pObj = new CObj;
				m_pObj->Initialize();
				iCount++;
				m_bIsObj = false;
			}
			else // 기존 오브젝트 피킹
			{
	/*			if (GetTickCount() - m_dwDragEndTime < 300)
				{
					pMiniView->Invalidate(FALSE);
					return;
				}*/

				if (CObjManager::Get_Instance()->m_bIsPicking)
				{
					CObjManager::Get_Instance()->m_bIsPicking = false;
					for (auto& pObj : CObjManager::Get_Instance()->m_vecObj)
					{
						if (!pObj->m_bIsSet)
						{
							pObj->Place_OnTile();
							CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
							pTransformInfo->Set_SelectedObj(pObj);
							pTransformInfo->Set_TransformSpin(pObj->m_tInfo.vPos, pObj->m_tInfo.vRot, pObj->m_tInfo.vSize);
							break;
						}
					}
				}
				else
				{
						for (auto& pObj : CObjManager::Get_Instance()->m_vecObj)
						{
							if (pObj->Picking_Obj(D3DXVECTOR3((float)point.x, (float)point.y, 0.f)))
							{
								CObjManager::Get_Instance()->m_bIsPicking = true;

								pTransformInfo->Set_SelectedObj(pObj);
								pTransformInfo->Set_TransformSpin(pObj->m_tInfo.vPos, pObj->m_tInfo.vRot, pObj->m_tInfo.vSize);

								break;
							}
						}
				}
			}
		}
	}
	m_bDragging = false;  
	//m_vSecondMousePos = D3DXVECTOR2(); 
	//m_vFirstMousePos = D3DXVECTOR2();  
	//Invalidate(FALSE); 
	CView::OnLButtonUp(nFlags, point);
}


void CToolView::Draw_Drag_Line()
{
	if (!m_DragLine || !m_bDragging) return;

	m_DragLine->SetWidth(3.f);
	m_DragLine->SetAntialias(TRUE); // 안티앨리어싱 키기
	m_DragLine->SetGLLines(TRUE);

	vector<D3DXVECTOR2> vecLine(5);
	float left = min(m_vFirstMousePos.x, m_vSecondMousePos.x);
	float top = min(m_vFirstMousePos.y, m_vSecondMousePos.y);
	// 우하단 좌표
	float right = max(m_vFirstMousePos.x, m_vSecondMousePos.x);
	float bottom = max(m_vFirstMousePos.y, m_vSecondMousePos.y);

	// 사각형의 네 모서리를 시계방향으로 설정
	vecLine[0] = D3DXVECTOR2(left, top);      // 좌상단
	vecLine[1] = D3DXVECTOR2(right, top);     // 우상단
	vecLine[2] = D3DXVECTOR2(right, bottom);  // 우하단
	vecLine[3] = D3DXVECTOR2(left, bottom);   // 좌하단
	vecLine[4] = vecLine[0];                  // 다시 처음으로 돌아가 사각형 완성

	m_DragLine->Draw(vecLine.data(), (DWORD)vecLine.size(), D3DCOLOR_ARGB(255, 255,255,255));
	m_DragLine->End();
}

void CToolView::Select_In_Line_Obj()
{
	float left = min(m_vFirstMousePos.x, m_vSecondMousePos.x);
	float top = min(m_vFirstMousePos.y, m_vSecondMousePos.y);
	float right = max(m_vFirstMousePos.x, m_vSecondMousePos.x);
	float bottom = max(m_vFirstMousePos.y, m_vSecondMousePos.y);

	// 메인프레임과 TransformInfo 가져오기
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CTransformInfo* pTransformInfo = dynamic_cast<CTransformInfo*>(pMainFrm->m_ThirdSplitter.GetPane(1, 0));

	if (!pTransformInfo)
		return;
	vector<CObj*> selectedObjs;

	for (auto& pObj : CObjManager::Get_Instance()->m_vecObj)
	{
		const D3DXVECTOR3& pos = pObj->Get_WorldPos();
		if (pos.x >= left && pos.x <= right &&
			pos.y >= top && pos.y <= bottom)
		{
			selectedObjs.push_back(pObj);
		}
	}
	pTransformInfo->m_vecSelectedObj = selectedObjs;
	pTransformInfo->m_pObj = nullptr;
	cout <<"드래그 안에 들어온 객체 수 "<< selectedObjs.size() << endl;
}
