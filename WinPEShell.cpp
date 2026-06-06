#include <windows.h>
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

#define ID_BTN_INSTALL   1001
#define ID_BTN_EXPLORER  1002
#define ID_BTN_CMD       1003
#define ID_BTN_DISKPART  1004
#define ID_BTN_NET       1005
#define ID_BTN_TOUCH     1006
#define ID_TIMER_REFRESH 2001

HINSTANCE g_hInst;
HWND g_hLog;

void Log(LPCSTR txt)
{
    int len = GetWindowTextLength(g_hLog);
    SendMessage(g_hLog, EM_SETSEL, len, len);
    SendMessage(g_hLog, EM_REPLACESEL, FALSE, (LPARAM)txt);
    SendMessage(g_hLog, EM_REPLACESEL, FALSE, (LPARAM)"\r\n");
}
void Run(LPCSTR cmd){Log(cmd);WinExec(cmd, SW_SHOW);}
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CREATE:
        {
			CreateWindow("button","Installation",WS_CHILD | WS_VISIBLE,2, 2, 90, 25,hWnd, (HMENU)ID_BTN_INSTALL, g_hInst, NULL);
			CreateWindow("button","Explorateur",WS_CHILD | WS_VISIBLE,100, 2, 90, 25,hWnd, (HMENU)ID_BTN_EXPLORER, g_hInst, NULL);
			CreateWindow("button","Terminal",WS_CHILD | WS_VISIBLE,200, 2, 90, 25,hWnd, (HMENU)ID_BTN_CMD, g_hInst, NULL);
			CreateWindow("button","Diskpart",WS_CHILD | WS_VISIBLE,300, 2, 90, 25,hWnd, (HMENU)ID_BTN_DISKPART, g_hInst, NULL);
			CreateWindow("button","Réseau",WS_CHILD | WS_VISIBLE,400, 2, 90, 25,hWnd, (HMENU)ID_BTN_NET, g_hInst, NULL);
			CreateWindow("button","Touche",WS_CHILD | WS_VISIBLE,500, 2, 90, 25,hWnd, (HMENU)ID_BTN_TOUCH, g_hInst, NULL);
            g_hLog = CreateWindowEx(
                WS_EX_CLIENTEDGE|WS_EX_DLGMODALFRAME, "EDIT", "",
                WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                5, 40, 630, 350,
                hWnd, NULL, g_hInst, NULL);

            SetTimer(hWnd, ID_TIMER_REFRESH, 3000, NULL);
        }
        break;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case ID_BTN_INSTALL:
            Run("X:\\Tools\\AutoInstall.cmd");
            break;

        case ID_BTN_EXPLORER:
            Run("explorer.exe");
            break;

        case ID_BTN_CMD:
            Run("cmd.exe");
            break;

        case ID_BTN_DISKPART:
            Run("diskpart.exe");
            break;

        case ID_BTN_NET:
            Run("cmd.exe /k ipconfig & ping 8.8.8.8");
            break;

        case ID_BTN_TOUCH:
            Run("X:\\Tools\\TouchTest.exe");
            break;
        }
        break;
		case WM_SIZE:{
			RECT rc;
			GetClientRect(hWnd,&rc);
			MoveWindow(g_hLog,2,32,rc.right-4,rc.bottom-50,true);
					 }break;

    case WM_TIMER:
        if (wParam == ID_TIMER_REFRESH)
            Log("Recherche réseau...");
        break;

    case WM_DESTROY:
        KillTimer(hWnd, ID_TIMER_REFRESH);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    g_hInst = hInstance;
    InitCommonControls();
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "WinPEShell";
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE+1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon=LoadIcon(wc.hInstance,(LPCTSTR)102);
    RegisterClass(&wc);
    HWND hWnd = CreateWindow("WinPEShell","Shell graphique pour WinPE - Outils & Installation",WS_OVERLAPPEDWINDOW | WS_MAXIMIZEBOX,CW_USEDEFAULT, CW_USEDEFAULT,650, 450,NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}
