#include "a11ucode.h"
#include "rtcom.h"
#include "pat.h"
#include "scaling.h"
#include "svc.h"
#include "mcu.h"

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

static int readCPad(int offset)
{
    return ((u8*)0x127384)[offset];
}

static int readGyro(int offset)
{
    u8 res;
    mcu_readRegSafe(0x45 + offset, &res, 1);
    return res;
}

extern "C" int handleCommand1(u8 param)
{
    if(param <= 2)
        return readCPad(param);

    // if(param == 4)
    // {
    //     return doScaleStuff();
    // }

    if(param == 0xFF)
        return mcu_init();

    if(param >= 3 && param <= 9)
        return readGyro(param - 3);

    return 0;
}