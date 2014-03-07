
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

