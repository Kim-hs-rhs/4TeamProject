#pragma once
#include "Include.h"
#include "CObjManager.h"

enum class UndoType
{
	TILE,
	OBJ,
	END
};
struct EditorState
{
	UndoType eType;
	vector<TILE> vecTileInfo;
	vector<CObj*> vecObjInfo;

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
        , vecTileInfo(std::move(other.vecTileInfo))
        , vecObjInfo(std::move(other.vecObjInfo))
    {
        other.vecObjInfo.clear();  // ������ ���� �� ������ ���
    }
    // �̵� ���� ������
    EditorState& operator=(EditorState&& other) noexcept
    {
        if (this != &other)
        {
            // ���� ��ü�� ����
            for (auto& pObj : vecObjInfo)
            {
                Safe_Delete(pObj);
            }
            vecObjInfo.clear();

            // �̵�
            eType = other.eType;
            vecTileInfo = std::move(other.vecTileInfo);
            vecObjInfo = std::move(other.vecObjInfo);

            other.vecObjInfo.clear();  // ������ ���� �� ������ ���
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
	const size_t MAX_UNDO_SIZE = 40;
	deque<EditorState> m_UndoStack;
};

