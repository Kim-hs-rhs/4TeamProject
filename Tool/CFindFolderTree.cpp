﻿// CFindFolderTree.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CFindFolderTree.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "CMyForm.h"
#include "CFileInfo.h"



// CFindFolderTree 대화 상자

IMPLEMENT_DYNAMIC(CFindFolderTree, CDialog)

CFindFolderTree::CFindFolderTree(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CFindFolderTree, pParent)
{

}

CFindFolderTree::~CFindFolderTree()
{
}

void CFindFolderTree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
	//DDX_Control(pDX, IDC_MFCSHELLTREE1, m_MFCTreeCtrl);
}

void CFindFolderTree::AddFolderToTree(HTREEITEM hParent, const CString& folderPath)
{
	CFileFind finder;
	BOOL working = finder.FindFile(folderPath + _T("\\*.*"));

	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;

		if (finder.IsDirectory())
		{
			CString* pFullPath = new CString(finder.GetFilePath());
			HTREEITEM hItem = m_treeCtrl.InsertItem(finder.GetFileName(), hParent);
			m_treeCtrl.SetItemData(hItem, (DWORD_PTR)pFullPath);
			AddFolderToTree(hItem, *pFullPath);
		}
	}
	finder.Close();
}

void CFindFolderTree::DeleteTreeData(HTREEITEM hItem)
{
	if (!hItem) return;

	CString* pStr = (CString*)m_treeCtrl.GetItemData(hItem);
	delete pStr;

	HTREEITEM hChild = m_treeCtrl.GetChildItem(hItem);
	while (hChild) {
		DeleteTreeData(hChild);
		hChild = m_treeCtrl.GetNextSiblingItem(hChild);
	}
}

void CFindFolderTree::RefreshTree(const CString& newRootPath)
{ 
	
	// 기존 트리 데이터 삭제
	DeleteTreeData(m_treeCtrl.GetRootItem());

	// 트리 컨트롤 초기화
	m_treeCtrl.DeleteAllItems();

	// 새 경로가 존재하는지 확인
	if (PathFileExists(newRootPath))
	{
		// 새 루트 아이템 생성
		HTREEITEM hNewRoot = m_treeCtrl.InsertItem(newRootPath);

		// 새 경로의 전체 경로 저장
		CString* pFullPath = new CString(newRootPath);
		m_treeCtrl.SetItemData(hNewRoot, (DWORD_PTR)pFullPath);

		// 하위 폴더 추가
		AddFolderToTree(hNewRoot, newRootPath);

		// 루트 아이템 펼치기
		m_treeCtrl.Expand(hNewRoot, TVE_EXPAND);
	}
}

void CFindFolderTree::PostNcDestroy()
{
}


BEGIN_MESSAGE_MAP(CFindFolderTree, CDialog)

	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CFindFolderTree::OnTvnSelchangedTree1)
	//ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE1, &CFindFolderTree::OnTvnSelchangedMfcshelltree1)
	ON_BN_CLICKED(IDOK, &CFindFolderTree::OnBnClickedOk)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CFindFolderTree::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(_T("텍스쳐 찾기"));
	CString path = _T("../Texture");
	if (PathFileExists(path))
	{
		HTREEITEM hRoot = m_treeCtrl.InsertItem(path);
		AddFolderToTree(hRoot, path);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFindFolderTree::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//HTREEITEM hItem = pNMTreeView->itemNew.hItem; // 새로 눌린거

	//CString* pFullPath = (CString*)m_treeCtrl.GetItemData(hItem);
	//CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());
	//CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	//pMyForm->m_TextureListBox.m_stCategory = m_treeCtrl.GetItemText(m_treeCtrl.GetParentItem(hItem));
	//pMyForm->m_TextureListBox.m_stFolderName = m_treeCtrl.GetItemText(hItem);
	//if (pFullPath)
	//{
	//	m_relativePath = CFileInfo::Convert_RelativePath(*pFullPath);
	//}
	//*pResult = 0;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	// 선택된 아이템이 없으면 처리하지 않음
	if (hItem == NULL)
	{
		*pResult = 0;
		return;
	}

	CString* pFullPath = (CString*)m_treeCtrl.GetItemData(hItem);
	if (!pFullPath)  // 경로 데이터가 없으면 처리하지 않음
	{
		*pResult = 0;
		return;
	}

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());
	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));

	// 현재 선택된 폴더 이름만 가져오기
	CString folderName = m_treeCtrl.GetItemText(hItem);
	// 부모 폴더 이름만 가져오기
	HTREEITEM hParent = m_treeCtrl.GetParentItem(hItem);
	if (hParent)
	{
		CString parentName = m_treeCtrl.GetItemText(hParent);
		// 부모가 루트가 아닐 경우에만 카테고리로 설정
		if (hParent != m_treeCtrl.GetRootItem())
		{
			pMyForm->m_TextureListBox.m_stCategory = parentName;
			pMyForm->m_TextureListBox.m_stFolderName = folderName;
		}
	}

	//// 루트 아이템인 경우에만 트리 갱신
	//if (hParent == NULL)
	//{
	//	RefreshTree(*pFullPath);
	//}
	m_relativePath = CFileInfo::Convert_RelativePath(*pFullPath);

	*pResult = 0;
}



void CFindFolderTree::OnBnClickedOk()
{
	if (!m_relativePath.empty())
	{
		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());
		CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
		pMyForm->m_TextureListBox.Load_TextureList(m_relativePath);
	}
	CDialog::OnOK();
}


void CFindFolderTree::OnDestroy()
{
	DeleteTreeData(m_treeCtrl.GetRootItem());
	ShowWindow(SW_HIDE);
	CDialog::OnDestroy();
}
