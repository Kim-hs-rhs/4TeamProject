#pragma once

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vRot;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	D3DXVECTOR3		vSize;

	D3DXMATRIX		matWorld;

}INFO;

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;   // 컴 객체

	D3DXIMAGE_INFO			tImgInfo;	// 구조체

}TEXINFO;



typedef struct tagTile
{
	D3DXVECTOR3	vPos;				// 타일의 중점 좌표
	D3DXVECTOR2 vSize;				// 타일의 가로, 세로 사이즈

	UINT		byOption;			// 0, 1번(장애물)
	UINT		byDrawID;			// 몇 번 타일 이미지
	wstring    wstrStateKey;		// 타일 이미지의 상태키
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
		if (ar.IsStoring()) // 저장할 때
		{
			ar << vPos.x << vPos.y << vPos.z;
			ar << vSize.x << vSize.y;
			ar << byOption << byDrawID;
			int strLength = wstrStateKey.length(); // 문자열 길이 저장
			ar << strLength;
			for (wchar_t ch : wstrStateKey)
			{
				ar << ch;
			}
			ar << bChange;
		}
		else // 불러올 때
		{
			ar >> vPos.x >> vPos.y >> vPos.z;
			ar >> vSize.x >> vSize.y;
			ar >> byOption >> byDrawID;
			// wstrStateKey 문자열 처리
			int strLength = 0;
			ar >> strLength; // 문자열 길이 읽기
			wstrStateKey.clear();
			for (int i = 0; i < strLength; ++i)
			{
				wchar_t ch;
				ar >> ch;
				wstrStateKey.push_back(ch);
			}
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
	float		fFrame; // 프레임을 세기 위한 변수
	float		fMax;	// 최대 이미지의 프레임 수

}FRAME;




