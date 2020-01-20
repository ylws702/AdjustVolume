// AdjustVolume.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <tchar.h>
#include <cpr/cpr.h>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 

enum MyEnum
{
    volumeUpID = 0x12345678,
    volumeDownID,
    volumeMuteID,
    mediaNextID,
    mediaPreviousID,
    mediaPlayPauseID,
    startEverytingID,
    keepLoginID,
};


#define ShowErrorAndExit(dwErrorCode)                       \
{                                                           \
    TCHAR msgBuff[500];                                     \
    FormatMessage(                                          \
        FORMAT_MESSAGE_FROM_SYSTEM,                         \
        nullptr,                                            \
        dwErrorCode,                                        \
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),          \
        msgBuff,                                            \
        500,                                                \
        nullptr);                                           \
    MessageBox(nullptr, msgBuff, _T("AdjustVolume"), 0);    \
    exit(0);                                                \
}


#define PressKey(bVk)                                       \
{                                                           \
    keybd_event(bVk,                                        \
        MapVirtualKey(bVk, 0),                              \
        KEYEVENTF_EXTENDEDKEY,                              \
        0);                                                 \
                                                            \
    keybd_event(bVk,                                        \
        MapVirtualKey(bVk, 0),                              \
        KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,            \
        0);                                                 \
}

int main(void)
{
    if (!RegisterHotKey(
        nullptr,
        volumeUpID,
        MOD_CONTROL | MOD_SHIFT,
        VK_OEM_PLUS))
    {
        ShowErrorAndExit(GetLastError());
    }

    if (!RegisterHotKey(nullptr,
        volumeDownID,
        MOD_CONTROL | MOD_SHIFT,
        VK_OEM_MINUS))
    {
        ShowErrorAndExit(GetLastError());
    }

    if (!RegisterHotKey(nullptr,
        volumeDownID,
        MOD_CONTROL | MOD_SHIFT,
        '0'))
    {
        ShowErrorAndExit(GetLastError());
    }

    if (!RegisterHotKey(nullptr,
        mediaNextID,
        MOD_CONTROL | MOD_SHIFT,
        VK_OEM_PERIOD))
    {
        ShowErrorAndExit(GetLastError());
    }

    if (!RegisterHotKey(nullptr,
        mediaPreviousID,
        MOD_CONTROL | MOD_SHIFT,
        VK_OEM_COMMA))
    {
        ShowErrorAndExit(GetLastError());
    }

    if (!RegisterHotKey(nullptr,
        mediaPlayPauseID,
        MOD_CONTROL | MOD_SHIFT,
        VK_OEM_2))
    {
        ShowErrorAndExit(GetLastError());
    }

    if (!RegisterHotKey(nullptr,
        startEverytingID,
        MOD_CONTROL | MOD_SHIFT,
        VK_NUMPAD0))
    {
        ShowErrorAndExit(GetLastError());
    }

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        switch (msg.message)
        {
        case WM_HOTKEY:
            switch (msg.wParam)
            {
            case volumeUpID:
                PressKey(VK_VOLUME_UP);
                break;
            case volumeDownID:
                PressKey(VK_VOLUME_DOWN);
                break;
            case volumeMuteID:
                PressKey(VK_VOLUME_MUTE);
                break;
            case mediaNextID:
                Sleep(300);
                PressKey(VK_MEDIA_NEXT_TRACK);
                break;
            case mediaPreviousID:
                Sleep(300);
                PressKey(VK_MEDIA_PREV_TRACK);
                break;
            case mediaPlayPauseID:
                Sleep(300);
                PressKey(VK_MEDIA_PLAY_PAUSE);
                break;
            case startEverytingID:
            {
                SHELLEXECUTEINFO shExInfo = { 0 };
                shExInfo.cbSize = sizeof(shExInfo);
                shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
                shExInfo.hwnd = 0;
                shExInfo.lpVerb = _T("runas");
                shExInfo.lpFile = _T(""); //Everything Path
                shExInfo.lpParameters = _T("");
                shExInfo.lpDirectory = 0;
                shExInfo.nShow = SW_SHOW;
                shExInfo.hInstApp = 0;

                if (ShellExecuteEx(&shExInfo))
                {
                    WaitForSingleObject(shExInfo.hProcess, INFINITE);
                    CloseHandle(shExInfo.hProcess);
                }
                else
                {
                    ShowErrorAndExit(GetLastError());
                }
            }
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
