#include "pch.h"
#include "CBSPMap.h"
#include "Include.h"

const vector<array<D3DXVECTOR2, 5>> CBSPMap::Get_Terrain_Grid()
{
	vector<array<D3DXVECTOR2, 5>> vecLines;

	// 각 방과 복도에 대해 다이아몬드 형태의 라인 생성
	for (const auto& pRoom : m_vecRoom)
	{
		for (int i = 0; i < pRoom->height; ++i)
		{
			for (int j = 0; j < pRoom->width; ++j)
			{
                float fY = (TILECY / 2.f) * (pRoom->y + i);
                float fX = (TILECX * (pRoom->x + j)) + ((pRoom->y + i) % 2) * (TILECX / 2.f);
				float fCX = (TILECX / 2.f);
				float fCY = (TILECY / 2.f);

				// 다이아몬드 형태의 5개 점 생성
				array<D3DXVECTOR2, 5> line = {
					D3DXVECTOR2(fX, fY + fCY),        // 상단
					D3DXVECTOR2(fX + fCX, fY),        // 우측
					D3DXVECTOR2(fX, fY - fCY),        // 하단
					D3DXVECTOR2(fX - fCX, fY),        // 좌측
					D3DXVECTOR2(fX, fY + fCY)         // 다시 상단(닫힌 형태)
				};

				vecLines.push_back(line);
			}
		}
	}
	return vecLines;
}
void CBSPMap::SetDirection(BSPNode* pNode)
{
    if (pNode->width > pNode->height)
        pNode->direction = Direction::VERTICAL;
    else if (pNode->width < pNode->height)
        pNode->direction = Direction::HORIZONTAL;
    else
        pNode->direction = (rand() % 2 == 0) ? Direction::VERTICAL : Direction::HORIZONTAL;
}

void CBSPMap::Generate_Room(int iWidth, int iHeight, int iMinRoomSize)
{
    BSPNode* root = new BSPNode(0, 0, iWidth, iHeight);
    m_vecBSPNode.push_back(root);

    RecursiveSplit(root, iMinRoomSize);
    CreateRoom(root);
    Connect_Room(root);
    Connect_Extra_Rooms();
}

void CBSPMap::RecursiveSplit(BSPNode* pNode, int iMinRoomSize, int depth)
{
    if (depth >= MAX_DEPTH ||
        pNode->width < iMinRoomSize  ||
        pNode->height < iMinRoomSize)
        return;

    // 방의 깊이 설정
    pNode->depth = depth;
    SetDirection(pNode);

    int ratio = 30 + (rand() % 41);  // 30-70 사이의 랜덤값

    if (pNode->direction == Direction::VERTICAL)
    {
        int split = (pNode->width * ratio) / 100;
       
		if (split >= iMinRoomSize && (pNode->width - split) >= iMinRoomSize) // 최소 방 크기보다 크고, 나눠진 방도 최소 방 크기보다 크다면
        {
            pNode->left = new BSPNode(pNode->x, pNode->y, split, pNode->height);
            pNode->right = new BSPNode(pNode->x + split, pNode->y, pNode->width - split, pNode->height);
        }
    }
    else
    {
        int split = (pNode->height * ratio) / 100;
		if (split >= iMinRoomSize && (pNode->height - split) >= iMinRoomSize) // 최소 방 크기보다 크고, 나눠진 방도 최소 방 크기보다 크다면
        {
            pNode->left = new BSPNode(pNode->x, pNode->y, pNode->width, split);
            pNode->right = new BSPNode(pNode->x, pNode->y + split, pNode->width, pNode->height - split);
        }
    }

	if (pNode->left && pNode->right) // 왼쪽, 오른쪽 자식이 모두 있다면
    {
        pNode->left->parent = pNode; 
        pNode->right->parent = pNode;
        m_vecBSPNode.push_back(pNode->left); 
        m_vecBSPNode.push_back(pNode->right);
		RecursiveSplit(pNode->left, iMinRoomSize, depth + 1); // 왼쪽 자식으로 재귀 호출
		RecursiveSplit(pNode->right, iMinRoomSize, depth + 1); // 오른쪽 자식으로 재귀 호출
    }
}

void CBSPMap::CreateRoom(BSPNode* pNode)
{
	if (pNode->left == nullptr && pNode->right == nullptr) // 리프 노드라면
    {
        Room* pRoom = new Room;
		int margin = min(pNode->width, pNode->height) / 4; // 방의 여백 설정


		pRoom->x = pNode->x + margin; // 방의 x좌표 설정
		pRoom->y = pNode->y + margin; // 방의 y좌표 설정
		pRoom->width = max(3, pNode->width - (margin * 2)); // 방의 너비 설정
		pRoom->height = max(3, pNode->height - (margin * 2)); // 방의 높이 설정


		pNode->room = pRoom; // 노드에 방 설정
		m_vecRoom.push_back(pRoom); // 방 벡터에 방 추가
    }
    else
    {
		if (pNode->left) CreateRoom(pNode->left);   // 왼쪽 자식이 있다면 왼쪽 자식으로 재귀 호출
		if (pNode->right) CreateRoom(pNode->right); // 오른쪽 자식이 있다면 오른쪽 자식으로 재귀 호출
    }
}

void CBSPMap::Connect_Room(BSPNode* pNode)
{
    if (pNode->left && pNode->right) // 자식 노드가 있는 경우
    {
        // 왼쪽 자식의 방과 오른쪽 자식의 방을 연결
        if (pNode->left->room && pNode->right->room)
        {
            CreateCorridor(pNode->left->room, pNode->right->room);
        }

        Connect_Room(pNode->left);   // 왼쪽 서브트리 처리
        Connect_Room(pNode->right);  // 오른쪽 서브트리 처리
    }
}

void CBSPMap::Connect_Extra_Rooms()
{
    vector<Room*> unconnected_rooms;

    // 실제 방(복도가 아닌)만 추가
    for (auto* room : m_vecRoom)
    {
        if (room->width > CORRIDOR_WIDTH && room->height > CORRIDOR_WIDTH)
        {
            unconnected_rooms.push_back(room);
        }
    }

    // 모든 방이 연결될 때까지
    while (!unconnected_rooms.empty())
    {
        size_t current_size = unconnected_rooms.size();

        // 현재 선택된 방
        Room* current_room = unconnected_rooms[0];
        Room* nearest_room = nullptr;
        float min_distance = FLT_MAX;

        // 가장 가까운 다른 방 찾기
        for (size_t i = 1; i < unconnected_rooms.size(); ++i)
        {
            Room* other_room = unconnected_rooms[i];

            // 중심점 거리 계산
            float x1 = current_room->x + current_room->width / 2.0f;
            float y1 = current_room->y + current_room->height / 2.0f;
            float x2 = other_room->x + other_room->width / 2.0f;
            float y2 = other_room->y + other_room->height / 2.0f;

            float distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

            if (distance < min_distance)
            {
                min_distance = distance;
                nearest_room = other_room;
            }
        }

        // 가장 가까운 방과 연결
        if (nearest_room)
        {
            CreateCorridor(current_room, nearest_room);

            // 연결된 방들을 목록에서 제거
            unconnected_rooms.erase(unconnected_rooms.begin());
        }
        else
        {
            // 더 이상 연결할 방이 없으면 현재 방 제거
            unconnected_rooms.erase(unconnected_rooms.begin());
        }

        // 안전장치: 크기가 변하지 않으면 종료
        if (current_size == unconnected_rooms.size())
        {
            break;
        }
    }
}

void CBSPMap::CreateCorridor(Room* pLeft, Room* pRight)
{
	int x1 = pLeft->x + pLeft->width / 2; // 왼쪽 방의 중심 x좌표
	int y1 = pLeft->y + pLeft->height / 2; // 왼쪽 방의 중심 y좌표
	int x2 = pRight->x + pRight->width / 2; // 오른쪽 방의 중심 x좌표
	int y2 = pRight->y + pRight->height / 2; // 오른쪽 방의 중심 y좌표

    Room* hCorridor = nullptr;
    Room* vCorridor = nullptr;

	if (rand() % 2 == 0)    // 50% 확률로 수평 복도 생성
    {
        hCorridor = new Room{
            min(x1, x2),
            y1 - CORRIDOR_WIDTH / 2,
            abs(x2 - x1) + CORRIDOR_WIDTH,
            CORRIDOR_WIDTH
        };

        vCorridor = new Room{
            x2 - CORRIDOR_WIDTH / 2,
            min(y1, y2),
            CORRIDOR_WIDTH,
            abs(y2 - y1) + CORRIDOR_WIDTH
        };
    }
	else // 50% 확률로 수직 복도 생성
    {   
        vCorridor = new Room{   
            x1 - CORRIDOR_WIDTH / 2,
            min(y1, y2),
            CORRIDOR_WIDTH,
            abs(y2 - y1) + CORRIDOR_WIDTH
        };  

        hCorridor = new Room{
            min(x1, x2),
            y2 - CORRIDOR_WIDTH / 2,
            abs(x2 - x1) + CORRIDOR_WIDTH,
            CORRIDOR_WIDTH
        };
    }

    m_vecRoom.push_back(hCorridor);
    m_vecRoom.push_back(vCorridor);

    ExtendCorridor(hCorridor);
    ExtendCorridor(vCorridor);
}

void CBSPMap::ExtendCorridor(Room* pCorridor)
{

    const int EXTENSION = 1;

    // 수평 복도인 경우
    if (pCorridor->width > pCorridor->height)
    {
        pCorridor->x -= EXTENSION;
        pCorridor->width += EXTENSION * 2;  // 양쪽으로 확장
    }
    // 수직 복도인 경우
    else
    {
        pCorridor->y -= EXTENSION;
        pCorridor->height += EXTENSION * 2;  // 양쪽으로 확장
    }

}
