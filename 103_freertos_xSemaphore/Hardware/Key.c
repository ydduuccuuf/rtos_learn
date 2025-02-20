#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "FreeRTOS.h"
#include "task.h"
//uint16_t count1=0;
//uint16_t count2=0;
//uint16_t count3=0;
uint16_t KeyNum = 0;
//void Key_Init(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource7);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
//    //配置中断通道的优先级
//  NVIC_InitTypeDef nitd;
//  nitd.NVIC_IRQChannel=EXTI9_5_IRQn;
//  nitd.NVIC_IRQChannelCmd=ENABLE;
//  nitd.NVIC_IRQChannelPreemptionPriority=1;   //抢占优先级
//  nitd.NVIC_IRQChannelSubPriority=1;          //响应优先级
//  NVIC_Init(&nitd);
//    //配置外部中断
//  EXTI_InitTypeDef eitd;
//  eitd.EXTI_Line=EXTI_Line6;              //GPIOA的6号引脚对应6号中断线
//  eitd.EXTI_LineCmd=ENABLE;
//  eitd.EXTI_Mode=EXTI_Mode_Interrupt;     //选择中断模式
//  eitd.EXTI_Trigger=EXTI_Trigger_Falling;  //上升沿触发,因为GPIO口配置为了下拉输入
//  EXTI_Init(&eitd);
//    
//  eitd.EXTI_Line=EXTI_Line7;              //GPIOA的7号引脚对应7号中断线
//  eitd.EXTI_Trigger=EXTI_Trigger_Falling; //下降沿触发,因为GPIO口配置为了上拉输入
//  EXTI_Init(&eitd);
//	
//	eitd.EXTI_Line=EXTI_Line5;              //GPIOA的7号引脚对应7号中断线
//  eitd.EXTI_Trigger=EXTI_Trigger_Falling; //下降沿触发,因为GPIO口配置为了上拉输入
//  EXTI_Init(&eitd);

//}

//void EXTI9_5_IRQHandler(void){
//    if(EXTI_GetFlagStatus(EXTI_Line6==0)){     //因为两条中断线用的都是同一个函数,因此通过获取中断标志位来判断是谁触发的中断
//        ++count1;
//        Delay_ms(10);                       //消除机械按键的抖动
//        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0);
//        Delay_ms(10);
//        EXTI_ClearITPendingBit(EXTI_Line6); //需要手动清除中断标志位,否则会一直重复触发.
//    }
//		else if(EXTI_GetFlagStatus(EXTI_Line7)){
//        ++count2;
//        Delay_ms(10);
//        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0);
//        Delay_ms(10);
//        EXTI_ClearITPendingBit(EXTI_Line7);
//    }
//		else if(EXTI_GetFlagStatus(EXTI_Line5)){
//        ++count3;
//        Delay_ms(10);
//        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==0);
//        Delay_ms(10);
//        EXTI_ClearITPendingBit(EXTI_Line5);
//    }
//}

//uint8_t Key_GetNum(void)
//{
//	uint8_t KeyNum = 0;
//	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
//	{
//		++count1;
//		Delay_ms(20);
//		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0);
//		Delay_ms(20);
//	}
//	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)
//	{
//		++count2;
//		Delay_ms(20);
//		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0);
//		Delay_ms(20);
//	}
//	
//	return KeyNum;
//}
void Key_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

uint16_t Key_GetNum(void)
{
//	static 
	u8 key_state=0;
	u8 count=0;
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
	{
//		key_state=1;
		vTaskDelay(pdMS_TO_TICKS(10));
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
		{
			if(key_state==0)
			{
				key_state=1;
				while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
				{
					vTaskDelay(pdMS_TO_TICKS(100));
					count++;
					if(count>6){break;}
				}
				if(count>6)
				{
					KeyNum = 5;
					count=0;
					key_state=0;
				__set_FAULTMASK(1); // 关闭所有中端
				NVIC_SystemReset(); // 复位
				}
				else
				{
					count=0;
					while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) ==0)
					{
						vTaskDelay(pdMS_TO_TICKS(100));
						count++;
						if(count>3){break;}
					}
					if(count>3)
					{
						KeyNum = 0;
						key_state=0;
					}
					else
					{
						KeyNum = 6;
						key_state=0;
						while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) ==1){vTaskDelay(pdMS_TO_TICKS(10));}
					}			
				}
			}
		}
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1)
		{
			KeyNum = 1;
		}
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == 1)
		{
			KeyNum = 2;
		}
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 1)
		{
			KeyNum = 3;
		}
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1)
	{
		vTaskDelay(pdMS_TO_TICKS(10));
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1)
		{
			KeyNum = 4;
		}
	}
	return KeyNum;
}
