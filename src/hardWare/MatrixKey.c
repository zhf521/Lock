#include "stm32f10x.h"
#include "Delay.h"

// 定义行
#define Row1 GPIO_Pin_0
#define Row2 GPIO_Pin_1
#define Row3 GPIO_Pin_2
#define Row4 GPIO_Pin_3
// 定义列
#define Col1 GPIO_Pin_4
#define Col2 GPIO_Pin_5
#define Col3 GPIO_Pin_6
#define Col4 GPIO_Pin_7
// 定义读取的电平值
#define Row1_Input GPIO_ReadInputDataBit(GPIOA, Row1)
#define Row2_Input GPIO_ReadInputDataBit(GPIOA, Row2)
#define Row3_Input GPIO_ReadInputDataBit(GPIOA, Row3)
#define Row4_Input GPIO_ReadInputDataBit(GPIOA, Row4)
#define Col1_Input GPIO_ReadInputDataBit(GPIOA, Col1)
#define Col2_Input GPIO_ReadInputDataBit(GPIOA, Col2)
#define Col3_Input GPIO_ReadInputDataBit(GPIOA, Col3)
#define Col4_Input GPIO_ReadInputDataBit(GPIOA, Col4)

// 初始化函数1，通过行输入和列输出查找按键在哪一行
void MatrixKey_Init1(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_Initstucture;
    GPIO_Initstucture.GPIO_Mode  = GPIO_Mode_IPD; // 将行所对应引脚配置下拉输入 (默认低电平)
    GPIO_Initstucture.GPIO_Pin   = Row1 | Row2 | Row3 | Row4;
    GPIO_Initstucture.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstucture);

    GPIO_Initstucture.GPIO_Mode  = GPIO_Mode_Out_PP; // 将列所对应引脚配置推挽输出
    GPIO_Initstucture.GPIO_Pin   = Col1 | Col2 | Col3 | Col4;
    GPIO_Initstucture.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstucture);
    GPIO_SetBits(GPIOA, Col1 | Col2 | Col3 | Col4); // 输出高电平
}
// 初始化函数2，通过列输入和行输出查找按键在哪一列
void MatrixKey_Init2(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_Initstucture;
    GPIO_Initstucture.GPIO_Mode  = GPIO_Mode_IPD; // 将列所对应引脚配置下拉输入 (默认低电平)
    GPIO_Initstucture.GPIO_Pin   = Col1 | Col2 | Col3 | Col4;
    GPIO_Initstucture.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstucture);

    GPIO_Initstucture.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Initstucture.GPIO_Pin   = Row1 | Row2 | Row3 | Row4;
    GPIO_Initstucture.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Initstucture);
    GPIO_SetBits(GPIOA, Row1 | Row2 | Row3 | Row4);
}
// 获取按键值
char MatrixKey_GetValue(void)
{
    char keyValue = 0;
    MatrixKey_Init1();                                                            // 行输入 列输出查找哪一行电平发生变化
    if (Row1_Input == 1 || Row2_Input == 1 || Row3_Input == 1 || Row4_Input == 1) // 如果任意一行电平变化
    {
        Delay_ms(10);        // 消抖
        if (Row1_Input == 1) // 第一行电平变化
        {
            MatrixKey_Init2(); // 改变行列输出输入模式
            Delay_ms(5);
            if (Col1_Input == 1) // 第一列
            {
                keyValue = '1';
            } else if (Col2_Input == 1) {
                keyValue = '2';
            } else if (Col3_Input == 1) {
                keyValue = '3';
            } else if (Col4_Input == 1) {
                keyValue = 'A';
            }
        } else if (Row2_Input == 1) {
            MatrixKey_Init2();
            Delay_ms(5);
            if (Col1_Input == 1) {
                keyValue = '4';
            } else if (Col2_Input == 1) {
                keyValue = '5';
            } else if (Col3_Input == 1) {
                keyValue = '6';
            } else if (Col4_Input == 1) {
                keyValue = 'B';
            }
        } else if (Row3_Input == 1) {
            MatrixKey_Init2();
            Delay_ms(5);
            if (Col1_Input == 1) {
                keyValue = '7';
            } else if (Col2_Input == 1) {
                keyValue = '8';
            } else if (Col3_Input == 1) {
                keyValue = '9';
            } else if (Col4_Input == 1) {
                keyValue = 'C';
            }
        } else if (Row4_Input == 1) {
            MatrixKey_Init2();
            Delay_ms(5);
            if (Col1_Input == 1) {
                keyValue = '*';
            } else if (Col2_Input == 1) {
                keyValue = '0';
            } else if (Col3_Input == 1) {
                keyValue = '#';
            } else if (Col4_Input == 1) {
                keyValue = 'D';
            }
        }
    }
    return keyValue;
}
