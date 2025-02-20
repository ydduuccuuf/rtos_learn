#include "stm32f10x.h"
#include "delay.h"
#include "led.h"  
#include "key.h"  
#include "usart.h"
#include "lcd_spi_154.h"
#include "FreeRTOS.h"
#include "FreeRTOSconfig.h"
#include "task.h"
#include "lvgl.h"
#include "lv_port_disp.h"
// LCD���Ժ��������������ڵײ�
void 	LCD_Test_Clear(void);			// ��������
void 	LCD_Test_Text(void);			   // �ı�����
void 	LCD_Test_Variable (void);	   // ������ʾ������������С��
void 	LCD_Test_Color(void);			// ����������
void 	LCD_Test_Grahic(void);		   // 2Dͼ�λ���
void 	LCD_Test_Image(void);			// ͼƬ��ʾ
void  LCD_Test_Direction(void);	   // ������ʾ����

static TaskHandle_t app_task1_handle;
static TaskHandle_t app_task2_handle;
static TaskHandle_t app_task3_handle;
static void App_Task1(void* pvParamenters);
static void App_Task2(void* pvParamenters);
static void App_Task3(void* pvParamenters);


int main(void)
{
	LED_Init();
	Usart_Config ();
	SPI_LCD_Init();			// LCD��ʼ��
	lv_init();
	lv_port_disp_init();
	GPIO_ResetBits(LED1_PORT,LED1_PIN);
	
//	while (1)
//	{
//		LCD_Test_Clear(); 		// ��������
//		LCD_Test_Text();			// �ı�����
//		LCD_Test_Variable();		// ������ʾ������������С��
//		LCD_Test_Color();			// ��ɫ����
//		LCD_Test_Grahic();		// 2Dͼ�λ���	
//		LCD_Test_Image();			// ͼƬ��ʾ
//		LCD_Test_Direction();	// ������ʾ����	
//	}
	
	xTaskCreate(App_Task1,"App_Task1",64,NULL,9,&app_task1_handle);
	xTaskCreate(App_Task2,"App_Task1",64,NULL,9,&app_task2_handle);
	xTaskCreate(App_Task3,"App_Task1",512,NULL,9,&app_task3_handle);		
	
	vTaskStartScheduler();
}
void App_Task1(void *pvParameters)
{
  for(;;)
	{
		GPIO_ResetBits(LED1_PORT,LED1_PIN);
		vTaskDelay(pdMS_TO_TICKS(1000));
		GPIO_SetBits(LED1_PORT,LED1_PIN);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
void App_Task2(void *pvParameters)
{

  for(;;)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
void App_Task3(void *pvParameters)
{
	lv_demo_benchmark();
  for(;;)
	{
		lv_task_handler();
		vTaskDelay(pdMS_TO_TICKS(1));
	}
}
void vApplicationTickHook()
{
	lv_tick_inc(1);
}


/*************************************************************************************************
*	�� �� ��:	LCD_Test_Clear
*
*	��������:	��������
*************************************************************************************************/
void LCD_Test_Clear(void)
{
	uint8_t	i = 0;			// ��������
			
	LCD_SetTextFont(&CH_Font24);			// ����2424��������,ASCII�����ӦΪ2412
	LCD_SetColor(LCD_BLACK);				// ���û�����ɫ

	for(i=0;i<8;i++)
	{
		switch (i)		// �л�����ɫ
		{
			case 0: LCD_SetBackColor(LIGHT_RED); 		break;	
			case 1: LCD_SetBackColor(LIGHT_GREEN); 	break;				
			case 2: LCD_SetBackColor(LIGHT_BLUE); 		break;
			case 3: LCD_SetBackColor(LIGHT_YELLOW); 	break;
			case 4: LCD_SetBackColor(LIGHT_CYAN);		break;
			case 5: LCD_SetBackColor(LIGHT_GREY); 		break;
			case 6: LCD_SetBackColor(LIGHT_MAGENTA); 	break;
			case 7: LCD_SetBackColor(LCD_WHITE); 		break;			
			default:	break;			
		}
		LCD_Clear();		// ����
		LCD_DisplayText(13, 70,"6666666");
		LCD_DisplayText(13,106,"6666666");
		LCD_DisplayText(13,142,"6666666");	
//		Delay_ms(1000);	// ��ʱ
	}
}


