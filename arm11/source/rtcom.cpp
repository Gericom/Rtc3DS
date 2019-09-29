#include "a11ucode.h"
#include "rtcom.h"

void rtcom_latchEx(u16 mask)
{
    mask |= RTC_CNT_ENABLE | RTC_CNT_LATCH;
    do { REG_RTC_CNT = mask;}
    while (REG_RTC_CNT & RTC_CNT_LATCH);
}

void rtcom_retByte(u8 retVal, bool last)
{
    REG_RTC_REG_CLKADJ = retVal;
    REG_RTC_REG_FREE = last ? RTCOM_STAT_DONE : RTCOM_STAT_ACK;
    rtcom_latchEx(RTC_CNT_LMASK_CLKADJ | RTC_CNT_LMASK_FREE);
}

bool rtcom_waitNext()
{
    int timeout = 1 << 16;
    while(REG_RTC_REG_FREE == RTCOM_STAT_ACK && timeout > 0)
        timeout--;
    return timeout > 0 && REG_RTC_REG_FREE == RTCOM_REQ_NEXT;
}