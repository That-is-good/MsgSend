// MsgSend.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "MsgSend.h"
#include <vector>
#include <xstring>

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Insert(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    Append(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    Edited(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MSGSEND, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MSGSEND));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MSGSEND));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MSGSEND);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_TILED,
      CW_USEDEFAULT, CW_USEDEFAULT, 600, 300, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//

void CMDUNSHOW(LPTSTR arg) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(NULL,
        arg,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NO_WINDOW,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        std::wstring temp;
        temp += TEXT("窗口创建失败,错误代码: ");
        temp += (int)GetLastError();
        MessageBox(NULL, temp.c_str(), TEXT("错误"), MB_ICONERROR);
        return;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

#define COUNTKONGJIAN 6
HWND KongJian[COUNTKONGJIAN];
std::vector<std::wstring> IPS;
std::vector<std::wstring> TEXTS;
std::vector<std::wstring> COUNTS;
std::vector<bool> IPSURE;
std::vector<std::wstring> TIMES;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HFONT FRONTWEIRAN;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int wmEvent = LOWORD(lParam);
            int selected = (int)SendMessage(KongJian[3], LB_GETCURSEL, 0, 0);
            // 分析菜单选择:
            switch (wmId)
            {
            case 1:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Append);
                break;
            case 2:
                {
                    SendMessage(KongJian[3], LB_DELETESTRING, selected, 0);
                    if (selected < 0) break;
                    IPS.erase(IPS.begin() + selected);
                    TEXTS.erase(TEXTS.begin() + selected);
                    COUNTS.erase(COUNTS.begin() + selected);
                    IPSURE.erase(IPSURE.begin() + selected);
                    TIMES.erase(TIMES.begin() + selected);
                }
                break;
            case 3:
                for (int i = 0; i < (int)SendMessage(KongJian[3], LB_GETCOUNT, 0, 0); i++)
                {
                    std::wstring temp = TEXT("msg ");
                    IPSURE[i] ? temp += TEXT("/SERVER:\"") : temp += TEXT("\"");
                    temp += IPS[i];
                    temp += TEXT("\" /TIME:\"");
                    temp += TIMES[i];
                    IPSURE[i] ? temp += TEXT("\" * \"") : temp += TEXT("\" \"");
                    temp += TEXTS[i]; temp += TEXT("\"");
                    int tp = _ttoi(COUNTS[i].c_str());
                    //MessageBox(NULL, temp.c_str(), TEXT("Your Message"), MB_ICONINFORMATION);
                    for (int j = 0; j < tp; j++)
                        CMDUNSHOW((LPTSTR)temp.c_str());
                }
                break;
            case 4:
                if (selected < 0) break;
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Edited);
                break;
            case 5:
                if (selected < 0) break;
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Insert);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_CREATE:
        {
        FRONTWEIRAN = CreateFont(-14, -7, 0, 0, DEFAULT_CHARSET, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE,
            TEXT("微软雅黑"));
        KongJian[0] = CreateWindow(
            TEXT("BUTTON"),   // predefined class  
            TEXT("添加"),       // button text  
            WS_VISIBLE | WS_CHILD | WS_BORDER,  // styles  
            10,         // starting x position  
            25,         // starting y position  
            100,        // button width  
            30,        // button height  
            hWnd,       // parent window  
            (HMENU)1,       // No menu  
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
            NULL);
        KongJian[1] = CreateWindow(
            TEXT("BUTTON"),   // predefined class  
            TEXT("删除"),       // button text  
            WS_VISIBLE | WS_CHILD | WS_BORDER,  // styles  
            10,         // starting x position  
            65,         // starting y position  
            100,        // button width  
            30,        // button height  
            hWnd,       // parent window  
            (HMENU)2,       // No menu  
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
            NULL);
        KongJian[2] = CreateWindow(
            TEXT("BUTTON"),   // predefined class  
            TEXT("开始"),       // button text  
            WS_VISIBLE | WS_CHILD | WS_BORDER,  // styles  
            10,         // starting x position  
            105,         // starting y position  
            100,        // button width  
            30,        // button height  
            hWnd,       // parent window  
            (HMENU)3,       // No menu  
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
            NULL);
        KongJian[3] = CreateWindow(
            TEXT("LISTBOX"),   // predefined class  
            TEXT(""),       // button text  
            WS_VISIBLE | WS_CHILD | WS_BORDER,  // styles  
            120,
            10,
            450,
            240,
            hWnd,
            NULL,
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
            NULL);
        KongJian[4] = CreateWindow(
            TEXT("BUTTON"),   // predefined class  
            TEXT("修改"),       // button text  
            WS_VISIBLE | WS_CHILD | WS_BORDER,  // styles  
            10,         // starting x position  
            145,         // starting y position  
            100,        // button width  
            30,        // button height  
            hWnd,       // parent window  
            (HMENU)4,       // No menu  
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
            NULL);
        KongJian[5] = CreateWindow(
            TEXT("BUTTON"),   // predefined class  
            TEXT("插入"),       // button text  
            WS_VISIBLE | WS_CHILD | WS_BORDER,  // styles  
            10,         // starting x position  
            185,         // starting y position  
            100,        // button width  
            30,        // button height  
            hWnd,       // parent window  
            (HMENU)5,       // No menu  
            (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
            NULL);
        }
        for (size_t i = 0; i < COUNTKONGJIAN; i++)
            SendMessage(KongJian[i], WM_SETFONT, (WPARAM)FRONTWEIRAN, NULL);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
// "添加"
//BST_CHECKED
TCHAR Texttemp02[65535];
INT_PTR CALLBACK Insert(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    int MAX = (int)SendMessage(KongJian[3], LB_GETCURSEL, 0, 0);
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDOK) {
            TCHAR temp[810];
            TCHAR temp01[255];
            TCHAR temp03[255];
            TCHAR temp04[255];
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT2), temp01, 255);
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT1), Texttemp02, 65535);
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT3), temp03, 255);
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT4), temp04, 255);
            IPS.insert(IPS.begin() + MAX, temp01);
            TEXTS.insert(TEXTS.begin() + MAX, Texttemp02);
            COUNTS.insert(COUNTS.begin() + MAX, temp03);
            IPSURE.insert(IPSURE.begin() + MAX,(IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED));
            TIMES.insert(TIMES.begin() + MAX, temp04);
            wsprintf(temp, TEXT("[USER]:%s | [TEXT]:%s | [COUNT]:%s | [TIME]:%s"), temp01, Texttemp02, temp03, temp04);
            SendMessage(KongJian[3], LB_INSERTSTRING, MAX, (LPARAM)temp);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        //else if (LOWORD(wParam) == IDC_EDIT4 && GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT4)) <= -1) {
        //    SetWindowText(GetDlgItem(hDlg, IDC_EDIT4), TEXT("0"));
        //}
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Append(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDOK) {
            TCHAR temp[810];
            TCHAR temp01[255];
            TCHAR temp03[255];
            TCHAR temp04[255];
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT2), temp01, 255);
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT1), Texttemp02, 65535);
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT3), temp03, 255);
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT4), temp04, 255);
            IPS.push_back(temp01);
            TEXTS.push_back(Texttemp02);
            COUNTS.push_back(temp03);
            IPSURE.push_back((IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED));
            TIMES.push_back(temp04);
            wsprintf(temp, TEXT("[USER]:%s | [TEXT]:%s | [COUNT]:%s | [TIME]:%s"), temp01, Texttemp02, temp03, temp04);
            SendMessage(KongJian[3], LB_ADDSTRING, 0, (LPARAM)temp);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        //else if (LOWORD(wParam) == IDC_EDIT4 && GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT4)) <= -1) {
        //    SetWindowText(GetDlgItem(hDlg, IDC_EDIT4), TEXT("0"));
        //}
        break;
    }
    return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Edited(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDOK) {
            int selected = (int)SendMessage(KongJian[3], LB_GETCURSEL, 0, 0);
            TCHAR temp[810];
            TCHAR temp01[255];
            TCHAR temp03[255];
            TCHAR temp04[255];
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT2), temp01, 255);
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT1), Texttemp02, 65535);
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT3), temp03, 255);
            GetWindowText(GetDlgItem(hDlg, IDC_EDIT4), temp04, 255);
            IPS[selected] = temp01;
            TEXTS[selected] = Texttemp02;
            COUNTS[selected] = temp03;
            IPSURE[selected] = (IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED);
            TIMES[selected] = temp04;
            wsprintf(temp, TEXT("[USER]:%s | [TEXT]:%s | [COUNT]:%s | [TIME]:%s"), temp01, Texttemp02, temp03, temp04);
            SendMessage(KongJian[3], LB_DELETESTRING, selected, 0);
            SendMessage(KongJian[3], LB_INSERTSTRING, selected,(LPARAM)temp);
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        //else if (LOWORD(wParam) == IDC_EDIT4 && GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT4)) <= -1) {
        //    SetWindowText(GetDlgItem(hDlg, IDC_EDIT4), TEXT("0"));
        //}
        break;
    }
    return (INT_PTR)FALSE;
}
// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
