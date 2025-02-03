#include "pch.h"
#include "CUndoManager.h"
#include "MainFrm.h"
#include "CTerrain.h"
#include "ToolView.h"


IMPLEMENT_SINGLETON(CUndoManager)

void CUndoManager::SaveState(const UndoType eUndoType)
{
	EditorState state;
	state.eType = eUndoType;

	switch (eUndoType)
	{
	case UndoType::TILE:
	{

		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

		CTerrain* pTerrain = pView->m_pTerrain;
		if (pTerrain)
		{
			state.vecTileInfo = pTerrain->m_vecTile;
		}
		break;
	}
	case UndoType::OBJ:
		state.vecObjInfo.reserve(CObjManager::Get_Instance()->m_vecObj.size());
		for (const auto* pObj : CObjManager::Get_Instance()->m_vecObj)
		{
			if (pObj)
			{
				state.vecObjInfo.push_back(new CObj(*pObj)); 
			}
		}
		break;
	case UndoType::GRID:
	{

		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

		CTerrain* pTerrain = pView->m_pTerrain;
		if (pTerrain)
		{
			state.vecGridInfo = pTerrain->m_vecLine;
		}
		break;
	}
	default:
		return;

	}
	m_UndoStack.push_back(move(state));
	if (MAX_UNDO_SIZE < m_UndoStack.size())
	{
		m_UndoStack.pop_front();
	}
	cout << "현재 Undo 스택의 크기 : " << m_UndoStack.size() << endl;
}

void CUndoManager::Undo()
{
	if (m_UndoStack.empty())
	{
		return;
		//throw runtime_error("되돌릴 수 없음");
	}

	EditorState currentState = move(m_UndoStack.back());
	m_UndoStack.pop_back();
	switch (currentState.eType)
	{
	case UndoType::TILE:
	{

		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

		CTerrain* pTerrain = pView->m_pTerrain;
		if (pTerrain)
		{
			pTerrain->m_vecTile = currentState.vecTileInfo;
			pView->Invalidate(FALSE);
		}
		break;
	}
	case UndoType::OBJ:
	{
		// 오브젝트 정보 복원 로직
		auto* pObjManager = CObjManager::Get_Instance();
		pObjManager->Release();
		for (auto* pObj : currentState.vecObjInfo)
		{
			if (pObj)
			{
				pObjManager->m_vecObj.push_back(new CObj(move(*pObj)));
			}
		}
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));
		if (pView && pView->m_pTerrain)
		{
			for (auto& pObj : CObjManager::Get_Instance()->m_vecObj)
			{
				if (pObj)
				{
					pObj->fCameraZoom = pView->m_pTerrain->fCameraZoom;
					pObj->vCameraOffset = pView->m_pTerrain->vCameraOffset;
				}
			}
			pView->Invalidate(FALSE);
		}
		break;
	}
	case UndoType::GRID:
	{

		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

		CTerrain* pTerrain = pView->m_pTerrain;
		if (pTerrain)
		{
			pTerrain->m_vecLine = currentState.vecGridInfo;
		}
		break;
	}
	}
}
