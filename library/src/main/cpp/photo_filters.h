//
// Created by 范鸿麟 on 2019/3/20.
//

#include <android/bitmap.h>

/**
 * 在hold_process前为bitmap加锁，之后解锁
 * @param env
 * @param bitmap
 * @param hold_process
 */
void bitmap_hold_pixels(JNIEnv *env, jobject bitmap, void (*hold_process)(AndroidBitmapInfo *, void *));


void brightness(AndroidBitmapInfo *info, void *pixels, jfloat brightnessValue);

void gray(AndroidBitmapInfo *info, void *pixels);

void inverted(AndroidBitmapInfo *info, void *pixels);

int rgb_clamp(int value);