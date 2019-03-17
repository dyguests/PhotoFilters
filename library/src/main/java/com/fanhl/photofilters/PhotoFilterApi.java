package com.fanhl.photofilters;

import android.graphics.Bitmap;

public class PhotoFilterApi {
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * 变亮
     *
     * @param bmp
     * @param brightness
     */
    public native static void brightness(Bitmap bmp, float brightness);

    /**
     * 反色
     *
     * @param bmp
     */
    public native static void inverted(Bitmap bmp);
}
