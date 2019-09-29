#include "a11ucode.h"
#include "rtcom.h"

// extern "C" bool handleCommand0(u8 param)
// {
//     return true;
// }

extern "C" int handleCommand1(u8 param)
{
    return ((u8*)0x127384)[param];
    // u8* pCpad = (u8*)0x127384;
    // rtcom_retByte(pCpad[0]);
    // if(!rtcom_waitNext())
    //     return false;
    // rtcom_retByte(pCpad[1]);
    // if(!rtcom_waitNext())
    //     return false;
    // return pCpad[2];
    //rtcom_retByte(pCpad[2]);
    //return true;
}