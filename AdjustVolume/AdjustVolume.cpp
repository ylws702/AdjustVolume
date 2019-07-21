// AdjustVolume.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <tchar.h>

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 

const int volumeUpID = 0x12345678;
const int volumeDownID = 0x12345679;
const int volumeMuteID = 0x12345670;

void ShowErrorAndExit(DWORD dwErrorCode)
{
    TCHAR msgBuff[500];
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr,
        dwErrorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        msgBuff,
        500,
        nullptr);
    MessageBox(nullptr, msgBuff, _T("AdjustVolume"), 0);
    exit(0);
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
                keybd_event(VK_VOLUME_UP,
                    MapVirtualKey(VK_VOLUME_UP, 0),
                    KEYEVENTF_EXTENDEDKEY,
                    0);

                keybd_event(VK_VOLUME_UP,
                    MapVirtualKey(VK_VOLUME_UP, 0),
                    KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                    0);
                break;
            case volumeDownID:
                keybd_event(VK_VOLUME_DOWN,
                    MapVirtualKey(VK_VOLUME_DOWN, 0),
                    KEYEVENTF_EXTENDEDKEY,
                    0);

                keybd_event(VK_VOLUME_DOWN,
                    MapVirtualKey(VK_VOLUME_DOWN, 0),
                    KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                    0);
                break;
            case volumeMuteID:
                keybd_event(VK_VOLUME_MUTE,
                    MapVirtualKey(VK_VOLUME_MUTE, 0),
                    KEYEVENTF_EXTENDEDKEY,
                    0);

                keybd_event(VK_VOLUME_MUTE,
                    MapVirtualKey(VK_VOLUME_MUTE, 0),
                    KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                    0);
                break;
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
