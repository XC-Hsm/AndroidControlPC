#include <winsock2.h>
#include <ws2tcpip.h>
#include <Winuser.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "27015"
#define MAXCLIENTS 5
sockaddr_in addrClient;
typedef struct message
{

    SOCKET client;
    int length;
    char mes[DEFAULT_BUFLEN];
} message, *messagePtr;

DWORD WINAPI MyEvent(LPVOID para);

DWORD WINAPI AnswerClientThread(LPVOID lparam)
{
    char buf[DEFAULT_BUFLEN];
    int retVal;
    SOCKET Client = (SOCKET)lparam;

    int keepAlive = 1;
    int keepIdle = 60;
    int keepInterval = 5;
    int keepCount = 3;

    setsockopt(Client, SOL_SOCKET, SO_KEEPALIVE, (char *)&keepAlive, sizeof(keepAlive));
    setsockopt(Client, IPPROTO_TCP, TCP_KEEPIDLE, (char *)&keepIdle, sizeof(keepIdle));
    setsockopt(Client, IPPROTO_TCP, TCP_KEEPINTVL, (char *)&keepInterval, sizeof(keepInterval));
    setsockopt(Client, IPPROTO_TCP, TCP_KEEPCNT, (char *)&keepCount, sizeof(keepCount));
    int retTime = 0;

    while (true)
    {
        ZeroMemory(buf, DEFAULT_BUFLEN);

        retVal = recv(Client, buf, DEFAULT_BUFLEN, 0);
        if (SOCKET_ERROR == retVal)
        {

            int err = WSAGetLastError();

            if (err == WSAEWOULDBLOCK)
            {

                Sleep(100);
                retTime = retTime + 100;
                continue;
            }
            else if (err == WSAETIMEDOUT || err == WSAENETDOWN || err == WSAECONNRESET)
            {
                printf("connect failed!\n");
                closesocket(Client);
                return -1;
            }
            else
            {
                printf("ERROR %d", err);
                return -1;
            }
        }
        if (retVal > 0)
        {
            messagePtr mesp = new message;
            memcpy(mesp->mes, buf, DEFAULT_BUFLEN);
            mesp->client = Client;
            mesp->length = retVal;
            retTime = 0;
            HANDLE th = CreateThread(NULL, 0, MyEvent, mesp, 0, NULL);

            // Echo the buffer back to the sender
        }
        else
        {
            Sleep(100);
            retTime = retTime + 100;
            if (retTime >= 1000 * 60)
            {
                return -1;
            }
        }
    }
}

int MySetClipBoard(char *str)
{
    int ilen = strlen(str);
    wchar_t *Lstr = new wchar_t[ilen];
    MultiByteToWideChar(CP_ACP, 0, str, -1, Lstr, ilen * 2);
    LPWSTR cwdBuffer = (LPWSTR)(LPCWSTR)Lstr;
    DWORD len = wcslen(cwdBuffer);
    HGLOBAL hdst;
    LPWSTR dst;
    hdst = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (len + 1) * sizeof(WCHAR));
    dst = (LPWSTR)GlobalLock(hdst);

    memcpy(dst, cwdBuffer, len * sizeof(WCHAR));
    dst[len] = 0;
    GlobalUnlock(hdst);

    if (!OpenClipboard(NULL))
        return GetLastError();
    EmptyClipboard();
    if (!SetClipboardData(CF_UNICODETEXT, hdst))
        return GetLastError();
    CloseClipboard();
    delete[] Lstr;
    return 0;
}

void SearchMusic(char *mus)
{
    // QQMusic_Daemon_Wnd
    ShellExecute(NULL, L"open", L"D:/Program Files (x86)/QQMusic/QQMusic.exe", NULL, NULL, SW_SHOW);
    char Title[MAX_PATH] = {0};
    HWND hwnd = FindWindow(L"QQMusic_Daemon_Wnd", 0);
    GetWindowTextA(hwnd, Title, MAX_PATH);
    printf(Title);
    wchar_t LTitle[MAX_PATH] = {0};
    MultiByteToWideChar(CP_ACP, 0, Title, -1, LTitle, MAX_PATH);
    hwnd = FindWindow(L"TXGuiFoundation", LTitle);
    std::cout << hwnd << std::endl;
    RECT rect;
    GetWindowRect(hwnd, &rect);

    INPUT input = {0};
    input.type = INPUT_MOUSE;
    std::cout << rect.left << rect.top << std::endl;
    input.mi.dx = static_cast<long>(65535.0f / (GetSystemMetrics(SM_CXSCREEN) - 1) * (rect.left + 400));
    input.mi.dy = static_cast<long>(65535.0f / (GetSystemMetrics(SM_CYSCREEN) - 1) * (rect.top + 38));
    std::cout << rect.left + 494 << rect.top + 50 << std::endl;
    input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
    MySetClipBoard(mus);
    Sleep(1000);
    ::SendInput(1, &input, sizeof(INPUT));
    Sleep(100);
    ::SendInput(1, &input, sizeof(INPUT));
    Sleep(100);
    ::SendInput(1, &input, sizeof(INPUT));
    Sleep(100);
    keybd_event(VK_CONTROL, 0, 0, 0);
    Sleep(100);
    keybd_event(0x56, 0, 0, 0);
    keybd_event(VK_CONTROL, 0, 2, 0);
    keybd_event(0x56, 0, 2, 0);
    Sleep(100);
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, 2, 0);
    Sleep(3000);
    input = {0};
    input.type = INPUT_MOUSE;
    std::cout << rect.left << rect.top << std::endl;
    input.mi.dx = static_cast<long>(65535.0f / (GetSystemMetrics(SM_CXSCREEN) - 1) * (rect.left + 476));
    input.mi.dy = static_cast<long>(65535.0f / (GetSystemMetrics(SM_CYSCREEN) - 1) * (rect.top + 368));
    std::cout << rect.left + 494 << rect.top + 50 << std::endl;
    input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
    ::SendInput(1, &input, sizeof(INPUT));
}

int __cdecl main(void)
{

    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET,
           ClientSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    hints;
    char recvbuf[DEFAULT_BUFLEN];
    int iResult, iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;
    HANDLE threads[MAXCLIENTS];
    int ClientCount = 0;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        printf("socket failed: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    int iMode = 1;
    int retVal = ioctlsocket(ListenSocket, FIONBIO, (u_long FAR *)&iMode);
    if (retVal == SOCKET_ERROR)
    {
        printf("ioctlsocket failer!\n");
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR)
    {
        printf("listen failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    int addrClientlen = sizeof(addrClient);
    while (true)
    {
        int threadId = 0;
        ClientSocket = accept(ListenSocket, (sockaddr FAR *)&addrClient, &addrClientlen);
        if (INVALID_SOCKET == ClientSocket)
        {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK)
            {
                Sleep(100);
                continue;
            }
            else
            {
                printf("accept failed!\n");
                closesocket(ListenSocket);
                WSACleanup();
                return -1;
            }
        }
        if (ClientCount < MAXCLIENTS)
        {
            DWORD dwThreadId;
            threads[ClientCount++] = CreateThread(NULL, 0, &AnswerClientThread, (LPVOID)ClientSocket, 0, &dwThreadId);
        }
        else
        {
            DWORD ecode = 0;
            GetExitCodeThread(threads[threadId], &ecode);
            if (ecode == -1)
            {
                DWORD dwThreadId;
                threads[threadId++] = CreateThread(NULL, 0, &AnswerClientThread, (LPVOID)ClientSocket, 0, &dwThreadId);
            }
            else
            {
                closesocket(ClientSocket);
            }
        }
    }
    closesocket(ListenSocket);
    WSACleanup();
    system("pause");
    return 0;
}
void utf8ToGbk(char *utf8String, char *gbkString)
{
    wchar_t *unicodeStr = NULL;
    int nRetLen = 0;
    nRetLen = MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, NULL, 0);
    unicodeStr = (wchar_t *)malloc(nRetLen * sizeof(wchar_t));
    nRetLen = MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, unicodeStr, nRetLen);
    nRetLen = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, NULL, 0, NULL, 0);
    nRetLen = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, gbkString, nRetLen, NULL, 0);
    free(unicodeStr);
}

DWORD WINAPI MyEvent(LPVOID para)
{
    messagePtr mesp = (messagePtr)para;
    mesp->mes[mesp->length] = '\0';
    utf8ToGbk(mesp->mes, mesp->mes);
    printf("date recv: %s\n", mesp->mes);
    if (strcmp(mesp->mes, "1") == 0)
    {
        ShellExecute(
            NULL,                                                              
            L"open",                                                           
            L"D:/Program Files (x86)/QQMusic/QQMusic1869.07.54.24/QQMusic.exe", 
            NULL,                                                            
            NULL,                                                             
            SW_SHOW                                                           
        );
        strcpy(mesp->mes, "aaaaaa");
        int iSendResult = send(mesp->client, mesp->mes, mesp->length, 0);
        if (iSendResult == SOCKET_ERROR)
        {
            printf("send failed: %d\n", WSAGetLastError());
            closesocket(mesp->client);
            return 1;
        }

        printf("date sent: %s\n", mesp->mes);
    }
    else if (strcmp(mesp->mes, "2") == 0)
    {
        keybd_event(VK_MEDIA_PLAY_PAUSE, 0, 0, 0);
    }
    else if (strcmp(mesp->mes, "3") == 0)
    {
        keybd_event(VK_MEDIA_PREV_TRACK, 0, 0, 0);
    }
    else if (strcmp(mesp->mes, "4") == 0)
    {
        keybd_event(VK_MEDIA_NEXT_TRACK, 0, 0, 0);
    }
    else if (strcmp(mesp->mes, "5") == 0)
    {
        keybd_event(VK_VOLUME_UP, 0, 0, 0);
    }
    else if (strcmp(mesp->mes, "6") == 0)
    {
        keybd_event(VK_VOLUME_DOWN, 0, 0, 0);
    }
    else if (strcmp(mesp->mes, "7") == 0)
    {
        keybd_event(VK_VOLUME_MUTE, 0, 0, 0);
    }
    else if (strcmp(mesp->mes, "8") == 0)
    {
        keybd_event(VK_MEDIA_STOP, 0, 0, 0);
    }
    else if (mesp->mes[0] == '9')
    {
        SearchMusic(mesp->mes + 1);
    }


    delete (mesp);
    return 0;
}
