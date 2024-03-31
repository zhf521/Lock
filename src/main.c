#include "stm32f10x.h"
#include "Relay.h"
#include "MatrixKey.h"
#include "OLED.h"
#include "Delay.h"
#include "string.h"

#define password "123456"

int main(void)
{
    // 初始化继电器、OLED
    Relay_Init();
    OLED_Init();
loop:
    // 显示欢迎信息
    OLED_ShowChinese(20, 0, "请输入密码：");
    OLED_ShowString(20, 24, "_ _ _ _ _ _", OLED_8X16);
    OLED_Update();
    // 输入的按键值
    char inputKeyValue = '\0';
    // 输入的密码
    char inputPassword[6] = {'\0'};
    // 当前输入的密码位数
    int count = 0;
    while (1) {
        // 检测按键是否按下
        inputKeyValue = MatrixKey_GetValue();
        if (inputKeyValue >= '0' && inputKeyValue <= '9') {
            Delay_ms(100);
            if (MatrixKey_GetValue() == inputKeyValue) {
                inputPassword[count] = inputKeyValue;
                // 显示按下的按键值
                OLED_ShowChar(20 + count * 16, 24, inputPassword[count], OLED_8X16);
                OLED_Update();
                count++;
                if (count > 5) {
                    count = 0;
                }
            }
        }
        if (inputKeyValue == '*') {
            Delay_ms(100);
            if (MatrixKey_GetValue() == inputKeyValue) {
                if (strcmp(inputPassword, password) == 0) {
                    OLED_Clear();
                    OLED_ShowChinese(0, 0, "密码正确，已开锁");
                    OLED_Update();
                    Relay_On();
                    Delay_s(2);
                    OLED_Clear();
                    Relay_Off();
                    goto loop;
                } else {
                    OLED_Clear();
                    OLED_ShowChinese(0, 0, "密码错误，请重试");
                    OLED_Update();
                    Delay_s(2);
                    OLED_Clear();
                    goto loop;
                }
            }
        }
        if (inputKeyValue == '#') {
            Delay_ms(100);
            if (MatrixKey_GetValue() == inputKeyValue) {
                memset(inputPassword, '\0', sizeof(inputPassword));
                count = 0;
                OLED_ShowString(20, 24, "_ _ _ _ _ _", OLED_8X16);
                OLED_Update();
            }
        }
    }
}
