#pragma once

#include "Include.h"
#include "ToolView.h"
#include "CMiniView.h"
#include "CTerrain.h"
#include "MainFrm.h"
#include "CTextureMgr.h"

class CObj
{
public:
	CObj();
	~CObj();
	CObj(const CObj&& other) noexcept
		: m_pTerrain(other.m_pTerrain), 
		m_AnimeTime(other.m_AnimeTime), 
		m_bIsSet(other.m_bIsSet), m_pMainView(other.m_pMainView),
		m_strStateKey(other.m_strStateKey),
		m_strObjKey(other.m_strObjKey)
	{
		m_tInfo = other.m_tInfo;
		m_tFrame = other.m_tFrame;
		fCameraZoom = other.fCameraZoom;
		vCameraOffset = other.vCameraOffset;
	}

	CObj& operator=(const CObj&& other) noexcept
	{
		if (this == &other)
			return *this;
		m_pTerrain = other.m_pTerrain;
		m_AnimeTime = other.m_AnimeTime;
		m_bIsSet = other.m_bIsSet;
		m_pMainView = other.m_pMainView;
		m_strStateKey = other.m_strStateKey;
		m_strObjKey = other.m_strObjKey;
		m_tInfo = other.m_tInfo;
		m_tFrame = other.m_tFrame;
		fCameraZoom = other.fCameraZoom;
		vCameraOffset = other.vCameraOffset;
		return *this;
	}

	CObj(const CObj& other)
	{
		m_pTerrain = other.m_pTerrain;
		m_AnimeTime = other.m_AnimeTime;
		m_bIsSet = other.m_bIsSet;
		m_pMainView = other.m_pMainView;
		m_strStateKey = other.m_strStateKey;
		m_strObjKey = other.m_strObjKey;
		m_tInfo = other.m_tInfo;
		m_tFrame = other.m_tFrame;
		fCameraZoom = other.fCameraZoom;
		vCameraOffset = other.vCameraOffset;
	}


public:
	void		Initialize();
	void		Update();
	void		Render();
	void Mini_Render();	
	void		Release();
	void		Place_OnTile();

	INFO Get_Info() { return m_tInfo; }
	D3DXVECTOR3 Get_WorldPos() { return D3DXVECTOR3(m_tInfo.vPos - (D3DXVECTOR3)vCameraOffset) * fCameraZoom; }

	bool Picking_Obj(const D3DXVECTOR3& mousePoint);

	void Set_Position(const D3DXVECTOR3& _vMouse);
	void Set_Position_bySpin(const D3DXVECTOR3& vPos) { m_tInfo.vPos = vPos;}
	void Set_Rotation_bySpin(const D3DXVECTOR3& vRot) { m_tInfo.vRot = vRot; }
	void Set_Scale_bySpin(const D3DXVECTOR3& vScale) { m_tInfo.vSize = vScale; }

	void Set_CameraOffsetX(float deltaX) { vCameraOffset.x += deltaX; }
	void Set_CameraOffsetY(float deltaY) { vCameraOffset.y += deltaY; }
	void Set_CameraZoom(bool bUp) { fCameraZoom = bUp ? fCameraZoom += 0.1f : max(0.f, fCameraZoom - 0.1f); }
	
	void Set_Terrain(CTerrain* _Terrain) { m_pTerrain = _Terrain; }
	void Set_Ratio(D3DXMATRIX& pOut, float _fX, float _fY);
	void Set_Sprite(const wstring& strStateKey, const wstring& strObjKey,int iMaxFrame);
	void Set_WallSprite(const wstring& strStateKey, const wstring& strObjKey, int iIndex);

	void Serialize(CArchive& ar);
public:
	wstring m_strStateKey;
	wstring m_strObjKey;
	bool m_bIsSet;
	float fCameraZoom;
	D3DXVECTOR2 vCameraOffset;
	CToolView* m_pMainView;
	INFO	m_tInfo;
	FRAME   m_tFrame;
	DWORD	m_AnimeTime;

	CTerrain* m_pTerrain;

	D3DXMATRIX m_matScale;
	D3DXMATRIX m_matRot;
	D3DXMATRIX m_matTrans;
};
