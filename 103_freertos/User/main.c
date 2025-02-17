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
	//�����ź���
//	g_xSemTicks=xSemaphoreCreateCounting(6, 1);
	g_xSemTicks=xSemaphoreCreateBinary();
	g_xSemTicks1=xSemaphoreCreateMutex();
	xSemaphoreGive(g_xSemTicks);
	aa=xQueueCreate(2,4);
  //��������
//	xTaskCreate((TaskFunction_t)App_Task1,          //������ں���
//	            (const char *)"App_Task1",          //��������
//							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
//							(void*)&g_car[0],                        //������ں�������
//							(UBaseType_t)11,                     //�������ȼ�
//							(TaskHandle_t*)&app_task1_handle);  //������
//	//��������
//	xTaskCreate((TaskFunction_t)App_Task1,          //������ں���
//	            (const char *)"App_Task2",          //��������
//							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
//							(void*)&g_car[1],                        //������ں�������
//							(UBaseType_t)10,                     //�������ȼ�
//							(TaskHandle_t*)&app_task2_handle);  //������
	xTaskCreate((TaskFunction_t)App_Task2,          //������ں���
	            (const char *)"App_Task4",          //��������
							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
							(void*)NULL,                        //������ں�������
							(UBaseType_t)7,                     //�������ȼ�
							(TaskHandle_t*)&app_task4_handle);  //������
	xTaskCreate((TaskFunction_t)App_Task3,          //������ں���
	            (const char *)"App_Task4",          //��������
							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
							(void*)NULL,                        //������ں�������
							(UBaseType_t)7,                     //�������ȼ�
							(TaskHandle_t*)&app_task5_handle);  //������
	//�������������						
	vTaskStartScheduler();
}
static int a=1;
//�ź���////////////////////////////////////////////////////////////////////////
 BaseType_t error_state;
u8 er=0;
//void App_Task1(void *pvParameters)
//{
//	struct car *pv=pvParameters;
//	//�����ź���
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
//			//�����ź���
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
//���񴴽������𡢾�׼��ʱ/////////////////////////////////////////////////////////////////////////////////////////////
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
//				xTaskCreate((TaskFunction_t)App_Task1,    //������ں���
//	            (const char *)"App_Task1",          //��������
//							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
//							(void*)NULL,                        //������ں�������
//							(UBaseType_t)1,                     //�������ȼ�
//							(TaskHandle_t*)&app_task1_handle);  //������
//							//��������1
//				vTaskSuspend(app_task3_handle);//��������3
//		}
//		if(count2==10)
//		{
//					vTaskDelete(app_task1_handle);//ɾȥ����1
//					app_task1_handle = NULL;//û�г��򶯲���
//					vTaskResume(app_task3_handle);//�ָ�����3
//		}
//		vTaskDelay(pdMS_TO_TICKS(1000));//pdMS_TO_TICKS()��׼��ʱ����
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

