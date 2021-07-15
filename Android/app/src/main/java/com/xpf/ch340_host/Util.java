package com.xpf.ch340_host;

import java.math.BigDecimal;

public class Util {
    public static String getHex(byte data)
    {
        String hex= Integer.toHexString(data);
        if(hex.length()<2)
        {
            return "0x0"+hex;
        }
        return "0x"+hex.replace("ffffff","");
    }







    // 寻找众数
    public static int getMode(int[] NumberSet)
    {

        int[] a = ary2num(NumberSet);
        return a[findmax(count(NumberSet,a))];
//        int[] Ten = new int[NumberSet.length];
//        for(int i=0;i<NumberSet.length;i++){
//            Ten[i] = NumberSet[i]/10;
//        }
//        int[] a = ary2num(Ten);
//        int findNumber = Ten[findmax(count(Ten,a))];
//        int SUM   = 0;
//        int Total = 0;
//        System.out.print("TestPress：");
//        for(int i=0;i<Ten.length;i++){
//            if(findNumber==Ten[i]){
//                System.out.print(NumberSet[i]+" ");
//                SUM+=NumberSet[i];
//                Total++;
//            }
//        }
//        System.out.println();
//        return SUM/Total;

    }

    // 寻找众数
    public static float getMode(float[] NumberSet)
    {
        float[] a = ary2num(NumberSet);
        return a[findmax(count(NumberSet,a))];
    }

    // 求平均值
    public static float getAverage(float[] NumberSet)
    {
        float sum = 0;
        for(int i=0;i<NumberSet.length;i++)
        {
            sum+=NumberSet[i];
        }
        return sum/NumberSet.length;
    }

    private static float[] ary2num(float[] ary){
        float[] num_1 = new float[ary.length];
        int n =0;
        int yes = 0;
        for(int i = 0;i < ary.length;i++) {
            for (int j = 0; j < num_1.length; j++) {
                if (ary[i] == num_1[j]){
                    yes = 1;
                    break;
                }
            }
            if(yes == 0) {
                num_1[n] = ary[i];
                n++;
            }
            yes = 0;
        }
        float[] num = new float[n];
        for(int i = 0;i < n;i++) {
            num[i] = num_1[i];
        }
        return num;
    }

    private static float[] count(float[] array,float[] num) {
        float[] count = new float[num.length];
        for (int i = 0; i < num.length; i++) {
            for (int j = 0; j < array.length; j++) {
                if(array[j] == num[i]) {
                    count[i]++;
                }
            }
        }
        return count;
    }

    private static int findmax(float[] array) {
        int max_index = 0;
        for (int i = 0; i < array.length-1; i++) {
            if (array[i+1] > array[i]) {
                max_index = i+1;
            }
        }
        return max_index;
    }

    public static double TempOperation(double f)
    {
        BigDecimal b = new BigDecimal(f);
        double f1 = b.setScale(2, BigDecimal.ROUND_HALF_UP).doubleValue();
        return f1;
    }



    //ary2num:实现由若干个个自然数的集合到其中所含自然数种类集合的方法（去除重复出现的自然数）。
    static int[] ary2num(int[] ary){
        int[] num_1 = new int[ary.length];
        int n =0;
        int yes = 0;//yes为0时，表示之前尚未出现该自然数，录入num_1
        for(int i = 0;i < ary.length;i++) {
            for (int j = 0; j < num_1.length; j++) {
                if (ary[i] == num_1[j]){
                    yes = 1;
                    break;
                }
            }
            if(yes == 0) {
                num_1[n] = ary[i];
                n++;
            }
            yes = 0;
        }
        //System.out.println(n);
        //去掉num_1数组中的0
        int[] num = new int[n];
        for(int i = 0;i < n;i++) {
            num[i] = num_1[i];
        }
        return num;
    }

    //对num数组中的各个自然数的重数做统计
    static int[] count(int[] array,int[] num) {
        int[] count = new int[num.length];
        for (int i = 0; i < num.length; i++) {
            for (int j = 0; j < array.length; j++) {
                if(array[j] == num[i]) {
                    count[i]++;
                }
            }
        }
        return count;
    }
    //寻找count中的最大值,返回最大值索引
    static int findmax(int[] array) {
        int max_index = 0;
        for (int i = 0; i < array.length-1; i++) {
            if (array[i+1] > array[i]) {
                max_index = i+1;
            }
        }
        return max_index;

    }


}
