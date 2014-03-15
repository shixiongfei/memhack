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


#define M               (1024 * 9)

ProcMem_t pm = {0};


int MH_GetProcList()
{
    HANDLE         hProcSnap;
    PROCESSENTRY32 pe32      = {0};

    hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(INVALID_HANDLE_VALUE == hProcSnap)
    {
        return 0;
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if(Process32First(hProcSnap, &pe32))
    {
        do
        {
            printf("ID: %04u,  File: %s\n", pe32.th32ProcessID, pe32.szExeFile);
        }while(Process32Next(hProcSnap, &pe32));
    }
    CloseHandle (hProcSnap);
    return 1;
}


int MH_GetWindowList()
{
    HWND hwnd;
    DWORD id;
    char title[260] = {0};

    hwnd = FindWindowEx(NULL, NULL, NULL, NULL);
    while(NULL != hwnd)
    {
        if(0 == GetWindow(hwnd, GW_OWNER))
        {
            if(IsWindowVisible(hwnd))
            {
                GetWindowText(hwnd, title, 260);
                if(strlen(title))
                {
                    if((0 != _stricmp(title, "Program Manager")) && (0 != _stricmp(title, "ProcessInfo")))
                    {
                        GetWindowThreadProcessId(hwnd, &id);
                        printf("ID: %04u,  Title: %s\n", id, title);
                    }
                }
            }
        }
        memset(title, 0, 260);
        hwnd = FindWindowEx(NULL, hwnd, NULL, NULL);
    }
    return 1;
}


int MH_FindMem(DWORD startpos, LPBYTE buffer, DWORD bsize, LPBYTE find, DWORD fsize)
{
    DWORD i;
    if(startpos > bsize)
    {
        return -1;
    }
    for(i = startpos; i < (bsize + 1); ++i)
    {
        if(0 == memcmp(&buffer[i], find, fsize))
        {
            return i;
        }
    }
    return -1;
}


void MH_FindProcess(int search, int next)
{
    DWORD size = 0;
    DWORD readsize = 0;
    DWORD cr = 0;
    LONG find = -1;
    DWORD fPos;
    unsigned char buffer[M] = {0};

    if(!next)
    {
        MH_ClearList();
    }
    else
    {
        MH_ClearFlagList();
    }


    pm.current = pm.base;
    while(pm.current <= pm.end)
    {
        readsize = 0;
        fPos = 0;
        /* query page */
        VirtualQueryEx(pm.handle, pm.current, &pm.mbi, sizeof(MEMORY_BASIC_INFORMATION));

        /* check page is usable */
        if((MEM_COMMIT == pm.mbi.State) && ((PAGE_READWRITE == pm.mbi.Protect) || (PAGE_EXECUTE_READWRITE == pm.mbi.Protect)))
        {
            while((readsize + 1) < pm.mbi.RegionSize)
            {
                /* calculate size */
                (M < ((DWORD)pm.mbi.RegionSize - readsize)) ? (cr = M) : (cr = (DWORD)pm.mbi.RegionSize - readsize);

                /* read memory */
                ReadProcessMemory(pm.handle, pm.current, buffer, cr, &size);
                readsize += size;

                do
                {
                    /* find buffer */
                    find = MH_FindMem(fPos, buffer, size, (LPBYTE)&search, sizeof(search));
                    if(-1 != find)
                    {
                        /* find it */
                        fPos = find + 1;
                        if(!next)
                        {
                            MH_AddToList((LPBYTE)pm.current + find);
                        }
                        else
                        {
                            MH_SetFlagList((LPBYTE)pm.current + find);
                        }
                    }
                }while(-1 != find);

                /* update current pointer */
                if((pm.mbi.RegionSize - readsize) > 0)
                {
                    (LPBYTE)pm.current += size;
                }
            }
        }
        /* update current pointer */
        pm.current = (LPBYTE)pm.mbi.BaseAddress + pm.mbi.RegionSize;
    }
    MH_UpdateList();
}


int MH_WriteProcMem(LPVOID address, LPVOID buffer, DWORD size)
{
    DWORD writesize = 0;
    WriteProcessMemory(pm.handle, address, buffer, size, &writesize);
    return (int)writesize;
}



void MH_CloseProc()
{
    if(pm.handle)
    {
        CloseHandle(pm.handle);
        pm.handle = 0;
    }
}


int MH_OpenProc(DWORD pid)
{
    if(pm.handle)
    {
        MH_CloseProc();
    }
    pm.handle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, pid);
    return 0 != pm.handle;
}


