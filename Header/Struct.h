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

	BYTE		byOption;			// 0, 1��(��ֹ�)
	BYTE		byDrawID;			// �� �� Ÿ�� �̹���
	bool bChange;
	tagTile(): vPos(D3DXVECTOR3()),vSize(D3DXVECTOR2()), byOption(0),byDrawID(0), bChange(false){}
	tagTile(const D3DXVECTOR3& _vPos, BYTE _byDrawID, const D3DXVECTOR2& _vSize, BYTE _byOption = 0)
		: vPos(_vPos), byDrawID(_byDrawID), vSize(_vSize), byOption(_byOption), bChange(false){}
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


