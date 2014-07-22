#include <windows.h>
#define T_TIMERS (WM_USER + 1)

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
void Count(static int *iHourH, static int *iHourL, static int *iMiniteH, static int *iMiniteL, static int *iSecondH, static int *iSecondL);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	                    PSTR szCmdLine, int iCmdShow)
{
    TCHAR szClassName[] = TEXT("HelloWin");
	RECT rect;
    HWND hwnd;          //窗口句柄
	MSG msg;           //消息结构
	WNDCLASS wndclass;      //窗口类结构
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szClassName;

//如果注册窗口失败，弹出错误对话框
	if (!RegisterClass (&wndclass))
    {
		MessageBox (NULL, TEXT ("注册窗口失败"), 
			           szClassName, MB_ICONERROR);     
		return 0;
	}
	rect.top = 0;
	rect.left = 0;
	rect.right = 300 + 2 * GetSystemMetrics(SM_CXFRAME);
	rect.bottom = 300 + GetSystemMetrics(SM_CYFRAME) + GetSystemMetrics(SM_CYCAPTION);

//创建窗口
	hwnd = CreateWindow (szClassName, TEXT ("计时器"),
                         WS_CAPTION | WS_SYSMENU,							 // 窗口类型
						 (int)(GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2,	// 窗口的左上角的x坐标
						 (int)(GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2,// 窗口的左上角的y坐标
						 rect.right,												 // 窗口的宽度
						 rect.bottom,										// 窗口的高度
						 NULL,										 // 父窗口句柄
						 NULL,										 // 窗口菜单句柄
						 hInstance,									 // 程序实例句柄
						 NULL);

	ShowWindow (hwnd, iCmdShow);  //显示窗口
	UpdateWindow (hwnd);          //重画显示区域

//消息循环，用于从消息队列中取出消息，并做相应处理
    while (GetMessage (&msg, NULL, 0, 0))
    {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int iSecondH = 0;	// 计秒
	static int iSecondL =0;
	static int iMiniteH = 0; // 计分
	static int iMiniteL = 0;
	static int iHourH = 0;	// 计时
	static int iHourL = 0;
	static int iSetTimer;	// 指示是否安装了定时器
	RECT rect;
	TCHAR szText[56];
	PAINTSTRUCT ps;
	HDC hdc;

	GetClientRect(hWnd, &rect);

	switch(message)
	{
	case WM_CREATE:		// 窗口正在被创建
		iSetTimer = FALSE;
		break;
	case WM_PAINT:		// 窗口客户区需要重画
		hdc = BeginPaint(hWnd, &ps);
		wsprintfW(szText, TEXT("%d%d:%d%d:%d%d"), iHourH, iHourL, iMiniteH, iMiniteL, iSecondH, iSecondL);
		TextOut(hdc, rect.right / 2, rect.bottom / 2, szText, lstrlen(szText));
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:		// 定时器时间已到
		if(wParam == T_TIMERS)
		{
			hdc = GetDC(hWnd);
			Count(&iHourH, &iHourL, &iMiniteH, &iMiniteL, &iSecondH, &iSecondL);
			wsprintfW(szText, TEXT("%d%d:%d%d:%d%d"), iHourH, iHourL, iMiniteH, iMiniteL, iSecondH, iSecondL);
			
			TextOut(hdc,rect.right / 2,rect.bottom / 2, szText, lstrlen(szText));

			// 发一声“嘟”的声音
			MessageBeep(MB_OK);
		}
		break;
	case WM_LBUTTONDOWN:	// 用户单击鼠标左键
		if(iSetTimer)
		{
			// 插销一个已经安装的定时器
			KillTimer(hWnd, T_TIMERS);
			iSetTimer = FALSE;
		}
		else
		{
			// 安装一个时间周期为250ms的定时器
			if(SetTimer(hWnd, T_TIMERS, 1000, NULL) == 0)
			// SetTimer函数调用成功会返回新的定时器的ID号，失败的话返回0
			{
				MessageBox(hWnd,TEXT( "安装定时器失败！"), TEXT("03Timer"), MB_OK);
			}
			else
			{
				iSetTimer = TRUE;
			}
		}
		break;
	case WM_CLOSE:		// 用户要求关闭窗口
		if(iSetTimer)
			KillTimer(hWnd, T_TIMERS);
		break;
	case WM_DESTROY:	// 窗口正在被销毁
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Count(static int *iHourH, static int *iHourL, static int *iMiniteH, static int *iMiniteL, static int *iSecondH, static int *iSecondL)
{
	(*iSecondL)++;
	if ((*iSecondL) == 10)
	{
		*iSecondL = 0;
		(*iSecondH)++;
	}
	if ((*iSecondH) == 6)
	{
		*iSecondH = 0;
		(*iMiniteL)++;
	}
	if ((*iMiniteL) == 10)
	{
		*iMiniteL = 0;
		(*iMiniteH)++;
	}
	if ((*iMiniteH) == 6)
	{
		*iMiniteH = 0;
		(*iHourL)++;
	}
	if((*iHourL) == 10)
	{
		*iHourL = 0;
		(*iHourH)++;
	}
}
