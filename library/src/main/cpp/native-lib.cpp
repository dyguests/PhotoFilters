#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>
//#include <photo_filters.h>
#include "photo_filters.h"

#define  LOG_TAG    "Jni ImageFilter"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C" JNIEXPORT void JNICALL Java_com_fanhl_photofilters_PhotoFilterApi_brightness(JNIEnv *env, jclass obj, jobject bitmap, jfloat brightnessValue) {
    AndroidBitmapInfo info;
    int ret;
    void *pixels;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    brightness(&info, pixels, brightnessValue);

    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL Java_com_fanhl_photofilters_PhotoFilterApi_inverted(JNIEnv *env, jclass type, jobject bitmap) {
    bitmap_hold_pixels(env, bitmap, [](AndroidBitmapInfo *info, void *pixels) {
        inverted(info, pixels);
    });
}

extern "C" JNIEXPORT void JNICALL Java_com_fanhl_photofilters_PhotoFilterApi_gray(JNIEnv *env, jclass type, jobject bitmap) {
    bitmap_hold_pixels(env, bitmap, [](AndroidBitmapInfo *info, void *pixels) {
        gray(info, pixels);
    });
}

