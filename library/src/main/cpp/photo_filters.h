//
// Created by 范鸿麟 on 2019/3/20.
//

#include <android/bitmap.h>

void brightness(AndroidBitmapInfo *info, void *pixels, jfloat brightnessValue);

void gray(AndroidBitmapInfo *info, void *pixels);

void inverted(AndroidBitmapInfo *info, void *pixels);

int rgb_clamp(int value);