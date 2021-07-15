package com.xpf.ch340_host;

import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.github.mikephil.charting.charts.LineChart;
import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.driver.UsbSerialProber;
import com.hoho.android.usbserial.util.SerialInputOutputManager;

import com.xpf.ch340_host.CH340.CH340Master;
import com.xpf.ch340_host.CH340.driver.InitCH340;
import com.xpf.ch340_host.CH340.runnable.BufferCallBack;
import com.xpf.ch340_host.CH340.runnable.ICallBack;
import com.xpf.ch340_host.CH340.utils.FileReadString;
import com.xpf.ch340_host.CH340.utils.MyFile;
import com.xpf.ch340_host.RingBuffer.CircleBuffer;
import com.xpf.ch340_host.UI.CirclePercentView;
import com.xpf.ch340_host.UI.DynamicLineChartManager;
import com.xpf.ch340_host.UI.UIOperation;
import com.zkk.view.rulerview.RulerView;

import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class MainActivity extends AppCompatActivity implements InitCH340.IUsbPermissionListener {

    private boolean isFirst;//判断是否打开
    private static final String ACTION_USB_PERMISSION = "com.linc.USB_PERMISSION";
    // 手机根目录
    private static String ROOT_PATH = "";
    // 仪器高度
    private static int InstrumentHeight = 220;


    public static MainActivity mainActivity;

    public ImageView HeartImg;  // 心跳动画
    public ImageView FAS;       // 烟雾报警
    public TextView  HeartText;
    public TextView  PeopleTemperatureText;
    public TextView  BMI;
    public TextView  WenDuShow;
    public TextView  ShiDuShow;
    public TextView  LightShow;

    public CirclePercentView PeopleTemperature;
    public CirclePercentView BloodOxygen;
    public TextView          BloodOxygenText;

    public LineChart Heart;          // 血氧相关
    public LineChart Environmental;  // 温湿度
    public LineChart Light;          // 亮度

    public TextView LengthText;    // 测距
    public TextView PressureText;  // 压力

    public RulerView Pressure;   // 压力
    public RulerView Length;     // 测距

    public int Temp = 0;


    public static String WeightStr = "";  // 体重

    public DynamicLineChartManager HeartDynamicLineChartManager;            // 血氧相关
    public DynamicLineChartManager EnvironmentalDynamicLineChartManager;    // 温湿度
    public DynamicLineChartManager LightDynamicLineChartManager;            // 亮度

    List<Integer> HeartDatas;         // 心率图
    List<Integer> EnvironmentalDatas; // 环境图


    public static boolean startShow = false;
    public static int     number4heart = 0;
    public ICallBack iCallBack = new ICallBack() {
        @Override
        public void bufferData(final Agreement agreement) {
                runOnUiThread(new Runnable() {
                    @RequiresApi(api = Build.VERSION_CODES.M)
                    @Override
                    public void run() {

                        // 心率更新
                        if(agreement.HemoglobinWaveformTwo>280 || agreement.HemoglobinWaveformTwo<-20){
                            agreement.HemoglobinWaveformTwo = Temp;
                            agreement.HemoglobinWaveformOne = Temp;
                        }
                        HeartDatas = new ArrayList<>();
                        HeartDatas.add((int)(agreement.HemoglobinWaveformTwo));
                        HeartDatas.add((int)(agreement.HemoglobinWaveformOne));
                        Temp = (int)agreement.HemoglobinWaveformOne;
                        HeartDynamicLineChartManager.addEntry(HeartDatas,1000);

                        // 环境更新
                        EnvironmentalDatas = new ArrayList<>();
                        EnvironmentalDatas.add((int)agreement.EnvironmentTemperature);
                        EnvironmentalDatas.add((int)agreement.EnvironmentHumidity);
                        EnvironmentalDynamicLineChartManager.addEntry(EnvironmentalDatas,500);

                        // 亮度更新
                        LightDynamicLineChartManager.addEntry((int)(agreement.light)+50);

                        WenDuShow.setText("温度"+agreement.EnvironmentTemperature+"");
                        ShiDuShow.setText("湿度"+agreement.EnvironmentHumidity+"");
                        LightShow.setText("亮度:"+agreement.light);

                        // 测距更新
                        int length = (int)(InstrumentHeight-agreement.distance);
                        Length.setValue(length,10,200,1);
                        LengthText.setText("身高:"+0+"cm");
                        // 压力更新
                        if(!WeightStr.equals(""))
                        {
                            float WeightNum = 0;
                            try{
                                 WeightNum = Float.valueOf(WeightStr);
                            }catch (Exception e){
                                WeightNum = 0;
                            }

                            if(WeightNum > 10)
                            {
                                Pressure.setValue(WeightNum,0,200,1);
                                PressureText.setText("体重:"+WeightNum+"kg");
                                LengthText.setText("身高:"+length+"cm");
                                float lengthSqrt = (length/(float)100.0)*(length/(float)100.0);
                                int bmi = (int)((WeightNum)/lengthSqrt);
                                if(bmi<19){
                                    BMI.setText("BMI指数:"+bmi+"("+"偏瘦"+")");
                                    BMI.setTextColor(Color.WHITE);
                                }else if(bmi>=19&&bmi<=24){
                                    BMI.setText("BMI指数:"+bmi+"("+"正常"+")");
                                    BMI.setTextColor(Color.GREEN);
                                }else if(bmi>24&&bmi<28){
                                    BMI.setText("BMI指数:"+bmi+"("+"过重"+")");
                                    BMI.setTextColor(Color.YELLOW);
                                }else if(bmi>=28){
                                    BMI.setText("BMI指数:"+bmi+"("+"肥胖"+")");
                                    BMI.setTextColor(Color.RED);
                                }
                            }else {
                                BMI.setText("");
                                Pressure.setValue(agreement.pressure,0,200,1);
                                PressureText.setText("体重:"+(agreement.pressure)+"kg");
                            }

                        }

                        if(!startShow){
                            // 开始检测是否应开始测量
                            if(agreement.HemoglobinWaveformTwo>160 || agreement.HemoglobinWaveformTwo<140){
                                startShow = true;
                            }
                            HeartText.setText(0+"");
                            BloodOxygen.setPercentage(0);
                            BloodOxygenText.setText(0+"%");
                            number4heart = 0;

                        }
                        // 开始进行心率测量
                        else {
                            // 心率更新
                            if(agreement.heartRate < 0)agreement.heartRate*=-1;
                            if(agreement.heartRate > 50)
                            {
                                HeartText.setText(agreement.heartRate+"");
                            }

                            // 血氧
                            if(agreement.OxygenSaturation>10){
                                BloodOxygen.setPercentage((float)(90+((int)agreement.OxygenSaturation*0.1)));
                                BloodOxygenText.setText(90+((int)agreement.OxygenSaturation*0.1)+"%");
                                number4heart++;
                                if(number4heart > 500){
                                    startShow = false;
                                }
                            }else{
                                BloodOxygen.setPercentage(0);
                                BloodOxygenText.setText(0+"%");
                            }
                        }

                        // 判断手指是否按压上去了
                        if(true){

                        }else{
                             }


                        // 人体测温
                        if(agreement.PeopleTemperature >= 37){
                            PeopleTemperatureText.setTextColor(Color.RED);
                        }else{
                            PeopleTemperatureText.setTextColor(getApplicationContext().getColor(R.color.people_wendu));
                        }
                        PeopleTemperature.setPercentage(agreement.PeopleTemperature);
                        PeopleTemperatureText.setText(Util.TempOperation(agreement.PeopleTemperature)+"°C");



                        if(agreement.EnvironmentSmoke==1){
                            FAS.setVisibility(View.INVISIBLE);
                        }else{
                            FAS.setVisibility(View.VISIBLE);
                        }

                    }
                });
        }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

//        CrashReport.initCrashReport(getApplicationContext(),"902fe8c1cb",true);
        initView();
        test();
        new Thread(new Runnable() {
            @Override
            public void run() {
                test1();
            }
        }).start();


        Agreement.init();
        CircleBuffer.init(1024);
        new BufferCallBack(iCallBack).start();

        initData();
    }
    public AnimatorSet set1;
    @RequiresApi(api = Build.VERSION_CODES.M)
    private void initView() {
        UIOperation.SetFullScreen(this);
        Heart         = findViewById(R.id.heart);
        Environmental = findViewById(R.id.environmental);
        Light         = findViewById(R.id.light);
        Pressure      = findViewById(R.id.pressure);
        Length        = findViewById(R.id.length);
        LengthText    = findViewById(R.id.length_text);
        PressureText  = findViewById(R.id.pressure_text);
        HeartImg      = findViewById(R.id.heart_img);
        HeartText     = findViewById(R.id.heart_text);
        FAS           = findViewById(R.id.fas);
        BMI           = findViewById(R.id.bmi);
        LightShow     = findViewById(R.id.light_show_2);
        WenDuShow     = findViewById(R.id.wendu_show);
        ShiDuShow     = findViewById(R.id.shidu_show);
        PeopleTemperature     = findViewById(R.id.people_temperature);
        PeopleTemperatureText = findViewById(R.id.people_temperature_text);
        BloodOxygen           = findViewById(R.id.blood_oxygen);
        BloodOxygenText       = findViewById(R.id.blood_oxygen_text);


        // 初始化根目录
        ROOT_PATH = MyFile.configure;
        System.out.println("日志:"+ROOT_PATH);

        // 读取高度
        InstrumentHeight = new Integer(FileReadString.read(ROOT_PATH));
        InstrumentHeight = 210;
        /********************************心跳动画********************************/
             ObjectAnimator anim1 = ObjectAnimator.ofFloat(HeartImg,"scaleX",1.2f,0.8f);
             anim1.setRepeatCount(-1);
             ObjectAnimator anim2 = ObjectAnimator.ofFloat(HeartImg,"scaleY",1.2f,0.8f);
             anim2.setRepeatCount(-1);
             AnimatorSet set = new AnimatorSet();
             set.play(anim1).with(anim2);
             set.setDuration(1000);
             set.start();
        /********************************心跳动画********************************/
        // 播放动画
        ObjectAnimator anim3 = ObjectAnimator.ofFloat(FAS,"scaleX",1.2f,0.8f);
        anim3.setRepeatCount(-1);
        ObjectAnimator anim4 = ObjectAnimator.ofFloat(FAS,"scaleY",1.2f,0.8f);
        anim4.setRepeatCount(-1);
        set1 = new AnimatorSet();
        set1.play(anim3).with(anim4);
        set1.setDuration(1000);
        set1.start();
        /********************************血氧********************************/
        BloodOxygen.setStartColor(getApplicationContext().getColor(R.color.start1));
        BloodOxygen.setEndColor(getApplicationContext().getColor(R.color.end1));
        /********************************测温********************************/


        Length.setValue(10, 10, 80, 1);
        Pressure.setValue(0, 0, 200, (float) 0.1);


        List<String>  HeartNames          = new ArrayList<>();
        List<String>  EnvironmentalNames  = new ArrayList<>();

        List<Integer> HeartColors         = new ArrayList<>();
        List<Integer> EnvironmentalColors = new ArrayList<>();


        PressureText.setText("体重:0.0kg");

        HeartNames.add("血红蛋白1");
        HeartNames.add("血红蛋白2");

        HeartColors.add(Color.RED);
        HeartColors.add(Color.CYAN);

        EnvironmentalNames.add("温度");
        EnvironmentalNames.add("湿度");

        EnvironmentalColors.add(Color.GREEN);
        EnvironmentalColors.add(Color.BLUE);


        Heart.setBackgroundColor(Color.argb(30,176,224,230));
        Environmental.setBackgroundColor(Color.argb(60,30,144,255));
        Light.setBackgroundColor(Color.argb(60,160,102,211));

        // 心率曲线初始化
        HeartDynamicLineChartManager         = new DynamicLineChartManager(Heart,HeartNames,HeartColors);
        HeartDynamicLineChartManager.setYAxis(300, -100, 10);
        HeartDynamicLineChartManager.setDescription("血红蛋白");

        // 环境温湿度曲线初始化
        EnvironmentalDynamicLineChartManager = new DynamicLineChartManager(Environmental,EnvironmentalNames,EnvironmentalColors);
        EnvironmentalDynamicLineChartManager.setYAxis(99,0,10);
        EnvironmentalDynamicLineChartManager.setDescription("");

        // 亮度曲线初始化
        LightDynamicLineChartManager         = new DynamicLineChartManager(Light,"亮度",Color.YELLOW);
        LightDynamicLineChartManager.setYAxis(2000, 0, 10);
        LightDynamicLineChartManager.setDescription("");

        mainActivity = this;


    }


    private void initData() {
        InitCH340.setListener(this);
        if (!isFirst) {
            isFirst = true;
            // 初始化 ch340-library
//            CH340Master.initialize(MyApplication.getContext());
        }
    }

    @Override
    public void result(boolean isGranted) {
        if (!isGranted) {
            PendingIntent mPermissionIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
            InitCH340.getmUsbManager().requestPermission(InitCH340.getUsbDevice(), mPermissionIntent);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        IntentFilter usbFilter = new IntentFilter();
        usbFilter.addAction(ACTION_USB_PERMISSION);
//        registerReceiver(mUsbReceiver, usbFilter);
    }


    private final ExecutorService mExecutor = Executors.newSingleThreadExecutor();
    private SerialInputOutputManager mSerialIoManager;

    private final SerialInputOutputManager.Listener mListener =
            new SerialInputOutputManager.Listener() {
                @Override
                public void onRunError(Exception e) {
                }

                @Override
                public void onNewData(final byte[] data) {
                    //TODO 新的数据
                    for (int i=0;i<data.length;i++){
                        CircleBuffer.getRingBuffer().putElement(data[i]);
                    }
                }
            };



    private final ExecutorService mExecutor1 = Executors.newSingleThreadExecutor();
    private SerialInputOutputManager mSerialIoManager1;

    private final SerialInputOutputManager.Listener mListener1 =
            new SerialInputOutputManager.Listener() {
                @Override
                public void onRunError(Exception e) {
                }

                @Override
                public void onNewData(final byte[] data) {
                    //TODO 新的数据
                    try{
                        String result = new String(data,"UTF-8");
                        int index = result.indexOf(',');
                        WeightStr = result.substring(0,index);
                    }catch (Exception e){

                    }

                }
            };


    public void test(){

        try{
            // Find all available drivers from attached devices.
            UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);
            List<UsbSerialDriver> availableDrivers = UsbSerialProber.getDefaultProber().findAllDrivers(manager);
            if (availableDrivers.isEmpty()) {
                return;
            }

            // Open a connection to the first available driver.
            UsbSerialDriver driver = availableDrivers.get(0);
            System.out.println("日志:"+driver.getDevice().getDeviceId());
            if(driver.getDevice().getDeviceId()<3000){
                driver = availableDrivers.get(1);
            }
            UsbDeviceConnection connection = manager.openDevice(driver.getDevice());
            if (connection == null) {
                // add UsbManager.requestPermission(driver.getDevice(), ..) handling here
                return;
            }

            UsbSerialPort port = driver.getPorts().get(0); // Most devices have just one port (port 0)
            port.open(connection);
            port.setParameters(115200, 8, UsbSerialPort.STOPBITS_1, UsbSerialPort.PARITY_NONE);
            mSerialIoManager = new SerialInputOutputManager(port, mListener);//添加监听

            mExecutor.submit(mSerialIoManager);

        }catch (Exception e){

        }

    }



    public void test1(){

        try{
            // Find all available drivers from attached devices.
            UsbManager manager = (UsbManager) getSystemService(Context.USB_SERVICE);
            List<UsbSerialDriver> availableDrivers = UsbSerialProber.getDefaultProber().findAllDrivers(manager);
            if (availableDrivers.isEmpty()) {
                return;
            }

            // Open a connection to the first available driver.
            UsbSerialDriver driver = availableDrivers.get(1);
            System.out.println("日志:"+driver.getDevice().getDeviceId());
            if(driver.getDevice().getDeviceId()>3000){
                driver = availableDrivers.get(0);
            }
            UsbDeviceConnection connection = manager.openDevice(driver.getDevice());
            if (connection == null) {
                // add UsbManager.requestPermission(driver.getDevice(), ..) handling here
                return;
            }
            UsbSerialPort port = driver.getPorts().get(0); // Most devices have just one port (port 0)
            port.open(connection);
            port.setParameters(9600, 8, UsbSerialPort.STOPBITS_1, UsbSerialPort.PARITY_NONE);
            mSerialIoManager1 = new SerialInputOutputManager(port, mListener1);//添加监听

            mExecutor1.submit(mSerialIoManager1);

        }catch (Exception e){

        }

    }


//    private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
//        public void onReceive(Context context, Intent intent) {
//            String action = intent.getAction();
//            if (ACTION_USB_PERMISSION.equals(action)) {
//                synchronized (this) {
//                    UsbDevice device = intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
//                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
//                        if (device != null) {
//                            Toast.makeText(MainActivity.this, "EXTRA_PERMISSION_GRANTED~", Toast.LENGTH_SHORT).show();
//                            InitCH340.loadDriver(MyApplication.getContext(), InitCH340.getmUsbManager());
//                        }
//                    } else {
//                        Toast.makeText(MainActivity.this, "EXTRA_PERMISSION_GRANTED null!", Toast.LENGTH_SHORT).show();
//                    }
//                }
//            }
//        }
//    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
//        unregisterReceiver(mUsbReceiver);
    }
}
