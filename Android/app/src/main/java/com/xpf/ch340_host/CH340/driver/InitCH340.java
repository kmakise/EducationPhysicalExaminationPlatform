package com.xpf.ch340_host.CH340.driver;

import android.annotation.SuppressLint;
import android.content.Context;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;

import com.xpf.ch340_host.CH340.logger.LogUtils;
import com.xpf.ch340_host.CH340.runnable.ReadDataRunnable;
import java.util.HashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import cn.wch.ch34xuartdriver.CH34xUARTDriver;


/**
 * Created by xpf on 2017/11/22.
 * Function:初始化 ch340 驱动
 */
public class InitCH340 {

    private static final String TAG = InitCH340.class.getSimpleName();
    private static final String ACTION_USB_PERMISSION = "com.linc.USB_PERMISSION";
    private static final ExecutorService mThreadPool = Executors.newSingleThreadExecutor();
    private static final byte parity = 0;
    private static final byte stopBit = 1;
    private static final byte dataBit = 8;
    private static final int baudRate = 115200;
    private static final int WeightbaudRate = 9600;  // 体重传感器波特率
    private static final byte flowControl = 0;
    private static boolean IsStart = false;  // 线程已开启？
    @SuppressLint("StaticFieldLeak")
    // 其他传感器
    private static CH34xUARTDriver Driver;
    // 体重传感器
    private static CH34xUARTDriver Weightdriver;
    private static boolean isOpenDeviceCH340 = false;
    private static boolean isOpenDeviceCH340Weight = false;
    private static ReadDataRunnable readDataRunnable;
    private static UsbManager mUsbManager;
    private static IUsbPermissionListener listener;
    // 其他传感器
    private static UsbDevice mUsbDevice;
    // 体重传感器
    private static UsbDevice WeightUsbDevice;


    public static UsbDevice getUsbDevice() {
        return mUsbDevice;
    }

    public static void setListener(IUsbPermissionListener listener) {
        InitCH340.listener = listener;
    }

    /**
     * initialize ch340 parameters.
     *
     * @param context Application context.
     */
    public static void initCH340(Context context) {
        if (context == null) return;
        Context appContext = context.getApplicationContext();
        mUsbManager = (UsbManager) appContext.getSystemService(Context.USB_SERVICE);
        if (mUsbManager != null) {
            HashMap<String, UsbDevice> deviceHashMap = mUsbManager.getDeviceList();
            for (UsbDevice device : deviceHashMap.values()) {
                System.out.println("日志消息:"+device.getDeviceId());
                switch (device.getProductName()){

                    case "USB Serial":
                        // 其他传感器
                        if(device.getDeviceId()>3000){
                            System.out.println("日志消息:"+"开始执行115200");
                            // 波特率115200
                            mUsbDevice = device;
                            if (mUsbManager.hasPermission(device)) {
                                loadDriver(appContext, mUsbManager,baudRate);
                            }
                        }
                        // 体重计
                        else{
                            System.out.println("日志消息:"+"开始执行9600");
                            WeightUsbDevice = device;
                            if (mUsbManager.hasPermission(device)) {
                                loadDriver(appContext, mUsbManager,WeightbaudRate);
                            }
                        }
                        break;
                }
            }
        }
    }

    /**
     * load ch340 driver.
     *
     * @param appContext
     * @param usbManager
     */
    public static void loadDriver(Context appContext, UsbManager usbManager,int mbaudRate) {
        switch (mbaudRate){
            case baudRate:
                Driver = new CH34xUARTDriver(usbManager, appContext, ACTION_USB_PERMISSION);
                openCH340(mbaudRate);
                break;
            case WeightbaudRate:
                Weightdriver = new CH34xUARTDriver(usbManager, appContext, ACTION_USB_PERMISSION);
                openCH340(mbaudRate);
                break;
        }

    }

    /**
     * config and open ch340.
     */
    private static void openCH340(int mbaudRate) {
        switch (mbaudRate){
            case baudRate:
                int ret_val = Driver.ResumeUsbList();
                // ResumeUsbList方法用于枚举CH34X设备以及打开相关设备
                if (ret_val == -1) {
                    Driver.CloseDevice();
                } else if (ret_val == 0) {
                    if (!Driver.UartInit()) {  //对串口设备进行初始化操作
                        return;
                    }
                    if (!isOpenDeviceCH340) {
                        isOpenDeviceCH340 = true;
                        configParameters(mbaudRate);//配置ch340的参数、需要先配置参数
                    }
                }
                break;
            case WeightbaudRate:
                int Wret_val = Weightdriver.ResumeUsbList();
                // ResumeUsbList方法用于枚举CH34X设备以及打开相关设备
                if (Wret_val == -1) {
                    Weightdriver.CloseDevice();
                } else if (Wret_val == 0) {
                    if (!Weightdriver.UartInit()) {  //对串口设备进行初始化操作
                        return;
                    }
                    if (!isOpenDeviceCH340Weight) {
                        isOpenDeviceCH340Weight = true;
                        configParameters(mbaudRate);//配置ch340的参数、需要先配置参数
                    }
                }
                break;
        }

    }

    /**
     * config ch340 parameters.
     * 配置串口波特率，函数说明可参照编程手册
     */
    private static void configParameters(int mbaudRate) {
        switch (mbaudRate){
            case baudRate:
                Driver.SetConfig(mbaudRate, dataBit, stopBit, parity, flowControl);
                break;
            case WeightbaudRate:
                Weightdriver.SetConfig(mbaudRate, dataBit, stopBit, parity, flowControl);
                break;
        }
        if(!IsStart){
            if (readDataRunnable == null) {
                readDataRunnable = new ReadDataRunnable();
            }
            mThreadPool.execute(readDataRunnable);
            IsStart = !IsStart;
        }
    }

    /**
     * 关闭线程池
     */
    public static void shutdownThreadPool() {
        if (!mThreadPool.isShutdown()) {
            mThreadPool.shutdown();
        }
    }

    /**
     * ch340 is or not open.
     *
     * @return
     */
    public static boolean isIsOpenDeviceCH340() {
        return isOpenDeviceCH340;
    }

    /**
     * get ch340 driver.
     *
     * @return
     */
    public static CH34xUARTDriver getDriver() {
        return Driver;
    }

    public static CH34xUARTDriver getWeightDriver() {
        return Weightdriver;
    }

    public static UsbManager getmUsbManager() {
        return mUsbManager;
    }

    public interface IUsbPermissionListener {
        void result(boolean isGranted);
    }
}
