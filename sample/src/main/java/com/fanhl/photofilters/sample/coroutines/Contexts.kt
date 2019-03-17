package com.fanhl.photofilters.sample.coroutines

import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.asCoroutineDispatcher
import kotlinx.coroutines.withContext
import java.util.concurrent.Executors

object Contexts {
    val UI = Dispatchers.Main
    val IO = Executors.newFixedThreadPool(4).asCoroutineDispatcher()
}

public suspend fun <T> withContextUI(
    block: suspend CoroutineScope.() -> T
) = withContext(Contexts.UI, block)

public suspend fun <T> withContextIO(
    block: suspend CoroutineScope.() -> T
) = withContext(Contexts.IO, block)