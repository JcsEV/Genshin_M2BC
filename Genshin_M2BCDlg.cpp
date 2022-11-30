
// Genshin_M2BCDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Genshin_M2BC.h"
#include "Genshin_M2BCDlg.h"
#include "afxdialogex.h"

#include "ReadFile.h"

#include <windows.h>
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define WM_SHOWTASK  WM_USER+1 //以及声明消息响应函数：
#define WM_RESHOW
const UINT WM_TaskbarRestart = RegisterWindowMessage(TEXT("TaskbarCreated")); 

map<string, string>store;
string road = ""; 


// CGenshinM2BCDlg 对话框



CGenshinM2BCDlg::CGenshinM2BCDlg(CWnd* pParent/* = nullptr*/)
	: CDialogEx(IDD_GENSHIN_M2BC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);	
}


void CGenshinM2BCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Edit_vension);
	DDX_Control(pDX, IDC_EDIT2, Edit_class);
	DDX_Control(pDX, IDC_EDIT3, Edit_Road);
}


BEGIN_MESSAGE_MAP(CGenshinM2BCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGenshinM2BCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON6, &CGenshinM2BCDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON2, &CGenshinM2BCDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CGenshinM2BCDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CGenshinM2BCDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CGenshinM2BCDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON7, &CGenshinM2BCDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CGenshinM2BCDlg::OnBnClickedButton8) 
	ON_REGISTERED_MESSAGE(WM_TaskbarRestart, &CGenshinM2BCDlg::OnTaskBarRestart)
	ON_MESSAGE(WM_SHOWTASK, OnShowTask)
	//ON_MESSAGE(WM_RESHOW, OnReshow)
	ON_BN_CLICKED(IDC_BUTTON9, &CGenshinM2BCDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CGenshinM2BCDlg 消息处理程序
BOOL CGenshinM2BCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	HANDLE g_hEvent = CreateEventW(NULL, 0, 0, L"Genshin_Trans");//定义一个句柄
	if (g_hEvent != 0) { SetEvent(g_hEvent); }
	if (g_hEvent && ERROR_ALREADY_EXISTS == GetLastError()) { //如果多开会退出
		MessageBox(_T("存在已启动的程序"));
		ExitProcess(0);
	}

	//应用程序定义的消息标示。当托盘图标区域发生鼠标事件或者使用键盘选择或激活图标时，
	//系统将使用此标示向由hWnd成员标示的窗口发送消息。消息响应函数的wParam参数标示了
	//消息事件发生的任务栏图标，lParam参数根据事件的不同，包含了鼠标或键盘的具体消息，
	//例如当鼠标指针移过托盘图标时，lParam将为WM_MOUSEMOVE。
	m_nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd = this->m_hWnd;
	m_nid.uID = IDR_MAINFRAME; //通过多次调用，你可以使用不同的uID将多个图标关联到一个窗口hWnd，例如QQ修改登录状态时，图标也会变
	m_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //此成员表明具体哪些其他成员为合法数据（即哪些成员起作用）
	m_nid.uCallbackMessage = WM_SHOWTASK;//自定义的消息名称 
	m_nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)); //图标的句柄
	//strcpy(m_nid.szTip, "VS2010_SerialPortDlg");    //信息提示条

	if (Read_Set(road) && Read_Tem(store)) {
		Edit_Road.SetWindowText(CA2T(road.c_str()));
		if (OnReRead(road)) { return TRUE; }
	}

	MessageBox(_T("读入失败，请重新启动"));
	//getchar();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CGenshinM2BCDlg::OnPaint()
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
HCURSOR CGenshinM2BCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CGenshinM2BCDlg::OnTaskBarRestart(WPARAM wParam, LPARAM lParam)
{
	Shell_NotifyIcon(NIM_ADD, &m_nid);
	return 0;
}


LRESULT CGenshinM2BCDlg::OnShowTask(WPARAM wParam, LPARAM lParam) //wParam接收的是图标的ID，而lParam接收的是鼠标的行为 
{
	if (wParam != IDR_MAINFRAME) { return 1; }
	switch (lParam) {
	case WM_RBUTTONUP: {//右键起来时弹出快捷菜单 
		LPPOINT lpoint = new tagPOINT;
		::GetCursorPos(lpoint);//得到鼠标位置 
		CMenu menu;
		menu.CreatePopupMenu();//声明一个弹出式菜单 
		menu.AppendMenu(MF_STRING, WM_DESTROY, _T("关闭"));//增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已隐藏），将程序结束。 
		//menu.AppendMenu(MF_STRING, WM_RESHOW, _T("打开"));//自己定义的命令，需要手动添加命令响应函数
		menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x - 125, lpoint->y - 25, this);//确定弹出式菜单的位置 
		HMENU hmenu = menu.Detach();//资源回收 
		menu.DestroyMenu();
		delete lpoint;
		break;
	}
	case WM_LBUTTONUP: {//左键起来时打开界面
		ShowWindow(SW_SHOW);//显示主窗口
		Shell_NotifyIcon(NIM_DELETE, &m_nid);    //在托盘区删除图标
		break;
	}
	case  WM_LBUTTONDBLCLK://左键双击，添加相应的功能
		break;
	}
	return 0;
}


void CGenshinM2BCDlg::OnReshow()
{
	ShowWindow(SW_SHOW);
	Shell_NotifyIcon(NIM_DELETE, &m_nid);    //在托盘区删除图标
}


void CGenshinM2BCDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ExitProcess(0);
}


void CGenshinM2BCDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	// 呼出文件夹
	TCHAR       szPath[MAX_PATH];
	BROWSEINFO  Docu = { 0 };
	Docu.hwndOwner = m_hWnd;
	Docu.pszDisplayName = szPath;
	Docu.lpszTitle = TEXT("选择Genshin Impact Game文件夹：");
	Docu.ulFlags = BIF_RETURNONLYFSDIRS;

	ITEMIDLIST* pid = SHBrowseForFolder(&Docu);

	if (pid) { SHGetPathFromIDList(pid, szPath); }
	else if (pid == NULL) { return; }// 如果没有选择文件路径，
	else {
		MessageBox(_T("路径选择存在错误，请重试"));
		return;
	}
	road = szPath;
	Edit_Road.SetWindowText(szPath);// szPath即是文件夹的路径
}


void CGenshinM2BCDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!Write_Set(road)) {
		MessageBox(_T("保存新路径失败，请重试"));
		return;
	}
	else {
		MessageBox(_T("保存成功"));
		OnReRead(road);
	}
	Edit_Road.SetWindowText(road.c_str());
}


void CGenshinM2BCDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	Shell_NotifyIcon(NIM_ADD, &m_nid);    //在托盘区添加图标 
	ShowWindow(SW_HIDE);    //隐藏主窗口,像QQ一登录就会有托盘和主界面同时存在，这样的话不需要隐藏主界面
}


void CGenshinM2BCDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	//HRESULT hr = CoInitialize(NULL);
	//if (SUCCEEDED(hr)) {
	//	IShellLink* pisl;
	//	hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
	//	if (SUCCEEDED(hr)) {
	//		IPersistFile* pIPF; //快捷方式的原始文件地址
	//		pisl->SetWorkingDirectory(road.c_str());
	//		pisl->SetPath((road + "\\YuanShen.exe").c_str()); //快捷方式起始位置
	//		hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);
	//		if (SUCCEEDED(hr)) { //创建快捷方式的目标地址
	//			pIPF->Save(L".\\Genshin_M2BC\\YuanShen.lnk", FALSE);
	//			pIPF->Release();
	//		}
	//		pisl->Release();
	//	}
	//	CoUninitialize();
	//}
	//ShellExecute(NULL, "open", ".\\Genshin_M2BC\\YuanShen.lnk", NULL, NULL, SW_SHOW);

	ShellExecute(NULL, "runas", "YuanShen.exe", NULL, (road + "\\").c_str(), SW_SHOW);

	Shell_NotifyIcon(NIM_ADD, &m_nid);    //在托盘区添加图标 
	ShowWindow(SW_HIDE);    //隐藏主窗口,像QQ一登录就会有托盘和主界面同时存在，这样的话不需要隐藏主界面
}


bool CGenshinM2BCDlg::OnReRead(std::string& rd)
{
	if (!Read_New(store, rd)) { return false; }
	else {
		Edit_class.SetWindowText(store["cps"].c_str());
		Edit_class.EnableWindow(FALSE);
		Edit_vension.SetWindowText(store["game_version"].c_str());
		//store["game_version"].c_str() == "" ?
		//	Edit_vension.EnableWindow(TRUE) :
		//	Edit_vension.EnableWindow(FALSE);
		return true;
	}
}


void CGenshinM2BCDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	store["channel"] = "1";
	store["cps"] = "mihoyo";
	store["plugin_sdk_version"] = "";
	store["sub_channel"] = "1";

	if (!(SDK_Delete(road) && Write_New(store, road))) {
		MessageBox(_T("官服转换失败，请重试"));
		return;
	}

	Edit_vension.SetWindowText(CA2T(store["game_version"].c_str()));
	Edit_class.SetWindowText(CA2T(store["cps"].c_str()));

	MessageBox(_T("转换成功"));
}


void CGenshinM2BCDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	store["channel"] = "14";
	store["cps"] = "bilibili";
	store["plugin_sdk_version"] = "3.5.0";
	store["sub_channel"] = "0";

	if (!(SDK_Write(road) && Write_New(store, road))) {
		MessageBox(_T("B服转换失败，请重试"));
		return;
	}
	Edit_vension.SetWindowText(CA2T(store["game_version"].c_str()));
	Edit_class.SetWindowText(CA2T(store["cps"].c_str()));

	MessageBox(_T("转换成功"));
}


void CGenshinM2BCDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("国际服转换失败，请重试"));
	return;

	store["channel"] = "1";
	store["cps"] = "mihoyo";
	store["plugin_sdk_version"] = "";
	store["sub_channel"] = "1";

	if (!(SDK_Delete(road) && Write_New(store, road))) {
		MessageBox(_T("国际服转换失败，请重试"));
		return;
	}
	Edit_vension.SetWindowText(CA2T(store["game_version"].c_str()));
	Edit_class.SetWindowText(CA2T(store["cps"].c_str()));

	MessageBox(_T("转换成功"));
}


void CGenshinM2BCDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	CString Cvension(_T(""));
	Edit_vension.GetWindowText(Cvension);
	string vension = CT2A(Cvension.GetString());
	if (vension == "") {
		MessageBox(_T("请在\"当前版本\"的输入框内输入版本号"));
		return;
	}
	if (!(SDK_Delete(road) && Write_Tem(road, vension))) {
		MessageBox(_T("还原失败，请重试"));
		return;
	}
	Edit_class.SetWindowText(string("mihoyo").c_str());

	if (!OnReRead(road)) {
		MessageBox(_T("重读失败，请重启"));
	}
	else {
		MessageBox(_T("转换成功"));
	}
}
