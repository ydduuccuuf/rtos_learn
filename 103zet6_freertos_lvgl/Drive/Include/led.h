#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

/*------------------------------------------------ LED���ú� -------------------------------------------*/

#define LED1_PIN             GPIO_Pin_13        	 // LED1 ����      
#define LED1_PORT            GPIOC                  // LED1 GPIO�˿�     
#define LED1_CLK             RCC_APB2Periph_GPIOC	 // LED1 GPIO�˿�ʱ��

/*------------------------------------------------ LED���ƺ� -------------------------------------------*/
					
#define LED1_ON 	  	GPIO_ResetBits(LED1_PORT,LED1_PIN);			// ����͵�ƽ������LED1	
#define LED1_OFF 	  	GPIO_SetBits(LED1_PORT,LED1_PIN);			// ����ߵ�ƽ���ر�LED1	
#define LED1_Toggle	LED_Toggle(LED1_PORT, LED1_PIN);				// LED״̬��ת

/*------------------------------------------------ �������� -------------------------------------------*/

void LED_Init(void);															//	LED��ʼ������
void LED_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);			// LED״̬��ת

#endif //__LED_H

