#include "u_oled.h"
#include "u_then.h"
#include <stdio.h>
#include <stdarg.h>
#include "u_console.h"
#include "mpu6050.h"

extern uint8_t fputc_src;
extern I2C_HandleTypeDef hi2c1;
extern Mpu6050_Typedef mpu6050;
Oled_Typedef oled;

// Co D/C# 0 0 0 0 0 0
// Co Continuation 恒为0
// D/C# 为1 则接下来一位是data
//      为0 则接下来一位是cmd

void WriteCmd(uint8_t cmd){
    uint8_t tmp[2] = {0x00, cmd};
    HAL_I2C_Master_Transmit(&hi2c1, 0x78, tmp, 2, 0xffff);
}

void WriteData(uint8_t data){
    uint8_t tmp[2] = {0x40, data};
    HAL_I2C_Master_Transmit(&hi2c1, 0x78, tmp, 2, 0xffff);
}
void OLED_Init_Set_Arg(void){
	WriteCmd(0xAE); //display off
	WriteCmd(0x20);	//Set Memory Addressing Mode	
	WriteCmd(0x00);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8);	//Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //亮度调节 0x00~0xff

	WriteCmd(0xa1); //设置COL127映射到SEG0
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //复用率（MUX ratio）即选通的COM行数
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //A4H 设置显示模式为正常模式，此时屏幕输出GDDRAM中的显示数据
	WriteCmd(0xd3); //垂直显示偏移即整个屏幕向上移动的行数，最顶部的行会移到最底行
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
    
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
}
void OLED_Init(void)
{
    HAL_Delay(1000);
    OLED_Init_Set_Arg();
    oled.x = 0;
    oled.y = 0;
    for(uint8_t i=0; i<8; i++){
        oled.cache[i] = "";
    }
		OLED_Clear();
		OLED_Printf_Line(7, "ok");
    OLED_Set_Pos(0,0);
}
void OLED_Printf_Line(uint8_t index, char *str) {
    OLED_Set_Pos(0, index);
    oled.cache[index] = str;
    OLED_Flush_Line(index);
}
void OLED_Flush_Line(uint8_t index){
    if(index >= 8) return;
    char *p = oled.cache[index];
    uint8_t count = 0;
    OLED_Set_Pos(0, index);
    while((*p != '\0') && (count < OLED_LINE_CHAR_MAX)){
        OLED_Show_Char(*p);
        p ++;
        count++;
    }
    // 剩下的全部填充为空白
    while(count < 21){
        OLED_Show_Char(' ');
        count++;
    }
}
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
    oled.x = x;
    oled.y = y;
	WriteCmd(0xb0+oled.y);
    // 设置起始列地址高位 （10H~1FH）
	WriteCmd(((x&0xf0)>>4)|0x10);
    // 设置起始列地址低位 （00H~0FH）
	WriteCmd(x&0x0f);
}
void OLED_Clear(void){
    OLED_Set_Pos(0,0);
    OLED_Fill(0);
}
void OLED_New_Line(void){
    OLED_Set_Pos(0,oled.y+1);
}
void OLED_Fill(uint8_t Fill_data)
{
	uint8_t m,n;
    // 会漏扫一行 所以这里在此换行
	for(m=0;m<8;m++){
        for(n=0;n<128;n++)
            WriteData(Fill_data);
        OLED_New_Line();
    }
}
void OLED_Show_Char(uint8_t c){
    if(c < 32){
        // 若是控制符号
        switch (c)
        {
        case 10:
            // 换行符
            OLED_Set_Pos(oled.x, oled.y+1);
            break;
        }
    } else {
        if(oled.x > 120) return;
        // 非控制符号
        for(uint8_t i=0; i<6; i++)
            WriteData(OLED_F6X8[c-32][i]);
            oled.x += 6;
    }
}
