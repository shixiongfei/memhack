
#include "head.h"


#pragma comment(lib,"Psapi.lib")


int __g_AppExit = 0;


int main(int argc, char* argv[])
{
    SYSTEM_INFO si;
    int cmd_size = 0;
    char cmdline[128] = {0};
    HWND hWnd = GetConsoleWindow();
    SetWindowText(hWnd, "Memory Hacker Tools");

    memset(&pm.mbi, 0, sizeof(MEMORY_BASIC_INFORMATION));
    GetSystemInfo(&si);

    pm.base = si.lpMinimumApplicationAddress;
    pm.end = si.lpMaximumApplicationAddress;

	SetConsoleOutputCP(CP_UTF8);

    printf("Copyright (c) 2008-2014 Jenson Shi.\n");
    printf("http://shixf.com/\n\n");

    printf("Welcome to use Memory Hacker Tools.\n\n");
    printf("type 'help' for how to use.\n\n");

    while(!__g_AppExit)
    {
        printf("> ");
        cmd_size = MH_GetCmd(cmdline, 128);
        MH_ParseCmd(cmdline, cmd_size);
    }
    MH_CloseProc();

    printf("\nBye Bye!\n");
    Sleep(500);

    return 0;
}

