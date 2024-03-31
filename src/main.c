#include "stm32f10x.h"
#include "Relay.h"
#include "MatrixKey.h"
#include "OLED.h"
#include "Delay.h"
#include "string.h"
#include "stdio.h"

int main(void)
{
    // 密码
    char password[6] = "123456";
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
    // 是否处于修改密码模式
    int changePasswordMode = 0;
    // 管理员密码
    char adminPassword[6] = "000000";
    // 当前输入的密码位数
    int count = 0;
    while (1) {
        // 检测按键是否按下
        inputKeyValue = MatrixKey_GetValue();
        // 输入密码
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
        // 确认输入密码
        if (inputKeyValue == '*') {
            Delay_ms(100);
            if (MatrixKey_GetValue() == inputKeyValue) {
                if (changePasswordMode == 1) {
                    // 判断是否处于修改密码模式(确认输入管理员密码)
                    if (strcmp(inputPassword, adminPassword) == 0) {
                        OLED_Clear();
                        OLED_ShowChinese(20, 0, "请输入新密码：");
                        OLED_ShowString(20, 24, "_ _ _ _ _ _", OLED_8X16);
                        OLED_Update();
                        changePasswordMode = 2;
                    } else {
                        OLED_Clear();
                        OLED_ShowChinese(10, 0, "管理员密码错误");
                        OLED_Update();
                        Delay_s(2);
                        OLED_Clear();
                        goto loop;
                    }
                } else if (changePasswordMode == 2) {
                    for (int i = 0; i < 6; i++) {
                        password[i] = inputPassword[i];
                    }
                    printf("密码已修改为：%s\n", password);
                    OLED_Clear();
                    OLED_ShowChinese(30, 0, "修改成功");
                    OLED_Update();
                    Delay_s(2);
                    OLED_Clear();
                    goto loop;
                } else {
                    if (strcmp(password, inputPassword) == 0) {
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
        }
        // 清除输入密码
        if (inputKeyValue == '#') {
            Delay_ms(100);
            if (MatrixKey_GetValue() == inputKeyValue) {
                memset(inputPassword, '\0', sizeof(inputPassword));
                count = 0;
                OLED_ShowString(20, 24, "_ _ _ _ _ _", OLED_8X16);
                OLED_Update();
            }
        }
        // 修改密码
        if (inputKeyValue == 'A') {
            Delay_ms(100);
            if (MatrixKey_GetValue() == inputKeyValue) {
                changePasswordMode = 1;
                count              = 0;
                OLED_Clear();
                OLED_ShowChinese(0, 0, "请输入管理员密码");
                OLED_ShowString(20, 24, "_ _ _ _ _ _", OLED_8X16);
                OLED_Update();
            }
        }
    }
}
