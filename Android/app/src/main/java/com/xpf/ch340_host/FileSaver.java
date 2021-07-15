package com.xpf.ch340_host;


import android.graphics.Bitmap;
import android.os.Environment;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintStream;

/**
 *
 *  此类用于保存何种类型的文件
 *  Author : DemonRatCreator
 *  Creation time : 2019/5/15
 *  Change Time   : ...(修改内容，添加内容，原因)
 *
 * */
public class FileSaver {

    public static void saveText(String text)
    {
        File file = new File(Environment.getExternalStorageDirectory().getPath()+"/"+"test.txt");
        try {
            FileOutputStream fileOutputStream = new FileOutputStream(file,true);
            PrintStream printStream = new PrintStream(fileOutputStream);
            printStream.println(text);
        }catch (Exception e)
        {
        }
    }

    public static void savePicture(Bitmap bm, String path) {
        try {
            File myCaptureFile = new File(path);
            if (!myCaptureFile.exists()) {
                myCaptureFile.createNewFile();
            }
            BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(myCaptureFile));
            bm.compress(Bitmap.CompressFormat.JPEG, 80, bos);
            bos.flush();
            bos.close();
        }catch (Exception e)
        {

        }
    }


    public static void savePicture(Bitmap bm, String path, String name){
        try {
            String subForder = path;
            File foder = new File(subForder);
            if (!foder.exists()) {
                foder.mkdirs();
            }
            File myCaptureFile = new File(subForder, name);
            if (!myCaptureFile.exists()) {
                myCaptureFile.createNewFile();
            }
            BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(myCaptureFile));
            bm.compress(Bitmap.CompressFormat.JPEG, 80, bos);
            bos.flush();
            bos.close();
        }catch (Exception e)
        {
        }
    }
}
