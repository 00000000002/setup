// setuptop.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
#include <Windows.h>
#include <cstring>
#include <cstdlib> // 添加头文件
#include <iostream>
#include <thread>
#include <map>
#include <set>

using namespace std;

wstring getTitle(HWND handler);


void setWindowTop(HWND handler);
void windowSwitch(int winId);
void cleanup();
enum class hotKey { setTop };
void cleanup(HWND handler);
HWND thisWindow;

wchar_t appTitle[] = L"窗口置顶器";

// 清理函数，用于在程序退出时执行清理逻辑
void cleanup() {
    setWindowTop(GetForegroundWindow());
    // 取消置顶
}

int main() {


    cout<<("按下 ALT + T 使当前窗口置顶,再次按下使当前窗口取消置顶")<<endl;
    thisWindow = GetConsoleWindow();
    if (thisWindow != INVALID_HANDLE_VALUE) {
        auto appMutex = CreateMutexW(NULL, TRUE, appTitle);
        auto lasterr = GetLastError();
        ERROR_INVALID_HANDLE;
        if (lasterr != ERROR_SUCCESS) {
            auto prevInstance = FindWindowW(NULL, appTitle);
            ShowWindow(prevInstance, SW_SHOW);
            BringWindowToTop(prevInstance);
            SetFocus(prevInstance);
            SetForegroundWindow(prevInstance);
            SetActiveWindow(prevInstance);
            exit(-1);
        }
        SetWindowTextW(thisWindow, appTitle);
        atexit(cleanup);
    }

    {
        auto check = RegisterHotKey(NULL, int(hotKey::setTop), MOD_ALT | MOD_NOREPEAT, 'T');
        if (check == false) {
            exit(1);
        }
    }

    // 注册退出处理函数


    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0) != 0) {
        if (msg.message == WM_HOTKEY) {
            if (msg.wParam == int(hotKey::setTop)) {
                setWindowTop(GetForegroundWindow());
                continue;
            }
        }
    }

    return 0;
}



void setWindowTop(HWND handler) {
    static set<HWND> topSet;
    bool isTop = topSet.count(handler);

    if (isTop) {
        topSet.erase(handler);
        cout << "窗口已经取消置顶 当前窗口是" << appTitle<<thisWindow << endl;
    }
    else {
        topSet.insert(handler);
        cout << "当前窗口已经置顶" << appTitle<<thisWindow<<endl;
    }
 
    

    HWND zLevel = isTop ? HWND_BOTTOM : HWND_TOPMOST;
    UINT flags = SWP_NOMOVE | SWP_NOSIZE;

    SetWindowPos(handler, zLevel, 0, 0, 0, 0, flags);
    // get back window from bottom
    BringWindowToTop(handler);
}

BOOL WINAPI consoleCtrlHandler(DWORD signal) {
    if (signal == CTRL_CLOSE_EVENT || signal == CTRL_C_EVENT || signal == CTRL_BREAK_EVENT) {
        cleanup();
    }
    return FALSE;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
