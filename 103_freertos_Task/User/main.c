#include "stm32f10x.h"                  // Device header
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "semphr.h"
#include "task.h"
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "MyI2C.h"
#include "MPU6050.h"
#include "MPU6050_Reg.h"
#include "Timer.h"
#include "Key.h"
#include "semphr.h"
static TaskHandle_t app_task1_handle;
static TaskHandle_t app_task2_handle;
static TaskHandle_t app_task3_handle;
StaticEventGroup_t group_event1;
SemaphoreHandle_t g_xSemTicks;
SemaphoreHandle_t MPU6050_START;
QueueHandle_t key_queue;
QueueHandle_t MP6050_queue;
void App_Task1(void* pvParamenters);
void App_Task2(void* pvParamenters);
void App_Task3(void* pvParamenters);
#define OLED_IIC_START		xSemaphoreTake(g_xSemTicks,portMAX_DELAY);
#define OLED_IIC_CLOSE		xSemaphoreGive(g_xSemTicks);
struct MP6050_DATA{
	int16_t AX;
	int16_t AY;
	int16_t AZ;
	int16_t GX;
	int16_t GY;
	int16_t GZ;
};
//struct car g_car[3]={
//	{1,1},
//	{2,1},
//	{3,1},
//};
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	Delay_ms(200);
	OLED_Init();
	Timer_Init();
	Serial_Init();
	Key_Init();
	TIM_Cmd(TIM2, ENABLE);
	key_queue=xQueueCreate(1,4);
	MP6050_queue=xQueueCreate(1,16);
	g_xSemTicks=xSemaphoreCreateMutex();
	xTaskCreate(App_Task1,"App_Task1",48,NULL,9,&app_task1_handle);
	xTaskCreate(App_Task2,"App_Task1",48,NULL,9,&app_task2_handle);
	xTaskCreate(App_Task3,"App_Task1",24,NULL,9,&app_task3_handle);		
	
	vTaskStartScheduler();
}

void App_Task1(void *pvParameters)
{
	u8 page=1;
	u8 choice=1;
	u8 key_value=0;
	struct MP6050_DATA MP6050_VALUE={0,0,0,0,0,0};
  for(;;)
	{
		xQueueReceive(key_queue,&key_value,10);
		xQueueReceive(MP6050_queue,&MP6050_VALUE,10);
		OLED_ShowNum(4,10,key_value,1);OLED_ShowNum(4,11,page,1);OLED_ShowNum(4,12,choice,1);
		
		switch(key_value)
		{
		 case 1:page=page+choice;OLED_Clear();key_value=0;break;
		 case 2:xTaskNotifyGive(app_task2_handle);;break;
		 case 3:page=1;OLED_Clear();key_value=0;break;
		 case 4:choice--;OLED_Clear();key_value=0;break;
		 case 5:choice++;OLED_Clear();key_value=0;break;
		}
		
		if(page==1)
		{
		OLED_ShowString(1,1,"1");
		OLED_ShowString(2,1,"2");
		OLED_ShowString(3,1,"3");
		if(choice==1){OLED_ShowString(1,2,"<=");}
		else if(choice==2){OLED_ShowString(2,2,"<=");}
		else if(choice==3){OLED_ShowString(3,2,"<=");}
		}
		else if(page==2)
		{
			OLED_ShowSignedNum(2, 1, MP6050_VALUE.AX, 5);
			OLED_ShowSignedNum(3, 1, MP6050_VALUE.AY, 5);
			OLED_ShowSignedNum(4, 1, MP6050_VALUE.AZ, 5);
			OLED_ShowSignedNum(2, 8, MP6050_VALUE.GX, 5);
			OLED_ShowSignedNum(3, 8, MP6050_VALUE.GY, 5);
			OLED_ShowSignedNum(4, 8, MP6050_VALUE.GZ, 5);
		}
		else if(page==3)
		{
			OLED_ShowString(3,1,"PAGE3");
		}
		else if(page==4)
		{
			OLED_ShowString(3,1,"PAGE4");
		}
		vTaskDelay(pdMS_TO_TICKS(50));		
	}
	
}
void App_Task2(void *pvParameters)
{
	struct MP6050_DATA MP6050_VALUE;
	MPU6050_Init();
//	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	
  for(;;)
	{
//		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		MPU6050_GetData(&MP6050_VALUE.AX,&MP6050_VALUE.AY,&MP6050_VALUE.AZ,&MP6050_VALUE.GX,&MP6050_VALUE.GY,&MP6050_VALUE.GZ);
		xQueueOverwrite(MP6050_queue,&MP6050_VALUE);
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}
void App_Task3(void *pvParameters)
{
	u8 key_value;
  for(;;)
	{
		switch(return_key())
		{
			case 1: key_value=1; xQueueOverwrite(key_queue,&key_value);break;
			case 2: key_value=2; xQueueOverwrite(key_queue,&key_value);break;
			case 3: key_value=3; xQueueOverwrite(key_queue,&key_value);break;
			case 4: key_value=4; xQueueOverwrite(key_queue,&key_value);break;
			case 5: key_value=5; xQueueOverwrite(key_queue,&key_value);break;
		}
		vTaskDelay(5);
	}
}

