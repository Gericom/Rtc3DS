#include "a11ucode.h"

__attribute__((optimize("Ofast"))) void* pat_memesearch(const void* patptr, const void* bitptr, const void* searchptr, u32 searchlen, u32 patsize)
{
    const u8* pat = (const u8*)patptr;
    const u8* bit = (const u8*)bitptr;
    const u8* src = (const u8*)searchptr;
    
    u32 i = 0;
    u32 j = 0;
    
    searchlen -= patsize;
    
    if(bit)
    {
        do
        {
            if((src[i + j] & ~bit[j]) == (pat[j] & ~bit[j]))
            {
                if(++j != patsize)
                    continue;
                
                return (void*)(src + i);
            }
            
            ++i;
            j = 0;
            
            continue;
        }
        while(i != searchlen);
    }
    else
    {
        do
        {
            if(src[i + j] == pat[j])
            {
                if(++j != patsize)
                    continue;
                
                return (void*)(src + i);
            }
            
            ++i;
            j = 0;
            
            continue;
        }
        while(i != searchlen);
    }
    
    return 0;
}