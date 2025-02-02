// CTransformInfo.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CTransformInfo.h"
#include "CUndoManager.h"
#include "CObjManager.h"


// CTransformInfo

IMPLEMENT_DYNCREATE(CTransformInfo, CFormView)

CTransformInfo::CTransformInfo()
	: CFormView(IDD_CTransformInfo), m_pObj(nullptr)
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
    if (!m_pObj) return;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[0].x = (float)(m_SpinCtrl[0].GetPos32());  // 현재 값을 가져옴

	if (nullptr != m_pObj)
	{
		m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
	}
	
	CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (!m_pObj) return;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
 
    m_vSpinValue[0].y -= pNMUpDown->iDelta;

	if (nullptr != m_pObj)
	{
		m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
	}
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (!m_pObj) return;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[0].z = (float)(m_SpinCtrl[2].GetPos32());  // 현재 값을 가져옴

	if (nullptr != m_pObj)
	{
		m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
	}
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (!m_pObj) return;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[1].x = (float)(m_SpinCtrl[3].GetPos32());  // 현재 값을 가져옴

	if (nullptr != m_pObj)
	{
		m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
	}
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin5(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (!m_pObj) return;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[1].y = (float)(m_SpinCtrl[4].GetPos32());  // 현재 값을 가져옴
	if (nullptr != m_pObj)
	{
		m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
	}
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin6(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (!m_pObj) return;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[1].z = (float)(m_SpinCtrl[5].GetPos32());  // 현재 값을 가져옴
	if (nullptr != m_pObj)
	{
		m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
	}
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin7(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (!m_pObj) return;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[2].x = (float)(m_SpinCtrl[6].GetPos32());  // 현재 값을 가져옴
	if (nullptr != m_pObj)
	{
		m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
	}
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin8(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (!m_pObj) return;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[2].y = (float)(m_SpinCtrl[7].GetPos32());  // 현재 값을 가져옴
	if (nullptr != m_pObj)
	{
		m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
	}
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin9(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (!m_pObj) return;
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[2].z = (float)(m_SpinCtrl[8].GetPos32());  // 현재 값을 가져옴
	if (nullptr != m_pObj)
	{
		m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
	}
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnEnChangeEdit3()  // Position X
{
    if (!m_pObj) return;
    if (!UpdateData(TRUE))
        return;

    CString strValue;
    GetDlgItem(IDC_EDIT3)->GetWindowText(strValue);

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

    if (nullptr != m_pObj)
    {
        m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit4()  // Position Y
{
    if (!UpdateData(TRUE))
        return;

    CString strValue;
    GetDlgItem(IDC_EDIT4)->GetWindowText(strValue);

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

    if (nullptr != m_pObj)
    {
        m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit5()  // Position Z
{
    if (!UpdateData(TRUE))
        return;

    CString strValue;
    GetDlgItem(IDC_EDIT5)->GetWindowText(strValue);

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

    if (nullptr != m_pObj)
    {
        m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit6()  // Rotation X
{
    if (!UpdateData(TRUE))
        return;

    CString strValue;
    GetDlgItem(IDC_EDIT6)->GetWindowText(strValue);

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

    if (nullptr != m_pObj)
    {
        m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit7()  // Rotation Y
{
    if (!UpdateData(TRUE))
        return;

    CString strValue;
    GetDlgItem(IDC_EDIT7)->GetWindowText(strValue);

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

    if (nullptr != m_pObj)
    {
        m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit8()  // Rotation Z
{
    if (!UpdateData(TRUE))
        return;

    CString strValue;
    GetDlgItem(IDC_EDIT8)->GetWindowText(strValue);

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

    if (nullptr != m_pObj)
    {
        m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit9()  // Scale X
{
    if (!UpdateData(TRUE))
        return;

    CString strValue;
    GetDlgItem(IDC_EDIT9)->GetWindowText(strValue);

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

    if (nullptr != m_pObj)
    {
        m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit10()  // Scale Y
{
    if (!UpdateData(TRUE))
        return;

    CString strValue;
    GetDlgItem(IDC_EDIT10)->GetWindowText(strValue);

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

    if (nullptr != m_pObj)
    {
        m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
    }
    CUndoManager::Get_Instance()->SaveState(UndoType::OBJ);
}

void CTransformInfo::OnEnChangeEdit11()  // Scale Z
{
    if (!UpdateData(TRUE))
        return;

    CString strValue;
    GetDlgItem(IDC_EDIT11)->GetWindowText(strValue);

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

    if (nullptr != m_pObj)
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
