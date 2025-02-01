#include "pch.h"
#include "CObjManager.h"


IMPLEMENT_SINGLETON(CObjManager)

void CObjManager::Initialize()
{
}

void CObjManager::Update()
{
	for (const auto& pObj : m_vecObj)
	{
		if (pObj)
		{
			pObj->Update();
		}
	}
}

void CObjManager::Render()
{
	for (const auto& pObj : m_vecObj)
	{
		if (pObj)
		{
			pObj->Render();
		}
	}
}

void CObjManager::Mini_Render()
{
	for (const auto& pObj : m_vecObj)
	{
		if (pObj)
		{
			pObj->Mini_Render();
		}
	}
}

void CObjManager::Release()
{
	for (auto& pObj : m_vecObj)
	{
		Safe_Delete(pObj);
	}
	m_vecObj.clear();
}

void CObjManager::Set_CameraOffsetX(float deltaX)
{
	for (const auto& pObj : m_vecObj)
	{
		if (pObj)
		{
			pObj->Set_CameraOffsetX(deltaX);
		}
	}
}

void CObjManager::Set_CameraOffsetY(float deltaY)
{
	for (const auto& pObj : m_vecObj)
	{
		if (pObj)
		{
			pObj->Set_CameraOffsetY(deltaY);
		}
	}
}

void CObjManager::Set_CameraZoom(bool bUp)
{
	for (const auto& pObj : m_vecObj)
	{
		if (pObj)
		{
			pObj->Set_CameraZoom(bUp);
		}
	}
}
