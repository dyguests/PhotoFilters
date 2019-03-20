//
// Created by 范鸿麟 on 2019/3/20.
//

#include <android/bitmap.h>
#include "photo_filters.h"


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