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
	g_xSemTicks1=xSemaphoreCreateMutex();
	xTaskCreate((TaskFunction_t)App_Task1,          //������ں���
	            (const char *)"App_Task1",          //��������
							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
							(void*)NULL,                        //������ں�������
							(UBaseType_t)9,                     //�������ȼ�
							(TaskHandle_t*)NULL);  							//������
	xTaskCreate((TaskFunction_t)App_Task2,          //������ں���
	            (const char *)"App_Task2",          //��������
							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
							(void*)NULL,                        //������ں�������
							(UBaseType_t)10,                     //�������ȼ�
							(TaskHandle_t*)&app_task2_handle);  //������
	xTaskCreate((TaskFunction_t)App_Task3,          //������ں���
	            (const char *)"App_Task3",          //��������
							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
							(void*)NULL,                        //������ں�������
							(UBaseType_t)10,                     //�������ȼ�
							(TaskHandle_t*)&app_task3_handle);  //������						
	//�������������						
	vTaskStartScheduler();
}

void App_Task1(void *pvParameters)
{
	u8 y=0;
  for(;;)
	{
		xSemaphoreTake(g_xSemTicks1,portMAX_DELAY);
		OLED_ShowNum(1, 1, y++, 5);
		xSemaphoreGive(g_xSemTicks1);
		if(y==10)
		{xTaskNotifyGive(app_task2_handle);}
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}
void App_Task2(void *pvParameters)
{
	u8 y=0;
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  for(;;)
	{
		xSemaphoreTake(g_xSemTicks1,portMAX_DELAY);
		OLED_ShowNum(2, 1, y++, 5);
		xSemaphoreGive(g_xSemTicks1);
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}
void App_Task3(void *pvParameters)
{
	u8 y=0;
  for(;;)
	{
		xSemaphoreTake(g_xSemTicks1,portMAX_DELAY);
		OLED_ShowNum(3, 1, y++, 5);
		xSemaphoreGive(g_xSemTicks1);
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

