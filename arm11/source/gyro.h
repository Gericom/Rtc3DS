#pragma once

int gyro_init();
int gyro_read(int reg, u8* dst, int count);
int gyro_write(int reg, const u8* dst, int count);
int gyro_mask(int reg, u8 mask, u8 value);
int gyro_setmask(int reg, u8 mask);
int gyro_clrmask(int reg, u8 mask);
