package com.fanhl.photofilters;

import android.graphics.Bitmap;

public class PhotoFilterApi {
    static {
        System.loadLibrary("native-lib");
    }

    public native static String test();

    /**
     * 过滤亮度
     *
     * @param bmp
     * @param brightness
     */
    public native static void brightness(Bitmap bmp, float brightness);
}
