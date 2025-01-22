#include "pch.h"
#include "CTerrain.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include <iostream>
#include "CPerformanceTimer.h"

CTerrain::CTerrain() :
	m_bCanRender(false), m_iChangeDrawId(0), m_dwContinuousTime(0ULL),
	m_dwDrawTileRenderTime(0ULL)
{
}

CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::Initialize()
{
	m_bCanRender = true;
	m_iChangeDrawId = 0;
	m_dwContinuousTime = 2000ULL; // 2��
	m_dwDrawTileRenderTime = GetTickCount64();
	//m_vecTile.resize(���� *����)
	//for (int j = 0; j < WINCY / TILEY; j++)
	//{
	//	for (int i = 0; i < WINCX / TILECX + 1; i++)
	//	{
	//		float fOffsetX = (j % 2 == 0) ? TILECX * 0.5f : 0.0f;

	//		m_vecTile.push_back(new TILE(
	//			{ float(i * TILECX) + fOffsetX , float(j * (TILECY * 0.5f)), 0.f },
	//			0, { TILECX, TILECY }));
	//	}
	//}
// ȭ�� �߾� ��ǥ
	float centerX = WINCX * 0.5f;
	float centerY = WINCY * 0.5f;

	// Ÿ�� ���� ��� (�� �������� �������� Ÿ�� ��)
	int halfTilesX = (WINCX / TILECX) / 2;  // ���� �������� ���� Ÿ�� ��
	int halfTilesY = (WINCY / TILEY) / 2;

	// �߾�(0,0)���� ����
	float localX = 0.0f;
	float localY = 0.0f;

	// -halfTiles���� +halfTiles���� �ݺ��Ͽ� 4�������� Ÿ�� ����
	for (int j = -halfTilesY; j <= halfTilesY; j++)
	{
		for (int i = -halfTilesX-1; i <= halfTilesX; i++)
		{
			float fOffsetX = (j % 2 == 0) ? TILECX * 0.5f : 0.0f;

			float posX = localX + (i * TILECX) + fOffsetX;
			float posY = localY + (j * (TILECY * 0.5f));
			float worldX = posX + centerX;
			float worldY = posY + centerY;
			m_vecTile.push_back(new TILE(
				{ worldX , worldY, 0.f },  // ���� ȭ�鿡 �׸� ���� �߾��� ���ϱ�
				0,
				{ TILECX, TILECY }));
		}
	}

	m_vecTileWorldMat.resize(m_vecTile.size());
	for (int i = 0; i < (int)m_vecTileWorldMat.size(); i++)
	{
		TILE* tile = m_vecTile[i];
		D3DXMATRIX  matScale, matTrans;
		auto& matWorld = m_vecTileWorldMat[i];

		D3DXMatrixIdentity(&matWorld);
		/*D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, tile->vPos.x, tile->vPos.y, tile->vPos.z);*/

		D3DXMatrixScaling(&matScale, fCameraZoom, fCameraZoom, 1.f);

		// Ÿ�� ��ġ�� ī�޶� ������ ����
		D3DXMatrixTranslation(&matTrans,
			(tile->vPos.x + vCameraOffset.x) * fCameraZoom,
			(tile->vPos.y + vCameraOffset.y) * fCameraZoom,
			tile->vPos.z);

		
		matWorld = matScale * matTrans;
	}
}

void CTerrain::Update()
{
	float screenCenterX = WINCX / 2.0f;
	float screenCenterY = WINCY / 2.0f;

	// �� ���� �� ȭ�� �߾� ������ ���� ������ ���
	float zoomOffsetX = screenCenterX - (screenCenterX + vCameraOffset.x) * fCameraZoom;
	float zoomOffsetY = screenCenterY - (screenCenterY + vCameraOffset.y) * fCameraZoom;
	for (int i = 0; i < (int)m_vecTile.size(); i++)
	{
		TILE* tile = m_vecTile[i];
		if (tile->bChange)
		{
			
		}
		D3DXMATRIX matWorld, matScale, matTrans;
		D3DXMatrixIdentity(&matWorld);
		/*D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, tile->vPos.x, tile->vPos.y, tile->vPos.z);*/
		D3DXMatrixScaling(&matScale, fCameraZoom, fCameraZoom, 1.f);

		// Ÿ�� ��ġ�� ī�޶� ������ ����
		D3DXMatrixTranslation(&matTrans,
			(tile->vPos.x + vCameraOffset.x) * fCameraZoom + zoomOffsetX,
			(tile->vPos.y + vCameraOffset.y) * fCameraZoom + zoomOffsetY,
			tile->vPos.z);
		matWorld = matScale * matTrans;
		m_vecTileWorldMat[i] = matWorld;
		tile->bChange = false;
	}
}

void CTerrain::Render()
{

#ifdef _DEBUG
	static CPerformanceTimer timer;
	static int frameCount = 0;
	static double totalRenderTime = 0.0;
	static int drawCalls = 0;
	static int textureSwaps = 0;
	timer.Start();
#endif
	const TEXINFO* lastTexInfo = nullptr;
//	CDevice::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
	BYTE currentDrawID = 255; // �ʱⰪ�� �߸��� ������ ����
	for (int i = 0; i < (int)m_vecTile.size(); i++)
	{
		TILE* tile = m_vecTile[i];
		
#ifdef _DEBUG
		// �ؽ�ó ���� ī��Ʈ
		if (currentDrawID != tile->byDrawID) 
		{
			textureSwaps++;
			currentDrawID = tile->byDrawID;
			lastTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", currentDrawID);
		}
		// ��ο��� ī��Ʈ
		drawCalls++;
#endif
		D3DXVECTOR3 vCenter(tile->vSize.x * 0.5f, tile->vSize.y * 0.5f, 0.f);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_vecTileWorldMat[i]);
		if (lastTexInfo)
		{
		CDevice::Get_Instance()->Get_Sprite()->Draw(lastTexInfo->pTexture,
			nullptr,
			&vCenter,   // �ؽ�ó�� �߽��� (�߽����� ���ؼ� �̵� ���Ѿ��� �� �׷���
			nullptr,    // �̹� matWorld�� ��ġ�� ���������Ƿ� nullptr
			D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		// D3DXMatrixTranslation(&m_vecTileWorldMat[i], 200.f, 200.f, 0.f);



	}

	for (int i = 0; i < (int)m_vecTile.size(); i++)
	{
		TILE* tile = m_vecTile[i];
		TCHAR	szBuf[MIN_STR] = L"";
		swprintf_s(szBuf, L"(%.f)", -(tile->vPos.y - WINCY * 0.5f));// , tile->vPos.y - WINCY * 0.5f);
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_vecTileWorldMat[i]);
		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,		// ����� ���ڿ�
			lstrlen(szBuf),  // ���ڿ� ������ ũ��
			nullptr,	// ����� ��Ʈ ��ġ
			0,			// ���� ����(�ɼ�)
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (m_bCanRender)
	{
		Render_Current_Draw_Tile();
	}

#ifdef _DEBUG
	// ���� ���� ��� (60�����Ӹ���)
	double renderTime = timer.End();
	totalRenderTime += renderTime;
	frameCount++;

	if (frameCount >= 60)
	{
		double averageRenderTime = totalRenderTime / frameCount;
		wchar_t szDebug[256] = {};
		swprintf_s(szDebug,
			L"[Terrain Info]\nAvg Render Time: %.3f ms\nDraw Calls: %d\nTexture Swaps: %d\nTile Count: %d\n\n",
			averageRenderTime, drawCalls / frameCount, textureSwaps / frameCount, (int)m_vecTile.size());
		OutputDebugString(szDebug);

		// ī���� ����
		frameCount = 0;
		totalRenderTime = 0.0;
		drawCalls = 0;
		textureSwaps = 0;
	}
#endif
//	CDevice::Get_Instance()->Get_Sprite()->End();
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<TILE*>);
}

void CTerrain::Picking_Tile(const D3DXVECTOR3& mousePoint)
{
	float screenCenterX = WINCX / 2.0f;
	float screenCenterY = WINCY / 2.0f;

	// �� ���� �� ȭ�� �߾� ������ ���� ������ ���
	float zoomOffsetX = screenCenterX - (screenCenterX + vCameraOffset.x) * fCameraZoom;
	float zoomOffsetY = screenCenterY - (screenCenterY + vCameraOffset.y) * fCameraZoom;
	D3DXVECTOR3 adjustedMousePoint = {
	   (mousePoint.x - vCameraOffset.x) / fCameraZoom,
	   (mousePoint.y - vCameraOffset.y) / fCameraZoom,
	   mousePoint.z
	};

	for (auto& tile : m_vecTile)
	{
		bool bInner = true;

		D3DXVECTOR3 upPoint, bottomPoint, leftPoint, rightPoint;

		upPoint = { tile->vPos.x, tile->vPos.y + tile->vSize.y * 0.5f, 0.f };
		bottomPoint = { tile->vPos.x, tile->vPos.y - tile->vSize.y * 0.5f,0.f };
		rightPoint = { tile->vPos.x + tile->vSize.x * 0.5f, tile->vPos.y,0.f };
		leftPoint = { tile->vPos.x - tile->vSize.x * 0.5f, tile->vPos.y ,0.f };

		// �ð����
		//vector<D3DXVECTOR3> vecCWStartPoints{ leftPoint ,upPoint, rightPoint, bottomPoint };
		//vector<D3DXVECTOR3> vecCWPolyVec{ upPoint - leftPoint,rightPoint - upPoint,bottomPoint - rightPoint,
		//leftPoint - bottomPoint };		
		//for (int i = 0; i < (int)vecCWStartPoints.size(); i++)
		//{
		//	D3DXVECTOR3 vMouse = mousePoint - vecCWStartPoints[i];
		//	D3DXVECTOR3 tmp(-vecCWPolyVec[i].y, vecCWPolyVec[i].x, vecCWPolyVec[i].z); //�������� �������͸� ����

		//	D3DXVec3Normalize(&vMouse, &vMouse);
		//	D3DXVec3Normalize(&tmp, &tmp);
		//	if (D3DXVec3Dot(&tmp, &vMouse) < 0.f)
		//	{
		//		bInner = false;
		//		break;
		//	}
		//}
		// �ݽð� ����
		vector<D3DXVECTOR3> vecCCWStartPoints{ upPoint,leftPoint,  bottomPoint,rightPoint };
		vector<D3DXVECTOR3> vecCCWPolyVec{leftPoint-upPoint,bottomPoint - leftPoint,
	 rightPoint - bottomPoint, upPoint-rightPoint };

		for (int i = 0; i < (int)vecCCWStartPoints.size(); i++)
		{
			D3DXVECTOR3 vMouse = adjustedMousePoint - vecCCWStartPoints[i];
			D3DXVECTOR3 tmp(-vecCCWPolyVec[i].y, vecCCWPolyVec[i].x, vecCCWPolyVec[i].z); // �ݽð�� ����(���� ȸ��)���� ���� ���͸� ����

			D3DXVec3Normalize(&vMouse, &vMouse);
			D3DXVec3Normalize(&tmp, &tmp);
			if (D3DXVec3Dot(&tmp, &vMouse) < 0.f)
			{
				bInner = false;
				break;
			}
		}


		//// ���
		//float a1 = (rightPoint.y - upPoint.y) / (rightPoint.x - upPoint.x);
		//float b1 = upPoint.y - a1 * upPoint.x; // b = y - ax
		//if (mousePoint.y < a1 * mousePoint.x + b1)
		//{
		//	continue;
		//}

		//// �»�
		//float a2 = (leftPoint.y - upPoint.y) / (leftPoint.x - upPoint.x);
		//float b2 = upPoint.y - a2 * upPoint.x;
		//if (mousePoint.y < a2 * mousePoint.x + b2)
		//{

		//	continue;
		//}

		//// ����
		//float a3 = (rightPoint.y - bottomPoint.y) / (rightPoint.x - bottomPoint.x);
		//float b3 = bottomPoint.y - a3 * bottomPoint.x;
		//if (mousePoint.y > a3 * mousePoint.x + b3)
		//{
		//	continue;
		//}

		//// ����
		//float a4 = (leftPoint.y - bottomPoint.y) / (leftPoint.x - bottomPoint.x);
		//float b4 = bottomPoint.y - a4 * bottomPoint.x;
		//if (mousePoint.y > a4 * mousePoint.x + b4)
		//{
		//	continue;
		//}

		if (bInner)
		{
			tile->byDrawID = m_iChangeDrawId;
			tile->bChange = true;
			break;
		}
	}
}

void CTerrain::Change_DrawID(bool bUp)
{
	fCameraZoom = bUp ? fCameraZoom += 0.1f : max(0.f,fCameraZoom - 0.1f);
	m_bCanRender = true;
	m_dwDrawTileRenderTime = GetTickCount64();
	Set_Picking_DrawId(bUp ? (m_iChangeDrawId + 1) % 36 : (m_iChangeDrawId - 1 < 0 ? m_iChangeDrawId = 35 : m_iChangeDrawId -= 1));
}

void CTerrain::Render_Current_Draw_Tile()
{
	if (m_bCanRender)
	{
		auto dwCur = GetTickCount64();
		if (dwCur - m_dwDrawTileRenderTime >= m_dwContinuousTime)
		{
			m_bCanRender = false;
		}
	}

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_iChangeDrawId);

	D3DXVECTOR3 vCenter(TILECX * 0.5f, TILECY * 0.5f, 0.f);

	D3DXMATRIX matWorld, matRotZ, matScale, matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, 700.f, 50.f, 0.f);
	matWorld = matScale * matTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&vCenter,   // �ؽ�ó�� �߽���    
		nullptr,    // �̹� matWorld�� ��ġ�� ���������Ƿ� nullptr
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

