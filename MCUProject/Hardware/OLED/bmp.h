//////////////////////////////////////////////////////////////////////////////////	 
//
//
//存储图片数据，图片大小为128*64像素
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __BMP_H
#define __BMP_H

unsigned char BMP_Start[] = 
{
/*--  调入了一幅图像：E:\1A_WorkSpace\MCU\STM32F407IGT6\STM32\STM32Demo\Hardware\OLED\start.bmp  --*/
/*--  宽度x高度=128x64  --*/
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x6B,0x6B,0xAC,0xAC,0xC1,0xAD,0xAD,
0xAD,0xFF,0x81,0xBD,0xBD,0x81,0xFF,0xFF,0xFF,0xF7,0x23,0xB5,0xB6,0xB6,0xB7,0x21,
0xFF,0x60,0xDB,0xDB,0xDD,0xC5,0xFF,0xFF,0x3F,0xDF,0x03,0xFC,0xF7,0xF7,0x07,0xF7,
0x00,0xE7,0x17,0xF7,0xF7,0xEB,0xFF,0xFF,0xF7,0x77,0x00,0xF7,0x57,0xDF,0x67,0xFB,
0x5C,0xDC,0xF3,0x6F,0xCF,0x9F,0xFF,0xFF,0xDF,0xDD,0x18,0xFB,0xFF,0xB7,0x37,0xB7,
0xF7,0x00,0xF7,0xF6,0xF5,0xFF,0xFF,0xFD,0x05,0x7D,0x0D,0x71,0xDF,0x6F,0xD7,0x59,
0xDE,0xD9,0xD7,0x77,0xCF,0xEF,0xFF,0xFF,0x9F,0xDF,0xCF,0xD7,0xDB,0xD9,0xDC,0xDE,
0xDF,0xDB,0xD7,0xCF,0x9F,0xDF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7E,0x7E,0x7F,0x7F,0x7E,0x7F,0x40,0x6A,0x6A,0x6A,
0x6A,0x6A,0x6A,0x6A,0x40,0x7F,0x7F,0x7F,0x7F,0x7F,0x40,0x7A,0x7A,0x5A,0x5A,0x60,
0x7F,0x40,0x5B,0x5D,0x5C,0x45,0x6F,0x7F,0x7F,0x7F,0x40,0x7F,0x71,0x7E,0x7B,0x7B,
0x40,0x7B,0x7B,0x7E,0x79,0x7B,0x7F,0x7F,0x78,0x7F,0x40,0x7F,0x6E,0x6F,0x68,0x6F,
0x6C,0x6F,0x63,0x6C,0x6F,0x6F,0x7F,0x7F,0x7F,0x6F,0x40,0x77,0x7F,0x67,0x70,0x77,
0x77,0x7F,0x78,0x67,0x4F,0x63,0x7F,0x7B,0x5B,0x5D,0x47,0x78,0x6F,0x6C,0x6B,0x6F,
0x6C,0x67,0x68,0x6F,0x6F,0x7F,0x7F,0x7F,0x7F,0x7F,0x40,0x6E,0x6E,0x6E,0x6E,0x6E,
0x6E,0x6E,0x6E,0x40,0x7F,0x7F,0x7F,0x7F,0x7E,0x7E,0x7E,0x7E,0x7F,0x7F,0x7F,0x7F,
0x00,0x3F,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,
0x80,0xC0,0xC0,0xC0,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0xC0,0xC0,0xC0,0xC0,0xE0,0x70,
0x30,0x30,0x30,0x70,0xE0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x3F,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0xF8,0xFC,0xFE,0xFE,0xFF,0x8F,0x07,
0x07,0x07,0x07,0x0F,0x0F,0x0F,0x1F,0x1F,0x3B,0x17,0x0F,0x1F,0x07,0x03,0x01,0x00,
0x00,0x00,0x00,0x80,0xF0,0xFF,0xDF,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFE,0xFF,0x03,0x03,0x03,0x03,0xE3,0xF3,0x1B,0xFB,0x03,0xFB,0xF3,0x03,0xF3,
0xF3,0xF3,0xF3,0x73,0x73,0x73,0xF3,0xF3,0xE3,0xE3,0xC3,0x03,0x03,0xF3,0xF3,0xF3,
0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0xF3,0x03,0x03,0xC3,0xC3,0xE3,0xF3,0xF3,
0x73,0x73,0x73,0xF3,0xF3,0xE3,0xE3,0xC3,0x03,0x03,0x03,0xC3,0xE3,0xE3,0xF3,0xF3,
0x73,0x73,0x73,0x73,0x73,0x03,0x03,0x03,0x03,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,
0xFC,0xF8,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x80,0x80,0x00,
0x00,0x00,0x00,0x01,0x01,0xC3,0xFF,0xFF,0xFE,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x38,0x7D,0xC7,0xFF,0xC3,0xFD,0x78,0x00,0xFF,
0xFF,0xFF,0xFF,0x38,0x38,0x38,0xFC,0xFF,0xFF,0xEF,0x87,0x00,0x00,0x00,0x00,0x00,
0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x81,0x00,
0x00,0x00,0x00,0x00,0x81,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x07,0x0F,0x1F,0x1F,0x3E,
0x3C,0x7C,0xF8,0xF8,0xF8,0xF0,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x1F,0xFF,0xFF,0xFF,0xFF,0x3F,0xDF,0xDF,0xBF,0xFF,0xDF,
0x1F,0xDF,0xFE,0x18,0xB8,0x70,0xB0,0x10,0xF8,0xD8,0xDE,0x3F,0xFE,0xDF,0xDF,0x3F,
0xFE,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0xFF,0xFF,0x00,0x00,0x60,0xE0,0xC7,0xFF,0xFE,0xCC,0xCC,0xCC,0xCC,0x00,0xCF,
0xCF,0xCF,0xCF,0xC0,0xC0,0xC0,0xC0,0xC7,0xCF,0xCF,0xCF,0xCC,0xC0,0xC0,0xC0,0xC0,
0xC0,0xCF,0xCF,0xCF,0xCF,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC3,0xC7,0xC7,0xCF,0xCF,
0xCE,0xCE,0xCE,0xCF,0xCF,0xC7,0xC7,0xC3,0xC0,0xC0,0xC0,0xCE,0xCE,0xCE,0xCE,0xCE,
0xCE,0xCE,0xCF,0xC7,0xC7,0xC3,0xC0,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x0F,0x1B,0x36,0x76,0x79,0xFF,0xFF,
0xF0,0xFF,0xFF,0xF0,0xFF,0xF0,0xFF,0xF0,0xFF,0xF6,0xF6,0xF9,0xFF,0xF1,0xF6,0xF7,
0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x0F,0x1F,0x18,0x18,0x18,0x18,0x18,0x19,0x19,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1F,0x0F,0x00,0x00,0x00,0x00,0x00,
0x00,0x7E,0x40,0x40,0x40,0x40,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x01,0x01,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x40,0x40,0x7E,0x00

};


#endif


