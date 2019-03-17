package com.fanhl.photofilters.sample.adapter

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import androidx.recyclerview.widget.RecyclerView
import com.chad.library.adapter.base.BaseQuickAdapter
import com.chad.library.adapter.base.BaseViewHolder
import com.fanhl.photofilters.sample.R
import com.fanhl.photofilters.sample.coroutines.Contexts
import com.fanhl.photofilters.sample.coroutines.withContextIO
import com.fanhl.photofilters.sample.domain.model.Filter
import kotlinx.android.synthetic.main.item_sample.view.*
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Job
import kotlinx.coroutines.launch
import kotlin.coroutines.CoroutineContext

class FilterAdapter : BaseQuickAdapter<Filter, BaseViewHolder>(R.layout.item_sample), CoroutineScope {
    //region CoroutineScope implementation
    private val rootJob = Job()

    override val coroutineContext: CoroutineContext = Contexts.UI + rootJob

    private val original by lazy { BitmapFactory.decodeResource(mContext.resources, R.drawable.test) }

    override fun onDetachedFromRecyclerView(recyclerView: RecyclerView) {
        super.onDetachedFromRecyclerView(recyclerView)
        rootJob.cancel()
    }

    override fun convert(helper: BaseViewHolder?, item: Filter?) {
        helper?.itemView?.apply {
            launch {
                withContextIO {
                    val bitmap = original.copy(Bitmap.Config.ARGB_8888, true)
                    item?.action?.invoke(bitmap)
//                    withContextUI {
                    img_type.setImageBitmap(bitmap)
//                    }
                }
            }
            tv_type.text = item?.name
        }
    }
}
