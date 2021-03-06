
// CodeDemoVS2015Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CodeDemoVS2015.h"
#include "CodeDemoVS2015Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CCodeDemoVS2015Dlg 对话框



CCodeDemoVS2015Dlg::CCodeDemoVS2015Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CODEDEMOVS2015_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCodeDemoVS2015Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCodeDemoVS2015Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CCodeDemoVS2015Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CCodeDemoVS2015Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCodeDemoVS2015Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CCodeDemoVS2015Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CCodeDemoVS2015Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CCodeDemoVS2015Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CCodeDemoVS2015Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CCodeDemoVS2015Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CCodeDemoVS2015Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CCodeDemoVS2015Dlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CCodeDemoVS2015Dlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CCodeDemoVS2015Dlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CCodeDemoVS2015Dlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CCodeDemoVS2015Dlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CCodeDemoVS2015Dlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CCodeDemoVS2015Dlg::OnBnClickedButton16)
END_MESSAGE_MAP()


// CCodeDemoVS2015Dlg 消息处理程序

BOOL CCodeDemoVS2015Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

void CCodeDemoVS2015Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCodeDemoVS2015Dlg::OnPaint()
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
HCURSOR CCodeDemoVS2015Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


#include "Regedit.h"

void CCodeDemoVS2015Dlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	
	wstring str = L"Version";
	wstring wStr =	GetBrowserRegitValueByKey(str.c_str());

	const wchar_t *p = wStr.c_str();
	wchar_t wszBuf[100] = { 0 };
	lstrcpy(wszBuf, wStr.c_str());


	char output[256] = {0};
	sprintf_s(output, "%ws", p);

	float i = stof(wStr);


	DWORD dwValue = GetBrowserRegitValueByKeyDWOD(L"NewInstallDate");


	return;
}

#include "logger.h"
#include "outputLogToFile.h"
void CCodeDemoVS2015Dlg::OnBnClickedButton2()
{

	// TODO: Add your control notification handler code here
	MyLog myLog;	

	#define OUTLOGERR(fm, ...)	myLog.traceEX(FALSE, __FILE__, __LINE__,fm,__VA_ARGS__)
	#define OUTLOGINF(fm, ...)	myLog.traceEX(TRUE, __FILE__, __LINE__,fm,__VA_ARGS__)
	OUTLOGINF("Begin ClickedButton2");
	OUTLOGERR("clickedButton 2 nRet = 0x%x ", GetLastError());
	OUTLOGINF("End clickedButton2");
	return;


	CLogger gLogger;
	gLogger.TraceInfo("generate infor for code");
	gLogger.TraceError("err infor ");
	gLogger.TraceWarning("Warning infor");
	gLogger.TraceFatal("end fuction");

}

//#include <time.h>
//#include <stdarg.h>
#include <sys/timeb.h>

__int64 getCurrent() {

	struct timeb tp_cur;
	ftime(&tp_cur);
	struct tm btm;
	localtime_s(&btm, &tp_cur.time);

	return (tp_cur.time * 1000 + tp_cur.millitm);
}


void CCodeDemoVS2015Dlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here

	double start, end, cost;
	start = clock();
//	sleep(1);
	end = clock();
	cost = end - start;

	__int64 nTim1 = getCurrent();
	
	long t1 = GetCurrentTime();//GetTickCount();//程序段开始前取得系统运行时间(ms)
	//Sleep(500);
	long t2 = GetCurrentTime(); //();//程序段结束后取得系统运行时间(ms)
	__int64 nTim2 = getCurrent();
	
	__int64 nTime = nTim2 - nTim1;

	long t3 = t2 - t1;

	t3 = t3 + 1;
}

#include <sstream>
std::string generate()
{
	ostringstream osm;
	osm << "xxxx " << 1111;

	// 清空之前的内容
	//osm.str("");

	//// 浮点数转换限制
	//double tmp = 123.1234554351236234;
	//osm.precision(15);
	//osm.setf(std::ios::fixed); // 将浮点数的位数限定为小数点之后的位数
	//osm << tmp;

	return osm.str();
}

#include <vector>
string FormatString(const char *lpcszFormat, ...)
{
	string strResult;
	if (NULL != lpcszFormat)
	{
		va_list marker;
		va_start(marker, lpcszFormat); //初始化变量参数
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //获取格式化字符串长度
		std::vector<char> vBuffer(nLength, '\0'); //创建用于存储格式化字符串的字符数组
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, lpcszFormat, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}
		va_end(marker); //重置变量参数
	}
	return strResult;
}

// std::string WString2String(const std::wstring& ws)
// {
// 	std::string strLocale = setlocale(LC_ALL, "");
// 	const wchar_t* wchSrc = ws.c_str();
// 
// 	char*CStr;
// 	size_t nDestSize = 0;
// 	CStr = (char*)malloc(ws.length() * sizeof(char));
// 	wcstombs_s(&nDestSize, CStr, ws.length(), wchSrc, _TRUNCATE);
// 	std::string strResult = CStr;
// 	free(CStr);
// 	setlocale(LC_ALL, strLocale.c_str());
// 	return strResult;
// }
// // string => wstring
// std::wstring String2WString(const std::string& s)
// {
// 	std::string strLocale = setlocale(LC_ALL, "");
// 	const char* chSrc = s.c_str();
// 	size_t nDestSize = 0;// mbstowcs_s(NULL, chSrc, 0) + 1;
// 	wchar_t*WStr;
// 	WStr = (wchar_t*)malloc(s.length() * sizeof(wchar_t));
// 
// 	mbstowcs_s(&nDestSize, WStr, s.length(), chSrc, _TRUNCATE);
// 
// 	std::wstring wstrResult = WStr;
// 	free(WStr);
// 	setlocale(LC_ALL, strLocale.c_str());
// 	return wstrResult;
// }
// 
// std::string WString2String(const std::wstring& ws)
// {
// 	std::string strLocale = setlocale(LC_ALL, "");
// 	const wchar_t* wchSrc = ws.c_str();
// 	size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
// 	char *chDest = new char[nDestSize];
// 	memset(chDest, 0, nDestSize);
// 	wcstombs(chDest, wchSrc, nDestSize);
// 	std::string strResult = chDest;
// 	delete[]chDest;
// 	setlocale(LC_ALL, strLocale.c_str());
// 	return strResult;
// }
// // string => wstring
// std::wstring String2WString(const std::string& s)
// {
// 	std::string strLocale = setlocale(LC_ALL, "");
// 	const char* chSrc = s.c_str();
// 	size_t nDestSize = mbstowcs(NULL, chSrc, 0) + 1;
// 	wchar_t* wchDest = new wchar_t[nDestSize];
// 	wmemset(wchDest, 0, nDestSize);
// 	mbstowcs(wchDest, chSrc, nDestSize);
// 	std::wstring wstrResult = wchDest;
// 	delete[]wchDest;
// 	setlocale(LC_ALL, strLocale.c_str());
// 	return wstrResult;
// }

string test()
{
	string str = "d，你safdasfs";
	return str;
}

void CCodeDemoVS2015Dlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here

	string str1 = test();
	const char *p1 = str1.c_str();
	MessageBoxA(NULL, p1, p1, NULL);
	return;
	time_t curTime;
	time(&curTime);
	tm tm1;
	localtime_s(&tm1, &curTime);
	//日志的名称如：201601012130.log
	std::string m_strLogName = FormatString("%04d-%02d-%02d.log", tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday);


	string strBuf = generate();

	istringstream iss("#123 1.23 aaa ,zzz kk,k oo.jjj");
	char ch;
	iss >> ch;
	MessageBoxA(NULL, iss.str().c_str(), NULL, NULL);

	const char *p = strBuf.c_str();
	return;
}


#include "Verifysignature.h"
void CCodeDemoVS2015Dlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	bool  bRet;

//	 bRet = IsFileSignedBySubject(L"C:\\Users\\liht\\Desktop\\install\\LenovoBrowser1.dll",TRUE);


	 bRet = IsSignature(L"C:\IotSdk.dll");

	BOOL f;
	return;
}


void CCodeDemoVS2015Dlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here

	std::wstring Str1 = L"23R1";
	std::wstring Str2 = L"23R";
	int nRet = std::wcscmp(Str1.c_str(), Str2.c_str());

	nRet = 0;

}

#include "UpdatePackage.h"

void CCodeDemoVS2015Dlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here

	BOOL bRet = DownLoadSmallFile( L"https://appdl.lenovo.com.cn/smartlenovotx/20180817/LenovoPcManager_SLBrowserTest.exe", L"D:\\LIHT\\1213https.exe");

	int i = 0;
	return;
}


void CCodeDemoVS2015Dlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here

	int nRet = isUsersPri();

	ostringstream buf;
	buf << "nRet=" << nRet << endl;


	MessageBoxA(NULL,buf.str().c_str(),NULL,NULL);
	return;
}


void CCodeDemoVS2015Dlg::OnBnClickedButton9()
{
	// TODO: Add your control notification handler code here
	char *pszVer1 = "1.0.1.2.4";
	char *pszVer2 = "1.0.2.1";
	int nRet = CmpVersionNum(pszVer1, pszVer2);
}


MyLog g_myLog;


CLogger gLogger;

#define OUTLOGERR(fm, ...)	g_myLog.traceEX(FALSE, __FILE__, __LINE__,fm,__VA_ARGS__)
#define OUTLOGINF(fm, ...)	g_myLog.traceEX(TRUE, __FILE__, __LINE__,fm,__VA_ARGS__)
DWORD WINAPI ThreadProFunc(LPVOID lpParam)
{
	int i = 0;
	for (i = 0; i < 100; i++)
	{
		OUTLOGINF("ThreadProFunc ClickedButton2");
		Sleep(200);
		OUTLOGERR("ThreadProFunc clickedButton 2 nRet = 0x%x ", GetLastError());
		Sleep(200);
		OUTLOGINF("ThreadProFunc End clickedButton2");
	}
	return 0;
}

	
void CCodeDemoVS2015Dlg::OnBnClickedButton10()
{
	// TODO: Add your control notification handler code here
	HANDLE hThread;
	DWORD dwThreadId;
//	g_myLog.Init("LogFile");
	hThread = CreateThread(NULL	// 默认安全属性
		, NULL		// 默认堆栈大小
		, ThreadProFunc // 线程入口地址
		, NULL	//传递给线程函数的参数
		, 0		// 指定线程立即运行
		, &dwThreadId	//线程ID号
	);
	int i = 0;
	for ( i = 0; i < 100; i++)
	{
		OUTLOGINF("Main ClickedButton2");
		Sleep(200);
		OUTLOGERR("Main clickedButton 2 nRet = 0x%x ", GetLastError());
		OUTLOGINF("Main End clickedButton2");
	}


	CloseHandle(hThread);	//关闭线程句柄，内核引用计数减一
}

void CCodeDemoVS2015Dlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	std::vector<string> resultVect;

	for (auto t = resultVect.begin(); t != resultVect.end(); t++)
	{
		string &tmp = *t;
	}
	return;
}
#include "openssl/x509.h"
#include "openssl/sha.h"
#include "openssl/md5.h"
#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")

void CCodeDemoVS2015Dlg::OnBnClickedButton11()
{
	// TODO: Add your control notification handler code here
	// 非对称

	RSA *r = NULL;
	int bits = 1024;
	unsigned long e = RSA_F4;

	r = RSA_generate_key(bits, e, NULL, NULL);

	FILE *fp=nullptr;
	fopen_s(&fp, "d:\\new_keys", "w");
	if (NULL == fp)
	{
		return;
	}

	RSA_print_fp(fp, r, 0);
	fclose(fp);

	unsigned char *n_b = (unsigned char *)calloc(RSA_size(r), sizeof(unsigned char));
	unsigned char *e_b = (unsigned char *)calloc(RSA_size(r), sizeof(unsigned char));


	int n_size = BN_bn2bin(r->n, n_b);
	int b_size = BN_bn2bin(r->e, e_b);	
	RSA *pubrsa = RSA_new();
	pubrsa->n = BN_bin2bn(n_b, n_size, NULL);			// 由字节数字 转为RSA
	pubrsa->e = BN_bin2bn(e_b, b_size, NULL);



	FILE *publicKey = NULL;
	fopen_s(&publicKey,"d:\\publicKey.pem", "wb");
	RSA_print_fp(publicKey, pubrsa,0);
	fclose(publicKey);




	int rsaSize = RSA_size(r);
	char szSrcData[1024] = { 0 };
	strcpy_s(szSrcData, 12, "1234");
	PBYTE input=(PBYTE)szSrcData; int inlen=strlen(szSrcData); 

	char szDecData[1024] = { 0 };
	
	PBYTE output=(PBYTE)szDecData;
	int ret = RSA_public_encrypt(inlen, input, output, r, RSA_PKCS1_PADDING);
	if (ret != rsaSize)
	{
		MessageBoxA(NULL, "err","err",NULL);
	}

	char szD[1024] = { 0 };
	input = (PBYTE)szD;
	RSA_private_decrypt(ret,  output,input, r, RSA_PKCS1_PADDING);

	return;
	// hash
	unsigned char md[33] = { 0 };
	unsigned char *p = nullptr;
	p = MD5((const unsigned char *)"hello", strlen("hello"), md);
	if (p==nullptr)
	{
		//ERR
	}
	SHA1((const unsigned char *)"hello", strlen("hello"), md);
	SHA256((const unsigned char *)"hello", strlen("hello"), md);
	
	

	int i = 0;
	char buf[65] = { 0 };
	char tmp[3] = { 0 };
	for (i = 0; i < 32; i++)
	{
		sprintf_s(tmp, "%02X", md[i]);
		strcat_s(buf, tmp);
	}

	printf("%s", buf);

	return;
}


#include "iostream"
#include <fstream>
BOOL ReadDataFromFile(string strFilePath,BYTE *pbData,DWORD *pdwLen)
{
	const char* pFilePath = strFilePath.c_str();
	if (! PathFileExistsA(pFilePath))
	{
		return FALSE;
	}

	ifstream is;
	DWORD dwLen;
	is.open(pFilePath, ios::binary);

	is.seekg(0, ios::end);
	dwLen = is.tellg();

	if (pbData == nullptr)
	{
		*pdwLen = dwLen;
		if (dwLen ==0)
		{
			return FALSE;
		}
		return TRUE;
	}
	if (*pdwLen<dwLen)
	{
		return FALSE;
	}
	is.seekg(0, ios::beg);
	is.read((char*)pbData, *pdwLen);
	is.close();

	return TRUE;
}

void testt()
{
	BYTE * buffer = nullptr;
	memcpy(buffer, "123", 4);
}
void CCodeDemoVS2015Dlg::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码


	return;
	DWORD length=0;
	BYTE * buffer = nullptr;
	testt();

	string strPath = "C:\\Users\\liht\\Downloads\\LenovoPcManager_SLBrowserTest.exe";

	BOOL bRet = ReadDataFromFile(strPath.c_str(),buffer,&length);

	buffer = new BYTE[length];

	bRet = ReadDataFromFile(strPath.c_str(), buffer, &length);


	delete[] buffer;
	buffer = nullptr;
}


void CCodeDemoVS2015Dlg::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码

	ASSERT(FALSE);			//debug 起作用
//	用法：ASSERT(expression)
//	解析：如果expression为真，则继续执行下面的程序；否则，暂停执行，并且弹出一个提示框。


	TRACE("11111111111111111");		// debug 起作用     debugview 查看
//		a.TRACE ，就是不带动态参数输出字符串, 类似C的printf("输出字符串")；
//		b.TRACE 中的字符串可以带参数输出, 类似C的printf("...%d", 变量)；


	OutputDebugStringA("11111");  // 输出信息到 debugview


	//    Ctrl + B  条件调试


}

void onFinish()
{
	OUTLOGINF("Chrild _ Thread__onFinish");
	MessageBoxA(NULL, "MainThread11111111111", "MainThread", NULL);
}

unsigned int __stdcall threadFunction2(void * object)
{
// 	Thread * thread = (Thread *)object;
// 	return (unsigned int)thread->run(thread->param);
	OUTLOGINF("Chrild _ Thread");
	MessageBoxA(NULL, "MainThread11111111111", "MainThread", NULL);
	return 0;
}


#include<thread>
void fun(int a) {
	a++;

	char szT[128];
	sprintf_s(szT, "SZT:A=%d", a);
	MessageBoxA(nullptr, szT, szT, NULL);
}

#include "MyThread.h"
void CCodeDemoVS2015Dlg::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码

// 	unsigned int threadID;
// 	HANDLE threadHandle = (HANDLE)_beginthreadex(NULL, 0, threadFunction2, this, 0, &threadID);
//	WaitForSingleObject(threadHandle, INFINITE);
	int a = 0;
 	std::thread t1(fun,a);	  // 为什么崩溃呢
 	t1.join();		// 等待线程结束  或者使用 
	
	std::thread t2;
	t2 = std::thread(fun,a);	
	t2.detach();		//使线程和主进程分离 
	return;


	char szT[128] = { "SDFA" };
	MyThread t;
	typedef void fonFinish();
	fonFinish *fun = onFinish;
	t.setOnFinish(fun);
	t.run((void*)szT);
	t.join();
//	t.detach();

	OUTLOGINF("MainThread_OnBnClickedButton15");
	MessageBoxA(NULL, "MainThread", "MainThread", NULL);
	int i = 0;
}

#include "HttpHelp.h"


static void ev_handlerE(struct mg_connection *c, int ev, void *p) {
	if (ev == MG_EV_HTTP_REPLY) {
		c->flags |= MG_F_CLOSE_IMMEDIATELY;
		struct http_message *hm = (struct http_message *) p;
		fwrite(hm->message.p, 1, hm->message.len, stdout);
		putchar('\n');
		exit_flag = 1;
	}
	else if (ev == MG_EV_CLOSE) {
		exit_flag = 1;
	};
}

static const char *URL = "http://www.baidu.com";

#include "http_client.h"

void handle_func(std::string rsp)
{
	// do sth according to rsp
	std::cout << "http rsp1: " << rsp << std::endl;
}


void CCodeDemoVS2015Dlg::OnBnClickedButton16()
{
	// TODO: 在此添加控件通知处理程序代码

	std::string url1 = "http://www.baidu.com";
	HttpClient::SendReq(url1, handle_func);

	std::string url2 = "https://www.baidu.com";
	HttpClient::SendReq(url2, [](std::string rsp) {
		std::cout << "http rsp2: " << rsp << std::endl;
	});




	struct mg_mgr mgr;

	mg_mgr_init(&mgr, NULL);
	mg_connect_http(&mgr, ev_handlerE, URL, NULL, NULL);


	while (exit_flag == 0) {
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);

}