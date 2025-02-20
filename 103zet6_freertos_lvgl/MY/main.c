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
// LCD测试函数，函数定义在底部
void 	LCD_Test_Clear(void);			// 清屏测试
void 	LCD_Test_Text(void);			   // 文本测试
void 	LCD_Test_Variable (void);	   // 变量显示，包括整数和小数
void 	LCD_Test_Color(void);			// 矩形填充测试
void 	LCD_Test_Grahic(void);		   // 2D图形绘制
void 	LCD_Test_Image(void);			// 图片显示
void  LCD_Test_Direction(void);	   // 更换显示方向

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
	SPI_LCD_Init();			// LCD初始化
	lv_init();
	lv_port_disp_init();
	GPIO_ResetBits(LED1_PORT,LED1_PIN);
	
//	while (1)
//	{
//		LCD_Test_Clear(); 		// 清屏测试
//		LCD_Test_Text();			// 文本测试
//		LCD_Test_Variable();		// 变量显示，包括整数和小数
//		LCD_Test_Color();			// 颜色测试
//		LCD_Test_Grahic();		// 2D图形绘制	
//		LCD_Test_Image();			// 图片显示
//		LCD_Test_Direction();	// 更换显示方向	
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
*	函 数 名:	LCD_Test_Clear
*
*	函数功能:	清屏测试
*************************************************************************************************/
void LCD_Test_Clear(void)
{
	uint8_t	i = 0;			// 计数变量
			
	LCD_SetTextFont(&CH_Font24);			// 设置2424中文字体,ASCII字体对应为2412
	LCD_SetColor(LCD_BLACK);				// 设置画笔颜色

	for(i=0;i<8;i++)
	{
		switch (i)		// 切换背景色
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
		LCD_Clear();		// 清屏
		LCD_DisplayText(13, 70,"6666666");
		LCD_DisplayText(13,106,"6666666");
		LCD_DisplayText(13,142,"6666666");	
//		Delay_ms(1000);	// 延时
	}
}


