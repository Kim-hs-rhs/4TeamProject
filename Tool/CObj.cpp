
#include "pch.h"
#include "CObj.h"

CObj::CObj() : m_pTerrain(nullptr), m_AnimeTime(0)
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

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Human/NPC/Akara/%d.png",
		TEX_MULTI, L"NPC", L"Akara", 13)))
	{
		AfxMessageBox(L"NPC Akara Insert Failed");
		return;
	}

	m_tInfo.vPos.x = 400.f;
	m_tInfo.vPos.y = 300.f;

	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = 13.f;
}

void CObj::Update()
{
	DWORD CurrentTime = GetTickCount();

	if (CurrentTime - m_AnimeTime > 300)
	{
		m_tFrame.fFrame = (int)(m_tFrame.fFrame + 1.f) % (int)m_tFrame.fMax;
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

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"NPC", L"Akara", m_tFrame.fFrame);

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
	//m_pTerrain->m_vecLine[m_pTerrain->m_LineIndex][0];


}

void CObj::Set_Position(const D3DXVECTOR3& _vMouse)
{
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
