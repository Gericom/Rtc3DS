#pragma once

int mcu_init();
bool mcu_readRegSafe(int reg, u8* dst, int count);
bool mcu_writeRegSafe(int reg, const u8* dst, int count);
