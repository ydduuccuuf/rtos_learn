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
TaskHandle_t app_task1_handle = NULL;
TaskHandle_t app_task2_handle = NULL;
//TaskHandle_t app_task3_handle = NULL;
TaskHandle_t app_task4_handle = NULL;
TaskHandle_t app_task5_handle = NULL;
StaticEventGroup_t group_event1;
SemaphoreHandle_t g_xSemTicks1;
SemaphoreHandle_t g_xSemTicks;
QueueHandle_t aa;
void App_Task1(void* pvParamenters);
void App_Task2(void* pvParamenters);
void App_Task3(void* pvParamenters);
uint16_t count1=0;
uint16_t count2=0;
uint16_t count3=0;

struct car{
	u8 x;
	u8 y;
};
struct car g_car[3]={
	{1,1},
	{2,1},
	{3,1},
};
uint8_t ID;
int16_t AX, AY, AZ, GX, GY, GZ;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	Delay_ms(200);
	OLED_Init();
	Timer_Init();
	Serial_Init();
	Key_Init();
	TIM_Cmd(TIM2, ENABLE);
	//计数信号量
//	g_xSemTicks=xSemaphoreCreateCounting(6, 1);
	g_xSemTicks=xSemaphoreCreateBinary();
	g_xSemTicks1=xSemaphoreCreateMutex();
	xSemaphoreGive(g_xSemTicks);
	aa=xQueueCreate(2,4);
  //创建任务
//	xTaskCreate((TaskFunction_t)App_Task1,          //任务入口函数
//	            (const char *)"App_Task1",          //任务名字
//							(uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
//							(void*)&g_car[0],                        //任务入口函数参数
//							(UBaseType_t)11,                     //任务优先级
//							(TaskHandle_t*)&app_task1_handle);  //任务句柄
//	//创建任务
//	xTaskCreate((TaskFunction_t)App_Task1,          //任务入口函数
//	            (const char *)"App_Task2",          //任务名字
//							(uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
//							(void*)&g_car[1],                        //任务入口函数参数
//							(UBaseType_t)10,                     //任务优先级
//							(TaskHandle_t*)&app_task2_handle);  //任务句柄
	xTaskCreate((TaskFunction_t)App_Task2,          //任务入口函数
	            (const char *)"App_Task4",          //任务名字
							(uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
							(void*)NULL,                        //任务入口函数参数
							(UBaseType_t)7,                     //任务优先级
							(TaskHandle_t*)&app_task4_handle);  //任务句柄
	xTaskCreate((TaskFunction_t)App_Task3,          //任务入口函数
	            (const char *)"App_Task4",          //任务名字
							(uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
							(void*)NULL,                        //任务入口函数参数
							(UBaseType_t)7,                     //任务优先级
							(TaskHandle_t*)&app_task5_handle);  //任务句柄
	//开启任务调度器						
	vTaskStartScheduler();
}
static int a=1;
//信号量////////////////////////////////////////////////////////////////////////
 BaseType_t error_state;
u8 er=0;
//void App_Task1(void *pvParameters)
//{
//	struct car *pv=pvParameters;
//	//计数信号量
//	
//  for(;;)
//	{
//		
//		xSemaphoreTake(g_xSemTicks,portMAX_DELAY);
//		a=0;
//		OLED_ShowNum(pv->x, 1, pv->y++, 5);
//		a=1;
//	
////		if(pv->y==10)
////		{
////			OLED_Clear();
//			//计数信号量
//			xSemaphoreGive(g_xSemTicks);
////			vTaskDelete(NULL);
////		}
//		vTaskDelay(pdMS_TO_TICKS(500));
//	}
//}
void App_Task2(void *pvParameters)
{
	u32 key_a=0;
	MPU6050_Init();
  for(;;)
	{
//		if(a==1)
//		{
		 xSemaphoreTake(g_xSemTicks1,portMAX_DELAY);
		 xQueueReceive(aa,&key_a,portMAX_DELAY);
		 OLED_ShowSignedNum(1, 8, key_a, 5);
		 OLED_ShowSignedNum(2, 8, get_time(), 5);
		 OLED_ShowSignedNum(3, 8, er, 5);
		 OLED_ShowSignedNum(4, 8, AX, 5);
		 xSemaphoreGive(g_xSemTicks1);
//		}
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void App_Task3(void *pvParameters)
{
	u32 key_a=0;
  for(;;)
	{
		key_a=Key_GetNum();
		AX=(MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L)<<8)|MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
		xQueueSendToBack(aa,&AX,portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//任务创建、挂起、精准计时/////////////////////////////////////////////////////////////////////////////////////////////
//static int a=1; 
//void App_Task1(void *pvParameters)
//{
//  for(;;)
//	{
//		
//		if(a==1)
//		{
//			a=0;
//			OLED_ShowNum(1, 1, count1++, 5);
//			a=1;
//		}
//		
//		vTaskDelay(pdMS_TO_TICKS(1000));
//	}
//}
//void App_Task2(void *pvParameters)
//{
//	
//  for(;;)
//	{
//		
//		if(a==1)
//		{
//			a=0;
//			OLED_ShowNum(2, 1, count2++, 5);
//			a=1;
//		}
//		if(count2==4)
//		{
//				xTaskCreate((TaskFunction_t)App_Task1,    //任务入口函数
//	            (const char *)"App_Task1",          //任务名字
//							(uint16_t)configMINIMAL_STACK_SIZE, //任务栈大小
//							(void*)NULL,                        //任务入口函数参数
//							(UBaseType_t)1,                     //任务优先级
//							(TaskHandle_t*)&app_task1_handle);  //任务句柄
//							//创建任务1
//				vTaskSuspend(app_task3_handle);//挂起任务3
//		}
//		if(count2==10)
//		{
//					vTaskDelete(app_task1_handle);//删去任务1
//					app_task1_handle = NULL;//没有程序动不了
//					vTaskResume(app_task3_handle);//恢复任务3
//		}
//		vTaskDelay(pdMS_TO_TICKS(1000));//pdMS_TO_TICKS()精准计时函数
//	}
//}

//void App_Task3(void *pvParameters)
//{
//  for(;;)
//	{
//		if(a==1)
//		{
//			a=0;
//			OLED_ShowNum(3, 1, count3++, 5);
//			a=1;
//		}
//		vTaskDelay(pdMS_TO_TICKS(1000));
//	}
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

