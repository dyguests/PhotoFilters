package com.fanhl.photofilters.sample.data.disk

import com.fanhl.photofilters.sample.domain.model.Filter

class DiskDataSource {
    fun getFilters() = listOf(
        Filter(
            "Brightness"
        )
    )
}