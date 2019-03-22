#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
//#include <photo_filters.h>
#include "photo_filters.h"

#define  LOG_TAG    "Jni native-lib"
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

extern "C" JNIEXPORT void JNICALL Java_com_fanhl_photofilters_PhotoFilterApi_convolution(JNIEnv *env, jclass obj, jobject bitmap, jobjectArray kernel) {
    jsize rows, cols;

    rows = env->GetArrayLength(kernel);

    if (rows % 2 == 0 || rows == 0) {
        LOGE("wrong kernel");
        return;
    }

    //目前只管3x3的
    if (rows != 3) {
        LOGE("wrong kernel");
        return;
    }

    jarray line = (jintArray) env->GetObjectArrayElement(kernel, 0);

    cols = env->GetArrayLength(line);

    if (rows != cols) {
        LOGE("wrong kernel");
        return;
    }

    jsize colsTmp;
    for (jint i = 1; i < rows; i++) {
        jarray lineTmp = (jintArray) env->GetObjectArrayElement(kernel, i);
        colsTmp = env->GetArrayLength(lineTmp);
        if (cols != colsTmp) {
            LOGE("wrong kernel");
            return;
        }
    }

    int kernelC[3][3];

    for (jint y = 0; y < rows; y++) {
        jarray col = (jintArray) env->GetObjectArrayElement(kernel, y);
        jint *colData = env->GetIntArrayElements((jintArray) col, nullptr);
        for (jint x = 0; x < cols; x++) {
            kernelC[rows][cols] = (int) colData[x];
        }
    }

    bitmap_hold_pixels(env, bitmap, [kernelC](AndroidBitmapInfo *info, void *pixels) {
        convolution(info, pixels, kernelC);
    });
}
