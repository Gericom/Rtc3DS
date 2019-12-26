#include <nds.h>
#include <dswifi7.h>
#include <maxmod7.h>
#include "rtcom.h"
#include "Rtc3DS_uc11.h"

static bool updateCPad()
{
	if(!rtcom_executeUCode(0))
		return false;
	u8 cpad0 = rtcom_getData();
	if(!rtcom_requestNext(1))
		return false;
	u8 cpad1 = rtcom_getData();
	if(!rtcom_requestNext(2))
		return false;
	u8 cpad2 = rtcom_getData();
	*(vu32*)0x02FFFE70 = cpad0 | (cpad1 << 8) | (cpad2 << 16);
	return true;
}

static bool updateGyro()
{
	if(!rtcom_executeUCode(3))
		return false;
	s16 gyro0 = rtcom_getData();
	if(!rtcom_requestNext(4))
		return false;
	gyro0 |= rtcom_getData() << 8;
	if(!rtcom_requestNext(5))
		return false;
	s16 gyro1 = rtcom_getData();
	if(!rtcom_requestNext(6))
		return false;
	gyro1 |= rtcom_getData() << 8;
	if(!rtcom_requestNext(7))
		return false;
	s16 gyro2 = rtcom_getData();
	if(!rtcom_requestNext(8))
		return false;
	gyro2 |= rtcom_getData() << 8;
	*(vs16*)0x02FFFE78 = gyro0;
	*(vs16*)0x02FFFE7A = gyro1;
	*(vs16*)0x02FFFE7C = gyro2;
	return true;
}

void VblankHandler(void)
{
	int savedIrq = enterCriticalSection();
	{
		rtcom_beginComm();
		updateCPad();
		updateGyro();
		rtcom_requestKill();
		rtcom_requestAsync(RTCOM_STAT_DONE);
		rtcom_endComm();
	}
	leaveCriticalSection(savedIrq);
	
	Wifi_Update();
}


//---------------------------------------------------------------------------------
void VcountHandler() {
//---------------------------------------------------------------------------------
	inputGetAndSend();
}

volatile bool exitflag = false;

//---------------------------------------------------------------------------------
void powerButtonCB() {
//---------------------------------------------------------------------------------
	exitflag = true;
}


//---------------------------------------------------------------------------------
int main() 
{
	// clear sound registers
	dmaFillWords(0, (void*)0x04000400, 0x100);

	REG_SOUNDCNT |= SOUND_ENABLE;
	writePowerManagement(PM_CONTROL_REG, ( readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_MUTE ) | PM_SOUND_AMP );
	powerOn(POWER_SOUND);

	readUserSettings();
	ledBlink(0);

	irqInit();
	// Start the RTC tracking IRQ
	//initClockIRQ();
	fifoInit();
	touchInit();

	mmInstall(FIFO_MAXMOD);

	SetYtrigger(80);

	installWifiFIFO();
	installSoundFIFO();

	installSystemFIFO();

	irqSet(IRQ_VCOUNT, VcountHandler);
	irqSet(IRQ_VBLANK, VblankHandler);

	int savedIrq = enterCriticalSection();
	{
		rtcom_beginComm();
		rtcom_requestAsync(1);
		rtcom_waitStatus(RTCOM_STAT_DONE);
		rtcom_uploadUCode(Rtc3DS_uc11, Rtc3DS_uc11_size);
		rtcom_requestKill();
		if(rtcom_executeUCode(0xFF)) //mcu init
			*(u8*)0x02FFFE74 = rtcom_getData();
		else
			*(u8*)0x02FFFE74 = 0xFF;
		rtcom_requestKill();
		rtcom_requestAsync(RTCOM_STAT_DONE);
		rtcom_endComm();
	}
	leaveCriticalSection(savedIrq);

	irqEnable( IRQ_VBLANK | IRQ_VCOUNT );//| IRQ_NETWORK);

	setPowerButtonCB(powerButtonCB);

	REG_SNDEXTCNT = 0x8000;

	*(u8*)0x02FFFE70 = 0;
	*(u8*)0x02FFFE71 = 0;
	*(u8*)0x02FFFE72 = 0;

	// *(vu32*)0x04700000 = 2;
	// *(vu16*)0x04700000 = 2;
	// *(vu8*)0x04700000 = 2;

	// Keep the ARM7 mostly idle
	while (!exitflag) {
		if ( 0 == (REG_KEYINPUT & (KEY_SELECT | KEY_START | KEY_L | KEY_R))) {
			exitflag = true;
		}		
		swiWaitForVBlank();
	}
	return 0;
}
