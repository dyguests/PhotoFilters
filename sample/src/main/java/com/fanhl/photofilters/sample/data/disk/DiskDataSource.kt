package com.fanhl.photofilters.sample.data.disk

import com.fanhl.photofilters.PhotoFilterApi
import com.fanhl.photofilters.sample.domain.model.Filter

class DiskDataSource {
    fun getFilters() = listOf(
        Filter("Brightness") { PhotoFilterApi.brightness(it, 2f) },
        Filter("Inverted") { PhotoFilterApi.inverted(it) },
        Filter("Gray") { PhotoFilterApi.gray(it) },
        Filter("Convolution") {
            PhotoFilterApi.convolution(
                it,
                arrayOf(
                    intArrayOf(1, 0, 0),
                    intArrayOf(0, 1, 0),
                    intArrayOf(0, 0, -1)
                )
            )
        }
    )
}