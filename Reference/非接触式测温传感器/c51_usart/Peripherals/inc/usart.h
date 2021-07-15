#ifndef __USART_H__
#define __USART_H__
#include <reg52.h>
#define uint32_t unsigned long
#define uint16_t unsigned int
#define int16_t  int
#define uint8_t unsigned char

void Usart_Int(uint32_t BaudRatePrescaler);
void USART_send_byte(uint8_t Tx_data);
void send_out(int16_t *Data,uint8_t length,uint8_t send);
void USART_Send(uint8_t *Buffer, uint8_t Length);
extern uint8_t send_ok,Receive_ok,TEMP_data[9];

#endif