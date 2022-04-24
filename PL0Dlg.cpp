
// PL0Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PL0.h"
#include "PL0Dlg.h"
#include "afxdialogex.h"
#include "transcode.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

transPL0 PL0;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CPL0Dlg 对话框



CPL0Dlg::CPL0Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PL0_DIALOG, pParent)
	, Output(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPL0Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT, m_input);
	DDX_Control(pDX, IDC_OUTPUT, m_output);
	DDX_Control(pDX, IDC_COMBO1, m_box);
	DDX_Text(pDX, IDC_OUTPUT, Output);
}

BEGIN_MESSAGE_MAP(CPL0Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON4, &CPL0Dlg::OnClickedButton4)
END_MESSAGE_MAP()


// CPL0Dlg 消息处理程序

BOOL CPL0Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPL0Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPL0Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPL0Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPL0Dlg::OnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	int indx = m_box.GetCurSel();
	if (indx == -1) {
		MessageBox(_T("请先选择处理程序！"));
		return;
	}

	if (indx == 0) {
		Output = "";
		//获取输入
		CString c_user;
		m_input.GetWindowText(c_user);
		string input = CT2A(c_user.GetBuffer());
		PL0.simulate(input);
		//输出
		ifstream ifs("ta.txt");  //读取文件
		while (!ifs.eof())  //行0 - 行lines对应strvect[0] - strvect[lines]
		{
			string inbuf;
			getline(ifs, inbuf, '\n');
			Output += inbuf.c_str();
			//if (inbuf[0] == '\r') {
			//	Output += "\r\n";
			//}
			Output += "\r\n";
		}
		UpdateData(FALSE); //更新显示
	}
	else if(indx == 1){
		Output = "";
		//获取输入
		CString c_user;
		m_input.GetWindowText(c_user);
		string input = CT2A(c_user.GetBuffer());
		PL0.toNFA(input);
		//输出
		ifstream ifs("toNFA.txt");  //读取文件
		while (!ifs.eof())  //行0 - 行lines对应strvect[0] - strvect[lines]
		{
			string inbuf;
			getline(ifs, inbuf, '\n');
			Output += inbuf.c_str();
			//if (inbuf[0] == '\r') {
			//	Output += "\r\n";
			//}
			Output += "\r\n";
		}
		UpdateData(FALSE); //更新显示
	}
	else if (indx == 2) {

	}

}
