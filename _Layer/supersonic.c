/** See a brief introduction (right-hand button) */
#include "supersonic.h"
/* Private include -----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*
	函数名称：SuperSonic_Init()
	函数输入：无
	函数输出：无
	函数作用：初始化超声波传感器
*/
void SuperSonic_Init(void)
{
	GPIO_InitTypeDef GPIO_SUPERSONIC_Init;
	TIM_TimeBaseInitTypeDef TIM9_TimeBaseInit;

	//时钟开启
	//TIM9
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
	//GPIOB
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//SYSCFG
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	
	//GPIOB配置
	//PB5 6 7配置为输入无上下拉模式
	//PB4 配置为开漏输出上拉模式
	GPIO_SUPERSONIC_Init.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_SUPERSONIC_Init.GPIO_OType = GPIO_OType_OD;
	GPIO_SUPERSONIC_Init.GPIO_Pin   = GPIO_Pin_4;
	GPIO_SUPERSONIC_Init.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_SUPERSONIC_Init.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOB,&GPIO_SUPERSONIC_Init);
	
	GPIO_SUPERSONIC_Init.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_SUPERSONIC_Init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_SUPERSONIC_Init.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	
	GPIO_Init(GPIOB,&GPIO_SUPERSONIC_Init);
	
	//TRIG脚刚开始必须是低电平
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	//刚开始可能会触发一次超声波 因此等一下消抖
	SysTickDelay(100);
	
	//清零TIM9计数值
	TIM9->CNT = 0;
	
	TIM9_TimeBaseInit.TIM_CounterMode       = TIM_CounterMode_Up;
	TIM9_TimeBaseInit.TIM_Prescaler         = 167;
	TIM9_TimeBaseInit.TIM_ClockDivision     = TIM_CKD_DIV1;
	TIM9_TimeBaseInit.TIM_Period            = 65535;
	TIM9_TimeBaseInit.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM9,&TIM9_TimeBaseInit);
	
	TIM_Cmd(TIM9,DISABLE);
	
	
}


/*
	函数名称：SuperSonic_GetDistant()
	函数输入：无
	函数输出：float类型超声波传感器探测到的距离
	函数作用：以阻塞方式获取前部超声波传感器距离障碍物的距离
	注意事项：调用间隔最小为100ms 调用间隔低于100ms
	注意事项：可能会引起回波干扰，影响判断精度
*/
float SuperSonic_GetDistant(void)
{
	float Distant = 0;
	
	//TRIG 20us高电平
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
	TIM_Cmd(TIM9,ENABLE);
	while(TIM_GetCounter(TIM9) < 20);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	TIM9->CNT = 0;
	
	//等待HCSR04 ECHO信号或者超时
	while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 0) && (TIM_GetCounter(TIM9) < 60000));
	
	//超时返回错误 距离9999米
	if(TIM_GetCounter(TIM9) >= 60000)
	{
		TIM_Cmd(TIM9,DISABLE);
		TIM9->CNT = 0;
		Distant = 9999;
		return Distant;
	}
	
	
	TIM9->CNT = 0;
	
	while((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == 1) && (TIM_GetCounter(TIM9) < 60000));

	//超时返回错误 距离9999米
	if(TIM_GetCounter(TIM9) >= 60000)
	{
		TIM_Cmd(TIM9,DISABLE);
		TIM9->CNT = 0;
		Distant = 9999;
		return Distant;
	}
	
	
	TIM_Cmd(TIM9,DISABLE);
	
	Distant = (TIM_GetCounter(TIM9) * (float)0.00034) / (float)2.0; 
	
	
	TIM_Cmd(TIM9,DISABLE);
	TIM9->CNT = 0;	
	return Distant;
}

