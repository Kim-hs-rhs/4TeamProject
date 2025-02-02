
#include "pch.h"
#include "CObj.h"

CObj::CObj() : m_pTerrain(nullptr), m_AnimeTime(0), m_bIsSet(false), m_pMainView(nullptr), m_strStateKey(L""), m_strObjKey(L"")
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&vCameraOffset, sizeof(D3DXVECTOR2));
}

CObj::~CObj()
{
}

void CObj::Initialize()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	m_pMainView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

	m_pTerrain = m_pMainView->m_pTerrain;
	fCameraZoom = m_pTerrain->fCameraZoom;
	vCameraOffset = m_pTerrain->vCameraOffset;
	m_bIsSet = false;
}

void CObj::Update()
{
	if (m_strObjKey == L"")
		return;
	DWORD CurrentTime = GetTickCount();

	if (CurrentTime - m_AnimeTime > 100)
	{
		m_tFrame.fFrame = float((int)(m_tFrame.fFrame + 1.f) % (int)m_tFrame.fMax);
		m_AnimeTime = CurrentTime;
	}
}

void CObj::Render()
{
	CDevice::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);

	D3DXMATRIX	matWorld, matScale, matTrans;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, fCameraZoom, fCameraZoom, 1.f);
	D3DXMatrixTranslation(&matTrans,
		(m_tInfo.vPos.x - vCameraOffset.x) * fCameraZoom,
		(m_tInfo.vPos.y - vCameraOffset.y) * fCameraZoom,
		m_tInfo.vPos.z);

	matWorld = matScale * matTrans;

	RECT	rc{};

	GetClientRect(m_pMainView->m_hWnd, &rc);

	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);

	Set_Ratio(matWorld, fX, fY);

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_strObjKey,m_strStateKey, (int)m_tFrame.fFrame);

	if (nullptr == pTexInfo)
		return;	
	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //출력할 텍스처 컴객체
		nullptr,		// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0기준으로 출력
		&vTemp,		// 출력할 이미지의 중심 좌표 vec3 주소, null인 경우 0, 0 이미지 중심
		nullptr,		// 위치 좌표에 대한 vec3 주소, null인 경우 스크린 상 0, 0 좌표 출력	
		D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 섞지 않고 원본 색상 유지

}

void CObj::Mini_Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans,
		m_tInfo.vPos.x,
		m_tInfo.vPos.y,
		m_tInfo.vPos.z);
<<<<<<< Updated upstream
	matWorld = matScale * matTrans;
	Set_Ratio(matWorld, 0.25f, 0.3f);
=======
	matWorld = matScale *matRot* matTrans;
	Set_Ratio(matWorld, 0.15f, 0.15f);
>>>>>>> Stashed changes
	RECT	rc{};
	GetClientRect(m_pMainView->m_hWnd, &rc);
	float	fX = WINCX / float(rc.right - rc.left);
	float	fY = WINCY / float(rc.bottom - rc.top);
	float fScale = min(fX, fY);
	Set_Ratio(matWorld, fScale, fScale);
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	if (m_strObjKey == L"")
		return;
	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_strObjKey, m_strStateKey, (int)m_tFrame.fFrame);
	float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;
	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //출력할 텍스처 컴객체
		nullptr,		// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0기준으로 출력
		&vTemp,		// 출력할 이미지의 중심 좌표 vec3 주소, null인 경우 0, 0 이미지 중심
		nullptr,		// 위치 좌표에 대한 vec3 주소, null인 경우 스크린 상 0, 0 좌표 출력	
		D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 섞지 않고 원본
}

void CObj::Release()
{
}

void CObj::Place_OnTile()
{
	if (!m_pTerrain||m_bIsSet)
		return;

	D3DXVECTOR3 upPoint, bottomPoint, leftPoint, rightPoint;
	auto& m_vecLine = m_pTerrain->m_vecLine;
	int iIndex = m_pTerrain->m_LineIndex;
	upPoint = { m_vecLine[iIndex][0].x, m_vecLine[iIndex][0].y, 0.f };
	rightPoint = { m_vecLine[iIndex][1].x, m_vecLine[iIndex][1].y, 0.f };
	bottomPoint = { m_vecLine[iIndex][2].x, m_vecLine[iIndex][2].y, 0.f };
	leftPoint = { m_vecLine[iIndex][3].x, m_vecLine[iIndex][3].y, 0.f };

	m_tInfo.vPos.x = (rightPoint.x + leftPoint.x) * 0.5f;
	m_tInfo.vPos.y = (upPoint.y + bottomPoint.y) * 0.5f;

	m_bIsSet = true;

}

bool CObj::Picking_Obj(const D3DXVECTOR3& mousePoint)
{
	if (m_strObjKey.empty())
		return false;

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(
		m_strObjKey.c_str(),
		m_strStateKey.c_str(),
		(int)m_tFrame.fFrame);

	if (nullptr == pTexInfo)
		return false;

	float fHalfWidth = pTexInfo->tImgInfo.Width * 0.5f;
	float fHalfHeight = pTexInfo->tImgInfo.Height * 0.5f;

	D3DXVECTOR3 worldMousePos;
	worldMousePos.x = mousePoint.x / fCameraZoom + vCameraOffset.x;
	worldMousePos.y = mousePoint.y / fCameraZoom + vCameraOffset.y;
	worldMousePos.z = 0.f;


	float left = m_tInfo.vPos.x - fHalfWidth;
	float right = m_tInfo.vPos.x + fHalfWidth;
	float top = m_tInfo.vPos.y - fHalfHeight;
	float bottom = m_tInfo.vPos.y + fHalfHeight;

	// 충돌 체크
	if (worldMousePos.x >= left && worldMousePos.x <= right &&
		worldMousePos.y >= top && worldMousePos.y <= bottom)
	{
		m_bIsSet = false;
		return true;  // 오브젝트가 선택됨
	}

	return false;
}

void CObj::Set_Position(const D3DXVECTOR3& _vMouse)
{
	if (m_bIsSet) return;
	m_tInfo.vPos.x = _vMouse.x / fCameraZoom + vCameraOffset.x;
	m_tInfo.vPos.y = _vMouse.y / fCameraZoom + vCameraOffset.y;
}

void CObj::Set_Ratio(D3DXMATRIX& pOut, float _fX, float _fY)
{
	pOut._11 *= _fX;
	pOut._21 *= _fX;
	pOut._31 *= _fX;
	pOut._41 *= _fX;

	pOut._12 *= _fY;
	pOut._22 *= _fY;
	pOut._32 *= _fY;
	pOut._42 *= _fY;
}

void CObj::Set_Sprite(const wstring& strStateKey, const wstring& strObjKey, int iMaxFrame)
{
	m_strStateKey = strStateKey;
	m_strObjKey = strObjKey;
	m_tFrame.fMax = (float)iMaxFrame;
	m_tFrame.fFrame = 0.f;
}

void CObj::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) // 저장할 때
	{	

		int strLength = m_strStateKey.length(); // 문자열 길이 저장
		ar << strLength;
		for (wchar_t ch : m_strStateKey)
		{
			ar << ch;
		}
		strLength = m_strObjKey.length();

		ar << strLength;
		for (wchar_t ch : m_strObjKey)
		{
			ar << ch;
		}
		ar << m_bIsSet;
		ar << m_tInfo.vPos.x << m_tInfo.vPos.y << m_tInfo.vPos.z;	
		ar << m_tFrame.fFrame << m_tFrame.fMax;
	}
	else // 불러올 때
	{
		int strLength = 0;
		ar >> strLength;
		m_strStateKey.clear();
		for (int i = 0; i < strLength; ++i)
		{
			wchar_t ch;
			ar >> ch;
			m_strStateKey += ch;
		}
		strLength = 0;
		ar >> strLength;
		m_strObjKey.clear();
		for (int i = 0; i < strLength; ++i)
		{
			wchar_t ch;
			ar >> ch;
			m_strObjKey += ch;
		}
		ar >> m_bIsSet;
		ar >> m_tInfo.vPos.x >> m_tInfo.vPos.y >> m_tInfo.vPos.z;
		ar >> m_tFrame.fFrame >> m_tFrame.fMax;
	}
}
