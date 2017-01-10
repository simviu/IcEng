/**
 * Created by schen on 12/22/16.
 */

package com.simviu.IcEng;

import android.content.res.AssetManager;

import java.io.FileOutputStream;

import android.util.Log;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import android.content.Context;

public class IcAssetHelper {

    public IcAssetHelper(Context context, String sPathDestRoot)
    {
        m_sPathDestRoot = sPathDestRoot;
        m_context = context;
    };
    protected String m_sPathDestRoot = "";
    protected Context m_context = null;
    //------------------------------
    //  copyAssets()
    //------------------------------
    public void copyAssetsDir(String sPath) {
        // Notes : Get assetManager and sPathDestRoot in Activity by :
        //      String sPathDestRoot = Environment.getExternalStorageDirectory().getPath().toString();
        //      AssetManager assetManager = getAssets();
        AssetManager assetManager = m_context.getAssets();
        String sPathDest = m_sPathDestRoot + "/"+ sPath;


        File outPath = new File(sPathDest);
        outPath.mkdirs();
        String[] files = null;
        try {
            files = assetManager.list(sPath);
        } catch (IOException e) {
            Log.e("tag", "Failed to get asset file list.", e);
        }
        if (files == null) return;
        for (String filename : files) {
            InputStream in = null;
            OutputStream out = null;

            try {
                //---- Check wether it's sub-dir
                String subPath = sPath + "/" + filename;
                String[] subFiles = assetManager.list(subPath);

                //---- Recursive copy subdir
                if(subFiles.length>0) {
                    copyAssetsDir(sPath + "/" + filename);
                    continue;
                }


                //---- It's file

                in = assetManager.open(sPath + "/" + filename);
                File outFile = new File(sPathDest + "/" + filename);
                out = new FileOutputStream(outFile);
                copyFile(in, out);
            } catch(IOException e) {
                Log.e("tag", "Failed to copy asset file: " + filename, e);
            }
            catch (Exception e)
            {
                Log.e("tag", "Unkown Exception: ", e);
            }
            finally {
                if (in != null) {
                    try {
                        in.close();
                    } catch (IOException e) {
                        // NOOP
                    }
                }
                if (out != null) {
                    try {
                        out.close();
                    } catch (IOException e) {
                        // NOOP
                    }
                }
            }
        }
    }
    //------------------------------
    //  copyFile()
    //------------------------------
    private void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
            out.write(buffer, 0, read);
        }
    }
}
