
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

	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //����� �ؽ�ó �İ�ü
		nullptr,		// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0�������� ���
		&vTemp,		// ����� �̹����� �߽� ��ǥ vec3 �ּ�, null�� ��� 0, 0 �̹��� �߽�
		nullptr,		// ��ġ ��ǥ�� ���� vec3 �ּ�, null�� ��� ��ũ�� �� 0, 0 ��ǥ ���	
		D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� �ʰ� ���� ���� ����

}

void CObj::Release()
{
}

void CObj::Picking_Obj()
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
	if (ar.IsStoring()) // ������ ��
	{	

		int strLength = m_strStateKey.length(); // ���ڿ� ���� ����
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
	else // �ҷ��� ��
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
