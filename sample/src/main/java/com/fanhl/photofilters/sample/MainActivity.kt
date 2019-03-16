package com.fanhl.photofilters.sample

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.fanhl.photofilters.PhotoFilterApi
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        tv_title.text = PhotoFilterApi.test()
    }
}
