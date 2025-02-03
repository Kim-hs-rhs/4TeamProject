#pragma once
#include "Include.h"
#include "CObjManager.h"

enum class UndoType
{
	TILE,
	OBJ,
	GRID,
	END
};
struct EditorState
{
	UndoType eType;
	vector<TILE> vecTileInfo;
	vector<CObj*> vecObjInfo;
	vector<array<D3DXVECTOR2, 5>> vecGridInfo; 

	EditorState() : eType(UndoType::END) {}
	~EditorState()
	{
		for (auto& pObj : vecObjInfo) 
		{
			Safe_Delete(pObj);
		}
		vecObjInfo.clear();
	}
    EditorState(const EditorState&) = delete;
    EditorState& operator=(const EditorState&) = delete;
    EditorState(EditorState&& other) noexcept
        : eType(other.eType)
        , vecTileInfo(move(other.vecTileInfo))
        , vecObjInfo(move(other.vecObjInfo))
        ,vecGridInfo(move(other.vecGridInfo))
    {
        other.vecObjInfo.clear();  // 소유권 이전 후 원본은 비움
    }
    // 이동 대입 연산자
    EditorState& operator=(EditorState&& other) noexcept
    {
        if (this != &other)
        {
            // 기존 객체들 정리
            for (auto& pObj : vecObjInfo)
            {
                Safe_Delete(pObj);
            }
            vecObjInfo.clear();

            // 이동
            eType = other.eType;
            vecTileInfo = std::move(other.vecTileInfo);
            vecObjInfo = std::move(other.vecObjInfo);

            other.vecObjInfo.clear();  // 소유권 이전 후 원본은 비움
        }
        return *this;
    }
};
class CUndoManager
{
	DECLARE_SINGLETON(CUndoManager)

public:
	void SaveState(const UndoType eUndoType);
	bool CanUndo() const { return !m_UndoStack.empty(); }
	void Undo();
private:
	CUndoManager(){}
	~CUndoManager() = default;
private:
	const size_t MAX_UNDO_SIZE = 60;
	deque<EditorState> m_UndoStack;
};

