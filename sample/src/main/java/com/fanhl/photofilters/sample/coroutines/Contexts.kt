package com.fanhl.photofilters.sample.coroutines

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.asCoroutineDispatcher
import java.util.concurrent.Executors

object Contexts {
    val UI = Dispatchers.Main
    val IO = Executors.newFixedThreadPool(4).asCoroutineDispatcher()
}
