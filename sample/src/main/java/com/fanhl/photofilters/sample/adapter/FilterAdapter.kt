package com.fanhl.photofilters.sample.adapter

import com.chad.library.adapter.base.BaseQuickAdapter
import com.chad.library.adapter.base.BaseViewHolder
import com.fanhl.photofilters.sample.model.Filter
import com.fanhl.photofilters.sample.R

class FilterAdapter : BaseQuickAdapter<Filter, BaseViewHolder>(R.layout.item_sample) {
    override fun convert(helper: BaseViewHolder?, item: Filter?) {
    }
}
