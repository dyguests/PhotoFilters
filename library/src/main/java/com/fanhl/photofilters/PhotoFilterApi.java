package com.fanhl.photofilters;

public class PhotoFilterApi {
    static {
        System.loadLibrary("native-lib");
    }

    public native static String test();
}
