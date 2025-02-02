// CMyForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CMyForm.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "CTerrain.h"
#include "CUndoManager.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_CMyForm)
{
	
}

CMyForm::~CMyForm()
{
	OnDestroy();
}

void CMyForm::OnButtonObjectList()
{
	if (nullptr == m_FindFolderTree.GetSafeHwnd())
		m_FindFolderTree.Create(IDD_CFindFolderTree);	// 해당 id에 맞는 다이얼로그 생성

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));
	pView->m_bIsTileMode = false;
	m_FindFolderTree.RefreshTree(_T("../Texture/Object"));
	m_FindFolderTree.ShowWindow(SW_SHOW);
//	m_TextureListBox.Load_TextureListOfObjcet();
}

void CMyForm::OnButtonTileList()
{
	if (nullptr == m_FindFolderTree.GetSafeHwnd())
		m_FindFolderTree.Create(IDD_CFindFolderTree);	// 해당 id에 맞는 다이얼로그 생성
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));
	m_FindFolderTree.RefreshTree(_T("../Texture/Stage/Terrain"));
	m_FindFolderTree.ShowWindow(SW_SHOW);
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_TextureListBox);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_TextrueImage);
}


BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMyForm::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnButtonObjectList)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyForm::OnButtonTileList)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyForm::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyForm::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK1, &CMyForm::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CMyForm 진단

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(0, 0)); // 스크롤 없애기
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
	//m_TextureListBox.Load_TextureList();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CMyForm::OnLbnSelchangeList1()
{
	int nIndex = m_TextureListBox.GetCurSel();
	if (nIndex != LB_ERR)
	{
		wstring strFileName;
		wstring CurCategory = m_TextureListBox.m_stCategory;
		strFileName = m_TextureListBox.Get_FilePath(nIndex);// FilePath() +
		CImage image;
		if (image.Load(strFileName.c_str()) == S_OK)
		{
			CClientDC dc(&m_TextrueImage);
			CRect rect;
			m_TextrueImage.GetClientRect(&rect);

			dc.FillSolidRect(rect, RGB(255, 255, 255));
			image.Draw(dc, rect);

			CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
			CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));
			if (CurCategory == L"Tile")
			{


			CTerrain* pTerrain = pView->m_pTerrain;
			pTerrain->m_stTileFolderName = m_TextureListBox.m_stFolderName;
			pTerrain->Change_DrawID(nIndex, m_TextureListBox.m_stFolderName);
			pView->m_bIsTileMode = true;
			}

			else if(CurCategory == L"Structure")
			{
				pView->m_bIsTileMode = false;
				pView->m_bIsObj = true;
				CObj* pObj = pView->m_pObj;
				pObj->Set_WallSprite(m_TextureListBox.m_stFolderName, m_TextureListBox.m_stCategory, nIndex);
			}

			else
			{
				pView->m_bIsTileMode = false;
				pView->m_bIsObj = true;
				CObj* pObj = pView->m_pObj;
				if (pObj)
				{
					pObj->Set_Sprite(m_TextureListBox.m_stFolderName, m_TextureListBox.m_stCategory, m_TextureListBox.GetCount());
				}
			}

		}
	}
}

//Save 버튼
void CMyForm::OnBnClickedButton3()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

	CTerrain* pTerrain = pView->m_pTerrain;

	//SaveTileData(pTerrain->m_vecTile, _T("../Data"), _T("tileDate.dat"));
	SaveMapData(pTerrain->m_vecTile, CObjManager::Get_Instance()->m_vecObj, _T("../Data"), _T("mapData.dat"));
}

//Load 버튼
void CMyForm::OnBnClickedButton4()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

	CTerrain* pTerrain = pView->m_pTerrain;

	//LoadTileData(pTerrain->m_vecTile, _T("../Data"), _T("tileDate.dat"));
	LoadMapData(pTerrain->m_vecTile, CObjManager::Get_Instance()->m_vecObj, _T("../Data"), _T("mapData.dat"));
}

void CMyForm::SaveTileData(vector<TILE>& vecTile, const CString& strFolderPath, const CString& strFileName)
{
	// 폴더 경로 확인 및 생성
	if (!PathFileExists(strFolderPath))
	{
		if (!CreateDirectory(strFolderPath, NULL))
		{
			AfxMessageBox(_T("폴더 생성에 실패했습니다."));
			return;
		}
	}

	// 전체 경로 생성
	CString strFullPath;
	strFullPath.Format(_T("%s\\%s"), strFolderPath, strFileName);

	CFile File;
	if (!File.Open(strFullPath, CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox(_T("파일을 생성할 수 없습니다."));
		return;
	}

	CArchive Ar(&File, CArchive::store);
	int iTileCount = vecTile.size();
	Ar << iTileCount;
	for (int i = 0; i < iTileCount; ++i)
	{
		vecTile[i].Serialize(Ar);
	}
	Ar.Close();
	File.Close();
	CString strSuccess;
	strSuccess.Format(_T("파일이 성공적으로 저장되었습니다.\n저장 경로: %s"), strFullPath);
	MessageBox(strSuccess, _T("저장 완료"), MB_ICONINFORMATION);
}

void CMyForm::LoadTileData(vector<TILE>& vecTile, const CString& strFolderPath, const CString& strFileName)
{
	// 전체 경로 생성
	CString strFullPath;
	strFullPath.Format(_T("%s\\%s"), strFolderPath, strFileName);

	// 폴더 경로 확인
	if (!PathFileExists(strFolderPath))
	{
		AfxMessageBox(_T("파일이 없습니다."));
		return;
	}

	CFile File;

	if (!File.Open(strFullPath, CFile::modeRead))
	{
		AfxMessageBox(_T("파일을 생성할 수 없습니다."));
		return;
	}

	CArchive Ar(&File, CArchive::load);

	int iTileCount = 0;

	Ar >> iTileCount;
	CUndoManager::Get_Instance()->SaveState(UndoType::TILE);
	vecTile.clear();
	vecTile.resize(iTileCount);
	for (int i = 0; i < iTileCount; ++i)
	{
		vecTile[i].Serialize(Ar);
	}
	Ar.Close();
	File.Close();

	CString strSuccess;
	strSuccess.Format(_T("파일을 성공적으로 불러왔습니다.\n파일 경로: %s"), strFullPath);
	MessageBox(strSuccess, _T("로드 완료"), MB_ICONINFORMATION);
}

void CMyForm::SaveMapData(vector<TILE>& vecTile, vector<CObj*>& vecObj, const CString& strFolderPath, const CString& strFileName)
{
	if (!PathFileExists(strFolderPath))
	{
		if (!CreateDirectory(strFolderPath, NULL))
		{
			AfxMessageBox(_T("폴더 생성에 실패했습니다."));
			return;
		}
	}

	// 전체 경로 생성
	CString strFullPath;
	strFullPath.Format(_T("%s\\%s"), strFolderPath, strFileName);

	CFile File;
	if (!File.Open(strFullPath, CFile::modeCreate | CFile::modeWrite))
	{
		AfxMessageBox(_T("파일을 생성할 수 없습니다."));
		return;
	}

	CArchive Ar(&File, CArchive::store);

	// 타일 데이터 저장
	int iTileCount = vecTile.size();
	Ar << iTileCount;
	for (int i = 0; i < iTileCount; ++i)
	{
		vecTile[i].Serialize(Ar);
	}

	// 오브젝트 데이터 저장
	int iObjCount = vecObj.size();
	Ar << iObjCount;
	for (int i = 0; i < iObjCount; ++i)
	{
		if (vecObj[i])
			vecObj[i]->Serialize(Ar);
	}

	Ar.Close();
	File.Close();

	CString strSuccess;
	strSuccess.Format(_T("맵 데이터가 성공적으로 저장되었습니다.\n저장 경로: %s"), strFullPath);
	MessageBox(strSuccess, _T("저장 완료"), MB_ICONINFORMATION);
}

void CMyForm::LoadMapData(vector<TILE>& vecTile, vector<CObj*>& vecObj, const CString& strFolderPath, const CString& strFileName)
{
	CString strFullPath;
	strFullPath.Format(_T("%s\\%s"), strFolderPath, strFileName);

	// 폴더 경로 확인
	if (!PathFileExists(strFolderPath))
	{
		AfxMessageBox(_T("파일이 없습니다."));
		return;
	}

	CFile File;
	if (!File.Open(strFullPath, CFile::modeRead))
	{
		AfxMessageBox(_T("파일을 열 수 없습니다."));
		return;
	}

	CArchive Ar(&File, CArchive::load);

	// 현재 상태 저장 (실행 취소를 위해)
	CUndoManager::Get_Instance()->SaveState(UndoType::TILE);
	CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);

	// 타일 데이터 로드
	int iTileCount = 0;
	Ar >> iTileCount;
	vecTile.clear();
	vecTile.resize(iTileCount);
	for (int i = 0; i < iTileCount; ++i)
	{
		vecTile[i].Serialize(Ar);
	}

	// 오브젝트 데이터 로드
	int iObjCount = 0;
	Ar >> iObjCount;

	// 기존 오브젝트 메모리 해제
	for (auto& pObj : vecObj)
	{
		if (pObj)
		{
			delete pObj;
		}
	}
	vecObj.clear();

	// 새 오브젝트 생성 및 로드
	for (int i = 0; i < iObjCount; ++i)
	{
		CObj* pNewObj = new CObj;
		pNewObj->Initialize();
		pNewObj->Serialize(Ar);
		vecObj.push_back(pNewObj);
	}

	Ar.Close();
	File.Close();

	CString strSuccess;
	strSuccess.Format(_T("맵 데이터를 성공적으로 불러왔습니다.\n파일 경로: %s"), strFullPath);
	MessageBox(strSuccess, _T("로드 완료"), MB_ICONINFORMATION);
}


void CMyForm::OnBnClickedCheck1()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

	CTerrain* pTerrain = pView->m_pTerrain;
	pTerrain->m_bOnGrid = !pTerrain->m_bOnGrid;
}
