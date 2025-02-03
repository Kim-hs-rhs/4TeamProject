#include "pch.h"
#include "CBSPMap.h"
#include "Include.h"
#include <set>

const vector<array<D3DXVECTOR2, 5>> CBSPMap::Get_Terrain_Grid()
{
	vector<array<D3DXVECTOR2, 5>> vecLines;

	set<pair<int,int>> setLinesPos;
	// 각 방과 복도에 대해 다이아몬드 형태의 라인 생성
	for (const auto& pRoom : m_vecRoom)
	{
		for (int i = 0; i < pRoom->height; i++)
		{
			for (int j = 0; j < pRoom->width; j++)
			{
                int tileX = pRoom->x + j;
                int tileY = pRoom->y + i;

                if (setLinesPos.find({ tileX, tileY }) == setLinesPos.end())
                {
                    setLinesPos.insert({ tileX, tileY });

                    // 화면 좌표 계산
                    float fY = (TILECY / 2.f) * (tileY);
                    float fX = (TILECX * (tileX)) + ((tileY) % 2) * (TILECX / 2.f);
                    float fCX = (TILECX / 2.f);
                    float fCY = (TILECY / 2.f);

                    // 다이아몬드 점 생성
                    array<D3DXVECTOR2, 5> line = {
                        D3DXVECTOR2(fX, fY + fCY),
                        D3DXVECTOR2(fX + fCX, fY),
                        D3DXVECTOR2(fX, fY - fCY),
                        D3DXVECTOR2(fX - fCX, fY),
                        D3DXVECTOR2(fX, fY + fCY)
                    };
                    vecLines.push_back(line);
                }
			}
		}
	}
	return vecLines;
}
void CBSPMap::SetDirection(BSPNode* pNode)
{
	if (pNode->width > pNode->height) // 가로가 세로보다 길다면
        pNode->direction = Direction::VERTICAL;
	else if (pNode->width < pNode->height) // 세로가 가로보다 길다면
        pNode->direction = Direction::HORIZONTAL;
	else // 가로, 세로가 같다면
        pNode->direction = (rand() % 2 == 0) ? Direction::VERTICAL : Direction::HORIZONTAL;
}

void CBSPMap::Generate_Room(int iWidth, int iHeight, int iMinRoomSize)
{
	BSPNode* root = new BSPNode(0, 0, iWidth, iHeight); // 루트 노드 생성
	m_vecBSPNode.push_back(root); // 나중에 해제하기 위해서 벡터에 추가

    RecursiveSplit(root, iMinRoomSize); // 재귀를 통해서 공간 분할
	CreateRoom(root); // 방 생성
	Connect_Room(root); // 방 연결
	Connect_Extra_Rooms(); // 추가적인 방 연결 (연결되지 못한 방들)
}

void CBSPMap::RecursiveSplit(BSPNode* pNode, int iMinRoomSize, int depth)
{
	if (depth >= MAX_DEPTH || // 최대 깊이에 도달하거나
		pNode->width < iMinRoomSize || // 너비가 최소 방 크기보다 작거나
		pNode->height < iMinRoomSize) // 높이가 최소 방 크기보다 작다면
        return;

    // 방의 깊이 설정
    pNode->depth = depth; 
	SetDirection(pNode); // 방향 설정

	int ratio = 30 + (rand() % 41);// 40 ~ 70% 사이의 비율로 나눔

    if (pNode->direction == Direction::VERTICAL)
    {
		int split = (pNode->width * ratio) / 100; // 기존 너비를 비율로 나눔
       
		if (split >= iMinRoomSize && (pNode->width - split) >= iMinRoomSize) // 최소 방 크기보다 크고, 나눠진 방도 최소 방 크기보다 크다면
        {
			pNode->left = new BSPNode(pNode->x, pNode->y, split, pNode->height); // 위치는 그대로 두고  너비만 변경
            pNode->right = new BSPNode(pNode->x + split, pNode->y, pNode->width - split, pNode->height); // x좌표를 변경하고 너비 변경
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
		int margin = min(pNode->width, pNode->height) / 4; // 방의 여백 설정, 더 크게 나누면 여백이 작아져서 방이 커짐


		pRoom->x = pNode->x + margin; // 방의 x좌표 설정 
		pRoom->y = pNode->y + margin; // 방의 y좌표 설정
		pRoom->width = max(4, pNode->width - (margin * 2)); // 좌우 여백을 제외한 방의 너비 설정 (margin x 2가 여백)
		pRoom->height = max(4, pNode->height - (margin * 2)); // 상하 여백을 제외한 방의 높이 설정


		pNode->room = pRoom; // 노드에 방 설정 (자식의 왼쪽 오른쪽이 재귀적으로 불려서 방들이 만들어짐)
		m_vecRoom.push_back(pRoom); // 방 벡터에 방 추가 (나중에 Grid 만들기 위함)
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
			CreateCorridor(pNode->left->room, pNode->right->room); // 복도 생성
        }

        Connect_Room(pNode->left);   // 왼쪽 서브트리 처리
        Connect_Room(pNode->right);  // 오른쪽 서브트리 처리
    }
}

void CBSPMap::Connect_Extra_Rooms()
{
    vector<Room*> unconnected_rooms; // 연결 안된 방들

    // 실제 방(복도가 아닌)만 추가
	for (auto* room : m_vecRoom) // 모든 방에 대해서
    {
        if (room->width > CORRIDOR_WIDTH && room->height > CORRIDOR_WIDTH) // 복도보다 큰 경우만
        {
            unconnected_rooms.push_back(room); // 방으로 생각하고 넣음
        }
    }

    // 모든 방이 연결될 때까지
    while (!unconnected_rooms.empty())
    {
        size_t current_size = unconnected_rooms.size(); // 연결이 안된 방들 수

        // 현재 선택된 방
        Room* current_room = unconnected_rooms[0];
        Room* nearest_room = nullptr;
        float min_distance = FLT_MAX;

        // 가장 가까운 다른 방 찾기
        for (size_t i = 1; i < unconnected_rooms.size(); i++) 
        {
            Room* other_room = unconnected_rooms[i]; // 다른 방 순차 탐색

            // 중심점 거리 계산
            float x1 = current_room->x + current_room->width / 2.0f;
            float y1 = current_room->y + current_room->height / 2.0f;
            float x2 = other_room->x + other_room->width / 2.0f;
            float y2 = other_room->y + other_room->height / 2.0f;

            float distance = sqrt(powf(x2 - x1, 2) + powf(y2 - y1, 2));

			if (distance < min_distance) // 제일 가까운 방 찾기
            {
                min_distance = distance;
                nearest_room = other_room;
            }
        }

        // 가장 가까운 방과 연결
        if (nearest_room)
        {
			CreateCorridor(current_room, nearest_room); // 복도 생성

            // 연결된 방들을 목록에서 제거 ( near은 나중에 또 비교해야하기 때문에 지우지 않음)
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
			min(x1, x2), // 왼쪽 방과 오른쪽 방 중 가장 작은 x좌표 (왼쪽부터 시작)
			y1 - CORRIDOR_WIDTH / 2, // 왼쪽 방의 중심 y좌표에서 복도의 높이의 절반만큼 빼줌 (복도의 중심점으로 정확하게 연결 시키기 위함)
			abs(x2 - x1) + CORRIDOR_WIDTH, // 두 방의 중심 x좌표의 차이 + 복도의 너비
			CORRIDOR_WIDTH // 복도의 높이
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

  /* ExtendCorridor(hCorridor);
    ExtendCorridor(vCorridor);*/
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
