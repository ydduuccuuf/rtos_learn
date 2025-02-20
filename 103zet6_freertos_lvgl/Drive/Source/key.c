/***
	*************************************************************************************************
	*	@version V1.0
	*	@author  鹿小班科技	
	*	@brief   按键接口相关函数
   *************************************************************************************************
   *  @description
	*
	*	实验平台：鹿小班STM32F103ZET6核心板 （型号：LXB103ZE-P1）
	* 客服信：19949278543
	*
>>>>> 文件说明：
	*
	*	初始化按键引脚，配置为上拉输入、速度等级2M。
	*
	************************************************************************************************
***/
#include "key.h"  


/*************************************************************************************************
*	函 数 名:	KEY_Init
*
*	函数功能:	按键IO口初始化
*	 
*************************************************************************************************/

void KEY_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体
	RCC_APB2PeriphClockCmd ( KEY_CLK, ENABLE); 	//初始化KEY时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		// 开启IO口复用时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//	禁用JTAG 只使用SWD	，不然PA15、PB3、PB4无法正常使用
	
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //速度选择
	GPIO_InitStructure.GPIO_Pin   = KEY_PIN;	 
	
	GPIO_Init(KEY_PORT, &GPIO_InitStructure);	

}

/*************************************************************************************************
*	函 数 名:	KEY_Scan
*
*	返 回 值:	KEY_ON - 按键按下，KEY_OFF - 按键放开
*
*	函数功能:	按键扫描
*	
*************************************************************************************************/
	
uint8_t	KEY_Scan(void)
{
	if( GPIO_ReadInputDataBit ( KEY_PORT,KEY_PIN) == 0 )	//检测按键是否被按下
	{	
//		Delay_ms(10);	//延时消抖
		if(GPIO_ReadInputDataBit ( KEY_PORT,KEY_PIN) == 0)	//再次检测是否为低电平
		{
			while(GPIO_ReadInputDataBit ( KEY_PORT,KEY_PIN) == 0);	//等待按键放开
			return KEY_ON;	//返回按键按下标志
		}
	}
	return KEY_OFF;	
}


