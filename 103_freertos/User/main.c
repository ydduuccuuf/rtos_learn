#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"
#include "Delay.h"
#include "OLED.h"

TaskHandle_t app_task1_handle = NULL;
TaskHandle_t app_task2_handle = NULL;
TaskHandle_t app_task3_handle = NULL;
StaticEventGroup_t group_event1;
void App_Task1(void* pvParamenters);
void App_Task2(void* pvParamenters);
void App_Task3(void* pvParamenters);
uint16_t count1=0;
uint16_t count2=0;
uint16_t count3=0;
int main(void)
{
	//将全部优先级设为抢占优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	OLED_Init();
  //创建任务
//	xTaskCreate((TaskFunction_t)App_Task1,          //任务入口函数
//	            (const char *)"App_Task1",          //任务名字
//							(uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
//							(void*)NULL,                        //任务入口函数参数
//							(UBaseType_t)1,                     //任务优先级
//							(TaskHandle_t*)&app_task1_handle);  //任务句柄
	//创建任务
	xTaskCreate((TaskFunction_t)App_Task2,          //任务入口函数
	            (const char *)"App_Task2",          //任务名字
							(uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
							(void*)NULL,                        //任务入口函数参数
							(UBaseType_t)2,                     //任务优先级
							(TaskHandle_t*)&app_task2_handle);  //任务句柄
	//创建任务
	xTaskCreate((TaskFunction_t)App_Task3,          //任务入口函数
	            (const char *)"App_Task3",          //任务名字
							(uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
							(void*)NULL,                        //任务入口函数参数
							(UBaseType_t)3,                     //任务优先级
							(TaskHandle_t*)&app_task3_handle);  //任务句柄
//	xEventGroupCreateStatic(group_event1);

	//开启任务调度器						
	vTaskStartScheduler();
}
 

static int a=1; 
void App_Task1(void *pvParameters)
{
  for(;;)
	{
		
		if(a==1)
		{
			a=0;
			OLED_ShowNum(1, 1, count1++, 5);
			a=1;
		}
		
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
void App_Task2(void *pvParameters)
{
	
  for(;;)
	{
		
		if(a==1)
		{
			a=0;
			OLED_ShowNum(2, 1, count2++, 5);
			a=1;
		}
		if(count2==4)
		{
				xTaskCreate((TaskFunction_t)App_Task1,          //任务入口函数
	            (const char *)"App_Task1",          //任务名字
							(uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
							(void*)NULL,                        //任务入口函数参数
							(UBaseType_t)1,                     //任务优先级
							(TaskHandle_t*)&app_task1_handle);  //任务句柄
				vTaskSuspend(app_task3_handle);		
		}
		if(count2==10)
		{
					vTaskDelete(app_task1_handle);
					app_task1_handle = NULL;
					vTaskResume(app_task3_handle);
		}
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void App_Task3(void *pvParameters)
{
  for(;;)
	{
		if(a==1)
		{
			a=0;
			OLED_ShowNum(3, 1, count3++, 5);
			a=1;
		}
		vTaskDelay(pdMS_TO_TICKS(1000));111111111111111111111111111111
	}
}


