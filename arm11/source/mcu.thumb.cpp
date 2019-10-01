#include "a11ucode.h"
#include "pat.h"
#include <stddef.h>

#define RELOC_ADDR ((void*)0x100000)
#define RELOC_SIZE (0x28000)

typedef void (*lockfunc_t)(void* unk1, int unk2);
typedef int (*mcureadreg_t)(void* someMcuPtr, int reg, u8* dst, int count);
typedef int (*mcuwritereg_t)(void* someMcuPtr, int reg, const u8* dst, int count);

static lockfunc_t sLockFunc = NULL;
static lockfunc_t sUnlockFunc = NULL;
static mcureadreg_t sMcuReadReg = NULL;
static mcuwritereg_t sMcuWriteReg = NULL;

static void* sSomeMcuPtr = NULL;

static int sIsInitialized = -1;

int mcu_init()
{
    size_t ptr = (size_t)pat_memesearch(
        (const uint8_t[]){ 0x0C, 0x20, 0x10, 0xB5, 0x41, 0x43, 0x02, 0x48, 0x08, 0x18, 0x00, 0xF0, 0x89, 0xF9, 0x10, 0xBD },
        (const uint8_t[]){ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x07, 0xFF, 0x07, 0x00, 0x00 },
        RELOC_ADDR, RELOC_SIZE, 16);
    if(!ptr)
        return 1;
    sUnlockFunc = (lockfunc_t)(ptr | 1);
    
    ptr = (size_t)pat_memesearch(
        (const uint8_t[]){ 0x0C, 0x20, 0x10, 0xB5, 0x41, 0x43, 0x02, 0x48, 0x08, 0x18, 0x00, 0xF0, 0x85, 0xF9, 0x10, 0xBD },
        (const uint8_t[]){ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x07, 0xFF, 0x07, 0x00, 0x00 },
        (void*)(ptr + 4), (size_t)(RELOC_ADDR) + RELOC_SIZE - (size_t)ptr, 16);
    if(!ptr)
        return 2;
    sLockFunc =  (lockfunc_t)(ptr | 1);
    
    ptr = (size_t)pat_memesearch(
        (const uint8_t[]){ 0x38, 0xB5, 0x0C, 0x46, 0x00, 0x93, 0x11, 0x46, 0x00, 0x68, 0x03, 0x22, 0x23, 0x46, 0xFE, 0xF7, 0x41, 0xFD, 0x38, 0xBD },
        (const uint8_t[]){ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x07, 0xFF, 0x07, 0x00, 0x00 },
        RELOC_ADDR, RELOC_SIZE, 20);
    if(!ptr)
        return 3;
    sMcuReadReg = (mcureadreg_t)(ptr | 1);
    
    ptr = (size_t)pat_memesearch(
        (const uint8_t[]){ 0x08, 0xB5, 0x00, 0x93, 0x13, 0x46, 0x0A, 0x46, 0x00, 0x68, 0x03, 0x21, 0xF5, 0xF7, 0xCE, 0xFB, 0x08, 0xBD },
        (const uint8_t[]){ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x03, 0xFF, 0x07, 0x00, 0x00 },
        RELOC_ADDR, RELOC_SIZE, 18);
    if(!ptr)
        return 4;
    sMcuWriteReg = (mcuwritereg_t)(ptr | 1);

    ptr = (size_t)pat_memesearch(
        (const uint8_t[]){ 0x70, 0xB5, 0x00, 0x21, 0x05, 0x68, 0x28, 0x46, 0x10, 0xF0, 0x5E, 0xFC, 0x04, 0x48 },
        (const uint8_t[]){ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x07, 0xFF, 0x07, 0xFF, 0x00 },
        RELOC_ADDR, RELOC_SIZE, 10);
    if(!ptr)
        return 5;

    //sSomeMcuPtr = (void*)*(u32*)(((u8*)simpleFunc) + 0x20);    
    sSomeMcuPtr = *(void**)((ptr + 12 + ((((s8*)ptr)[12] * 4) + 4)) & ~3);

    sIsInitialized = 1;
    return 0;
}

int mcu_readRegSafeThumb(int reg, u8* dst, int count)
{
    sLockFunc(NULL, 0);
    int result = sMcuReadReg(sSomeMcuPtr, reg, dst, count);
    sUnlockFunc(NULL, 0);
    return result;
}

bool mcu_readRegSafe(int reg, u8* dst, int count)
{
    if(sIsInitialized != 1)
        return false;
    int result = mcu_readRegSafeThumb(reg, dst, count);
    return !result;
}

int mcu_writeRegSafeThumb(int reg, const u8* dst, int count)
{
    sLockFunc(NULL, 0);
    int result = sMcuWriteReg(sSomeMcuPtr, reg, dst, count);
    sUnlockFunc(NULL, 0);
    return result;
}

bool mcu_writeRegSafe(int reg, const u8* dst, int count)
{
    if(sIsInitialized != 1)
        return false;
    int result = mcu_writeRegSafeThumb(reg, dst, count);
    return !result;
}
