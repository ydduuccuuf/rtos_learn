#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "FreeRTOS.h"
#include "Delay.h"
void AHT10_SendBA()
{
	
}
uint8_t AHT10_ASK()
{
	u8 ack;
	MyI2C_Start();
	MyI2C_SendByte(0x71);
	ack=MyI2C_ReceiveAck();
	MyI2C_Stop();
	return ack;
}

uint8_t AHT10_Read_Cal_Enable(void)
{
	uint8_t val = 0;
	val = AHT10_ASK();
	
	if((val & 0x68)==0x08)  //判断NOR模式和校准输出是否有效
	  return 1;
	  else  return 0;
}

void AHT10_SendAC(void)
{
	MyI2C_Start();
	MyI2C_SendByte(0x70);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(0xAC);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(0x33);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
}
u32 AHT10_Read()
{
	uint8_t Byte_1th = 0;
	uint8_t Byte_2th = 0;
	uint8_t Byte_3th = 0;
	uint8_t Byte_4th = 0;
	uint8_t Byte_5th = 0;
	uint8_t Byte_6th = 0;
	uint16_t cnt = 0;
	AHT10_SendAC();
	cnt = 0;
	vTaskDelay(pdMS_TO_TICKS(30));
	while(((AHT10_ASK()&0x80)==0x80))//等待忙状态结束
	{
	 
	 if(cnt++>=100)
	 {
		vTaskDelay(pdMS_TO_TICKS(5));
	  break;
	 }
	}
	MyI2C_Start();
	MyI2C_SendByte(0x71);
	MyI2C_ReceiveAck();
	Byte_1th = MyI2C_ReceiveByte();
	MyI2C_ReceiveAck();
	Byte_2th = MyI2C_ReceiveByte();
	MyI2C_ReceiveAck();
	Byte_3th = MyI2C_ReceiveByte();
	MyI2C_ReceiveAck();
	Byte_4th = MyI2C_ReceiveByte();
	MyI2C_ReceiveAck();
	Byte_5th = MyI2C_ReceiveByte();
	MyI2C_ReceiveAck();
	Byte_6th = MyI2C_ReceiveByte();
	MyI2C_ReceiveAck();
	MyI2C_Stop();
	return (Byte_4th<<16)|(Byte_5th<<16)|(Byte_6th);
}

u8 AHT10_Init(void)
{
	uint16_t count=0;
	MyI2C_Init();
	MyI2C_Start();
	MyI2C_SendByte(0x70);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(0xE1);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(0x08);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(0x00);
	MyI2C_ReceiveAck();
	MyI2C_Stop();

	Delay_ms(500);
	
	while( AHT10_Read_Cal_Enable() == 0)//如果校准位没有置1，就一直初始化
	{
		AHT10_SendBA();
		Delay_ms(100);
		
		MyI2C_Start();
		MyI2C_SendByte(0x70);		//进行写操作
		MyI2C_ReceiveAck();
		MyI2C_SendByte(0xE1);		//写系统配置寄存器
		MyI2C_ReceiveAck();
		MyI2C_SendByte(0x08);		//校准位置1
		MyI2C_ReceiveAck();
		MyI2C_SendByte(0x00);
		MyI2C_ReceiveAck();
		MyI2C_Stop();
	
		
		if(count++>=10)return 0;
		Delay_ms(500);

	}
	return 1;

}