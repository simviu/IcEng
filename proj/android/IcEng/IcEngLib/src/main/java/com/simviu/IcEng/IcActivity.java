package com.simviu.IcEng;

import android.app.Activity;
import android.os.Bundle;

/**
 * Created by schen on 1/4/17.
 */

public class IcActivity extends Activity implements IcEngView.RendererCallBack{


    IcEngView mView;
    protected String m_sPathRes=""; // Res Path, Default set by getCacheDir().toString()

    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        mView = new IcEngView(getApplication(), this);
        setContentView(mView);
        m_sPathRes = getCacheDir().toString();
    }

    @Override protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }

    protected void copyAssetDir(String sPath)
    {
        IcAssetHelper hlpr = new IcAssetHelper(this, m_sPathRes);
        hlpr.copyAssetsDir(sPath);
    }
    //-------------------------------------
    //  IcEng Renderer call back
    //-------------------------------------
    public void IcEng_onInit()
    {
        IcEngJNI.onInit(m_sPathRes);
    }
    public void IcEng_onViewSize(int w, int h)
    {
        IcEngJNI.onScreenSize(w,h);
    }
    public void IcEng_onDrawUpdate(float deltaT)
    {
        IcEngJNI.onDrawUpdate(deltaT);
    }

}
