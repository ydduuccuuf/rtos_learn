/***
	*************************************************************************************************
	*	@version V1.0
	*	@author  ¹С��Ƽ�	
	*	@brief   �����ӿ���غ���
   *************************************************************************************************
   *  @description
	*
	*	ʵ��ƽ̨��¹С��STM32F103ZET6���İ� ���ͺţ�LXB103ZE-P1��
	* �ͷ��ţ�19949278543
	*
>>>>> �ļ�˵����
	*
	*	��ʼ���������ţ�����Ϊ�������롢�ٶȵȼ�2M��
	*
	************************************************************************************************
***/
#include "key.h"  


/*************************************************************************************************
*	�� �� ��:	KEY_Init
*
*	��������:	����IO�ڳ�ʼ��
*	 
*************************************************************************************************/

void KEY_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ��
	RCC_APB2PeriphClockCmd ( KEY_CLK, ENABLE); 	//��ʼ��KEYʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		// ����IO�ڸ���ʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//	����JTAG ֻʹ��SWD	����ȻPA15��PB3��PB4�޷�����ʹ��
	
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //�ٶ�ѡ��
	GPIO_InitStructure.GPIO_Pin   = KEY_PIN;	 
	
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);	

}

/*************************************************************************************************
*	�� �� ��:	KEY_Scan
*
*	�� �� ֵ:	KEY_ON - �������£�KEY_OFF - �����ſ�
*
*	��������:	����ɨ��
*	
*************************************************************************************************/
	
uint8_t	KEY_Scan(void)
{
	if( GPIO_ReadInputDataBit ( KEY_PORT,KEY_PIN) == 0 )	//��ⰴ���Ƿ񱻰���
	{	
//		Delay_ms(10);	//��ʱ����
		if(GPIO_ReadInputDataBit ( KEY_PORT,KEY_PIN) == 0)	//�ٴμ���Ƿ�Ϊ�͵�ƽ
		{
			while(GPIO_ReadInputDataBit ( KEY_PORT,KEY_PIN) == 0);	//�ȴ������ſ�
			return KEY_ON;	//���ذ������±�־
		}
	}
	return KEY_OFF;	
}


