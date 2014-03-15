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

#ifndef __MH_HEAD_H__
#define __MH_HEAD_H__

#define _WIN32_WINNT                    0x0500

#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>


#ifdef __cplusplus
extern "C" {
#endif


    typedef struct _ProcMem_t
    {
        HANDLE                          handle;
        /* PROCESS_MEMORY_COUNTERS         pmc; */
        MEMORY_BASIC_INFORMATION        mbi;
        LPVOID                          base;
        LPVOID                          end;
        LPVOID                          current;
    }ProcMem_t;


    extern LPVOID MH_GetAddress(const char* address);
    extern int MH_IsNumeric(const char* str);
    extern int MH_IsHex(const char* str);

    extern void MH_AddToList(LPVOID address);
    extern void MH_SetFlagList(LPVOID address);
    extern void MH_ClearFlagList();
    extern void MH_UpdateList();
    extern void MH_ClearList();

    extern int MH_GetProcList();
    extern int MH_GetWindowList();
    extern void MH_FindProcess(int search, int next);
    extern int MH_ReadProcMem(LPVOID address, LPVOID buffer, DWORD size);
    extern int MH_WriteProcMem(LPVOID address, LPVOID buffer, DWORD size);
    extern void MH_CloseProc();
    extern int MH_OpenProc(DWORD pid);

    extern void MH_ShowHelp();
    extern void MH_ListCmd();
    extern void MH_FindCmd(const char* argv, int arg_sz);
    extern void MH_FindNextCmd(const char* argv, int arg_sz);
    extern void MH_ReadCmd(const char* argv, int arg_sz);
    extern void MH_WriteCmd(const char* argv, int arg_sz);
    extern void MH_RunCmd(const char* cmd, const char* argv, int arg_sz);
    extern void MH_ParseCmd(const char* cmdline, int size);
    extern int MH_GetCmd(char* cmdline, int size);


    extern ProcMem_t pm;
    extern int __g_AppExit;


    #pragma warning( disable : 4311 4312 )


#ifdef __cplusplus
}   /* extern "C" */
#endif


#endif  /* __MH_HEAD_H__ */
