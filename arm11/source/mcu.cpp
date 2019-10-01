#include "a11ucode.h"
#include "pat.h"

static const u8 sLockFuncSig[16] = 
    { 0x0C, 0x20, 0x10, 0xB5, 0x41, 0x43, 0x02, 0x48, 0x08, 0x18, 0x00, 0xF0, 0x89, 0xF9, 0x10, 0xBD };

static const u8 sUnlockFuncSig[16] =
    { 0x0C, 0x20, 0x10, 0xB5, 0x41, 0x43, 0x02, 0x48, 0x08, 0x18, 0x00, 0xF0, 0x85, 0xF9, 0x10, 0xBD };

static const u8 sMcuReadRegSig[16]
    { 0x38, 0xB5, 0x0C, 0x46, 0x00, 0x93, 0x11, 0x46, 0x00, 0x68, 0x03, 0x22, 0x23, 0x46, 0xFE, 0xF7 };


static const u8 sAnySimpleMcuFunc[10] =
    { 0x70, 0xB5, 0x00, 0x21, 0x05, 0x68, 0x28, 0x46, 0x10, 0xF0 };

typedef void (*lockfunc_t)(void* unk1, int unk2);
typedef int (*mcureadreg_t)(void* someMcuPtr, int reg, u8* dst, int count);

static lockfunc_t sLockFunc = NULL;
static lockfunc_t sUnlockFunc = NULL;
static mcureadreg_t sMcuReadReg = NULL;

static void* sSomeMcuPtr = NULL;

static int sIsInitialized = -1;

bool mcu_init()
{
    u32 ptr = (u32)pat_memesearch(sLockFuncSig, NULL, (const void*)0x100000, 0x20000, sizeof(sLockFuncSig));
    if(!ptr)
        return false;
    sLockFunc = (lockfunc_t)(ptr | 1);
    ptr = (u32)pat_memesearch(sUnlockFuncSig, NULL, (const void*)0x100000, 0x20000, sizeof(sUnlockFuncSig));
    if(!ptr)
        return false;
    sUnlockFunc =  (lockfunc_t)(ptr | 1);
    ptr = (u32)pat_memesearch(sMcuReadRegSig, NULL, (const void*)0x100000, 0x20000, sizeof(sMcuReadRegSig));
    if(!ptr)
        return false;
    sMcuReadReg = (mcureadreg_t)(ptr | 1);

    void* simpleFunc = pat_memesearch(sAnySimpleMcuFunc, NULL, (const void*)0x100000, 0x20000, sizeof(sAnySimpleMcuFunc));
    if(!simpleFunc)
        return false;

    sSomeMcuPtr = (void*)*(u32*)(((u8*)simpleFunc) + 0x20);    

    sIsInitialized = 1;
    return true;
}

bool mcu_readRegSafe(int reg, u8* dst, int count)
{
    if(sIsInitialized != 1)
        return false;
    sLockFunc(NULL, 0);
    int result = sMcuReadReg(sSomeMcuPtr, reg, dst, count);
    sUnlockFunc(NULL, 0);
    return (result >> 31) != -1;
}