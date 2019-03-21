//
// Created by 范鸿麟 on 2019/3/20.
//

#include <jni.h>
#include <android/bitmap.h>
#include <android/log.h>
#include <functional>
#include "photo_filters.h"

#define  LOG_TAG    "photo_filters"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

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

void convolution(AndroidBitmapInfo *info, void *pixels, jobjectArray kernel) {
    int x, y, r, g, b;
    uint32_t *line;

    for (y = 0; y < info->height; y++) {
        line = (uint32_t *) pixels;
        for (x = 0; x < info->width; x++) {

        }
    }
    pixels = (char *) pixels + info->stride;
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
