#pragma once

#define RTC_CNT_LMASK_STAT1     0x0001
#define RTC_CNT_LMASK_STAT2     0x0002
#define RTC_CNT_LMASK_CLKADJ    0x0004
#define RTC_CNT_LMASK_FREE      0x0008
#define RTC_CNT_LMASK_TIME      0x0010
#define RTC_CNT_LMASK_ALRMTIM1  0x0020
#define RTC_CNT_LMASK_ALRMTIM2  0x0040
#define RTC_CNT_LMASK_COUNT     0x0080
#define RTC_CNT_LMASK_FOUT1     0x0100
#define RTC_CNT_LMASK_FOUT2     0x0200
#define RTC_CNT_LMASK_ALRMDAT1  0x0400
#define RTC_CNT_LMASK_ALRMDAT2  0x0800

#define RTC_CNT_UNK             0x2000 //seems ds to 3ds writing related, triggers a read from time regs in update code
#define RTC_CNT_LATCH           0x4000
#define RTC_CNT_ENABLE          0x8000

#define REG_RTC_CNT             (*(vu16*)(0xEB00000 + 0x10147100))

#define REG_RTC_REG_STAT1       (*(vu8*)(0xEB00000 + 0x10147110))
#define REG_RTC_REG_STAT2       (*(vu8*)(0xEB00000 + 0x10147111))
#define REG_RTC_REG_CLKADJ      (*(vu8*)(0xEB00000 + 0x10147112))
#define REG_RTC_REG_FREE        (*(vu8*)(0xEB00000 + 0x10147113))

#define RTCOM_STAT_READY        0x00
#define RTCOM_STAT_ACK          0x80
#define RTCOM_STAT_DONE         0x82

#define RTCOM_REQ_KIL           0x00
#define RTCOM_REQ_NEXT          0x81

void rtcom_latchEx(u16 mask);
void rtcom_retByte(u8 retVal, bool last = false);
bool rtcom_waitNext();