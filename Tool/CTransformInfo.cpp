﻿// CTransformInfo.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CTransformInfo.h"
#include "CUndoManager.h"
#include "CObjManager.h"


// CTransformInfo

IMPLEMENT_DYNCREATE(CTransformInfo, CFormView)

CTransformInfo::CTransformInfo()
	: CFormView(IDD_CTransformInfo), m_pObj(nullptr), m_bMultiSelected(false)
{
	ZeroMemory(&m_vSpinValue, sizeof(D3DXVECTOR3) * 3);
}

CTransformInfo::~CTransformInfo()
{
}

void CTransformInfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SPIN1, m_SpinCtrl[0]);
	DDX_Control(pDX, IDC_SPIN2, m_SpinCtrl[1]);
	DDX_Control(pDX, IDC_SPIN3, m_SpinCtrl[2]);
	DDX_Control(pDX, IDC_SPIN4, m_SpinCtrl[3]);
	DDX_Control(pDX, IDC_SPIN5, m_SpinCtrl[4]);
	DDX_Control(pDX, IDC_SPIN6, m_SpinCtrl[5]);
	DDX_Control(pDX, IDC_SPIN7, m_SpinCtrl[6]);
	DDX_Control(pDX, IDC_SPIN8, m_SpinCtrl[7]);
	DDX_Control(pDX, IDC_SPIN9, m_SpinCtrl[8]);

	DDX_Text(pDX, IDC_EDIT3, m_vSpinValue[0].x);
	DDX_Text(pDX, IDC_EDIT4, m_vSpinValue[0].y);
	DDX_Text(pDX, IDC_EDIT5, m_vSpinValue[0].z);
	DDX_Text(pDX, IDC_EDIT6, m_vSpinValue[1].x);
	DDX_Text(pDX, IDC_EDIT7, m_vSpinValue[1].y);
	DDX_Text(pDX, IDC_EDIT8, m_vSpinValue[1].z);
	DDX_Text(pDX, IDC_EDIT9, m_vSpinValue[2].x);
	DDX_Text(pDX, IDC_EDIT10, m_vSpinValue[2].y);
	DDX_Text(pDX, IDC_EDIT11, m_vSpinValue[2].z);
}

BEGIN_MESSAGE_MAP(CTransformInfo, CFormView)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CTransformInfo::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CTransformInfo::OnDeltaposSpin2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CTransformInfo::OnDeltaposSpin3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CTransformInfo::OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CTransformInfo::OnDeltaposSpin5)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CTransformInfo::OnDeltaposSpin6)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CTransformInfo::OnDeltaposSpin7)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN8, &CTransformInfo::OnDeltaposSpin8)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN9, &CTransformInfo::OnDeltaposSpin9)
	ON_EN_CHANGE(IDC_EDIT3, &CTransformInfo::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &CTransformInfo::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CTransformInfo::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &CTransformInfo::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, &CTransformInfo::OnEnChangeEdit7)
	ON_EN_CHANGE(IDC_EDIT8, &CTransformInfo::OnEnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT9, &CTransformInfo::OnEnChangeEdit9)
	ON_EN_CHANGE(IDC_EDIT10, &CTransformInfo::OnEnChangeEdit10)
	ON_EN_CHANGE(IDC_EDIT11, &CTransformInfo::OnEnChangeEdit11)
END_MESSAGE_MAP()


// CTransformInfo 진단

#ifdef _DEBUG
void CTransformInfo::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTransformInfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTransformInfo 메시지 처리기


void CTransformInfo::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
		SetScrollSizes(MM_TEXT, CSize(0, 0));

		for (int i = 0; i < 9; ++i)
		{
			int scale = 10;
			m_SpinCtrl[i].SetRange32(0, 800 * scale);  // 최소값 0, 최대값 100
			m_SpinCtrl[i].SetPos32(0);  // 초기값 설정
		}
		
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CTransformInfo::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	

    if (m_vecSelectedObj.empty())
    {
	if (nullptr != m_pObj)
	{// 스핀 버튼의 값이 변경될 때 호출되는 함수
        m_vSpinValue[0].x += pNMUpDown->iDelta; // 현재 값을 가져옴
		m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
	}
    }
    else
    {
        bool bSamePos = true;
        float firstX = m_vecSelectedObj[0]->m_tInfo.vPos.x;

        // 모든 객체가 같은 x좌표를 가지고 있는지 체크
        for (size_t i = 1; i < m_vecSelectedObj.size(); ++i)
        {
            if (m_vecSelectedObj[i]->m_tInfo.vPos.x != firstX)
            {
                bSamePos = false;
                break;
            }
        }

        if (bSamePos)
        {
            if (m_vecSelectedObj.size() == (size_t)1)
            {
                Set_TransformSpin(m_vecSelectedObj.back()->m_tInfo.vPos, m_vecSelectedObj.back()->m_tInfo.vRot, m_vecSelectedObj.back()->m_tInfo.vSize);
            }
            m_vSpinValue[0].x += pNMUpDown->iDelta;
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vPos.x = m_vSpinValue[0].x;
                }
            }
        }
        else
        {
            // 위치가 다르면 상대적으로 이동
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vPos.x += pNMUpDown->iDelta;
                }
            }
            CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
            // UI에는 "-" 표시
            SetDlgItemText(IDC_EDIT3, L"-");
            *pResult = 0;
            return;
        }
    }
	
	CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    if (m_vecSelectedObj.empty())
    {
        if (nullptr != m_pObj)
        {// 스핀 버튼의 값이 변경될 때 호출되는 함수
            m_vSpinValue[0].y -= pNMUpDown->iDelta;
            m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
        }
    }
    else
    {
        bool bSamePos = true;
        float firstY = m_vecSelectedObj[0]->m_tInfo.vPos.y;

        for (size_t i = 1; i < m_vecSelectedObj.size(); ++i)
        {
            if (m_vecSelectedObj[i]->m_tInfo.vPos.y != firstY)
            {
                bSamePos = false;
                break;
            }
        }

        if (bSamePos)
        {
            if (m_vecSelectedObj.size() == (size_t)1)
            {
                Set_TransformSpin(m_vecSelectedObj.back()->m_tInfo.vPos, m_vecSelectedObj.back()->m_tInfo.vRot, m_vecSelectedObj.back()->m_tInfo.vSize);
            }
            m_vSpinValue[0].y -= pNMUpDown->iDelta;
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vPos.y -= pNMUpDown->iDelta;
                }
            }
        }
        else
        {
            // 위치가 다르면 상대적으로 이동
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vPos.y -= pNMUpDown->iDelta;
                }
            }
            CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
            // UI에는 "-" 표시
            SetDlgItemText(IDC_EDIT4, L"-");
            *pResult = 0;
            return;
        }
    }

    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult) // Position Z
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if (m_vecSelectedObj.empty())
    {
        if (nullptr != m_pObj)
        {
            m_vSpinValue[0].z += pNMUpDown->iDelta;
            m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
        }
    }
    else
    {
        bool bSamePos = true;
        float firstZ = m_vecSelectedObj[0]->m_tInfo.vPos.z;

        for (size_t i = 1; i < m_vecSelectedObj.size(); ++i)
        {
            if (m_vecSelectedObj[i]->m_tInfo.vPos.z != firstZ)
            {
                bSamePos = false;
                break;
            }
        }

        if (bSamePos)
        {
            if (m_vecSelectedObj.size() == (size_t)1)
            {
                Set_TransformSpin(m_vecSelectedObj.back()->m_tInfo.vPos, m_vecSelectedObj.back()->m_tInfo.vRot, m_vecSelectedObj.back()->m_tInfo.vSize);
            }
            m_vSpinValue[0].z += pNMUpDown->iDelta;
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vPos.z = m_vSpinValue[0].z;
                }
            }
        }
        else
        {
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vPos.z += pNMUpDown->iDelta;
                }
            }
            CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
            SetDlgItemText(IDC_EDIT5, L"-");
            *pResult = 0;
            return;
        }
    }

    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
    UpdateData(FALSE);
    *pResult = 0;
}

void CTransformInfo::OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult) // Rotation X
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if (m_vecSelectedObj.empty())
    {
        if (nullptr != m_pObj)
        {
            m_vSpinValue[1].x += pNMUpDown->iDelta;
            m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
        }
    }
    else
    {
        bool bSameRot = true;
        float firstRotX = m_vecSelectedObj[0]->m_tInfo.vRot.x;

        for (size_t i = 1; i < m_vecSelectedObj.size(); ++i)
        {
            if (m_vecSelectedObj[i]->m_tInfo.vRot.x != firstRotX)
            {
                bSameRot = false;
                break;
            }
        }

        if (bSameRot)
        {
            if (m_vecSelectedObj.size() == (size_t)1)
            {
                Set_TransformSpin(m_vecSelectedObj.back()->m_tInfo.vPos, m_vecSelectedObj.back()->m_tInfo.vRot, m_vecSelectedObj.back()->m_tInfo.vSize);
            }
            m_vSpinValue[1].x += pNMUpDown->iDelta;
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vRot.x += pNMUpDown->iDelta;
                }
            }
        }
        else
        {
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vRot.x += pNMUpDown->iDelta;
                }
            }
            CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
            SetDlgItemText(IDC_EDIT6, L"-");
            *pResult = 0;
            return;
        }
    }

    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
    UpdateData(FALSE);
    *pResult = 0;
}

void CTransformInfo::OnDeltaposSpin5(NMHDR* pNMHDR, LRESULT* pResult) // Rotation Y
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if (m_vecSelectedObj.empty())
    {
        if (nullptr != m_pObj)
        {
            m_vSpinValue[1].y += pNMUpDown->iDelta;
            m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
        }
    }
    else
    {
        bool bSameRot = true;
        float firstRotY = m_vecSelectedObj[0]->m_tInfo.vRot.y;

        for (size_t i = 1; i < m_vecSelectedObj.size(); ++i)
        {
            if (m_vecSelectedObj[i]->m_tInfo.vRot.y != firstRotY)
            {
                bSameRot = false;
                break;
            }
        }

        if (bSameRot)
        {
            if (m_vecSelectedObj.size() == (size_t)1)
            {
                Set_TransformSpin(m_vecSelectedObj.back()->m_tInfo.vPos, m_vecSelectedObj.back()->m_tInfo.vRot, m_vecSelectedObj.back()->m_tInfo.vSize);
            }
            m_vSpinValue[1].y += pNMUpDown->iDelta;
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vRot.y += pNMUpDown->iDelta;
                }
            }
        }
        else
        {
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vRot.y += pNMUpDown->iDelta;
                }
            }
            CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
            SetDlgItemText(IDC_EDIT7, L"-");
            *pResult = 0;
            return;
        }
    }

    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
    UpdateData(FALSE);
    *pResult = 0;
}

void CTransformInfo::OnDeltaposSpin6(NMHDR* pNMHDR, LRESULT* pResult) // Rotation Z
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if (m_vecSelectedObj.empty())
    {
        if (nullptr != m_pObj)
        {
            m_vSpinValue[1].z += pNMUpDown->iDelta;
            m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
        }
    }
    else
    {
        bool bSameRot = true;
        float firstRotZ = m_vecSelectedObj[0]->m_tInfo.vRot.z;

        for (size_t i = 1; i < m_vecSelectedObj.size(); ++i)
        {
            if (m_vecSelectedObj[i]->m_tInfo.vRot.z != firstRotZ)
            {
                bSameRot = false;
                break;
            }
        }

        if (bSameRot)
        {
            if (m_vecSelectedObj.size() == (size_t)1)
            {
                Set_TransformSpin(m_vecSelectedObj.back()->m_tInfo.vPos, m_vecSelectedObj.back()->m_tInfo.vRot, m_vecSelectedObj.back()->m_tInfo.vSize);
            }
            m_vSpinValue[1].z += pNMUpDown->iDelta;
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vRot.z += pNMUpDown->iDelta;
                }
            }
        }
        else
        {
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vRot.z += pNMUpDown->iDelta;
                }
            }
            CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
            SetDlgItemText(IDC_EDIT8, L"-");
            *pResult = 0;
            return;
        }
    }

    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
    UpdateData(FALSE);
    *pResult = 0;
}


void CTransformInfo::OnDeltaposSpin7(NMHDR* pNMHDR, LRESULT* pResult) // Scale X
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if (m_vecSelectedObj.empty())
    {
        if (nullptr != m_pObj)
        {
            m_vSpinValue[2].x += pNMUpDown->iDelta;
            m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
        }
    }
    else
    {
        bool bSameScale = true;
        float firstScaleX = m_vecSelectedObj[0]->m_tInfo.vSize.x;

        for (size_t i = 1; i < m_vecSelectedObj.size(); ++i)
        {
            if (m_vecSelectedObj[i]->m_tInfo.vSize.x != firstScaleX)
            {
                bSameScale = false;
                break;
            }
        }

        if (bSameScale)
        {
            if (m_vecSelectedObj.size() == (size_t)1)
            {
                Set_TransformSpin(m_vecSelectedObj.back()->m_tInfo.vPos, m_vecSelectedObj.back()->m_tInfo.vRot, m_vecSelectedObj.back()->m_tInfo.vSize);
            }
            m_vSpinValue[2].x += pNMUpDown->iDelta;
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vSize.x += pNMUpDown->iDelta;
                }
            }
        }
        else
        {
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vSize.x += pNMUpDown->iDelta;
                }
            }
            CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
            SetDlgItemText(IDC_EDIT9, L"-");
            *pResult = 0;
            return;
        }
    }

    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
    UpdateData(FALSE);
    *pResult = 0;
}

void CTransformInfo::OnDeltaposSpin8(NMHDR* pNMHDR, LRESULT* pResult) // Scale Y
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if (m_vecSelectedObj.empty())
    {
        if (nullptr != m_pObj)
        {
            m_vSpinValue[2].y += pNMUpDown->iDelta;
            m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
        }
    }
    else
    {
        bool bSameScale = true;
        float firstScaleY = m_vecSelectedObj[0]->m_tInfo.vSize.y;

        for (size_t i = 1; i < m_vecSelectedObj.size(); ++i)
        {
            if (m_vecSelectedObj[i]->m_tInfo.vSize.y != firstScaleY)
            {
                bSameScale = false;
                break;
            }
        }

        if (bSameScale)
        {
            if (m_vecSelectedObj.size() == (size_t)1)
            {
                Set_TransformSpin(m_vecSelectedObj.back()->m_tInfo.vPos, m_vecSelectedObj.back()->m_tInfo.vRot, m_vecSelectedObj.back()->m_tInfo.vSize);
            }
            m_vSpinValue[2].y += pNMUpDown->iDelta;
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vSize.y += pNMUpDown->iDelta;
                }
            }
        }
        else
        {
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vSize.y += pNMUpDown->iDelta;
                }
            }
            CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
            SetDlgItemText(IDC_EDIT10, L"-");
            *pResult = 0;
            return;
        }
    }

    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
    UpdateData(FALSE);
    *pResult = 0;
}

void CTransformInfo::OnDeltaposSpin9(NMHDR* pNMHDR, LRESULT* pResult) // Scale Z
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if (m_vecSelectedObj.empty())
    {
        if (nullptr != m_pObj)
        {
            m_vSpinValue[2].z += pNMUpDown->iDelta;
            m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
        }
    }
    else
    {
        bool bSameScale = true;
        float firstScaleZ = m_vecSelectedObj[0]->m_tInfo.vSize.z;

        for (size_t i = 1; i < m_vecSelectedObj.size(); ++i)
        {
            if (m_vecSelectedObj[i]->m_tInfo.vSize.z != firstScaleZ)
            {
                bSameScale = false;
                break;
            }
        }

        if (bSameScale)
        {
            if (m_vecSelectedObj.size() == (size_t)1)
            {
                Set_TransformSpin(m_vecSelectedObj.back()->m_tInfo.vPos, m_vecSelectedObj.back()->m_tInfo.vRot, m_vecSelectedObj.back()->m_tInfo.vSize);
            }
            m_vSpinValue[2].z += pNMUpDown->iDelta;
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vSize.z += pNMUpDown->iDelta;
                }
            }
        }
        else
        {
            for (auto& obj : m_vecSelectedObj)
            {
                if (obj)
                {
                    obj->m_tInfo.vSize.z += pNMUpDown->iDelta;
                }
            }
            CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
            SetDlgItemText(IDC_EDIT11, L"-");
            *pResult = 0;
            return;
        }
    }

    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
    UpdateData(FALSE);
    *pResult = 0;
}

void CTransformInfo::OnEnChangeEdit3()  // Position X
{
    CString strValue;
    GetDlgItem(IDC_EDIT3)->GetWindowText(strValue);
    if (!m_vecSelectedObj.empty() && strValue == "-")
        return;
    if (!UpdateData(TRUE))
        return;
    if (!m_pObj && m_vecSelectedObj.empty())
        return;

    for (int i = 0; i < strValue.GetLength(); i++)
    {
        if (i == 0 && strValue[i] == '-') continue;
        if (!isdigit(strValue[i]))
        {
            m_SpinCtrl[0].SetPos32((int)m_vSpinValue[0].x);
            UpdateData(FALSE);
            return;
        }
    }

    m_SpinCtrl[0].SetPos32((int)m_vSpinValue[0].x);
    if (!m_vecSelectedObj.empty())
    {
        for (auto& obj : m_vecSelectedObj)
        {
            if (obj)
            {
                obj->m_tInfo.vPos.x = m_vSpinValue[0].x;
            }
        }
    }
    else if (nullptr != m_pObj)
    {
        m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit4()  // Position Y
{
    CString strValue;
    GetDlgItem(IDC_EDIT4)->GetWindowText(strValue);
    if (!m_vecSelectedObj.empty() && strValue == "-")
        return;
    if (!UpdateData(TRUE))
        return;
    if (!m_pObj && m_vecSelectedObj.empty())
        return;

    for (int i = 0; i < strValue.GetLength(); i++)
    {
        if (i == 0 && strValue[i] == '-') continue;
        if (!isdigit(strValue[i]))
        {
            m_SpinCtrl[1].SetPos32((int)m_vSpinValue[0].y);
            UpdateData(FALSE);
            return;
        }
    }

    m_SpinCtrl[1].SetPos32((int)m_vSpinValue[0].y);

    if (!m_vecSelectedObj.empty())
    {
        for (auto& obj : m_vecSelectedObj)
        {
            if (obj)
            {
                obj->m_tInfo.vPos.y = m_vSpinValue[0].y;
            }
        }
    }
    else if (nullptr != m_pObj)
    {
        m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit5()  // Position Z
{
    CString strValue;
    GetDlgItem(IDC_EDIT5)->GetWindowText(strValue);
    if (!m_vecSelectedObj.empty() && strValue == "-")
        return;
    if (!UpdateData(TRUE))
        return;
    if (!m_pObj && m_vecSelectedObj.empty())
        return;

    for (int i = 0; i < strValue.GetLength(); i++)
    {
        if (i == 0 && strValue[i] == '-') continue;
        if (!isdigit(strValue[i]))
        {
            m_SpinCtrl[2].SetPos32((int)m_vSpinValue[0].z);
            UpdateData(FALSE);
            return;
        }
    }

    m_SpinCtrl[2].SetPos32((int)m_vSpinValue[0].z);

    if (!m_vecSelectedObj.empty())
    {
        for (auto& obj : m_vecSelectedObj)
        {
            if (obj)
            {
                obj->m_tInfo.vPos.z = m_vSpinValue[0].z;
            }
        }
    }
    else if (nullptr != m_pObj)
    {
        m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit6()  // Rotation X
{
    CString strValue;
    GetDlgItem(IDC_EDIT6)->GetWindowText(strValue);
    if (!m_vecSelectedObj.empty() && strValue == "-")
        return;
    if (!UpdateData(TRUE))
        return;
    if (!m_pObj && m_vecSelectedObj.empty())
        return;

    for (int i = 0; i < strValue.GetLength(); i++)
    {
        if (i == 0 && strValue[i] == '-') continue;
        if (!isdigit(strValue[i]))
        {
            m_SpinCtrl[3].SetPos32((int)m_vSpinValue[1].x);
            UpdateData(FALSE);
            return;
        }
    }

    m_SpinCtrl[3].SetPos32((int)m_vSpinValue[1].x);

    if (!m_vecSelectedObj.empty())
    {
        for (auto& obj : m_vecSelectedObj)
        {
            if (obj)
            {
                obj->m_tInfo.vRot.x = m_vSpinValue[1].x;
            }
        }
    }
    else if (nullptr != m_pObj)
    {
        m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit7()  // Rotation Y
{
    CString strValue;
    GetDlgItem(IDC_EDIT7)->GetWindowText(strValue);
    if (!m_vecSelectedObj.empty() && strValue == "-")
        return;
    if (!UpdateData(TRUE))
        return;
    if (!m_pObj && m_vecSelectedObj.empty())
        return;

    for (int i = 0; i < strValue.GetLength(); i++)
    {
        if (i == 0 && strValue[i] == '-') continue;
        if (!isdigit(strValue[i]))
        {
            m_SpinCtrl[4].SetPos32((int)m_vSpinValue[1].y);
            UpdateData(FALSE);
            return;
        }
    }

    m_SpinCtrl[4].SetPos32((int)m_vSpinValue[1].y);

    if (!m_vecSelectedObj.empty())
    {
        for (auto& obj : m_vecSelectedObj)
        {
            if (obj)
            {
                obj->m_tInfo.vRot.y = m_vSpinValue[1].y;
            }
        }
    }
    else if (nullptr != m_pObj)
    {
        m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit8()  // Rotation Z
{
    CString strValue;
    GetDlgItem(IDC_EDIT8)->GetWindowText(strValue);
    if (!m_vecSelectedObj.empty() && strValue == "-")
        return;
    if (!UpdateData(TRUE))
        return;
    if (!m_pObj && m_vecSelectedObj.empty())
        return;

    for (int i = 0; i < strValue.GetLength(); i++)
    {
        if (i == 0 && strValue[i] == '-') continue;
        if (!isdigit(strValue[i]))
        {
            m_SpinCtrl[5].SetPos32((int)m_vSpinValue[1].z);
            UpdateData(FALSE);
            return;
        }
    }

    m_SpinCtrl[5].SetPos32((int)m_vSpinValue[1].z);

    if (!m_vecSelectedObj.empty())
    {
        for (auto& obj : m_vecSelectedObj)
        {
            if (obj)
            {
                obj->m_tInfo.vRot.z = m_vSpinValue[1].z;
            }
        }
    }
    else if (nullptr != m_pObj)
    {
        m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit9()  // Scale X
{
    CString strValue;
    GetDlgItem(IDC_EDIT9)->GetWindowText(strValue);
    if (!m_vecSelectedObj.empty() && strValue == "-")
        return;
    if (!UpdateData(TRUE))
        return;
    if (!m_pObj && m_vecSelectedObj.empty())
        return;
    for (int i = 0; i < strValue.GetLength(); i++)
    {
        if (i == 0 && strValue[i] == '-') continue;
        if (!isdigit(strValue[i]))
        {
            m_SpinCtrl[6].SetPos32((int)m_vSpinValue[2].x);
            UpdateData(FALSE);
            return;
        }
    }

    m_SpinCtrl[6].SetPos32((int)m_vSpinValue[2].x);

    if (!m_vecSelectedObj.empty())
    {
        for (auto& obj : m_vecSelectedObj)
        {
            if (obj)
            {
                obj->m_tInfo.vSize.x = m_vSpinValue[2].x;
            }
        }
    }
    else if (nullptr != m_pObj)
    {
        m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit10()  // Scale Y
{
    CString strValue;
    GetDlgItem(IDC_EDIT10)->GetWindowText(strValue);
    if (!m_vecSelectedObj.empty() && strValue == "-")
        return;
    if (!UpdateData(TRUE))
        return;
    if (!m_pObj && m_vecSelectedObj.empty())
        return;
    for (int i = 0; i < strValue.GetLength(); i++)
    {
        if (i == 0 && strValue[i] == '-') continue;
        if (!isdigit(strValue[i]))
        {
            m_SpinCtrl[7].SetPos32((int)m_vSpinValue[2].y);
            UpdateData(FALSE);
            return;
        }
    }

    m_SpinCtrl[7].SetPos32((int)m_vSpinValue[2].y);

    if (!m_vecSelectedObj.empty())
    {
        for (auto& obj : m_vecSelectedObj)
        {
            if (obj)
            {
                obj->m_tInfo.vSize.y = m_vSpinValue[2].y;
            }
        }
    }
    else if (nullptr != m_pObj)
    {
        m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit11()  // Scale Z
{
    CString strValue;
    GetDlgItem(IDC_EDIT11)->GetWindowText(strValue);
    if (!m_vecSelectedObj.empty() && strValue == "-")
        return;
    if (!UpdateData(TRUE))
        return;
    if (!m_pObj && m_vecSelectedObj.empty())
        return;
    for (int i = 0; i < strValue.GetLength(); i++)
    {
        if (i == 0 && strValue[i] == '-') continue;
        if (!isdigit(strValue[i]))
        {
            m_SpinCtrl[8].SetPos32((int)m_vSpinValue[2].z);
            UpdateData(FALSE);
            return;
        }
    }

    m_SpinCtrl[8].SetPos32((int)m_vSpinValue[2].z);

    if (!m_vecSelectedObj.empty())
    {
        for (auto& obj : m_vecSelectedObj)
        {
            if (obj)
            {
                obj->m_tInfo.vSize.z = m_vSpinValue[2].z;
            }
        }
    }
    else if (nullptr != m_pObj)
    {
        m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::Set_TransformSpin(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vRot, const D3DXVECTOR3& vScale)
{
	m_vSpinValue[0] = vPos;
	m_vSpinValue[1] = vRot;
	m_vSpinValue[2] = vScale;

	m_SpinCtrl[0].SetPos32((int)m_vSpinValue[0].x);
	m_SpinCtrl[1].SetPos32((int)m_vSpinValue[0].y);
	m_SpinCtrl[2].SetPos32((int)m_vSpinValue[0].z);
	
	m_SpinCtrl[3].SetPos32((int)m_vSpinValue[1].x);
	m_SpinCtrl[4].SetPos32((int)m_vSpinValue[1].y);
	m_SpinCtrl[5].SetPos32((int)m_vSpinValue[1].z);

	m_SpinCtrl[6].SetPos32((int)m_vSpinValue[2].x);
	m_SpinCtrl[7].SetPos32((int)m_vSpinValue[2].y);
	m_SpinCtrl[8].SetPos32((int)m_vSpinValue[2].z);

	UpdateData(false);
}
