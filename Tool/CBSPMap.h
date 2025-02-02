#pragma once
#include "Include.h"
  

class CBSPMap
{
private:
    enum class Direction { VERTICAL, HORIZONTAL,END };

    struct Room
    {
        int x, y;
        int width, height;
    };

    struct BSPNode
    {
        BSPNode* parent;
        BSPNode* left;
        BSPNode* right;
        Room* room;
        int x, y, width, height;
        Direction direction;
        int depth;

        BSPNode(int _x = 0, int _y = 0, int _w = 0, int _h = 0)
            : parent(nullptr), left(nullptr), right(nullptr), room(nullptr),
			x(_x), y(_y), width(_w), height(_h), depth(0), direction(Direction::END)
        {
        }
    };
public:
	~CBSPMap()
	{
		for (auto& pNode : m_vecBSPNode)
		{
			delete pNode;
			pNode = nullptr;
		}
		for (auto& pRoom : m_vecRoom)
		{
			delete pRoom;
			pRoom = nullptr;
		}
	}
public:
    const vector<array<D3DXVECTOR2, 5>> Get_Terrain_Grid();
    void Generate_Room(int iWidth, int iHeight, int iMinRoomSize);

private:
    void SetDirection(BSPNode* pNode);
    void RecursiveSplit(BSPNode* pNode, int iMinRoomSize, int depth = 0);
    void CreateRoom(BSPNode* pNode);
    void Connect_Room(BSPNode* pNode);
    void Connect_Extra_Rooms();
    void CreateCorridor(Room* pLeft, Room* pRight);
    void ExtendCorridor(Room* pCorridor);
    bool CanExtendCorridor(int x, int y);
    void Connect_Extra_Rooms();

private:
    vector<BSPNode*> m_vecBSPNode;
    vector<Room*> m_vecRoom;
    const int MAX_DEPTH = 3;
    const int CORRIDOR_WIDTH = 2;
};

