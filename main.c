/*
  Copyright (c) 2014 Jenson Shi <jenson.shixf@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

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

