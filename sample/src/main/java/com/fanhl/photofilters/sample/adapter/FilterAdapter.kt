package com.fanhl.photofilters.sample.adapter

import com.chad.library.adapter.base.BaseQuickAdapter
import com.chad.library.adapter.base.BaseViewHolder
import com.fanhl.photofilters.sample.R
import com.fanhl.photofilters.sample.domain.model.Filter
import kotlinx.android.synthetic.main.item_sample.view.*

class FilterAdapter : BaseQuickAdapter<Filter, BaseViewHolder>(R.layout.item_sample) {
    override fun convert(helper: BaseViewHolder?, item: Filter?) {
        helper?.itemView?.apply {
            img_type
            tv_type.text = item?.name
        }
    }
}
