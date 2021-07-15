package com.xpf.ch340_host.CH340.utils;

import android.os.Environment;

/**
 *  文件总类
 *
 */
public class MyFile {

    // 配置文件
    public static String configure = "configure.txt";

    /**
     *  得到本机文件目录
     *
     */
    public static String getPhonePath()
    {
        return Environment.getExternalStorageDirectory().getPath();
    }
}
