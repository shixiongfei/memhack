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


void MH_ShowHelp()
{
    printf("exit\t\texit the application\n");
    printf("list\t\tdisplay the list of all process\n");
    printf("find\t\tfind memory in process. format: find pid number\n");
    printf("findnext\tfind next memory in process. format: findnext number\n");
    printf("write\t\twrite memory to process. format: write addr number\n");
}


void MH_ListCmd()
{
    MH_GetProcList();
    printf("\n");
    MH_GetWindowList();
    printf("\n");
}


void MH_FindCmd(const char* argv, int arg_sz)
{
    char* find = strchr(argv, ' ');
    char pid_s[11] = {0};
    char search_s[11] = {0};
    DWORD pid = 0;
    DWORD search = 0;
    int pid_sz = 0;

    if(!find)
    {
        printf("find parameter is error\n");
        return;
    }
    pid_sz = (int)(find++ - argv);
    if(pid_sz > 10)
    {
        pid_sz = 10;
    }
    strncpy(pid_s, argv, pid_sz);
    strncpy(search_s, find, 10);
    if(!MH_IsNumeric(pid_s))
    {
        printf("pid[%s] is error\n", pid_s);
        return;
    }
    if(!MH_IsNumeric(search_s))
    {
        printf("search[%s] is error\n", search_s);
        return;
    }
    pid = atol(pid_s);
    search = atol(search_s);

    if(!MH_OpenProc(pid))
    {
        printf("can not open process pid[%u]\n", pid);
        return;
    }
    MH_FindProcess(search, 0);
}


void MH_FindNextCmd(const char* argv, int arg_sz)
{
    char search_s[11] = {0};
    DWORD search = 0;

    if(!pm.handle)
    {
        printf("please execute 'find' command first\n");
        return;
    }
    if(!arg_sz)
    {
        printf("findnext parameter is error\n");
        return;
    }
    strncpy(search_s, argv, 10);
    if(!MH_IsNumeric(search_s))
    {
        printf("search[%s] is error\n", search_s);
        return;
    }
    search = atol(search_s);
    MH_FindProcess(search, 1);
}


void MH_WriteCmd(const char* argv, int arg_sz)
{
    char* find = strchr(argv, ' ');
    char addr_s[9] = {0};
    char tmp[9] = {0};
    char value_s[11] = {0};
    int skip = 0;
    LPVOID addr = 0;
    DWORD value = 0;
    int addr_sz = 0;
    int write_sz;

    if(!pm.handle)
    {
        printf("please execute 'find' command first\n");
        return;
    }
    if(!find)
    {
        printf("write parameter is error\n");
        return;
    }
    addr_sz = (int)(find++ - argv);
    if(addr_sz > 8)
    {
        addr_sz = 8;
    }
    strncpy(tmp, argv, addr_sz);
    strncpy(value_s, find, 10);
    skip = 8 - addr_sz;             /* skip character */
    memset(addr_s, '0', skip);      /* fill '0' */
    strcpy(addr_s + skip, tmp);     /* copy address */
    if(!MH_IsHex(addr_s))
    {
        printf("address[%s] is error\n", addr_s);
        return;
    }
    if(!MH_IsNumeric(value_s))
    {
        printf("values[%s] is error\n", value_s);
        return;
    }
    addr = MH_GetAddress(addr_s);
    value = atol(value_s);

    write_sz = MH_WriteProcMem(addr, &value, sizeof(value));
    printf("%i bytes has written\n", write_sz);
}


void MH_RunCmd(const char* cmd, const char* argv, int arg_sz)
{
    if(0 == _stricmp(cmd, "help"))
    {
        MH_ShowHelp();
    }
    else if(0 == _stricmp(cmd, "exit"))
    {
        __g_AppExit = 1;
    }
    else if(0 == _stricmp(cmd, "list"))
    {
        MH_ListCmd();
    }
    else if(0 == _stricmp(cmd, "find"))
    {
        MH_FindCmd(argv, arg_sz);
    }
    else if(0 == _stricmp(cmd, "findnext"))
    {
        MH_FindNextCmd(argv, arg_sz);
    }
    else if(0 == _stricmp(cmd, "write"))
    {
        MH_WriteCmd(argv, arg_sz);
    }
    else
    {
        printf("%s is bad command.\n", cmd);
    }
}


void MH_ParseCmd(const char* cmdline, int size)
{
    char cmd[128] = {0};
    char argv[128] = {0};
    int cmd_sz = size;
    int arg_sz = 0;
    char* find = strchr(cmdline, ' ');
    if(find)
    {
        cmd_sz = (int)(find++ - cmdline);
        arg_sz = size - cmd_sz;
        if(arg_sz)
        {
            --arg_sz;
        }
        strncpy(argv, find, arg_sz);
    }
    strncpy(cmd, cmdline, cmd_sz);

    MH_RunCmd(cmd, argv, arg_sz);
}


int MH_GetCmd(char* cmdline, int size)
{
    int cur = 0;
    int ch = 0;
    memset(cmdline, 0, size--);
    for(;;)
    {
        ch = _getch();
        if((ch >= 0x00) && (ch <= 0x7F))
        {
            if('\r' == ch)      /* enter */
            {
                break;
            }
            if('\b' == ch)      /* backspace */
            {
                if(cur)
                {
                    printf("\b \b");    /* clear character */
                    cmdline[--cur] = 0;
                }
                continue;
            }
            if(cur < size)      /* input key */
            {
                cmdline[cur++] = ch;
                printf("%c", ch);
            }
        }
    }
    printf("\n");
    return cur;
}

