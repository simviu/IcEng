package com.simviu.IcEngDemo;

import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.simviu.IcEng.IcEngJNI;
import com.simviu.IcEng.IcEngView;

import java.util.StringTokenizer;

public class DemoListActivity extends ListActivity {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("IcEngDemo");
    }


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //---- Init App at very beginning
        String sPathCache = getCacheDir().toString();
        initIcApp(sPathCache);
        IcEngJNI.copyAssetDir(this, "IcShader");
        IcEngJNI.copyAssetDir(this, "IcDemo");


        //----- Build List
        // The list is sent back from DemoApp::onCmd, via JNI interface,
        // Strings separated by ";";
        String sDemos = IcEngJNI.sendAppCmd("list");
        String[] sDemoAry = sDemos.split(";");
        setListAdapter(new ArrayAdapter<String>(this, R.layout.activity_demo_list,sDemoAry));

        ListView listView = getListView();
        listView.setTextFilterEnabled(true);

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id) {

                //----- For Debug
                // When clicked, show a toast with the TextView text
               Toast.makeText(getApplicationContext(),
                        ((TextView) view).getText(), Toast.LENGTH_SHORT).show();

                 //---- Start DemoActivity
                Intent intent = new Intent(DemoListActivity.this, DemoActivity.class);
                DemoListActivity.this.startActivity(intent);

                //---- Send App cmd
                String sCmd = "sel:"+Integer.toString(position);
                IcEngJNI.sendAppCmd(sCmd);

            }
        });

    }
    static native void initIcApp(String sPathCache);

}
