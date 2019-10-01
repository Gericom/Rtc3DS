#include "a11ucode.h"

extern "C" bool twlLgyIsAnyEnabled(void* state);

extern "C" int idivmod(int a, int b);

u32 twlLgyScreenIO[4] = { 0x1EC11000, 0x1EC10000, 0x1EE11000, 0x1EE10000 };

static s16 lgyScaleKernelNative[6] =
{
    0,
    0,
    0,
    0x4000,
    0,
    0
};

static s16 lgyScaleKernelUpscale[30] =
{
    0, 0x4E, 0x11D, 0x1E3, 0x1C1, 
    0, -859, -1840, -2403, -1933,
    0, 0xD47, 0x1E35, 0x2F08, 0x3B6F,
    0x4000, 0x3B6F, 0x2F08, 0x1E35, 0xD47,
    0, -1933, -2403, -1840, -859, 
    0, 0x1C1, 0x1E3, 0x11D, 0x4E
};

static s16 lgyScaleKernelUpscale2[36] =
{
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    0, 0x2000, 0x4000, 0, 0x2000, 0x4000,
    0x4000, 0x2000, 0, 0x4000, 0x2000, 0,
    0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};

bool twlLgyCalculateScaleKernel(u32* kWidth, u32* kPatBits, s16* kernel, int scalingMode)
{
    if(!kWidth || !kPatBits || !kernel)
        return false;
    switch (scalingMode)
    {
        default:
        {
            *kWidth = 4;
            *kPatBits = 0xF;
            for(int y = 0; y < 6; y++)
                for(int x = 0; x < 4; x++)
                    kernel[x * 6 + y] = lgyScaleKernelNative[y];
            break;
        }
        case 1:
        {
            *kWidth = 5;
            *kPatBits = 0xF;
            for(int y = 0; y < 6; y++)
                for(int x = 0; x < 5; x++)
                    kernel[x * 6 + y] = lgyScaleKernelUpscale[y * 5 + x];
            break;
        }
        case 2:
        {
            *kWidth = 6;
            *kPatBits = 0x1B;
            for(int y = 0; y < 6; y++)
                for(int x = 0; x < 6; x++)
                    kernel[x * 6 + y] = lgyScaleKernelUpscale2[y * 6 + x];
            break;
        }
    }
    return true;
}

bool twlLgyFbSetKernel(void* state, int width, int patBits, const s16* kernel, int divisor)
{
    if(!kernel)
        return false;
    int unk = 0;
    switch(divisor)
    {
        case 0:
            return false;
        case 1:
            unk = 128;
            break;
        case 2:
            unk = 192;
            break;
        case 3:
            unk = 224;
            break;
        case 4:
            unk = 240;
            break;
        case 5:
            unk = 248;
            break;
        case 6:
            unk = 252;
            break;
        case 7:
            unk = 254;
            break;
        case 8:
            unk = 255;
            break;
    }
    int screenCount = *(u32*)(((u8*)state) + 0x10);
    for(int i = 0; i < screenCount; i++)
    {
        u32 base = twlLgyScreenIO[i];
        *(vu32*)(base + 0x300) = width - 1;
        *(vu32*)(base + 0x200) = width - 1;
        *(vu32*)(base + 0x304) = patBits;
        *(vu32*)(base + 0x204) = patBits;
        for(int y = 0; y < 6; y++)
        {
            for(int x = 0; x < width; x++)
            {
                s16 val = kernel[x * 6 + y];
                int newVal = idivmod(val * 255, unk);
                u32 valBase = base + 0x20 * y + 4 * x;
                *(vu32*)(valBase + 0x240) = newVal + 8;
                *(vu32*)(valBase + 0x340) = val + 8;
            }
        }
    }
    return true;
}

bool twlLgyGetControlb12c1(void* state, bool* dst)
{
    if(!dst)
        return false;
    *dst = ((*(vu32*)0x1EC11000) & 0x1000) != 0;
    return true;
}

bool twlLgyFbGetInputScreenMode(void* state, u8* dst)
{
    if(!dst)
        return false;
    *dst = ((*(vu32*)0x1EC11000) << 20) >> 30;
    return true;
}

bool twlLgyFbGetOutputScreenMode(void* state, u8* dst)
{
    if(!dst)
        return false;
    *dst = ((*(vu32*)0x1EC11000) << 22) >> 30;
    return true;
}

bool twlLgyFbGetOutputSize(void* state, u32* width, u32* height)
{
    if(!width || !height)
        return false;
    *width = ((*(vu32*)0x1EC11000) & 0x1FF) + 1;
    *height = (((*(vu32*)0x1EC11000) >> 16) & 0x1FF) + 1;
    return true;
}

void twlLgyInitDmaConfig(void* state)
{
    u32 width, height;
    twlLgyFbGetOutputSize(state, &width, &height);
    if(!width || !height)
        return;
    u8 outMode;
    twlLgyFbGetOutputScreenMode(state, &outMode);
    int mul;
    switch (outMode)
    {
        case 0:
            mul = 4;
            break;
        case 1:
            mul = 3;
            break;
        case 2:
        case 3:
            mul = 2;
            break;
    }
    u16 readStride = 8 * width * mul;
    *(u16*)(((u8*)state) + 0x20) = readStride;
    u16 writeStride = readStride;
    u32 customStride = *(u32*)(((u8*)state) + 0x14);
    if(customStride)
        writeStride = 8 * customStride * mul;
    *(u16*)(((u8*)state) + 0x24) = writeStride;
    *(u32*)(((u8*)state) + 0x58) = width * height * mul;
}

bool twlLgyFbSetSize(void* state, int width, int height)
{
    bool unk;
    twlLgyGetControlb12c1(state, &unk);
    // u32 unaligned;
    // if(unk)
    //     unaligned = (width | height) << 29;
    // else
    // {
    //     u8 inMode;
    //     twlLgyFbGetInputScreenMode(state, &inMode);
    //     u8 outMode;
    //     twlLgyFbGetOutputScreenMode(state, &outMode);
    //     if(outMode == 0)
    //         unaligned = 0;
    //     else if(outMode == 1)
    //         unaligned = height << 30;
    //     else
    //         unaligned = height << 31;
    // }
    // if(unaligned)
    //     return false;
    int screenCount = *(u32*)(((u8*)state) + 0x10);
    for(int i = 0; i < screenCount; i++)
        *(vu32*)(twlLgyScreenIO[i] + 4) = ((width - 1) & 0x1FF) | (((height - 1) & 0x1FF) << 16);
    twlLgyInitDmaConfig(state);
    return true;
}

bool twlLgyFbSetCustomWidthStride(void* state, int stride)
{
    *(u32*)(((u8*)state) + 0x14) = stride;
    twlLgyInitDmaConfig(state);
    return true;
}

bool twlLgyInitScaling(void* state, int scalingMode)
{
    int w = 256;
    int h = 192;
    int stride = 256;
    switch(scalingMode)
    {
        default:
            h = 200;
            break;
        case 1:
            w = 320;
            h = 240;
            stride = 512;
            break;
        case 2:
            w = 384;
            h = 288;
            stride = 512;
            break;
    }
    u32 kWidth, kPatBits;
    s16 kernel[36];
    *(u32*)(((u8*)state) + 0x58) = 0;
    return twlLgyCalculateScaleKernel(&kWidth, &kPatBits, kernel, scalingMode) &&
           twlLgyFbSetKernel(state, kWidth, kPatBits, kernel, 6) &&
           twlLgyFbSetSize(state, w, h) &&
           twlLgyFbSetCustomWidthStride(state, stride);
}

bool doneScaling = false;

bool twlLgyIsAnyEnabled(void* state)
{
    int result = 0;
    int screenCount = *(u32*)(((u8*)state) + 0x10);
    for(int i = 0; i < screenCount; i++)
        result |= *(vu32*)twlLgyScreenIO[i] & 1;
    return result != 0;
}

extern "C" void twlLgyStartFbDMAHook_part2(void* state)
{
    if(twlLgyIsAnyEnabled(state))
        return;
    if(!doneScaling)
    {
        twlLgyInitScaling(state, 0);
        doneScaling = true;
    }
}