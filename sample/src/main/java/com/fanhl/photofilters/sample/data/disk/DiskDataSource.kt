package com.fanhl.photofilters.sample.data.disk

import com.fanhl.photofilters.PhotoFilterApi
import com.fanhl.photofilters.sample.domain.model.Filter

class DiskDataSource {
    fun getFilters() = listOf(
        Filter("Default") { },
        Filter("Brightness") { PhotoFilterApi.brightness(it, 2f) },
        Filter("Inverted") { PhotoFilterApi.inverted(it) },
        Filter("Gray") { PhotoFilterApi.gray(it) },
        Filter("Sharpen") {
            PhotoFilterApi.convolution(
                it,
                arrayOf(
                    intArrayOf(-1, -1, -1),
                    intArrayOf(-1, 9, -1),
                    intArrayOf(-1, -1, -1)
                )
            )
        },
        Filter("Edge Sharpen") {
            PhotoFilterApi.convolution(
                it,
                arrayOf(
                    intArrayOf(0, -1, 0),
                    intArrayOf(-1, 5, -1),
                    intArrayOf(0, -1, 0)
                )
            )
        },
        Filter("Prewitt") {
            PhotoFilterApi.convolution(
                it,
                arrayOf(
                    intArrayOf(-1, 0, 1),
                    intArrayOf(-1, 0, 1),
                    intArrayOf(-1, 0, 1)
                )
            )
        },
        Filter("Vertical Prewitt") {
            PhotoFilterApi.convolution(
                it,
                arrayOf(
                    intArrayOf(-1, -1, 1),
                    intArrayOf(0, 0, 0),
                    intArrayOf(-1, 1, 1)
                )
            )
        },
        Filter("Soble") {
            PhotoFilterApi.convolution(
                it,
                arrayOf(
                    intArrayOf(-1, 0, 1),
                    intArrayOf(-2, 0, 2),
                    intArrayOf(-1, 0, 1)
                )
            )
        },
        Filter("Vertical Soble") {
            PhotoFilterApi.convolution(
                it,
                arrayOf(
                    intArrayOf(-1, -2, 1),
                    intArrayOf(0, 0, 0),
                    intArrayOf(-1, 2, 1)
                )
            )
        },
        Filter("Laplacian") {
            PhotoFilterApi.convolution(
                it,
                arrayOf(
                    intArrayOf(1, 1, 1),
                    intArrayOf(1, -8, 1),
                    intArrayOf(1, 1, 1)
                )
            )
        }
    )
}