package com.fanhl.photofilters.sample.adapter

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import com.chad.library.adapter.base.BaseQuickAdapter
import com.chad.library.adapter.base.BaseViewHolder
import com.fanhl.photofilters.sample.R
import com.fanhl.photofilters.sample.domain.model.Filter
import kotlinx.android.synthetic.main.item_sample.view.*

class FilterAdapter : BaseQuickAdapter<Filter, BaseViewHolder>(R.layout.item_sample) {
    private val original by lazy { BitmapFactory.decodeResource(mContext.resources, R.drawable.test) }

    override fun convert(helper: BaseViewHolder?, item: Filter?) {
        helper?.itemView?.apply {
            val bitmap = original.copy(Bitmap.Config.ARGB_8888, true)
            item?.action?.invoke(bitmap)
            img_type.setImageBitmap(bitmap)
            tv_type.text = item?.name
        }
    }
}
