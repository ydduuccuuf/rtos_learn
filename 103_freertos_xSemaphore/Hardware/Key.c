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
//    //�����ж�ͨ�������ȼ�
//  NVIC_InitTypeDef nitd;
//  nitd.NVIC_IRQChannel=EXTI9_5_IRQn;
//  nitd.NVIC_IRQChannelCmd=ENABLE;
//  nitd.NVIC_IRQChannelPreemptionPriority=1;   //��ռ���ȼ�
//  nitd.NVIC_IRQChannelSubPriority=1;          //��Ӧ���ȼ�
//  NVIC_Init(&nitd);
//    //�����ⲿ�ж�
//  EXTI_InitTypeDef eitd;
//  eitd.EXTI_Line=EXTI_Line6;              //GPIOA��6�����Ŷ�Ӧ6���ж���
//  eitd.EXTI_LineCmd=ENABLE;
//  eitd.EXTI_Mode=EXTI_Mode_Interrupt;     //ѡ���ж�ģʽ
//  eitd.EXTI_Trigger=EXTI_Trigger_Falling;  //�����ش���,��ΪGPIO������Ϊ����������
//  EXTI_Init(&eitd);
//    
//  eitd.EXTI_Line=EXTI_Line7;              //GPIOA��7�����Ŷ�Ӧ7���ж���
//  eitd.EXTI_Trigger=EXTI_Trigger_Falling; //�½��ش���,��ΪGPIO������Ϊ����������
//  EXTI_Init(&eitd);
//	
//	eitd.EXTI_Line=EXTI_Line5;              //GPIOA��7�����Ŷ�Ӧ7���ж���
//  eitd.EXTI_Trigger=EXTI_Trigger_Falling; //�½��ش���,��ΪGPIO������Ϊ����������
//  EXTI_Init(&eitd);

//}

//void EXTI9_5_IRQHandler(void){
//    if(EXTI_GetFlagStatus(EXTI_Line6==0)){     //��Ϊ�����ж����õĶ���ͬһ������,���ͨ����ȡ�жϱ�־λ���ж���˭�������ж�
//        ++count1;
//        Delay_ms(10);                       //������е�����Ķ���
//        while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==0);
//        Delay_ms(10);
//        EXTI_ClearITPendingBit(EXTI_Line6); //��Ҫ�ֶ�����жϱ�־λ,�����һֱ�ظ�����.
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
				__set_FAULTMASK(1); // �ر������ж�
				NVIC_SystemReset(); // ��λ
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
