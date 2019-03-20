//
// Created by 范鸿麟 on 2019/3/20.
//

#include <jni.h>
#include <android/bitmap.h>
#include "photo_filters.h"


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
