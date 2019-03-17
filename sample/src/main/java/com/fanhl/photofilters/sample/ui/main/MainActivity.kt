package com.fanhl.photofilters.sample.ui.main

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.Observer
import androidx.lifecycle.ViewModelProvider
import com.fanhl.photofilters.sample.R
import com.fanhl.photofilters.sample.adapter.FilterAdapter
import com.fanhl.photofilters.sample.arch.BaseViewModel
import com.fanhl.photofilters.sample.coroutines.withContextIO
import com.fanhl.photofilters.sample.data.disk.DiskDataSource
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {

    private val viewModel by lazy { ViewModelProvider.AndroidViewModelFactory.getInstance(application).create(ViewModel::class.java) }

    private val adapter by lazy { FilterAdapter() }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        assignViews()
        initData()
        refreshData()
    }

    private fun assignViews() {
        viewModel.state.observe(this@MainActivity, Observer { viewState ->
            viewState?.let { render(it) }
        })
    }

    private fun initData() {
        recycler_view.adapter = adapter
    }

    private fun refreshData() {
        viewModel.refreshData()
    }

    private fun render(viewState: MainViewState) {
        when (viewState) {
            is Initial -> {
            }
            is Loaded -> {
                adapter.setNewData(viewState.filters)
            }
        }
    }

    private class ViewModel : BaseViewModel<MainViewState>(Initial) {
        private val diskDataSource by lazy { DiskDataSource() }
        fun refreshData() = launch {
            val list = withContextIO {
                diskDataSource.getFilters()
            }

            viewState = Loaded(list)
        }
    }
}
