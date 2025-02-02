// CTransformInfo.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CTransformInfo.h"


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
			m_bStart[i] = true;
		}
		
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CTransformInfo::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[0].x = (float)(m_SpinCtrl[0].GetPos32());  // 현재 값을 가져옴

	if (nullptr != m_pObj)
	{
		m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
	}
	
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[0].y = (float)(m_SpinCtrl[1].GetPos32());  // 현재 값을 가져옴

	if (nullptr != m_pObj)
	{
		m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
	}

	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[0].z = (float)(m_SpinCtrl[2].GetPos32());  // 현재 값을 가져옴

	if (nullptr != m_pObj)
	{
		m_pObj->Set_Position_bySpin(D3DXVECTOR3(m_vSpinValue[0].x, m_vSpinValue[0].y, m_vSpinValue[0].z));
	}

	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[1].x = (float)(m_SpinCtrl[3].GetPos32());  // 현재 값을 가져옴

	if (nullptr != m_pObj)
	{
		m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
	}
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin5(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[1].y = (float)(m_SpinCtrl[4].GetPos32());  // 현재 값을 가져옴
	if (nullptr != m_pObj)
	{
		m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
	}
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin6(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[1].z = (float)(m_SpinCtrl[5].GetPos32());  // 현재 값을 가져옴
	if (nullptr != m_pObj)
	{
		m_pObj->Set_Rotation_bySpin(D3DXVECTOR3(m_vSpinValue[1].x, m_vSpinValue[1].y, m_vSpinValue[1].z));
	}
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin7(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[2].x = (float)(m_SpinCtrl[6].GetPos32());  // 현재 값을 가져옴
	if (nullptr != m_pObj)
	{
		m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
	}
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin8(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[2].y = (float)(m_SpinCtrl[7].GetPos32());  // 현재 값을 가져옴
	if (nullptr != m_pObj)
	{
		m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
	}
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnDeltaposSpin9(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	// 스핀 버튼의 값이 변경될 때 호출되는 함수
	m_vSpinValue[2].z = (float)(m_SpinCtrl[8].GetPos32());  // 현재 값을 가져옴
	if (nullptr != m_pObj)
	{
		m_pObj->Set_Scale_bySpin(D3DXVECTOR3(m_vSpinValue[2].x, m_vSpinValue[2].y, m_vSpinValue[2].z));
	}
	UpdateData(FALSE);  // 컨트롤의 값을 변수에 업데이트

	*pResult = 0;
}


void CTransformInfo::OnEnChangeEdit3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
}


void CTransformInfo::OnEnChangeEdit4()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTransformInfo::OnEnChangeEdit5()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTransformInfo::OnEnChangeEdit6()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTransformInfo::OnEnChangeEdit7()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTransformInfo::OnEnChangeEdit8()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTransformInfo::OnEnChangeEdit9()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTransformInfo::OnEnChangeEdit10()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTransformInfo::OnEnChangeEdit11()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
