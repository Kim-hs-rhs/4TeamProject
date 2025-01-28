#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	D3DXVECTOR2		vSize;

	D3DXMATRIX		matWorld;

}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;   // �� ��ü

	D3DXIMAGE_INFO			tImgInfo;	// ����ü

}TEXINFO;



typedef struct tagTile
{
	D3DXVECTOR3	vPos;				// Ÿ���� ���� ��ǥ
	D3DXVECTOR2 vSize;				// Ÿ���� ����, ���� ������

	UINT		byOption;			// 0, 1��(��ֹ�)
	UINT		byDrawID;			// �� �� Ÿ�� �̹���
	wstring    wstrStateKey;		// Ÿ�� �̹����� ����Ű
	bool bChange;

	tagTile(): vPos(D3DXVECTOR3()),vSize(D3DXVECTOR2()), byOption(0),byDrawID(0), bChange(false){}
	tagTile(const D3DXVECTOR3& _vPos, UINT _byDrawID, const D3DXVECTOR2& _vSize, 
		const wstring& strStateKey, UINT _byOption = 0)
		: vPos(_vPos), byDrawID(_byDrawID), vSize(_vSize), wstrStateKey(strStateKey),
		byOption(_byOption), bChange(false){}

	friend void to_json(json& j, const tagTile& t) {
		j = json{
			{"vPos", {t.vPos.x, t.vPos.y, t.vPos.z}},
			{"vSize", {t.vSize.x, t.vSize.y}},
			{"byOption", t.byOption},
			{"byDrawID", t.byDrawID},
			{"bChange", t.bChange},
		    {"folder",t.wstrStateKey }
		};
	}

	friend void from_json(const json& j, tagTile& t) {
		j.at("vPos").at(0).get_to(t.vPos.x);
		j.at("vPos").at(1).get_to(t.vPos.y);
		j.at("vPos").at(2).get_to(t.vPos.z);
		j.at("vSize").at(0).get_to(t.vSize.x);
		j.at("vSize").at(1).get_to(t.vSize.y);
		j.at("byOption").get_to(t.byOption);
		j.at("byDrawID").get_to(t.byDrawID);
		j.at("bChange").get_to(t.bChange);
		j.at("folder").get_to(t.wstrStateKey);
	}

	void Serialize(CArchive& ar)
	{
		if (ar.IsStoring()) // ������ ��
		{
			ar << vPos.x << vPos.y << vPos.z;
			ar << vSize.x << vSize.y;
			ar << byOption << byDrawID;
			ar << (wchar_t*)wstrStateKey.c_str();  // wide string ó��
			ar << bChange;
		}
		else // �ҷ��� ��
		{
			ar >> vPos.x >> vPos.y >> vPos.z;
			ar >> vSize.x >> vSize.y;
			ar >> byOption >> byDrawID;
			CString tempStr;  // CString�� ����Ͽ� ����Ű�� ����
			ar >> tempStr;
			wstrStateKey = tempStr.GetString();  // CString�� wstring���� ��ȯ
			ar >> bChange;
		}
	}

}TILE;

typedef	struct tagUnitData
{

	CString strName;
	int		iHp;
	int		iAttack;
	BYTE	byJobIndex;
	BYTE	byItem;

}UNITDATA;

typedef struct tagTexturePath
{
	wstring		wstrObjKey		= L"";
	wstring		wstrStateKey	= L"";
	wstring		wstrPath		= L"";
	int			iCount			= 0;

}IMGPATH;

typedef	struct tagFrame
{
	float		fFrame; // �������� ���� ���� ����
	float		fMax;	// �ִ� �̹����� ������ ��

}FRAME;

