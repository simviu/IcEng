package com.simviu.IcEngTest;

import android.app.Activity;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.widget.FrameLayout;

import com.simviu.IcEng.IcEngJNI;
import com.simviu.IcEng.IcEngView;

public class MyTestActivity extends Activity {

    IcEngView m_IcEngView = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my_test);
        m_IcEngView = (IcEngView)findViewById(R.id.id_ic_eng_view);
        m_IcEngView.initWithContext(this);
    }


    @Override
    protected void onPause()
    {
        super.onPause();
        if(m_IcEngView!=null)
            m_IcEngView.onPause();
    }
    @Override
    protected void onResume()
    {
        super.onResume();
        if(m_IcEngView!=null)
            m_IcEngView.onResume();
    }
}
