package com.xpf.ch340_host;

import com.xpf.ch340_host.CH340.utils.ByteUtil;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.nio.ByteBuffer;

public class Agreement {

    private static int     BufSize                    = 30;
    private static float[] HemoglobinWaveformTwoBuf   = new float[BufSize];
    private static int     HemoglobinWaveformTwoIndex = 0;


    private static float[] HemoglobinWaveformOneBuf   = new float[BufSize];
    private static int     HemoglobinWaveformOneIndex = 0;


    public float distance;                   // 测距
    public float pressure;                   // 压力
    public float heartRate;                  // 心率
    public float OxygenSaturation;           // 血氧饱和度
    public float HemoglobinWaveformOne;      // 血红蛋白波形1
    public float HemoglobinWaveformTwo;      // 血红蛋白波形2
    public float PeopleTemperature;          // 人体非接触测温
    public float EnvironmentSmoke;           // 环境烟雾
    public float EnvironmentTemperature;     // 环境温度
    public float EnvironmentHumidity;        // 环境湿度
    public float light;                      // 光照度


    private static Agreement agreement;    // 单例模式


    public static void init(){
        agreement = new Agreement();
    }


    public static Agreement getAgreement(){
        return agreement;
    }

    // 更新数据
    public Agreement update(byte[] data) {
        agreement.distance               = getDistance(data);
        agreement.pressure               = getPressure(data);
        agreement.heartRate              = getHeartRate(data);
        agreement.OxygenSaturation       = getOxygenSaturation(data);
        agreement.EnvironmentSmoke       = getEnvironmentSmoke(data);
        agreement.EnvironmentTemperature = getEnvironmentTemperature(data);
        agreement.EnvironmentHumidity    = getEnvironmentHumidity(data);
        agreement.PeopleTemperature      = getPeopleTemperature(data);
        agreement.HemoglobinWaveformOne  = getHemoglobinWaveformOne(data);
        agreement.HemoglobinWaveformTwo  = getHemoglobinWaveformTwo(data);
        agreement.light                  = getLight(data);
        return agreement;
    }

    public String show(){
        return agreement.distance+"cm  "+
               agreement.pressure+"kg  "+
               agreement.heartRate+"%  "+
               agreement.OxygenSaturation+"%  "+
               agreement.HemoglobinWaveformOne+" One  "+
               agreement.HemoglobinWaveformTwo+" Two  "+
               agreement.PeopleTemperature+"°C  "+
               agreement.EnvironmentSmoke+"  "+
               agreement.EnvironmentTemperature+"°C  "+
               agreement.EnvironmentHumidity+"  "+
               agreement.light+"L  ";
    }


    // 测距
    public float getDistance(byte[] data) {
        return data[4];
    }

    public float getPressure(byte[] data) {
        byte[] data_ = new byte[2];
        data_[0] = data[5];
        data_[1] = data[6];
        float data__ = ByteUtil.getChar(data_);
//        if(data__/100 > 40){
//            if(pressureNumber != -1){
//                pressureBuf[pressureNumber] = (int)(data__/100);
//                pressureNumber++;
//                if(pressureNumber == 300){
//                    // 归位
//                    pressureNumber = -1;
//                    // 返回众数 并+1000 体重标红表示测量结果
//                    StablePressure = Util.getMode(pressureBuf)+100000;
//                    return StablePressure;
//                }
//                return data__/100;
//            }
//        }else {
//            if(pressureNumber == -1)
//            {
//                StablePressure = 0;
//                pressureNumber = 0;
//            }
//        }
        return data__;
    }

    // 心率
    public float getHeartRate(byte[] data) {
        return data[7];
    }

    public float getOxygenSaturation(byte[] data) {
        byte[] data_ = new byte[4];
        data_[0] = data[8];
        data_[1] = data[9];
        data_[2] = data[10];
        data_[3] = data[11];
        return ByteUtil.getFloat(data_);
    }

    public float getHemoglobinWaveformOne(byte[] data) {
        byte[] data_ = new byte[4];
        data_[0] = data[12];
        data_[1] = data[13];
        data_[2] = data[14];
        data_[3] = data[15];
        float dataTemp = ByteUtil.getFloat(data_);
        float return_data = 0;
        HemoglobinWaveformOneBuf[HemoglobinWaveformOneIndex] = dataTemp;

        for(int i=0;i<BufSize;i++)
        {
            return_data+=HemoglobinWaveformOneBuf[i];
        }
        return_data/=BufSize;
        HemoglobinWaveformOneIndex = (HemoglobinWaveformOneIndex < BufSize-1)?HemoglobinWaveformOneIndex+1:0;
        return return_data;
    }

    public float getHemoglobinWaveformTwo(byte[] data) {
        byte[] data_ = new byte[4];
        data_[0] = data[16];
        data_[1] = data[17];
        data_[2] = data[18];
        data_[3] = data[19];
        float dataTemp = ByteUtil.getFloat(data_);
        float return_data = 0;
        HemoglobinWaveformTwoBuf[HemoglobinWaveformTwoIndex] = dataTemp;

        for(int i=0;i<BufSize;i++)
        {
            return_data+=HemoglobinWaveformTwoBuf[i];
        }
        return_data/=BufSize;
        HemoglobinWaveformTwoIndex = (HemoglobinWaveformTwoIndex < BufSize-1)?HemoglobinWaveformTwoIndex+1:0;
        return return_data;
    }

    public float getPeopleTemperature(byte[] data) {
        byte[] data_ = new byte[4];
        data_[0] = data[20];
        data_[1] = data[21];
        data_[2] = data[22];
        data_[3] = data[23];
        return ByteUtil.getFloat(data_);
    }

    // 环境烟雾
    public float getEnvironmentSmoke(byte[] data) {
        return data[24]& 0xff;
    }

    // 环境温度
    public float getEnvironmentTemperature(byte[] data) {
        return data[25]& 0xff;
    }

    // 环境湿度
    public float getEnvironmentHumidity(byte[] data) {
        return data[26]& 0xff;
    }

    public float getLight(byte[] data) {
        byte[] data_ = new byte[2];
        data_[1] = data[28];
        data_[0] = data[27];
        return ByteUtil.getChar(data_);
    }
}
