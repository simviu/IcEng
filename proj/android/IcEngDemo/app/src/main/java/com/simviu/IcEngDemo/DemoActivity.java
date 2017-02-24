package com.simviu.IcEngDemo;

import android.app.Activity;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.simviu.IcEng.IcEngJNI;
import com.simviu.IcEng.IcAssetHelper;
import com.simviu.IcEng.IcEngView;

public class DemoActivity  extends Activity{

    IcEngView m_icEngView = null;
    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        setContentView(R.layout.activity_demo);
        m_icEngView = (IcEngView)findViewById(R.id.id_ic_eng_view);
    }

    @Override protected void onPause() {
        super.onPause();
        if(m_icEngView!=null)
            m_icEngView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        if(m_icEngView!=null)
            m_icEngView.onResume();
    }


    public void onButton_Close(View v) {
        // does something
        finish();
    }
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
}
