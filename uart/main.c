#include <reg51.h>
#include <intrins.h>

#include "wifi.h"

unsigned  char tmp;
//<< code�ؼ���ʹ�������ݲ��ɱ��ı�
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
	TMOD = 0x20;			// ��ʱ��1������8λ�Զ�����ģʽ, ���ڲ���������
	TH1 = 0xFD;				// ������9600  11.0592
	TL1 = 0xFD;	
	SCON = 0x50;			// �趨���пڹ�����ʽ
	//PCON |= 0x80;			// �����ʱ���	  4800
	TR1 = 1;				// ������ʱ��1
	IE = 0x0;				// ��ֹ�κ��ж�
	while(1)
	{
		//wifi_uart_service();
		//wifi_protocol_init();
		if(!K1)		// ɨ�谴��
		{
			delayms(10);			// ��ʱȥ����
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
				//<<���break������flagֵ�������һ��Ԫ�ؾ���
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
		while(!TI);				// �������ݴ���
		TI = 0;					// ������ݴ��ͱ�־
		i++;					// ��һ���ַ�
   }	
}

void get_str()
{
	if(RI)						// �Ƿ������ݵ���
	{
		RI = 0;
		temparray[j++] = SBUF;  // �ݴ���յ�������
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
    