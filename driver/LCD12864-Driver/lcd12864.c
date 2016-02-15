/***************************
** LCD12864������������   **
** by IMA-����ƽ 2016-2-2 **
** Github:ChenZiping      **
****************************/
#include "lcd12864.h"

void initLCD12864()
{
	RCC->APB2ENR |= 0x28; // ʹ��PB��PDʱ��
	GPIOB->CRL &= 0xFF000FFF; 
	GPIOB->CRL |= 0x00777000; // PB3-5 ��©���
	GPIOD->CRL &= 0x00000000;
	GPIOD->CRL |= 0x77777777; // PD0-7 ��©���
	LCD_OPEN;
	delayUs_(100);
	LCD_CURSOR_OPEN;
	delayUs_(100);
	writeLCD12864(0x36); //��չָ���������ͼ��
	LCD_CLEAR;
	delayUs_(100);
	LCD_RETURN;
	delayUs_(100);
}

//////////////////////////////////////
//�������ƣ�waitLCD12864()
//��������
//���ܣ��ȴ�LCD��Ϊ����״̬
//////////////////////////////////////
void waitLCD12864()
{
	u16 flag = 0;
	RSout(0);
	RWout(1);
	do
	{
		ENout(0);
		delayUs_(5); //��ʱʱ����Ҫ���ݲ�ͬMCU����
		ENout(1);
		delayUs_(5); //��ʱʱ����Ҫ���ݲ�ͬMCU����
		flag = BUSY_STATE;
	}while(flag);
}

///////////////////////////////////////////
//�������ƣ�writeLCD12864(data)
//������data ָ�����Ϊһ��һ�ֽ�����
//���ܣ�дָ�����һ��һ�ֽ����ݵ�LCD12864��
//Ҳ����������ʾһ���ַ�
///////////////////////////////////////////
void writeLCD12864(u8 data)
{
	waitLCD12864();
	RSout(0);
	RWout(0);
	ENout(1);
	DATA_OUT |= data;
	delayUs_(600); //��ʱʱ����Ҫ���ݲ�ͬMCU����
	ENout(0);
	RWout(1);
	RSout(1);//
}

//////////////////////////////////////
//�������ƣ�setCursor(row, col)
//������row ��1-4, col ��1-8
//���ܣ�����LCD12864�Ĺ��λ��
//˵����	
//LCD12864��Ļ��
//��һ�У�0x80 0x81 ---- 0x86 0x87 
//�ڶ��У�0x90 0x91 ---- 0x96 0x97	
//�����У�0x88 0x89 ---- 0x8E 0x8F	
//�����У�0x98 0x99 ---- 0x9E 0x9F 
//////////////////////////////////////
void setCursor(u8 row, u8 col)
{
	u8 pos = 0x00;
	if(row > 2) pos = 0x08;
	if(row & 1) pos |= 0x80;
	else pos |= 0x90;
	pos += col - 1;
	writeLCD12864(pos);
}

//////////////////////////////////////
//�������ƣ�dispChar(row, col, ch)
//������row ��1-4, col ��1-8, ch �ַ�
//���ܣ�LCD12864��ʾһ���ַ� 
//////////////////////////////////////
void dispChar(u8 row, u8 col, char ch)
{
	setCursor(row, col);
	writeLCD12864(ch);
}

//////////////////////////////////////////
//�������ƣ�dispStr(row, col, str[])
//������row ��1-4, col ��1-8, str[] �ַ���
//���ܣ�LCD12864��ʾһ���ַ���
//////////////////////////////////////////
void dispStr(u8 row, u8 col, char str[])
{
	u16 i;
	setCursor(row, col);
	for(i = 0; str[i] != '\0'; i ++) writeLCD12864(str[i]);
}

//////////////////////////////////////////
//�������ƣ�dispPic(picture[])
//������picture[] ͼƬ����
//���ܣ�LCD12864��ʾһ��128*64��ͼƬ
//˵����
//12864Һ����Ϊ�ϰ������°�����������Ҫ����ĳ���ط�ʱ��
//������д����ط���ˮƽ��ַ������д�����Ĵ�ֱ��ַ��
//ˮƽ��ַҺ�������Զ���1������ֱ��ַ���ᡣÿ����ֱ��
//ַֻȷ��һ�ţ�����ˮƽ�ʹ�ֱ��ַ����ȷ��ĳ�����λ�ã�
//ֻ��ȷ��ĳ�����ֽڵ�λ�ã�ͨ��д��2�ֽ�����ȷ������
//ĳ����򼸸��㡣
//////////////////////////////////////////
void dispPic(const unsigned char picture[])
{
	u8 i, j;
	LCD_EXT;
	delayMs_(20);
	for(i = 0;i < 32;i ++)
	{
		writeLCD12864(0x80 + i);
		delayUs_(150);
		writeLCD12864(0x80);
		delayUs_(150);
		for(j = 0;j < 16;j ++)
		{
			writeLCD12864(*picture);
			picture ++;
			delayUs_(150);
		}
		delayUs_(150);
	}
	for(i = 0;i < 32;i ++)
	{
		writeLCD12864(0x80 + i);
		delayUs_(150);
		writeLCD12864(0x88);
		delayUs_(150);
		for(j = 0;j < 16;j ++)
		{
			writeLCD12864(*picture);
			picture ++;
			delayUs_(150);
		}
		delayUs_(150);
	}
	writeLCD12864(0x36); //д������,��ͼ����ʾ
	delayMs_(50);
}

//////////////////////////////////////////
//�������ƣ�flickerScreen()
//��������
//���ܣ�LCD12864��Ļ��˸
//////////////////////////////////////////
void flickerScreen()
{
	LCD_CLOSE;
	delayMs_(500);
	LCD_OPEN;
	delayMs_(500);
}