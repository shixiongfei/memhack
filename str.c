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


static char ChrToHex(char ch)
{
    if((ch >= '0') && (ch <= '9'))
    {
        return ch - 48;
    }
    if((ch >= 'A') && (ch <= 'F'))
    {
        return ch - 55;
    }
    if((ch >= 'a') && (ch <= 'f'))
    {
        return ch - 87;
    }
    return 0;
}


LPVOID MH_GetAddress(const char* address)
{
	int pos = 0;
    unsigned char buffer[8] = {0};
    unsigned long long addr = 0;
	unsigned int idx, size = strlen(address) >> 1;

	for(idx = 0; idx < size; ++idx)
	{
		pos = idx << 1;
		buffer[idx] = (BYTE)((ChrToHex(address[pos]) << 4) | (ChrToHex(address[pos + 1])));
	}

	for(idx = 0; idx < size; ++idx)
	{
		addr <<= 8;
		addr |= buffer[idx];
	}
    return (LPVOID)addr;
}


int MH_IsNumeric(const char* str)
{
    char* ptr;
    for(ptr = (char*)str; '\0' != (*ptr); ++ptr)
    {
        if(((*ptr) < '0') || ((*ptr) > '9'))
        {
            return 0;
        }
    }
    return 1;
}


int MH_IsHex(const char* str)
{
    char* ptr;
    for(ptr = (char*)str; '\0' != (*ptr); ++ptr)
    {
        if(((*ptr) < '0') || ((*ptr) > '9'))
        {
            if(((*ptr) < 'a') || ((*ptr) > 'f'))
            {
                if(((*ptr) < 'A') || ((*ptr) > 'F'))
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

