package com.fanhl.photofilters.sample.common

import com.fanhl.photofilters.sample.coroutines.Contexts
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Job

abstract class BaseViewModel : androidx.lifecycle.ViewModel(), CoroutineScope {
    override val coroutineContext by lazy { Contexts.UI + Job() }
}