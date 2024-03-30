#include "stm32f10x.h"

// 继电器初始化
void Relay_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    // 定义一个 GPIO 类型的结构体
    GPIO_InitTypeDef GPIO_Initstructure;
    // 配置参数：模式
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
    // 配置参数：引脚
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;
    // 配置参数：速率
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    // 用 GPIO_Init 函数将结构体存储
    GPIO_Init(GPIOB, &GPIO_Initstructure);
}

// 打开继电器
void Relay_On(void)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);
}

// 关闭继电器
void Relay_Off(void)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);
}