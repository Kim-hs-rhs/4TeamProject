#pragma once
#include "Include.h"
#include "CObj.h"
class CObjManager
{

	DECLARE_SINGLETON(CObjManager)
public:
	void		Initialize();
	void		Update();
	void		Render();
	void Mini_Render();
	void		Release();
	void Add_Obj(CObj* pObj) { m_vecObj.push_back(pObj); }
	void Set_CameraOffsetX(float deltaX);
	void Set_CameraOffsetY(float deltaY);
	void Set_CameraZoom(bool bUp);
public:
	vector<CObj*> m_vecObj;
	bool m_bIsPicking = false;
private:
	CObjManager() = default;
	~CObjManager() { Release(); }


};

