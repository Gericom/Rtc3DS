#include "a11ucode.h"
#include "pat.h"
#include <stddef.h>

#define RELOC_ADDR ((void*)0x100000)
#define RELOC_SIZE (0x28000)

typedef int (*i2creadbuf8_t)(void* instance, u8* dst, int dev, int reg, int count);
typedef int (*i2cwritebuf8_t)(void* instance, int dev, int reg, const u8* dst, int count);

static i2creadbuf8_t  sReadBuf8 = NULL;
static i2cwritebuf8_t sWriteBuf8 = NULL;

static int sIsInitialized = 0;

int gyro_init()
{
    size_t ptr = (size_t)pat_memesearch(
        (const uint8_t[]){ 0x38, 0xB5, 0x04, 0x9C, 0x00, 0x94, 0x00, 0x68, 0xF9, 0xF7, 0xFA, 0xF9, 0x38, 0xBD },
        (const uint8_t[]){ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x07, 0x00, 0x00 },
        RELOC_ADDR, RELOC_SIZE, 14);
    if(!ptr)
        return 1;
    sReadBuf8 = (i2creadbuf8_t)(ptr | 1);
    
    ptr = (size_t)pat_memesearch(
        (const uint8_t[]){ 0x00, 0x00, 0x03, 0x00, 0x38, 0xB5, 0x04, 0x9C, 0x00, 0x94, 0x00, 0x68, 0xF9, 0xF7, 0xFA, 0xF9, 0x38, 0xBD },
        (const uint8_t[]){ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x07, 0xFF, 0x07, 0x00, 0x00 },
        RELOC_ADDR, RELOC_SIZE, 18);
    if(!ptr)
        return 2;
    sWriteBuf8 = (i2cwritebuf8_t)(ptr | 1);
    
    sIsInitialized = 1;
    return 0;
}

static int inc = 0;

int gyro_read(int reg, u8* dst, int count)
{
    if(!sReadBuf8 || !sIsInitialized)
    {
        *dst = ++inc;
        return 0;
    }
    
    u32 magic = 4;
    u32* deref = &magic;
    
    return sReadBuf8(&deref, dst, 11, reg & 0xFF, count);
}

int gyro_write(int reg, const u8* dst, int count)
{
    if(!sWriteBuf8 || !sIsInitialized)
        return 0;
    
    u32 magic = 4;
    u32* deref = &magic;
    
    return sWriteBuf8(&deref, 11, reg & 0xFF, dst, count);
}

int gyro_mask(int reg, u8 mask, u8 val)
{
    u8 data = 0xA5;
    if(gyro_read(reg, &data, 1))
        return 1;
    data = (data & ~mask) | val;
    if(gyro_write(reg, &data, 1))
        return 2;
    
    return 0;
}

int gyro_setmask(int reg, u8 mask)
{
    u8 data = 0xA5;
    if(gyro_read(reg, &data, 1))
        return 1;
    data = data | mask;
    if(gyro_write(reg, &data, 1))
        return 2;
    
    return 0;
}

int gyro_clrmask(int reg, u8 mask)
{
    u8 data = 0xA5;
    if(gyro_read(reg, &data, 1))
        return 1;
    data = data & ~mask;
    if(gyro_write(reg, &data, 1))
        return 2;
    
    return 0;
}
