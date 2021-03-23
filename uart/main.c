#include <reg51.h>
#include <intrins.h>

#include "wifi.h"

unsigned  char tmp;
//<< code关键字使数组内容不可被改变
char code str_get[] = {0x7F, 0x05, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1A};
char code str_send[] = {0x7F, 0x05, 0x12, 0x01, 0x05, 0x00, 0x00, 0x00, 0x03, 0x20};
char temparray[10] = {0};
sbit K1 = P2^7;
unsigned char j = 0;

void send_str();
void get_str();
void delayms(unsigned char ms);
void send_char(unsigned char txd);

void main(void)
{
	unsigned char count, flag, emptycount = 0;
	TMOD = 0x20;			// 定时器1工作于8位自动重载模式, 用于产生波特率
	TH1 = 0xFD;				// 波特率9600  11.0592
	TL1 = 0xFD;	
	SCON = 0x50;			// 设定串行口工作方式
	//PCON |= 0x80;			// 波特率倍增	  4800
	TR1 = 1;				// 启动定时器1
	IE = 0x0;				// 禁止任何中断
	while(1)
	{
		//wifi_uart_service();
		//wifi_protocol_init();
		if(!K1)		// 扫描按键
		{
			delayms(10);			// 延时去抖动
			if(K1)
				break;
			while(!K1)
			{
				delayms(10);
			}
			send_str();	
		}
		get_str();

		for(count = 0; count < 9; count++)
		{
		    if(temparray[count] == str_get[count])
			{
			    flag = 1;  
			}
			else
			{
                flag = 0;
				count = 0;
				//<<添加break，否则flag值将由最后一个元素决定
				break;
			}
		}
		if((j == 10) && (flag == 1))
		{
		    send_str();
			j = 0; 
			count = 0;
		}		
	}
}

void send_str()
{
    unsigned char i = 0;
    while(i < 10)
    {
		SBUF = str_send[i];
		while(!TI);				// 等特数据传送
		TI = 0;					// 清除数据传送标志
		i++;					// 下一个字符
   }	
}

void get_str()
{
	if(RI)						// 是否有数据到来
	{
		RI = 0;
		temparray[j++] = SBUF;  // 暂存接收到的数据
	}		    
}

void delayms(unsigned char ms)	
{						
    unsigned char i;
    while(ms--)
    {
	    for(i = 0; i < 120; i++);
    }
}
    