#include "a11ucode.h"
#include "rtcom.h"
#include "pat.h"
#include "scaling.h"
#include "svc.h"
#include "mcu.h"
#include "gyro.h"

// extern "C" bool handleCommand0(u8 param)
// {
//     return true;
// }


// static u32 doScaleStuff()
// {
//     // void* twlLgyStartFbDMA = pat_memesearch(
//     //     (const uint8_t[]){ 0xF8, 0xB5, 0x00, 0x25, 0x04, 0x46, 0x1A, 0xE0 },
//     //     NULL, (const void*)0x100000, 0x20000, 8
//     // );

//     // if(!twlLgyStartFbDMA)
//     //     return 0;

//     // doneScaling = false;

//     // u32 dst = (u32)&twlLgyStartFbDMAHook;
//     // u32 relOffset = ((dst - ((u32)twlLgyStartFbDMA + 2) - 2) >> 1) & 0x003FFFFF;
//     // ((u16*)twlLgyStartFbDMA)[1] = 0xF000 | ((relOffset >> 11) & 0x7FF);
//     // ((u16*)twlLgyStartFbDMA)[2] = 0xE800 | (relOffset & 0x7FE);

//     // svcAddCodeSegment((void*)0x100000, 0x20000);

//     // return 1;

//     void* stateStuff = pat_memesearch(
//         (const uint8_t[]){0x68, 0x60, 0x0D, 0xD9, 0x00, 0x20, 0x68, 0x60},
//         NULL, (const void*)0x100000, 0x20000, 8
//     );
//     u16 blPart1 = *(u16*)(((u8*)stateStuff) + 8);
//     u16 blPart2 = *(u16*)(((u8*)stateStuff) + 10);
//     u32 addr = ((u32)stateStuff) + 8 + 4 + (((blPart1 & 0x7FF) << 12) | ((blPart2 & 0x7FF) << 1));
//     void* state = (void*)*(u32*)(addr + 4);
//     twlLgyInitScaling(state, 0);
//     return (u32)state & 0xFF;
// }

static inline void sleepnanos(uint64_t nanos)
{
    asm volatile("MOV r0, %Q0\n\t"
                 "MOV r1, %R0\n\t"
                 "SVC 0xA"
                 :
                 : "r"(nanos)
                 : "r0", "r1");
}

static int readCPad(int offset)
{
    return ((u8*)0x127384)[offset];
}

static int readGyro(int offset)
{
    u8 res = 0x00;
    gyro_read(offset, &res, 1);
    return res;
}

static int gyro_inited = 0;

extern "C" int handleCommand1(u8 param, u32 stage)
{
    if(param <= 2)
        return readCPad(param);
    
    // if(param == 4)
    // {
    //     return doScaleStuff();
    // }

    if(param == 0xFF && !gyro_inited)
    {
        int ret = gyro_init();
        if(ret > 0)
            return ret;
        
        // unknown GPIO stuff
        //*(vu32*)0x1EC47020 &= 0x40000;
        //*(vu32*)0x1EC47024 = (*(vu32*)0x1EC47024 & ~4) | 0x40000;
        
        u8 val = 0;
        
        if(ret == -3)
        {
            // reset
            //if(gyro_setmask(0x6B, 0x80))
                //return 0x10;
            
            //sleepnanos(10000000);
            
            // turn off sleep mode
            if(gyro_clrmask(0x6B, 0x40))
                return 0x11;
            
            sleepnanos(10000000);
            
            // gyro X PLL clocksource
            if(gyro_mask(0x6B, 0x07, 0x01))
                return 0x12;
            
            sleepnanos(10000000);
            
            // Low Pass Filter = ~42Hz
            if(gyro_mask(0x1A, 0x07, 0x03))
                return 0x13;
            
            sleepnanos(15000000);
            
            // Samplerate = 1kHz / (val + 1)
            val = 4;
            if(gyro_write(0x19, &val, 1))
                return 0x14;
            
            sleepnanos(10000000);
            
            // ~250deg / s range, 0x10000 == 250deg of rotation
            if(gyro_mask(0x1B, 0x18, 0x00))
                return 0x15;
            
            sleepnanos(10000000);
            
            // 2g force, 0x10000 == 2g of force
            if(gyro_mask(0x1C, 0x18, 0x00))
                return 0x16;
            
            sleepnanos(10000000);
            
            // turn off sleep mode again (bug?)
            if(gyro_clrmask(0x6B, 0x40))
                return 0x19;
            
            sleepnanos(10000000);
            
            // sleep mode self-test
            if(gyro_read(0x6B, &val, 1))
                return 0x19 | 0x80;
            
            if(val & 0x40)
                return 0x19 | 0x40;
            
            sleepnanos(10000000);
        }
        gyro_inited = 1;
        
        return ret;
    }

    if(param >= 3 && param <= 8)
        // Read X Y Z as Little-Endian
        return readGyro((((param - 3) ^ 1) + 0x43));

    return 0;
}
