#pragma once
#include "CUnitTool.h"
#include "CTextureListBox.h"
#include "CFindFolderTree.h"
#include "CObjManager.h"
#include "Include.h"


// CMyForm 폼 보기

class CMyForm : public CFormView
{
	DECLARE_DYNCREATE(CMyForm)

protected:
	CMyForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CMyForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMyForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
	CFont		m_Font;
public:
	CUnitTool	m_UnitTool;
	void SaveTileData(vector<TILE>& vecTile, const CString& strFolderPath, const CString& strFileName);
	void LoadTileData(vector<TILE>& vecTile, const CString& strFolderPath, const CString& strFileName);
	void SaveMapData(vector<TILE>& vecTile, vector<CObj*>& vecObj, const CString& strFolderPath, const CString& strFileName);
	void LoadMapData(vector<TILE>& vecTile, vector<CObj*>& vecObj, const CString& strFolderPath, const CString& strFileName);
	void OnButtonObjectList();
	void OnButtonTileList();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void OnLbnSelchangeList1();
	// 텍스쳐 매핑 리스트박스
	CTextureListBox m_TextureListBox;
	CFindFolderTree m_FindFolderTree;
	CStatic m_TextrueImage;

	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedCheck1();
};


