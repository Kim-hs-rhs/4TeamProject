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

public:
	void		Initialize();
	void		Update();
	void		Render();
	void		Release();
	void		Picking_Obj();

	void Set_Position(const D3DXVECTOR3& _vMouse);

	void Set_CameraOffsetX(float deltaX) { vCameraOffset.x += deltaX; }
	void Set_CameraOffsetY(float deltaY) { vCameraOffset.y += deltaY; }
	void Set_CameraZoom(bool bUp) { fCameraZoom = bUp ? fCameraZoom += 0.1f : max(0.f, fCameraZoom - 0.1f); }
	
	void Set_Terrain(CTerrain* _Terrain) { m_pTerrain = _Terrain; }
	void Set_Ratio(D3DXMATRIX& pOut, float _fX, float _fY);
	void Set_Sprite(const wstring& strStateKey, const wstring& strObjKey,int iMaxFrame);
public:
	wstring m_strStateKey;
	wstring m_strObjKey;
	bool m_bIsSet;
private:
	float fCameraZoom = 1.0f;  // �� ���� (1.0�� �⺻)
	D3DXVECTOR2 vCameraOffset;
	CToolView* m_pMainView;
	INFO	m_tInfo;
	FRAME   m_tFrame;
	DWORD	m_AnimeTime;


	int m_LineIndex;
	CTerrain* m_pTerrain;
};
