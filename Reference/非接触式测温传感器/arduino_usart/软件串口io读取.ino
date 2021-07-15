/////////////////////
/*
GY-MCU9061X----MINI
VCC----VCC
GND----GND
1:GY9061x_TX---10,
2:GY9061x_RX---11,send A5 45 EA to MCU9061X
*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // arduino-RX, arduino-TX
//////////////////
unsigned char Re_buf[11],counter=0;
unsigned char sign=0;
float TO=0,TA=0;
void setup() {
   Serial.begin(115200); 
   mySerial.begin(115200);
   mySerial.listen(); 
  delay(10);    
  mySerial.write(0XA5); 
  mySerial.write(0X45);    //初始化,连续输出模式
  mySerial.write(0XEA);    //初始化,连续输出模式
}

void loop() {
  unsigned char i=0,sum=0;
    while (mySerial.available()) {   
    Re_buf[counter++]=(unsigned char)mySerial.read();
    if(counter==1&&Re_buf[0]!=0x5A) {counter=0;return;}      // 检查帧头         
    if(counter==2&&Re_buf[1]!=0x5A) {counter=0;return;}   
    if(counter==9)                //接收到数据
    {    
       counter=0;                 //重新赋值，准备下一帧数据的接收 
       sign=1;
    }      
  }
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

