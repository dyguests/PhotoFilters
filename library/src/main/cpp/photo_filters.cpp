//
// Created by 范鸿麟 on 2019/3/20.
//

#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <functional>
#include "photo_filters.h"

#define  LOG_TAG    "photo_filters"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

uint32_t get_pixel_clamp(AndroidBitmapInfo *info, void *pixels, int x, int y);

uint32_t get_pixel(AndroidBitmapInfo *info, void *pixels, int x, int y);

void operateKernel(AndroidBitmapInfo *info, void *pixels, int x, int y, int kernel[3][3], uint32_t *pixel);

/**
 * 在hold_process前为bitmap加锁，之后解锁
 * @param env
 * @param bitmap
 * @param hold_process
 */
void bitmap_hold_pixels(JNIEnv *env, jobject bitmap, const std::function<void(AndroidBitmapInfo *, void *)> &hold_process) {
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

    hold_process(&info, pixels);

    AndroidBitmap_unlockPixels(env, bitmap);
}

jobject copyBitmap(JNIEnv *env, jobject bitmap) {
    AndroidBitmapInfo info;
    int ret;
    void *pixels;
    void *newPixels;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return NULL;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return NULL;
    }
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    jclass bitmapCls = env->GetObjectClass(bitmap);
    jmethodID createBitmapFunction = env->GetStaticMethodID(bitmapCls, "createBitmap", "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jstring configName = env->NewStringUTF("ARGB_8888");
    jclass bitmapConfigClass = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID valueOfBitmapConfigFunction = env->GetStaticMethodID(bitmapConfigClass, "valueOf", "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");
    jobject bitmapConfig = env->CallStaticObjectMethod(bitmapConfigClass, valueOfBitmapConfigFunction, configName);
    jobject newBitmap = env->CallStaticObjectMethod(bitmapCls, createBitmapFunction, info.height, info.height, bitmapConfig);

    if ((ret = AndroidBitmap_lockPixels(env, newBitmap, &newPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return NULL;
    }

    for (int i = 0; i < info.width * info.height; i++) {
        ((char *) newPixels)[i * 4] = ((char *) pixels)[i * 4];
        ((char *) newPixels)[i * 4 + 1] = ((char *) pixels)[i * 4 + 1];
        ((char *) newPixels)[i * 4 + 2] = ((char *) pixels)[i * 4 + 2];
        ((char *) newPixels)[i * 4 + 3] = ((char *) pixels)[i * 4 + 3];
    }

    AndroidBitmap_unlockPixels(env, newBitmap);
    AndroidBitmap_unlockPixels(env, bitmap);

    return newBitmap;
}

/**
 * 过滤亮度
 * @param info
 * @param pixels
 * @param brightnessValue
 */
void brightness(AndroidBitmapInfo *info, void *pixels, jfloat brightnessValue) {
    int xx, yy, red, green, blue;
    uint32_t *line;

    for (yy = 0; yy < info->height; yy++) {
        line = (uint32_t *) pixels;
        for (xx = 0; xx < info->width; xx++) {
            //extract the RGB values from the pixel
            red = (int) ((line[xx] & 0x00FF0000) >> 16);
            green = (int) ((line[xx] & 0x0000FF00) >> 8);
            blue = (int) (line[xx] & 0x00000FF);

            //manipulate each value
            red = rgb_clamp((int) (red * brightnessValue));
            green = rgb_clamp((int) (green * brightnessValue));
            blue = rgb_clamp((int) (blue * brightnessValue));

            // set the new pixel back in
            line[xx] = (line[xx] & 0xFF000000)
                       | ((red << 16) & 0x00FF0000)
                       | ((green << 8) & 0x0000FF00)
                       | (blue & 0x000000FF);
        }

        pixels = (char *) pixels + info->stride;
    }
}

void gray(AndroidBitmapInfo *info, void *pixels) {
    int xx, yy, red, green, blue;
    uint32_t *line;

    for (yy = 0; yy < info->height; yy++) {
        line = (uint32_t *) pixels;
        for (xx = 0; xx < info->width; xx++) {
            //extract the RGB values from the pixel
            red = (int) ((line[xx] & 0x00FF0000) >> 16);
            green = (int) ((line[xx] & 0x0000FF00) >> 8);
            blue = (int) (line[xx] & 0x00000FF);

            int average = (red + green + blue) / 3;
            //manipulate each value
            red = average;
            green = average;
            blue = average;

            // set the new pixel back in
            line[xx] = (line[xx] & 0xFF000000)
                       | ((red << 16) & 0x00FF0000)
                       | ((green << 8) & 0x0000FF00)
                       | (blue & 0x000000FF);
        }

        pixels = (char *) pixels + info->stride;
    }
}

void inverted(AndroidBitmapInfo *info, void *pixels) {
    int xx, yy, red, green, blue;
    uint32_t *line;

    for (yy = 0; yy < info->height; yy++) {
        line = (uint32_t *) pixels;
        for (xx = 0; xx < info->width; xx++) {
            //extract the RGB values from the pixel
            red = (int) ((line[xx] & 0x00FF0000) >> 16);
            green = (int) ((line[xx] & 0x0000FF00) >> 8);
            blue = (int) (line[xx] & 0x00000FF);

            //manipulate each value
            red = rgb_clamp(255 - red);
            green = rgb_clamp(255 - green);
            blue = rgb_clamp(255 - blue);

            // set the new pixel back in
            line[xx] = (line[xx] & 0xFF000000)
                       | ((red << 16) & 0x00FF0000)
                       | ((green << 8) & 0x0000FF00)
                       | (blue & 0x000000FF);
        }

        pixels = (char *) pixels + info->stride;
    }
}

void convolution(JNIEnv *env, jobject bitmap, jobjectArray kernel) {
    // 把 jobjectArray 转换成 c数组
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
        int32_t *colData = env->GetIntArrayElements((jintArray) col, nullptr);
        for (jint x = 0; x < cols; x++) {
            kernelC[y][x] = (int) colData[x];
        }
    }

    jobject newBitmap = copyBitmap(env, bitmap);

    AndroidBitmapInfo info/*, newInfo*/;
    int ret;
    void *pixels, *newPixels;

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

//    if ((ret = AndroidBitmap_getInfo(env, newBitmap, &newInfo)) < 0) {
//        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
//        return;
//    }
//    if (newInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
//        LOGE("Bitmap format is not RGBA_8888 !");
//        return;
//    }
    if ((ret = AndroidBitmap_lockPixels(env, newBitmap, &newPixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    convolution(&info, pixels, newPixels, kernelC);

    AndroidBitmap_unlockPixels(env, newBitmap);
    AndroidBitmap_unlockPixels(env, bitmap);
}

void convolution(AndroidBitmapInfo *info, void *pixels, void *pixelsCopy, int kernel[3][3]) {
    uint32_t height = info->height;
    uint32_t width = info->width;

    //回家写 2019/03/22
    int x, y;//, red, green, blue;
    uint32_t pixel;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            operateKernel(info, pixelsCopy, x, y, kernel, &pixel);
            ((uint32_t *) ((char *) pixels + y * (info->stride)))[x] = pixel;
        }
    }
}

void operateKernel(AndroidBitmapInfo *info, void *pixels, int x, int y, int kernel[3][3], uint32_t *pixel) {
    int sumA = 0, sumR = 0, sumG = 0, sumB = 0;
    uint32_t argb;
    int alpha, red, green, blue;
    int kernelXY;
    for (int ky = -1; ky < 3 - 1; ky++) {
        for (int kx = -1; kx < 3 - 1; kx++) {
            argb = get_pixel_clamp(info, pixels, x + kx, y + ky);

//            if (ky == 0 && kx == 0) {
//                LOGI("原值 %d", argb);
//            }
            kernelXY = kernel[ky + 1][kx + 1];

            alpha = (int) ((argb & 0xFF000000) >> 24);
            red = (int) ((argb & 0x00FF0000) >> 16);
            green = (int) ((argb & 0x0000FF00) >> 8);
            blue = (int) (argb & 0x00000FF);

            alpha = alpha * kernelXY;
            red = red * kernelXY;
            green = green * kernelXY;
            blue = blue * kernelXY;

            sumA += alpha;
            sumR += red;
            sumG += green;
            sumB += blue;
        }
    }

    sumA = rgb_clamp(sumA);
    sumR = rgb_clamp(sumR);
    sumG = rgb_clamp(sumG);
    sumB = rgb_clamp(sumB);

//    *pixel = get_pixel_clamp(info, pixels, x, y);
    *pixel = ((sumA << 24) & 0xFF000000)
             | ((sumR << 16) & 0x00FF0000)
             | ((sumG << 8) & 0x0000FF00)
             | (sumB & 0x000000FF);

//    LOGI("新值 %d", &pixel);
}

uint32_t get_pixel_clamp(AndroidBitmapInfo *info, void *pixels, int x, int y) {
    if (x < 0) {
        x = 0;
    } else if (x >= info->width) {
        x = info->width - 1;
    }
    if (y < 0) {
        y = 0;
    } else if (y >= info->height) {
        y = info->height - 1;
    }

    return get_pixel(info, pixels, x, y);
}

uint32_t get_pixel(AndroidBitmapInfo *info, void *pixels, int x, int y) {
    return ((uint32_t *) ((char *) pixels + y * (info->stride)))[x];
}

/**
 * 单个颜色值在区间在 [0,255] 之间
 * @param value
 * @return
 */
int rgb_clamp(int value) {
    if (value > 255) {
        return 255;
    }
    if (value < 0) {
        return 0;
    }
    return value;
}
