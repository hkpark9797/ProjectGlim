
// CircleTestDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "CircleTest.h"
#include "CircleTestDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <iostream>
using namespace std;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCircleTestDlg 대화 상자



CCircleTestDlg::CCircleTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CIRCLETEST_DIALOG, pParent)
	, m_nStartX(0)
	, m_nStartY(0)
	, m_nEndX(0)
	, m_nEndY(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCircleTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SX, m_nStartX);
	DDX_Text(pDX, IDC_EDIT_SY, m_nStartY);
	DDX_Text(pDX, IDC_EDIT_EX, m_nEndX);
	DDX_Text(pDX, IDC_EDIT_EY, m_nEndY);
}

BEGIN_MESSAGE_MAP(CCircleTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_DRAW_CIRCLE, &CCircleTestDlg::OnBnClickedBtnDrawCircle)
	ON_BN_CLICKED(IDC_BTN_ACTION, &CCircleTestDlg::OnBnClickedBtnAction)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CCircleTestDlg::OnBnClickedBtnOpen)
END_MESSAGE_MAP()


// CCircleTestDlg 메시지 처리기

BOOL CCircleTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.
	TCHAR chFilePath[_MAX_PATH + 1] = { 0, };
	CString tmpPath;
	GetModuleFileName(NULL, chFilePath, _MAX_PATH);
	tmpPath = chFilePath;
	tmpPath = tmpPath.Left(tmpPath.ReverseFind('\\'));
	if (FolderCheck(tmpPath)) {
		strFolderPath = tmpPath + _T("\\image");		
	}
	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetDlgItemText(IDC_DISPLAY_POS, _T("(x,y)"));
	InitImage();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCircleTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCircleTestDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		if (m_image) {
			m_image.Draw(dc, 0, 0);
			CPoint center;
			center = findCircleCenter();
			if(center.x > 0)
				drawPointCenter(center.x, center.y);
		}

		CDialogEx::OnPaint();
	}


}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCircleTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCircleTestDlg::OnBnClickedBtnDrawCircle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nPitch = m_image.GetPitch();
	if (nPitch < 0) {
		m_image.Destroy();
		InitImage();
	}


	int radius = rand() % 100;

	if (radius < 10) {
		radius = 10;
	}
	//memset(fm, 0, nWidth * nHeight);

	m_nRadius = radius;

	UpdateData(TRUE);

	//cout << m_nStartX << endl;
	//(IDC_DISPLAY_POS, _T("RRR"));
	UpdateData(false);


	drawCircle(m_nStartX, m_nStartY, radius);

	UpdateDisplay();
	
} 

void CCircleTestDlg::drawCircle(int stX, int stY, int radius) {

	int nWidth = m_image.GetWidth();  
	int nHeight = m_image.GetHeight(); 
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	int nPitch = m_image.GetPitch();

	memset(fm, 0, nWidth * nHeight); 

	// 원의 반지름을 설정한다.
	//int r = 24;

	for (int j = stY - radius; j < stY + radius; j++) {
		for (int i = stX - radius; i < stX + radius; i++) {
			int dx = i - stX;
			int dy = j - stY;
			if (vaildImgPos(i, j) && (dx * dx + dy * dy <= radius * radius)) {
				fm[j * nPitch + i] = 255;
			}
		}
	}

}

BOOL CCircleTestDlg::vaildImgPos(int x, int y)
{
	int nWidth = m_image.GetWidth(); 
	int nHeight = m_image.GetHeight(); 

	CRect rect(0, 0, nWidth, nHeight);
	
	return rect.PtInRect(CPoint(x, y)); 
}


void CCircleTestDlg::OnBnClickedBtnAction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString filepath;
	CString fileNum;
	m_image.Save(filepath);
	int j = 1;
	
	m_nMoveX = m_nStartX;
	m_nMoveY = m_nStartY;

	for (int i = m_nStartX; i < m_nEndX; i++) {
		moveCircle();

		if (i % ((m_nEndX - m_nStartX)/5) == 0) { //5 image save
			fileNum.Format(_T("%d"), j);
			filepath = strFolderPath + _T("\\save") + fileNum + _T(".bmp");
			m_image.Save(filepath);
			j++;
		}
		Sleep(10);
	}

}


void CCircleTestDlg::OnBnClickedBtnOpen()
{

	static TCHAR BASED_CODE szFilter[] = _T("이미지 파일(*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG;*.bmp;*.jpg;*.gif |모든파일(*.*)|*.*||");
	
	CFileDialog dlg(TRUE, _T("*.jpg"), _T("image"), OFN_HIDEREADONLY, szFilter);

	CString pathName;
	CPoint center;

	dlg.m_ofn.lpstrInitialDir = strFolderPath;
	if (IDOK == dlg.DoModal())
	{
		pathName = dlg.GetPathName();
		//MessageBox(pathName);
	}


	if (m_image != NULL) {
		m_image.Destroy();
	}

	m_image.Load(pathName);

	//center = findCircleCenter();
	//drawPointCenter(center.x, center.y);
	UpdateDisplay();
	Invalidate();
}
void CCircleTestDlg::drawPointCenter(int x, int y) {
	// 이미지의 중심 좌표 계산

	CClientDC dc(this);


	// X 모양 그리기
	dc.MoveTo(x - 10, y - 10);
	dc.LineTo(x + 10, y + 10);
	dc.MoveTo(x + 10, y - 10);
	dc.LineTo(x - 10, y + 10);

	// 좌표값 표시
	CString str;
	str.Format(_T("(%d, %d)"), x, y);
	dc.TextOut(x + 15, y, str);
	//m_image.Draw(dc, 0, 0);

	//Invalidate();
}

void CCircleTestDlg::InitImage()
{
	int nWidth = 640;
	int nHeight = 480;
	int nBpp = 8;

	m_image.Create(nWidth, -nHeight, nBpp);

	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	//memset(fm, 0xff, nWidth * nHeight);
	memset(fm, 0x00, nWidth * nHeight);
}

BOOL CCircleTestDlg::FolderCheck(CString tmpPath) {

	CFileFind file;
	CString strFile = tmpPath + _T("\\image"); // 폴더 경로
	// 경로 폴더가 있다면 1, 없다면 0을 반환
	BOOL bResult = file.FindFile(strFile);

	if (!bResult)
	{
		// 폴더 생성
		bResult = CreateDirectory(strFile + "\\", NULL);
		if (!bResult)
		{
			/*Error*/
			return FALSE;
		}
		return TRUE;
	}
	return TRUE;
}

void CCircleTestDlg::moveCircle(void) 
{
	//static int nSttX = m_nStartX;
	//static int nSttY = m_nStartY;
	int nWhite = 0xff;
	int nWidth = m_image.GetWidth(); 
	int nHeight = m_image.GetHeight(); 
	unsigned char* fm = (unsigned char*)m_image.GetBits(); 
	int nPitch = m_image.GetPitch();

	memset(fm, 0, nWidth * nHeight); 

	// 원의 반지름을 설정한다.
	int r = m_nRadius;
	
	for (int j = m_nMoveY - r; j < m_nMoveY + r; j++) {
		for (int i = m_nMoveX - r; i < m_nMoveX + r; i++) {
			int dx = i - m_nMoveX;
			int dy = j - m_nMoveY;
			if (vaildImgPos(i, j) && (dx * dx + dy * dy <= r * r)) {
				fm[j * nPitch + i] = nWhite;
			}
		}
	}
	m_nMoveX++;
	m_nMoveY++;
	UpdateDisplay();

}

void CCircleTestDlg::UpdateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);
}

CPoint CCircleTestDlg::findCircleCenter(void) {

	int nWhite = 0xff;
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	int nPitch = m_image.GetPitch();

	int sumX = 0, sumY = 0, count = 0;

	for (int j = 0; j < nHeight; j++) {
		for (int i = 0; i < nWidth; i++) {
			if (fm[j * nPitch + i] == nWhite) {
				sumX += i;
				sumY += j;
				++count;
			}
		}
	}

	if (count > 0) {
		int centerX = sumX / count;
		int centerY = sumY / count;
		return CPoint(centerX, centerY);
	}
	else {
		return CPoint(-1, -1);
	}


}
