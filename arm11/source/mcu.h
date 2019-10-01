#pragma once

bool mcu_init();
bool mcu_readRegSafe(int reg, u8* dst, int count);