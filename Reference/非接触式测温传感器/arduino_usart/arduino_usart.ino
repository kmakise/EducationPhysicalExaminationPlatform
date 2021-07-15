/////////////////////
/*
GY-MCU90614----arduino pro MINI
VCC----VCC
GND----GND
1:RX---TX 
2:TX---RX

3:MINI_TX---FT232_RX
*/
//请先下载好程序后再接线！  
//////////////////
unsigned char Re_buf[11],counter=0;
unsigned char sign=0;
float TO=0,TA=0;
void setup() {
   Serial.begin(9600);  
  delay(1);    
  Serial.write(0XA5); 
  Serial.write(0X45);    //初始化,连续输出模式
  Serial.write(0XEA);    //初始化,连续输出模式
}

void loop() {
  unsigned char i=0,sum=0;
  
  if(sign)
  {   
     sign=0;
     for(i=0;i<8;i++)
      sum+=Re_buf[i]; 
     if(sum==Re_buf[i] )        //检查帧头，帧尾
     {  	       
           TO=(float)(Re_buf[4]<<8|Re_buf[5])/100;
           Serial.print("TO:");
           Serial.println(TO);  
           TA=(float)(Re_buf[6]<<8|Re_buf[7])/100;
           Serial.print("TA:");
           Serial.println(TA);             
   }
  } 

}
void serialEvent() {
  while (Serial.available()) {   
    Re_buf[counter]=(unsigned char)Serial.read();
    if(counter==0&&Re_buf[0]!=0x5A) return;      // 检查帧头         
    counter++;       
    if(counter==9)                //接收到数据
    {    
       counter=0;                 //重新赋值，准备下一帧数据的接收 
       sign=1;
    }      
  }
}
