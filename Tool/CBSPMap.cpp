#include "pch.h"
#include "CBSPMap.h"

const vector<array<D3DXVECTOR2, 5>> CBSPMap::Get_Terrain_Grid()
{
	vector<array<D3DXVECTOR2, 5>> vecLines;

	// �� ��� ������ ���� ���̾Ƹ�� ������ ���� ����
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

				// ���̾Ƹ�� ������ 5�� �� ����
				array<D3DXVECTOR2, 5> line = {
					D3DXVECTOR2(fX, fY + fCY),        // ���
					D3DXVECTOR2(fX + fCX, fY),        // ����
					D3DXVECTOR2(fX, fY - fCY),        // �ϴ�
					D3DXVECTOR2(fX - fCX, fY),        // ����
					D3DXVECTOR2(fX, fY + fCY)         // �ٽ� ���(���� ����)
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

    RecursiveSplit(root, iMinRoomSize,4);
    CreateRoom(root);
    Connect_Room(root);
    Connect_Extra_Rooms();
}

void CBSPMap::RecursiveSplit(BSPNode* pNode, int iMinRoomSize, int depth)
{
    if (depth >= MAX_DEPTH ||
        pNode->width < iMinRoomSize * 2 ||
        pNode->height < iMinRoomSize * 2)
        return;

    pNode->depth = depth;
    SetDirection(pNode);

    int ratio = 30 + (rand() % 41);  // 30-70 ������ ������

    if (pNode->direction == Direction::VERTICAL)
    {
        int split = (pNode->width * ratio) / 100;
        if (split >= iMinRoomSize && (pNode->width - split) >= iMinRoomSize)
        {
            pNode->left = new BSPNode(pNode->x, pNode->y, split, pNode->height);
            pNode->right = new BSPNode(pNode->x + split, pNode->y, pNode->width - split, pNode->height);
        }
    }
    else
    {
        int split = (pNode->height * ratio) / 100;
        if (split >= iMinRoomSize && (pNode->height - split) >= iMinRoomSize)
        {
            pNode->left = new BSPNode(pNode->x, pNode->y, pNode->width, split);
            pNode->right = new BSPNode(pNode->x, pNode->y + split, pNode->width, pNode->height - split);
        }
    }

    if (pNode->left && pNode->right)
    {
        pNode->left->parent = pNode;
        pNode->right->parent = pNode;
        m_vecBSPNode.push_back(pNode->left);
        m_vecBSPNode.push_back(pNode->right);
        RecursiveSplit(pNode->left, iMinRoomSize, depth + 1);
        RecursiveSplit(pNode->right, iMinRoomSize, depth + 1);
    }
}

void CBSPMap::CreateRoom(BSPNode* pNode)
{
    if (pNode->left == nullptr && pNode->right == nullptr)
    {
        Room* pRoom = new Room;
        int margin = min(pNode->width, pNode->height) / 4;

<<<<<<< Updated upstream
        pRoom->x = pNode->x + margin;
        pRoom->y = pNode->y + margin;
        pRoom->width = max(3, pNode->width - (margin * 2));
        pRoom->height = max(3, pNode->height - (margin * 2));
=======
		pRoom->x = pNode->x + margin; // ���� x��ǥ ����
		pRoom->y = pNode->y + margin; // ���� y��ǥ ����
		pRoom->width = max(4, pNode->width - (margin * 2)); // ���� �ʺ� ����
		pRoom->height = max(4, pNode->height - (margin * 2)); // ���� ���� ����
>>>>>>> Stashed changes

        pNode->room = pRoom;
        m_vecRoom.push_back(pRoom);
    }
    else
    {
        if (pNode->left) CreateRoom(pNode->left);
        if (pNode->right) CreateRoom(pNode->right);
    }
}

void CBSPMap::Connect_Extra_Rooms()
{
    vector<Room*> unconnected_rooms;

    // ���� ��(������ �ƴ�)�� �߰�
    for (auto* room : m_vecRoom)
    {
        if (room->width > CORRIDOR_WIDTH && room->height > CORRIDOR_WIDTH)
        {
            unconnected_rooms.push_back(room);
        }
    }

    // ��� ���� ����� ������
    while (!unconnected_rooms.empty())
    {
        size_t current_size = unconnected_rooms.size();

        // ���� ���õ� ��
        Room* current_room = unconnected_rooms[0];
        Room* nearest_room = nullptr;
        float min_distance = FLT_MAX;

        // ���� ����� �ٸ� �� ã��
        for (size_t i = 1; i < unconnected_rooms.size(); ++i)
        {
            Room* other_room = unconnected_rooms[i];

            // �߽��� �Ÿ� ���
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

        // ���� ����� ��� ����
        if (nearest_room)
        {
            CreateCorridor(current_room, nearest_room);

            // ����� ����� ��Ͽ��� ����
            unconnected_rooms.erase(unconnected_rooms.begin());
        }
        else
        {
            // �� �̻� ������ ���� ������ ���� �� ����
            unconnected_rooms.erase(unconnected_rooms.begin());
        }

        // ������ġ: ũ�Ⱑ ������ ������ ����
        if (current_size == unconnected_rooms.size())
        {
            break;
        }
    }
}

void CBSPMap::CreateCorridor(Room* pLeft, Room* pRight)
{
    int x1 = pLeft->x + pLeft->width / 2;
    int y1 = pLeft->y + pLeft->height / 2;
    int x2 = pRight->x + pRight->width / 2;
    int y2 = pRight->y + pRight->height / 2;

    Room* hCorridor = nullptr;
    Room* vCorridor = nullptr;

    if (rand() % 2 == 0)
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
    else
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

bool CBSPMap::CanExtendCorridor(int x, int y)
{
    // �� ��� üũ
    if (x < 0 || y < 0)
        return false;

    // �ٸ� ���̳� ������ �浹 üũ
    for (const auto& room : m_vecRoom)
    {
        if (x >= room->x && x < room->x + room->width &&
            y >= room->y && y < room->y + room->height)
            return false;
    }

    return true;
}

void CBSPMap::ExtendCorridor(Room* pCorridor)
{
<<<<<<< Updated upstream
    if (pCorridor->height == CORRIDOR_WIDTH)  // ���� ����
    {
        while (CanExtendCorridor(pCorridor->x - 1, pCorridor->y))
        {
            pCorridor->x--;
            pCorridor->width++;
        }
        while (CanExtendCorridor(pCorridor->x + pCorridor->width, pCorridor->y))
        {
            pCorridor->width++;
        }
    }
    else if (pCorridor->width == CORRIDOR_WIDTH)  // ���� ����
    {
        while (CanExtendCorridor(pCorridor->x, pCorridor->y - 1))
        {
            pCorridor->y--;
            pCorridor->height++;
        }
        while (CanExtendCorridor(pCorridor->x, pCorridor->y + pCorridor->height))
        {
            pCorridor->height++;
        }
=======
    const int EXTENSION = 1;

    // ���� ������ ���
    if (pCorridor->width > pCorridor->height)
    {
        pCorridor->x -= EXTENSION;
        pCorridor->width += EXTENSION * 2;  // �������� Ȯ��
    }
    // ���� ������ ���
    else
    {
        pCorridor->y -= EXTENSION;
        pCorridor->height += EXTENSION * 2;  // �������� Ȯ��
>>>>>>> Stashed changes
    }
}

void CBSPMap::Connect_Room(BSPNode* pNode)
{
    // ���� ��尡 �ƴ� ��쿡�� ó��
    if (pNode->left && pNode->right)
    {
        // ���� �ڽ��� ��� ������ �ڽ��� ���� ã��
        Room* pLeftRoom = nullptr;
        Room* pRightRoom = nullptr;

        // ���� �ڽĿ��� �� ã��
        if (pNode->left->room)
            pLeftRoom = pNode->left->room;
        else if (pNode->left->left && pNode->left->right)
            pLeftRoom = pNode->left->left->room ? pNode->left->left->room : pNode->left->right->room;

        // ������ �ڽĿ��� �� ã��
        if (pNode->right->room)
            pRightRoom = pNode->right->room;
        else if (pNode->right->left && pNode->right->right)
            pRightRoom = pNode->right->left->room ? pNode->right->left->room : pNode->right->right->room;

        // �� ���� ��� �����ϸ� ������ ����
        if (pLeftRoom && pRightRoom)
        {
            CreateCorridor(pLeftRoom, pRightRoom);
        }

        // ��������� �ڽ� ���� ó��
        Connect_Room(pNode->left);
        Connect_Room(pNode->right);
    }
}

void CBSPMap::Connect_Extra_Rooms()
{
    const float DISTANCE_THRESHOLD = 10.0f;
    const int CONNECTION_CHANCE = 30;  // 30% Ȯ��

    for (size_t i = 0; i < m_vecRoom.size(); ++i)
    {
        for (size_t j = i + 1; j < m_vecRoom.size(); ++j)
        {
            Room* r1 = m_vecRoom[i];
            Room* r2 = m_vecRoom[j];

            float dist = sqrt(pow(r1->x - r2->x, 2.0f) + pow(r1->y - r2->y, 2.0f));
            if (dist < DISTANCE_THRESHOLD && (rand() % 100 < CONNECTION_CHANCE))
            {
                CreateCorridor(r1, r2);
            }
        }
    }
}