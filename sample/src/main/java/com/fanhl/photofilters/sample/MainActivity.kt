package com.fanhl.photofilters.sample

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
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
        
    }
}
