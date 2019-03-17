package com.fanhl.photofilters.sample.ui.main

import com.fanhl.photofilters.sample.domain.model.Filter

sealed class MainViewState

object Initial : MainViewState()

class Loaded(val filters: List<Filter>) : MainViewState()