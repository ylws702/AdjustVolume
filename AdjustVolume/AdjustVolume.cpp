// AdjustVolume.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <tchar.h>
#include <bluetoothapis.h>
#pragma comment(lib,"Bthprops.lib") 

#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 

const enum ID
{
    volumeUpID = 0x12345678,
    volumeDownID,
    volumeMuteID,
    mediaNextID,
    mediaPreviousID,
    mediaPlayPauseID,
    bluetoothScanID,
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

void Register(void)
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
        bluetoothScanID,
        MOD_SHIFT | MOD_ALT,
        'Z'))
    {
        ShowErrorAndExit(GetLastError());
    }
}

void BluetoothScan(void)
{
    HBLUETOOTH_RADIO_FIND hbf = nullptr;
    HANDLE hbr = nullptr;
    HBLUETOOTH_DEVICE_FIND hbdf = nullptr;
    //调用BluetoothFindFirstDevice搜索本机蓝牙收发器所需要的搜索参数对象 
    BLUETOOTH_FIND_RADIO_PARAMS btfrp =
    {
        sizeof(BLUETOOTH_FIND_RADIO_PARAMS)
    };
    //初始化一个储存蓝牙收发器信息（BLUETOOTH_RADIO_INFO）的对象bri
    BLUETOOTH_RADIO_INFO bri = { sizeof(BLUETOOTH_RADIO_INFO) };
    //调用BluetoothFindFirstDevice搜索本所需要的搜索参数对象 
    BLUETOOTH_DEVICE_SEARCH_PARAMS btsp =
    { 
        sizeof(BLUETOOTH_DEVICE_SEARCH_PARAMS) 
    };
    //初始化一个远程蓝牙设备信息（BLUETOOTH_DEVICE_INFO）对象btdi，以储存搜索到的蓝牙设备信息
    BLUETOOTH_DEVICE_INFO btdi = { sizeof(BLUETOOTH_DEVICE_INFO) };
    //得到第一个被枚举的蓝牙收发器的句柄hbf可用于BluetoothFindNextRadio，
    //hbr可用于BluetoothFindFirstDevice。若没有找到本机的蓝牙收发器，则得到的句柄hbf=NULL，
    //具体可参考https://msdn.microsoft.com/en-us/library/aa362786(v=vs.85).aspx 
    hbf = BluetoothFindFirstRadio(&btfrp, &hbr);

    if (hbf == nullptr)
    {
        ShowErrorAndExit(GetLastError());
    }
    //获取蓝牙收发器的信息，储存在bri中  
    if (BluetoothGetRadioInfo(hbr, &bri) == ERROR_SUCCESS)
    {
        //设置执行搜索设备所在的句柄，应设为执行BluetoothFindFirstRadio函数所得到的句柄
        btsp.hRadio = hbr;
        //是否搜索已配对的设备  
        btsp.fReturnAuthenticated = TRUE;
        //是否搜索已连接的设备 
        btsp.fReturnConnected = TRUE;
        //是否搜索已记忆的设备  
        btsp.fReturnRemembered = TRUE;
        //是否搜索未知设备 
        btsp.fReturnUnknown = TRUE;
        //是否重新搜索，True的时候会执行新的搜索，时间较长，FALSE的时候会直接返回上次的搜索结果。
        btsp.fIssueInquiry = TRUE;
        //指示查询超时的值，以1.28秒为增量
        //例如，12.8秒的查询的cTimeoutMultiplier值为10.此成员的最大值为48
        //当使用大于48的值时，调用函数立即失败并返回 
        btsp.cTimeoutMultiplier = 1;
        //通过找到第一个设备得到的HBLUETOOTH_DEVICE_FIND句柄hbdf来枚举远程蓝牙设备，
        //搜到的第一个远程蓝牙设备的信息储存在btdi对象中。
        //若没有远程蓝牙设备，hdbf=NULL。  
        hbdf = BluetoothFindFirstDevice(&btsp, &btdi);
        //bool bfind = hbdf != NULL;
        //while (bfind)
        //{
        //    //wcout << "[Name]:" << btdi.szName;  //远程蓝牙设备的名字
        //    //wcout << ",[Address]:0x" << uppercase 
        //    //<< hex << btdi.Address.ullLong << endl;
        //    //通过BluetoothFindFirstDevice得到的HBLUETOOTH_DEVICE_FIND句柄
        //    //来枚举搜索下一个远程蓝牙设备，并将远程蓝牙设备的信息储存在btdi中  
        //    bfind = BluetoothFindNextDevice(hbdf, &btdi);
        //}
        //使用完后记得关闭HBLUETOOTH_DEVICE_FIND句柄hbdf。  
        BluetoothFindDeviceClose(hbdf);
    }
    else
    {
        ShowErrorAndExit(GetLastError());
    }
    CloseHandle(hbr);
    //使用完后记得关闭HBLUETOOTH_RADIO_FIND句柄hbf。  
    BluetoothFindRadioClose(hbf);
}

int main(void)
{
    Register();

    BluetoothScan();

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
            case bluetoothScanID:
                BluetoothScan();
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
