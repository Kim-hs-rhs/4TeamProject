#pragma once

#include "CObjManager.h"
#include "CObj.h"

// CTransformInfo 폼 보기

class CTransformInfo : public CFormView
{
	DECLARE_DYNCREATE(CTransformInfo)

protected:
	CTransformInfo();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTransformInfo();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTransformInfo };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin5(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin6(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin7(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin8(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin9(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnEnChangeEdit8();
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnEnChangeEdit11();

public:
	void Set_TransformSpin(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vRot, const D3DXVECTOR3& vScale);
	void Get_SelectedObj(CObj* pObj) { m_pObj = pObj; }

public:
	bool m_bStart[9];
	CSpinButtonCtrl m_SpinCtrl[9];
	D3DXVECTOR3 m_vSpinValue[3];
	CObj* m_pObj;
};


