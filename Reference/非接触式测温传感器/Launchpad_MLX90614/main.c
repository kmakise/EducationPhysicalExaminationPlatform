


//MLX90614 Pin Config

//GND-----1.5.....I/O supply the MLX90614
//SCL-----2.0
//SCL-----2.1
//POW-----2.2.....I/O supply the MLX90614

#include  <msp430g2553.h> // System define for the micro that I am using

unsigned int A4,A5,A6,A7,A8;
unsigned char dis_code[10] = {48,49,50,51,52,  //段码表  
                            //0  1  2  3  4      对应内容 
                              53,54,55,56,57};   
                            //5  6  7  8  9 

void Delay(unsigned int n);
void start_bit();
void stop_bit();
void send_bit(unsigned char bit_out);
unsigned char receive_bit();
unsigned char slave_ack();
void TX_byte(unsigned char TX_buffer);
unsigned char RX_byte(unsigned char ack_nack);
unsigned char PEC_cal(unsigned char pec[],int n);
unsigned long int MEM_READ( unsigned char slave_addR, unsigned char cmdR );						 
void CALTEMP(unsigned long int TEMP);

void mlx90614_POW_0() { P1OUT &= ~0x20;}  // define P1.5 ---> POW
void mlx90614_POW_1() { P1OUT |= 0x20;}

void mlx90614_GND_0() { P2OUT &= ~0x04;}  // define P2.2 ---> GND
void mlx90614_GND_1() { P2OUT |= 0x04;}

void mlx90614_SCL_0() { P2OUT &= ~0x01;}  // define P2.0 ---> SCL
void mlx90614_SCL_1() { P2OUT |= 0x01;}

void mlx90614_SDA_0() { P2OUT &= ~0x02;}  // define P2.1 ---> SDA
void mlx90614_SDA_1() { P2OUT |= 0x02;}


#define _SDA_OUTPUT P2DIR |=0x02; //Set SDA as Output 
#define _SDA_INPUT P2DIR &=~0x02; //Set SDA as Input

#define SDA ((P2IN & BIT1)>>1) //define input pin

#define RXD      BIT1 
#define TXD      BIT2 

void SendByte(unsigned char Data);
void UartTXdata(void);

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;         // Stop Watch dog timer
  
  BCSCTL1 = CALBC1_16MHZ;            // Set DCO to 16 MHz
  DCOCTL = CALDCO_16MHZ;
  
  P1DIR = 0xFF;     // All P1.x outputs
  P1OUT = 0;        // All P1.x reset
  P2DIR = 0xFF;     // All P2.x outputs
  P2OUT = 0;        // All P2.x reset

  P1SEL = RXD + TXD ;                // Select TX and RX functionality for P1.1 & P1.2
  P1SEL2 = RXD + TXD ;              //

  UCA0CTL1 |= UCSSEL_2;             // Have USCI use System Master Clock: AKA core clk 1MHz

  UCA0BR0 = 131;                    // 16MHz 9600, see user manual
  UCA0BR1 = 6;                      //

  UCA0MCTL = UCBRS0;                // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;             // Start USCI state machine
  
  mlx90614_GND_0();    //powr supply 
  mlx90614_POW_1();

  while(1)                          // While 1 is equal to 1 (forever)
  {
   unsigned long int DATA;
                              
   DATA=MEM_READ(0x5a,0x07);
   CALTEMP(DATA);
    
   //Delay(1); //delay 340nS
   //Delay(10); //delay 4uS
   //Delay(60); //delay 20uS
   //Delay(100); //delay 34uS
   //SendByte(0x55);
    
    SendByte('T');
    SendByte(':');
    SendByte(32);
    SendByte(dis_code[A4]);
    SendByte(dis_code[A5]);
    SendByte(dis_code[A6]);
    SendByte('.');
    SendByte(dis_code[A7]);
    SendByte(dis_code[A8]);
    SendByte('\n');
    Delay(32000);
    Delay(32000);
    Delay(32000);
    Delay(32000);
    Delay(32000);
    Delay(32000);
    Delay(32000);
    Delay(32000);
    Delay(32000);
  }
}

void SendByte(unsigned char Data) 
{
  while ((UCA0STAT & UCBUSY)); 
  UCA0TXBUF = Data; 
}


// USER CODE BEGIN (MAIN_General,10)

void Delay(unsigned int n)
{
  unsigned int i;
  for(i=0;i<n;i++)
  _NOP();
}


//----------------------------------------------------------------------------------------------------------------------------------------//
//Name: start_bit
//----------------------------------------------------------------------------------------------------------------------------------------//
void start_bit()
{
  _SDA_OUTPUT; //Set SDA as output 
  Delay(5); 
  mlx90614_SDA_1(); 
  //Delay(30); 
  mlx90614_SCL_1();
  
  Delay(30); 
  mlx90614_SDA_0();
  Delay(30); 
  mlx90614_SCL_0();
  Delay(30); 
  
}

//----------------------------------------------------------------------------------------------------------------------------------------//
//Name: stop_bit
//----------------------------------------------------------------------------------------------------------------------------------------//
void stop_bit()
{
  _SDA_OUTPUT; //Set SDA as output
  Delay(5); 
  //mlx90614_SCL_0();
  mlx90614_SDA_0();
  Delay(30); 
  mlx90614_SCL_1();
  Delay(30); 
  mlx90614_SDA_1();
}

//----------------------------------------------------------------------------------------------------------------------------------------//
//Name: send_bit
//----------------------------------------------------------------------------------------------------------------------------------------//
void send_bit(unsigned char bit_out)
{
  _SDA_OUTPUT; //Set SDA as output
  Delay(5); 
  if(bit_out==0) {mlx90614_SDA_0();}else{mlx90614_SDA_1();}
  Delay(5); 
  mlx90614_SCL_1();
  Delay(30); 
  mlx90614_SCL_0();
  Delay(30); 
}

//----------------------------------------------------------------------------------------------------------------------------------------//
//Name: receive_bit
//----------------------------------------------------------------------------------------------------------------------------------------//
unsigned char receive_bit()
{
  unsigned char bit_in;
  _SDA_INPUT; //Set SDA as input
  Delay(5); 
  mlx90614_SCL_1();
  Delay(5); 
  if(SDA==1){bit_in=1;}else{bit_in=0;}
  Delay(10); 
  mlx90614_SCL_0();
  Delay(30); 
  return bit_in;
}

//----------------------------------------------------------------------------------------------------------------------------------------//
//Name: slave_ack
//1 - ACK
//0 -NACK
//----------------------------------------------------------------------------------------------------------------------------------------//
unsigned char slave_ack()
{
  unsigned char ack;
  ack=0;
  _SDA_INPUT; //Set SDA as input
  Delay(5); 
  mlx90614_SCL_1();
  Delay(10); 
  if(SDA==1){ack=0;}else{ack=1;}
  Delay(10); 
  mlx90614_SCL_0();
  Delay(30); 
  return ack;
}

//----------------------------------------------------------------------------------------------------------------------------------------//
//Name: TX_byte
//----------------------------------------------------------------------------------------------------------------------------------------//
void TX_byte(unsigned char TX_buffer)
{
  unsigned char Bit_counter;
  unsigned char bit_out;
  for(Bit_counter=8;Bit_counter;Bit_counter--)
  {
    if(TX_buffer&0x80){bit_out=1;}else{bit_out=0;} 
    send_bit(bit_out); //Send the current bit on SMBus
    TX_buffer<<=1; //Get next bit to check
  }
 }

//----------------------------------------------------------------------------------------------------------------------------------------//
//Name: RX_byte
//Parameters: unsigned char ack_nack (acknowledgment bit)
//0 - Master device sends ACK
//1 - Master device sends NACK
//----------------------------------------------------------------------------------------------------------------------------------------//
unsigned char RX_byte(unsigned char ack_nack)
{
	unsigned char RX_buffer;
	unsigned char Bit_counter;
	for(Bit_counter=8;Bit_counter;Bit_counter--)
	{
	 if(receive_bit()==1) //Read a bit from the SDA line
	  {
	   RX_buffer<<=1; //If the bit is HIGH save 1 in RX_buffer
	   RX_buffer|=0x01;
	  }
	 else //If the bit is LOW save 0 in RX_buffer
	  {
	   RX_buffer<<=1;
	   RX_buffer&=0xfe;
	  }
	}
	send_bit(ack_nack); //Sends acknowledgment bit
	return RX_buffer;
}


//----------------------------------------------------------------------------------------------------------------------------------------//
//CALCULATE THE PEC PACKET
//----------------------------------------------------------------------------------------------------------------------------------------//
unsigned char PEC_cal(unsigned char pec[],int n)
{
    unsigned char crc[6];
    unsigned char Bitposition=47;
    unsigned char shift;
    unsigned char i;
    unsigned char j;
    unsigned char temp;
  do{
    crc[5]=0; //Load CRC value 0x000000000107
    crc[4]=0;
    crc[3]=0;
    crc[2]=0;
    crc[1]=0x01;
    crc[0]=0x07;
    Bitposition=47; //Set maximum bit position at 47
    shift=0;        //Find first 1 in the transmitted bytes
    i=5; //Set highest index (package byte index)
    j=0; //Byte bit index, from lowest
    while((pec[i]&(0x80>>j))==0 && (i>0))
    {
     Bitposition--;
     if(j<7){ j++;}
     else {j=0x00;i--;}
    }//the position of highest "1" bit in Bitposition is calculated
    shift=Bitposition-8; //Get shift value for CRC value
	
    while(shift)
    {
      for(i=5;i<0xFF;i--)
      {
       if((crc[i-1]&0x80) && (i>0)) //Check if the MSB of the byte lower is "1"
        { //Yes - current byte + 1
         temp=1; //No - current byte + 0
        } //So that "1" can shift between bytes
       else { temp=0;}
      crc[i]<<=1;
      crc[i]+=temp;
      }
    shift--;
    }
    //Exclusive OR between pec and crc
    for(i=0;i<=5;i++) { pec[i]^=crc[i]; }
    }
    while(Bitposition>8);
    return pec[0];
    }
	
//----------------------------------------------------------------------------------------------------------------------------------------//
//READ DATA FROM RAM/EEPROM
//----------------------------------------------------------------------------------------------------------------------------------------//
unsigned long int MEM_READ(unsigned char slave_addR, unsigned char cmdR)
{
  unsigned char DataL; //
  unsigned char DataH; //Data packets from MLX90614
  unsigned char PEC; //
  unsigned long int Data; //Register value returned from MLX90614
  unsigned char Pecreg; //Calculated PEC byte storage
  unsigned char arr[6]; //Buffer for the sent bytes
  unsigned char ack_nack;
  unsigned char SLA;
  SLA=(slave_addR<<1);
begin:
  start_bit(); //Send start bit
  TX_byte(SLA); //Send slave address, write
  if(slave_ack()==0){stop_bit();goto begin;} //Send command
  TX_byte(cmdR);
  if(slave_ack()==0){stop_bit();goto begin;}//Send Repeated start bit
  start_bit(); //Send slave address, read
  TX_byte(SLA+1);
  if(slave_ack()==0){stop_bit();goto begin;}
  DataL=RX_byte(0); //
  //Read two bytes data
  DataH=RX_byte(0); //
  PEC=RX_byte(ack_nack); //Read PEC from MLX90614
  if(ack_nack==1) //Master sends ack or nack
  //This depends on the pec calculation, 
  //if the PEC is not correct, send nack and goto begin
  {stop_bit();goto begin;} //Send stop bit
  stop_bit();
  arr[5]=(SLA);
  arr[4]=cmdR;
  arr[3]=(SLA+1);
  arr[2]=DataL;
  arr[1]=DataH;
  arr[0]=0;
  Pecreg=PEC_cal(arr,6); //Calculate CRC
  if(PEC==Pecreg){ ack_nack=0;}
  else{ ack_nack=1;}
  Data=(DataH*256)+DataL;
  return Data;
}

//---------------------------------------
//Name: CALTEMP           
//Temperature data is T=(Data)*0.02-273.15
//---------------------------------------
void CALTEMP(unsigned long int TEMP)
{
   unsigned long int T;
   unsigned int a,b;
   T=TEMP*2;
   if(T>=27315)
   {
     T=T-27315;
     a=T/100;
     b=T-a*100;
     if(a>=100){A4=a/100;a=a%100;A5=a/10;a=a%10;A6=a;}
     else if(a>=10){A4=0;A5=a/10;a=a%10;A6=a;}
     else {A4=0;A5=0;A6=a;}
     if(b>=10){A7=b/10;b=b%10;A8=b;}
     else{A7=0;A8=b;}
    }
   else
    {
     T=27315-T;
     a=T/100;
     b=T-a*100;
     A4=9;
     if(a>=10){A5=a/10;a=a%10;A6=a;}
     else{A5=0;A6=a;}
     if(b>=10){ A7=b/10;b=b%10;A8=b;}
     else{A7=0;A8=b;}
    }
}