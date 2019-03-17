package com.fanhl.photofilters.sample.ui.main

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModelProvider
import com.fanhl.photofilters.sample.R
import com.fanhl.photofilters.sample.adapter.FilterAdapter
import com.fanhl.photofilters.sample.arch.BaseViewModel
import kotlinx.android.synthetic.main.activity_main.*
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {

    private val viewModel by lazy { ViewModelProvider.AndroidViewModelFactory.getInstance(application).create(ViewModel::class.java) }

    private val adapter by lazy { FilterAdapter() }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        initData()
        refreshData()
    }

    private fun initData() {
        recycler_view.adapter = adapter
    }

    private fun refreshData() {
        viewModel.refreshData()
    }

    private class ViewModel : BaseViewModel<MainViewState>(Initial) {
        fun refreshData() = launch {

        }
    }
}
