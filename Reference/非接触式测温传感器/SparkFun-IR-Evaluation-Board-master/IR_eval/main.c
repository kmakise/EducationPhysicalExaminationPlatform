/*
    12-6-10
    Copyright Spark Fun Electronics© 2010
    Aaron Weiss
    aaron at sparkfun.com
	
	firmware v13
    
    MLX90614 IR Temperature Sensor
	
	ATMega328 w/ external 8MHz resonator
	High Fuse D9
    Low Fuse E2
	
	Prints temperature in Farenhight at 38400bps
*/

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "types.h"
#include "defs.h"
#include "i2c.h"

#define FOSC 8000000
#define BAUD 38400

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

#define WRITE_sda() DDRC = DDRC | 0b00010000 //SDA must be output when writing
#define READ_sda()  DDRC = DDRC & 0b11101111 //SDA must be input when reading

///============Function Prototypes=========/////////////////
void MLX(void);

///============I2C Prototypes=============//////////////////
void i2cSendStart(void);
void i2cSendStop(void);
void i2cWaitForComplete(void);
void i2cSendByte(unsigned char data);
void i2cInit(void);
void i2cHz(long uP_F, long scl_F);
void i2c_receive_pec(void);

///============Initialize Prototypes=====//////////////////
void ioinit(void);
void UART_Init(unsigned int ubrr);
static int uart_putchar(char c, FILE *stream);
void put_char(unsigned char byte);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
void delay_ms(uint16_t x);
 
double Tf;
uint16_t Toreg;

/////===================================////////////////////

int main(void)
{
	ioinit();
	i2cInit();
	delay_ms(1000);
	
	while(1)
	{
		cbi(PORTB, 0);
		delay_ms(400); 
		MLX();
		//printf("%d - ", Toreg);
		printf("%.2fF\n", Tf);
		sbi(PORTB, 0);
		delay_ms(400); 
	}
}

void MLX(void)
{		
	int xh, xl;
	
	//Slave Address (SA) can be 0x00 for any MLX90614
	//using Read Word: SA(write) - Command - SA(read) - LSByte - MSByte - PEC
	i2cSendStart();
	i2cWaitForComplete();
	i2cSendByte(0x00);    //SA + write to slave
	i2cWaitForComplete();
	i2cSendByte(0x07);    //RAM address 0x07, object temperature 1
	i2cWaitForComplete();
	i2cSendStop();
	
	i2cSendStart();
	i2cWaitForComplete();
	i2cSendByte(0x01);   //SA + read from slave     
	i2cWaitForComplete();
	i2cReceiveByte(TRUE);
	i2cWaitForComplete();
	xl = i2cGetReceivedByte();	//low byte
	i2cWaitForComplete();

	i2cReceiveByte(TRUE);
	i2cWaitForComplete();
	xh = i2cGetReceivedByte();	//high byte
	i2cWaitForComplete();
	i2c_receive_pec() ;
	i2cSendStop();
	
	Toreg = (xh << 8)|xl; //concatinate MSB and LSB
	
    if(Toreg & 0x8000) //if MSbit is 1 there is an error
	{
		printf("error\n"); 
	}
	
	//Tk = Toreg / 200; //temp Kelvin
	Tf = (((long)Toreg * (3.6)) - 45967)/100; //temp F
}

/*********************
 ****Initialize****
 *********************/
 
void ioinit (void)
{
    //1 = output, 0 = input
	DDRB = 0b01100001; //PORTB4, B5 output
    DDRC = 0b00010000; //PORTC4 (SDA), PORTC5 (SCL), PORTC all others are inputs
    DDRD = 0b11111110; //PORTD (RX on PD0), PD2 is status output
	PORTC = 0b00110000; //pullups on the I2C bus
	
	//UART_Init((unsigned int)(FOSC/16/(BAUD*2)-1));		// ocillator fq/16/baud rate -1	
	UART_Init(12);//38400
}

void UART_Init( unsigned int ubrr)
{
	// Set baud rate 
	UBRR0H = ubrr>>8;
	UBRR0L = ubrr;
	
	// Enable receiver and transmitter 
	UCSR0A = (0<<U2X0);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	// Set frame format: 8 bit, no parity, 1 stop bit,   
	UCSR0C = (1<<UCSZ00)|(1<<UCSZ01);
	
	stdout = &mystdout; //Required for printf init
}

static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n') uart_putchar('\r', stream);
  
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    
    return 0;
}

void put_char(unsigned char byte)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = byte;
}

void delay_ms(uint16_t x)
{
  uint8_t y, z;
  for ( ; x > 0 ; x--){
    for ( y = 0 ; y < 90 ; y++){
      for ( z = 0 ; z < 6 ; z++){
        asm volatile ("nop");
      }
    }
  }
}

/*********************
 ****I2C Functions****
 *********************/

void i2cInit(void)
{
	// set i2c bit rate to 80KHz, for some reason much less than this will not work??
	i2cSetBitrate(30);
	// enable TWI (two-wire interface)
	//sbi(TWCR, TWEN);
}

void i2cSetBitrate(unsigned short bitrateKHz)
{
	unsigned char bitrate_div;
	// set i2c bitrate
	// SCL freq = F_CPU/(16+2*TWBR))
	//#ifdef TWPS0
		// for processors with additional bitrate division (mega128)
		// SCL freq = F_CPU/(16+2*TWBR*4^TWPS)
		// set TWPS to zero
		cbi(TWSR, TWPS0);
		cbi(TWSR, TWPS1);
	//#endif
	// calculate bitrate division	
	bitrate_div = ((F_CPU/4000l)/bitrateKHz);
	if(bitrate_div >= 16)
		bitrate_div = (bitrate_div-16)/2;
	outb(TWBR, bitrate_div);
}

void i2cSendStart(void)
{
	WRITE_sda();
	// send start condition
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
}

void i2cSendStop(void)
{
	// transmit stop condition
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void i2cWaitForComplete(void)
{
	int i = 0;		//time out variable
	
	// wait for i2c interface to complete operation
    while ((!(TWCR & (1<<TWINT))) && (i < 150)) //90
	i++;
}

void i2cSendByte(unsigned char data)
{

	WRITE_sda();
	// save data to the TWDR
	TWDR = data;
	// begin send
	TWCR = (1<<TWINT)|(1<<TWEN);
}

void i2cReceiveByte(unsigned char ackFlag)
{
	// begin receive over i2c
	if( ackFlag )
	{
		// ackFlag = TRUE: ACK the recevied data
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
	}
	else
	{
		// ackFlag = FALSE: NACK the recevied data
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
	}
}

unsigned char i2cGetReceivedByte(void)
{
	// retieve received data byte from i2c TWDR
	return( inb(TWDR) );
}

unsigned char i2cGetStatus(void)
{
	// retieve current i2c status from i2c TWSR
	return( inb(TWSR) );
}

void i2c_receive_pec(void) 
{
	TWCR = (1 << TWINT)|(1 << TWEN); // start PEC reception, transmit NACK
	while (!(TWCR & (1 << TWINT)));
} 