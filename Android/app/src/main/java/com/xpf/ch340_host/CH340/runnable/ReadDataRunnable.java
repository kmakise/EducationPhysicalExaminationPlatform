package com.xpf.ch340_host.CH340.runnable;

import com.xpf.ch340_host.CH340.driver.InitCH340;
import com.xpf.ch340_host.RingBuffer.CircleBuffer;
import com.xpf.ch340_host.Util;

/**
 * Created by xpf on 2017/12/20.
 * Function:ReadDataRunnable
 */
public class ReadDataRunnable implements Runnable {

    private static final String TAG = "ReadDataRunnable";
    private boolean mStop = false; // 是否停止线程
    public static int number = 0;

    public static byte[] data;

    @Override
    public void run() {
        data = new byte[9300];
        startReadThread();
    }

    /**
     * 开启读取数据线程
     */
    private void startReadThread() {
        while (!mStop) {
            byte[] receiveBuffer = new byte[1];// 接收数据数组
            byte[] receiveBufferWeight = new byte[1];// 接收数据数组
            CircleBuffer circleBuffer = CircleBuffer.getRingBuffer();

            if(InitCH340.getDriver()!=null){
                // 读取缓存区的数据长度
                int length       = InitCH340.getDriver().ReadData(receiveBuffer, 1);
                if (length!=0)
                {
                    System.out.println("日志消息:"+ Util.getHex(receiveBuffer[0]));
                    circleBuffer.putElement(receiveBuffer[0]);

                }
            }

        }
    }

    /**
     * 停止读取任务
     */
    public void stopTask() {
        mStop = true;
    }

}
