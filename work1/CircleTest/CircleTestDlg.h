
// CircleTestDlg.h: 헤더 파일
//

#pragma once


// CCircleTestDlg 대화 상자
class CCircleTestDlg : public CDialogEx
{
// 생성입니다.
private:
	CImage m_image;
	void drawCircle(int x,  int y, int radius);
	BOOL vaildImgPos(int x, int y);
	void moveCircle(void);
	void UpdateDisplay(void);
	CPoint findCircleCenter(void);
	void drawPointCenter(int x, int y);

public:
	CCircleTestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CIRCLETEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnDrawCircle();
	afx_msg void OnBnClickedBtnAction();
	afx_msg void OnBnClickedBtnOpen();
	void InitImage();
	BOOL FolderCheck(CString tmpPath);
	int m_nStartX;
	int m_nStartY;
	int m_nEndX;
	int m_nEndY;
	int m_nRadius;

	int m_nMoveX;
	int m_nMoveY;

	CString strFolderPath;
};
