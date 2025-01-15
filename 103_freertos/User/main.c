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
	//��ȫ�����ȼ���Ϊ��ռ���ȼ�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	OLED_Init();
  //��������
//	xTaskCreate((TaskFunction_t)App_Task1,          //������ں���
//	            (const char *)"App_Task1",          //��������
//							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
//							(void*)NULL,                        //������ں�������
//							(UBaseType_t)1,                     //�������ȼ�
//							(TaskHandle_t*)&app_task1_handle);  //������
	//��������
	xTaskCreate((TaskFunction_t)App_Task2,          //������ں���
	            (const char *)"App_Task2",          //��������
							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
							(void*)NULL,                        //������ں�������
							(UBaseType_t)2,                     //�������ȼ�
							(TaskHandle_t*)&app_task2_handle);  //������
	//��������
	xTaskCreate((TaskFunction_t)App_Task3,          //������ں���
	            (const char *)"App_Task3",          //��������
							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
							(void*)NULL,                        //������ں�������
							(UBaseType_t)3,                     //�������ȼ�
							(TaskHandle_t*)&app_task3_handle);  //������
//	xEventGroupCreateStatic(group_event1);

	//�������������						
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
				xTaskCreate((TaskFunction_t)App_Task1,          //������ں���
	            (const char *)"App_Task1",          //��������
							(uint16_t)configMINIMAL_STACK_SIZE, //����ջ��С
							(void*)NULL,                        //������ں�������
							(UBaseType_t)1,                     //�������ȼ�
							(TaskHandle_t*)&app_task1_handle);  //������
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


