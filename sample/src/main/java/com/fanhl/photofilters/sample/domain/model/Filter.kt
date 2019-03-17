package com.fanhl.photofilters.sample.domain.model

import android.graphics.Bitmap

/**
 * 滤镜参数
 */
data class Filter(
    val name: String,
    val action: (Bitmap) -> Unit
)
