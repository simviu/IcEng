package com.simviu.IcVRDemo;

import android.app.Activity;
import android.os.Bundle;

import com.simviu.IcEng.IcEngView;

/**
 * Created by schen on 3/16/17.
 */

public class VRDemoActivity extends Activity {
    IcEngView m_icEngView = null;
    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        setContentView(R.layout.activity_vrdemo);
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
}
