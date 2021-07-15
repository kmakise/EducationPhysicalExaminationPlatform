package com.xpf.ch340_host.CH340.runnable;

import com.xpf.ch340_host.Agreement;
import com.xpf.ch340_host.RingBuffer.CircleBuffer;
import com.xpf.ch340_host.Util;

public class BufferCallBack extends Thread{


    public ICallBack iCallBack;

    public BufferCallBack(ICallBack iCallBack){
        this.iCallBack = iCallBack;
    }

    @Override
    public void run() {
        super.run();
        CircleBuffer circleBuffer = CircleBuffer.getRingBuffer();
        byte data;


        int AgreementLength = 31;                      // 协议长度
        int CurrentIndex    = 0;                       // 当前下标
        byte[] Agreement = new byte[AgreementLength];  // 协议
        Agreement agreement;

        while(true)
        {
            data = circleBuffer.getElement();
            CurrentIndex = 0;
            if(data == 90)
            {
                Agreement[CurrentIndex] = data;
                CurrentIndex++;
                data = circleBuffer.getElement();
                if(data == -91)
                {
                    Agreement[CurrentIndex] = data;
                    CurrentIndex++;
                    for(int i=CurrentIndex;i<AgreementLength;i++){
                        data = circleBuffer.getElement();
                        Agreement[i] = data;
                    }
                    // 更新协议
                    agreement = com.xpf.ch340_host.Agreement.getAgreement().update(Agreement);

                    System.out.println(getAgreement(Agreement));
                    // 发送回调测试
                    this.iCallBack.bufferData(agreement);
                    // 复位
                    CurrentIndex = 0;
                }
            }
        }
    }


    public String getAgreement(byte[] data){
        String result = "协议: ";
        for(int i=0;i<data.length;i++){
            result += Util.getHex(data[i])+",";
        }
        return result;
    }
}
