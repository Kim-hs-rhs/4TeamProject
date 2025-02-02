
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "Tool.h"
#include "Include.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "CMyForm.h"
#include "CMiniView.h"
#include "CTransformInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);  // IDR_MAINFRAME는 새로 추가한 아이콘의 리소스 ID
	//SetIcon(hIcon, TRUE);  // 큰 아이콘 설정
	//SetIcon(hIcon, TRUE); // 작은 아이콘 설정

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_MainSplitter.CreateStatic(this, 1, 2); // 1행 2열 짜리 만듦
	// 2번째 행에 메인 뷰
	//m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CToolView), CSize(WINCX, WINCY), pContext);

	//IdFromRowCol(행, 열) : 지정한 행과 열에 해당하는 창의 id 값을 반환

	// 첫번째 행에 2행 1열짜리 만들기
	m_SecondSplitter.CreateStatic(&m_MainSplitter, 2, 1,
		WS_CHILD | WS_VISIBLE,
		m_MainSplitter.IdFromRowCol(0, 0));

	// 미니맵
	m_SecondSplitter.CreateView(0, 0, RUNTIME_CLASS(CMiniView), CSize(300, 300), pContext);
	// 다이어로그
	m_SecondSplitter.CreateView(1, 0, RUNTIME_CLASS(CMyForm), CSize(300, 100), pContext);
	//SetColumnInfo(열 번호, 열의 크기 지정, 허용 가능한 최소크기)

	m_ThirdSplitter.CreateStatic(&m_MainSplitter, 2, 1,
		WS_CHILD | WS_VISIBLE,
		m_MainSplitter.IdFromRowCol(0, 1));

	m_ThirdSplitter.CreateView(0, 0, RUNTIME_CLASS(CToolView), CSize(500, 300), pContext);
	m_ThirdSplitter.CreateView(1, 0, RUNTIME_CLASS(CTransformInfo), CSize(500, 100), pContext);
	m_MainSplitter.SetColumnInfo(0, 400, 10); // 1열에 (왼쪽 창) 300을 기본으로 10까지만 줄일 수 있게
	m_ThirdSplitter.SetRowInfo(0, 600, 10);
	m_ThirdSplitter.SetRowInfo(1, 100, 10);

	return TRUE;
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	if (m_MainSplitter.GetSafeHwnd() &&
		m_SecondSplitter.GetSafeHwnd() &&
		m_ThirdSplitter.GetSafeHwnd())
	{
		/*m_MainSplitter.SetRowInfo(0, cy, 10);
		m_MainSplitter.SetColumnInfo(0, 400, 10);
		m_MainSplitter.SetColumnInfo(1, cx - 400, 10);

		m_SecondSplitter.SetRowInfo(0, 300, 10);
		m_SecondSplitter.SetRowInfo(1, cy - 300, 10);

		m_ThirdSplitter.SetRowInfo(0, (cy * 0.60), 10);
		m_ThirdSplitter.SetRowInfo(1, (cy * 0.40), 10);

		m_MainSplitter.RecalcLayout();*/
	}
}
