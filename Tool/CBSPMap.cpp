#include "pch.h"
#include "CBSPMap.h"
#include "Include.h"
#include <set>

const vector<array<D3DXVECTOR2, 5>> CBSPMap::Get_Terrain_Grid()
{
	vector<array<D3DXVECTOR2, 5>> vecLines;

	set<pair<int,int>> setLinesPos;
	// �� ��� ������ ���� ���̾Ƹ�� ������ ���� ����
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

                    // ȭ�� ��ǥ ���
                    float fY = (TILECY / 2.f) * (tileY);
                    float fX = (TILECX * (tileX)) + ((tileY) % 2) * (TILECX / 2.f);
                    float fCX = (TILECX / 2.f);
                    float fCY = (TILECY / 2.f);

                    // ���̾Ƹ�� �� ����
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
	if (pNode->width > pNode->height) // ���ΰ� ���κ��� ��ٸ�
        pNode->direction = Direction::VERTICAL;
	else if (pNode->width < pNode->height) // ���ΰ� ���κ��� ��ٸ�
        pNode->direction = Direction::HORIZONTAL;
	else // ����, ���ΰ� ���ٸ�
        pNode->direction = (rand() % 2 == 0) ? Direction::VERTICAL : Direction::HORIZONTAL;
}

void CBSPMap::Generate_Room(int iWidth, int iHeight, int iMinRoomSize)
{
	BSPNode* root = new BSPNode(0, 0, iWidth, iHeight); // ��Ʈ ��� ����
	m_vecBSPNode.push_back(root); // ���߿� �����ϱ� ���ؼ� ���Ϳ� �߰�

    RecursiveSplit(root, iMinRoomSize); // ��͸� ���ؼ� ���� ����
	CreateRoom(root); // �� ����
	Connect_Room(root); // �� ����
	Connect_Extra_Rooms(); // �߰����� �� ���� (������� ���� ���)
}

void CBSPMap::RecursiveSplit(BSPNode* pNode, int iMinRoomSize, int depth)
{
	if (depth >= MAX_DEPTH || // �ִ� ���̿� �����ϰų�
		pNode->width < iMinRoomSize || // �ʺ� �ּ� �� ũ�⺸�� �۰ų�
		pNode->height < iMinRoomSize) // ���̰� �ּ� �� ũ�⺸�� �۴ٸ�
        return;

    // ���� ���� ����
    pNode->depth = depth; 
	SetDirection(pNode); // ���� ����

	int ratio = 30 + (rand() % 41);// 40 ~ 70% ������ ������ ����

    if (pNode->direction == Direction::VERTICAL)
    {
		int split = (pNode->width * ratio) / 100; // ���� �ʺ� ������ ����
       
		if (split >= iMinRoomSize && (pNode->width - split) >= iMinRoomSize) // �ּ� �� ũ�⺸�� ũ��, ������ �浵 �ּ� �� ũ�⺸�� ũ�ٸ�
        {
			pNode->left = new BSPNode(pNode->x, pNode->y, split, pNode->height); // ��ġ�� �״�� �ΰ�  �ʺ� ����
            pNode->right = new BSPNode(pNode->x + split, pNode->y, pNode->width - split, pNode->height); // x��ǥ�� �����ϰ� �ʺ� ����
        }
    }
    else
    {
        int split = (pNode->height * ratio) / 100;
		if (split >= iMinRoomSize && (pNode->height - split) >= iMinRoomSize) // �ּ� �� ũ�⺸�� ũ��, ������ �浵 �ּ� �� ũ�⺸�� ũ�ٸ�
        {
            pNode->left = new BSPNode(pNode->x, pNode->y, pNode->width, split);
            pNode->right = new BSPNode(pNode->x, pNode->y + split, pNode->width, pNode->height - split);
        }
    }

	if (pNode->left && pNode->right) // ����, ������ �ڽ��� ��� �ִٸ�
    {
        pNode->left->parent = pNode; 
        pNode->right->parent = pNode;
        m_vecBSPNode.push_back(pNode->left); 
        m_vecBSPNode.push_back(pNode->right);
		RecursiveSplit(pNode->left, iMinRoomSize, depth + 1); // ���� �ڽ����� ��� ȣ��
		RecursiveSplit(pNode->right, iMinRoomSize, depth + 1); // ������ �ڽ����� ��� ȣ��
    }
}

void CBSPMap::CreateRoom(BSPNode* pNode)
{
	if (pNode->left == nullptr && pNode->right == nullptr) // ���� �����
    {
        Room* pRoom = new Room;
		int margin = min(pNode->width, pNode->height) / 4; // ���� ���� ����, �� ũ�� ������ ������ �۾����� ���� Ŀ��


		pRoom->x = pNode->x + margin; // ���� x��ǥ ���� 
		pRoom->y = pNode->y + margin; // ���� y��ǥ ����
		pRoom->width = max(4, pNode->width - (margin * 2)); // �¿� ������ ������ ���� �ʺ� ���� (margin x 2�� ����)
		pRoom->height = max(4, pNode->height - (margin * 2)); // ���� ������ ������ ���� ���� ����


		pNode->room = pRoom; // ��忡 �� ���� (�ڽ��� ���� �������� ��������� �ҷ��� ����� �������)
		m_vecRoom.push_back(pRoom); // �� ���Ϳ� �� �߰� (���߿� Grid ����� ����)
    }
    else
    {
		if (pNode->left) CreateRoom(pNode->left);   // ���� �ڽ��� �ִٸ� ���� �ڽ����� ��� ȣ��
		if (pNode->right) CreateRoom(pNode->right); // ������ �ڽ��� �ִٸ� ������ �ڽ����� ��� ȣ��
    }
}

void CBSPMap::Connect_Room(BSPNode* pNode)
{
    if (pNode->left && pNode->right) // �ڽ� ��尡 �ִ� ���
    {
        // ���� �ڽ��� ��� ������ �ڽ��� ���� ����
        if (pNode->left->room && pNode->right->room)
        {
			CreateCorridor(pNode->left->room, pNode->right->room); // ���� ����
        }

        Connect_Room(pNode->left);   // ���� ����Ʈ�� ó��
        Connect_Room(pNode->right);  // ������ ����Ʈ�� ó��
    }
}

void CBSPMap::Connect_Extra_Rooms()
{
    vector<Room*> unconnected_rooms; // ���� �ȵ� ���

    // ���� ��(������ �ƴ�)�� �߰�
	for (auto* room : m_vecRoom) // ��� �濡 ���ؼ�
    {
        if (room->width > CORRIDOR_WIDTH && room->height > CORRIDOR_WIDTH) // �������� ū ��츸
        {
            unconnected_rooms.push_back(room); // ������ �����ϰ� ����
        }
    }

    // ��� ���� ����� ������
    while (!unconnected_rooms.empty())
    {
        size_t current_size = unconnected_rooms.size(); // ������ �ȵ� ��� ��

        // ���� ���õ� ��
        Room* current_room = unconnected_rooms[0];
        Room* nearest_room = nullptr;
        float min_distance = FLT_MAX;

        // ���� ����� �ٸ� �� ã��
        for (size_t i = 1; i < unconnected_rooms.size(); i++) 
        {
            Room* other_room = unconnected_rooms[i]; // �ٸ� �� ���� Ž��

            // �߽��� �Ÿ� ���
            float x1 = current_room->x + current_room->width / 2.0f;
            float y1 = current_room->y + current_room->height / 2.0f;
            float x2 = other_room->x + other_room->width / 2.0f;
            float y2 = other_room->y + other_room->height / 2.0f;

            float distance = sqrt(powf(x2 - x1, 2) + powf(y2 - y1, 2));

			if (distance < min_distance) // ���� ����� �� ã��
            {
                min_distance = distance;
                nearest_room = other_room;
            }
        }

        // ���� ����� ��� ����
        if (nearest_room)
        {
			CreateCorridor(current_room, nearest_room); // ���� ����

            // ����� ����� ��Ͽ��� ���� ( near�� ���߿� �� ���ؾ��ϱ� ������ ������ ����)
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
	int x1 = pLeft->x + pLeft->width / 2; // ���� ���� �߽� x��ǥ
	int y1 = pLeft->y + pLeft->height / 2; // ���� ���� �߽� y��ǥ
	int x2 = pRight->x + pRight->width / 2; // ������ ���� �߽� x��ǥ
	int y2 = pRight->y + pRight->height / 2; // ������ ���� �߽� y��ǥ

    Room* hCorridor = nullptr;
    Room* vCorridor = nullptr;

	if (rand() % 2 == 0)    // 50% Ȯ���� ���� ���� ����
    {
        hCorridor = new Room{
			min(x1, x2), // ���� ��� ������ �� �� ���� ���� x��ǥ (���ʺ��� ����)
			y1 - CORRIDOR_WIDTH / 2, // ���� ���� �߽� y��ǥ���� ������ ������ ���ݸ�ŭ ���� (������ �߽������� ��Ȯ�ϰ� ���� ��Ű�� ����)
			abs(x2 - x1) + CORRIDOR_WIDTH, // �� ���� �߽� x��ǥ�� ���� + ������ �ʺ�
			CORRIDOR_WIDTH // ������ ����
        };

        vCorridor = new Room{
            x2 - CORRIDOR_WIDTH / 2,
            min(y1, y2),
            CORRIDOR_WIDTH,
            abs(y2 - y1) + CORRIDOR_WIDTH
        };
    }
	else // 50% Ȯ���� ���� ���� ����
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
    }

}
