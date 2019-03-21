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
     *         bitmap
     * @param brightness
     *         亮度 [0,+max)
     */
    public native static void brightness(Bitmap bmp, float brightness);

    /**
     * 反色
     *
     * @param bmp
     *         bitmap
     */
    public native static void inverted(Bitmap bmp);

    /**
     * 反色
     *
     * @param bmp
     *         bitmap
     */
    public native static void gray(Bitmap bmp);

    /**
     * 卷积
     *
     * @param bmp
     *         bitmap
     * @param kernel
     *         卷积核
     */
    public native static void convolution(Bitmap bmp, byte[][] kernel);
}
