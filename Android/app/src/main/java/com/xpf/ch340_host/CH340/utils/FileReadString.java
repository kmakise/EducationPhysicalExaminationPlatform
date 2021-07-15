package com.xpf.ch340_host.CH340.utils;

import java.io.File;
import java.io.FileInputStream;
import java.util.Scanner;

public class FileReadString {
    public static String read(String path)
    {
        String result = "";
            try {
                File file = new File(MyFile.getPhonePath()+"/"+path);
                FileInputStream fileInputStream = new FileInputStream(file);
                Scanner sc = new Scanner(fileInputStream);
                while (sc.hasNext())
                {
                    result+=sc.nextLine();
                }
                return result;
            }catch (Exception e)
            {
            }
        return "";
    }
}
