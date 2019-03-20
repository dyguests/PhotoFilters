#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>

#define  LOG_TAG    "Jni ImageFilter"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

void brightness(AndroidBitmapInfo *info, void *pixels, jfloat brightnessValue);

void inverted(AndroidBitmapInfo *info, void *pixels);

int rgb_clamp(int value);

void gray(AndroidBitmapInfo *info, void *pixels);

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

    inverted(&info, pixels);

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

int rgb_clamp(int value) {
    if (value > 255) {
        return 255;
    }
    if (value < 0) {
        return 0;
    }
    return value;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_fanhl_photofilters_PhotoFilterApi_gray(JNIEnv *env, jclass type, jobject bitmap) {
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

    gray(&info, pixels);

    AndroidBitmap_unlockPixels(env, bitmap);
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