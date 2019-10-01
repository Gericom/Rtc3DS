#pragma once

extern bool doneScaling;

bool twlLgyInitScaling(void* state, int scalingMode);

extern "C" void twlLgyStartFbDMAHook(void* state);