#include <jni.h>
#include <android/bitmap.h>
//#include <photo_filters.h>
#include "photo_filters.h"

#define  LOG_TAG    "Jni ImageFilter"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C" JNIEXPORT void JNICALL Java_com_fanhl_photofilters_PhotoFilterApi_brightness(JNIEnv *env, jclass obj, jobject bitmap, jfloat brightnessValue) {
    bitmap_hold_pixels(env, bitmap, [brightnessValue](AndroidBitmapInfo *info, void *pixels) {
        brightness(info, pixels, brightnessValue);
    });
}

extern "C" JNIEXPORT void JNICALL Java_com_fanhl_photofilters_PhotoFilterApi_inverted(JNIEnv *env, jclass obj, jobject bitmap) {
    bitmap_hold_pixels(env, bitmap, [](AndroidBitmapInfo *info, void *pixels) {
        inverted(info, pixels);
    });
}

extern "C" JNIEXPORT void JNICALL Java_com_fanhl_photofilters_PhotoFilterApi_gray(JNIEnv *env, jclass obj, jobject bitmap) {
    bitmap_hold_pixels(env, bitmap, [](AndroidBitmapInfo *info, void *pixels) {
        gray(info, pixels);
    });
}

extern "C" JNIEXPORT void JNICALL Java_com_fanhl_photofilters_PhotoFilterApi_convolution(JNIEnv *env, jclass type, jobject bitmap, jobjectArray kernel) {
    bitmap_hold_pixels(env, bitmap, [kernel](AndroidBitmapInfo *info, void *pixels) {
        convolution(info, pixels, kernel);
    });
}