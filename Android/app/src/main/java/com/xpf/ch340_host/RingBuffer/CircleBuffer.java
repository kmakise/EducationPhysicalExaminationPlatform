package com.xpf.ch340_host.RingBuffer;


import java.util.LinkedHashMap;

public class CircleBuffer {




    private Object[] buffer = null; // 数据容器
    private int capacity = 0; // 缓冲区长度
    private int indexForPut = 0; // put索引 （下一个要写入的位置）
    private int indexForGet = 0; // get索引 （下一个要读取的位置）

    /**
     * 线程的几种状态
     */
    enum ThreadState {
        BLOCK, /* 暂时阻塞 */
        RUNNING, /* 运行 */
        OVER, /* 线程顺利完成 */
        INTERRUPT /* 中断,(暂时用不到) */
    }

    /* put 和 get 的线程状态标记 */
    private ThreadState putThreadState = ThreadState.RUNNING;
    private ThreadState getThreadState = ThreadState.RUNNING;
    LinkedHashMap<Integer, String> msgs = new LinkedHashMap<Integer, String>();

    private static CircleBuffer ringBuffer;
    public static CircleBuffer init(int size) {
        ringBuffer = new CircleBuffer(size);
        return ringBuffer;
    }
    public static CircleBuffer getRingBuffer() {
        return ringBuffer;
    }

    /**
     * 指定长度的缓冲区
     *
     * @param capacity
     */
    private CircleBuffer(int capacity) {
        this.capacity = capacity;
        this.buffer = new Byte[this.capacity];
    }

    /**
     * 写入数据，注意此函数会导致阻塞
     *
     * @param element
     */
    public void putElement(byte element) {
        // 有空位就插入～
        // 没空位就轮询，直到有空位可插入为止~
        while (null != this.buffer[this.indexForPut]) {
            try {
                this.setPutThreadState(ThreadState.BLOCK); // put线程标记为：阻塞
                Thread.sleep(100);

            } catch (InterruptedException e) {
                e.printStackTrace(); // 例行公事
                System.out.println("线程意外停止,正在检查");

            }
        }
        this.setPutThreadState(ThreadState.RUNNING); // put线程标记为：正在运行

        // 填入元素，将put索引指向下一元素位~
        this.buffer[this.indexForPut] = element;
        this.indexForPut++;
        this.indexForPut %= this.capacity;
    }

    /**
     * 取数据，注意此函数会阻塞，若put线程结束且缓冲区为空时函数会返回null
     *
     * @return 下一个T元素 或者 null
     */
    @SuppressWarnings("unchecked")
    public byte getElement() {
        // 有元素就拿出～
        // 没元素就轮询，直到有元素可拿为止~ 若是put完毕、 数据取空，则返回null以告知调用者
        while (null == this.buffer[this.indexForGet]) {
            try {
                this.setGetThreadState(ThreadState.BLOCK); // get线程标记为：阻塞
                if (this.isPutedOver() && this.isEmputy()) {
                    this.setGetThreadState(ThreadState.OVER);
                    return '@';
                }
                Thread.sleep(100);

            } catch (InterruptedException e) {
                e.printStackTrace();
                System.out.println("线程意外停止");

            }
        }
        this.setGetThreadState(ThreadState.RUNNING); // get线程标记为：正在运行

        /* 拿到元素，buffer腾出该元素位，将get索引指向下一元素~ */
        Object temp = this.buffer[this.indexForGet];
        this.buffer[this.indexForGet] = null;
        this.indexForGet++;
        this.indexForGet %= this.capacity;

        return (byte) temp; // 返回拿到的元素引用
    }

    /**
     * 检查此环形缓冲区是否为空
     *
     * @return boolean true则表示为空，false则不为空
     */
    public boolean isEmputy() {
        // 新元素是以 索引0 向 索引length 的顺序 put入
        // 有鉴于此，这里倒过来枚举，防止出现“同向追赶”导致落空的的囧事；
        for (int i = this.buffer.length - 1; i > 0; i--) {
            if (this.buffer[i] != null) {
                return false;
            }
        }
        return true;
    }

    /**
     * 检查 put 线程和 get 线程是否同时阻塞,
     *
     * @return boolean true-挂球 false-良好
     */
    public boolean isAllBlock() {
        return (this.getThreadState == ThreadState.BLOCK && this.putThreadState == ThreadState.BLOCK);
    }

    /**
     * 检查数据源是否缓冲完毕
     *
     * @return boolean true-完成 false-未完成
     */
    public boolean isPutedOver() {
        return this.putThreadState == ThreadState.OVER;
    }

    public void setPutThreadState(ThreadState putThreadState) {
        this.putThreadState = putThreadState;
    }

    private void setGetThreadState(ThreadState getThreadState) {
        this.getThreadState = getThreadState;
    }


}



