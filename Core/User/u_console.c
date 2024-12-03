#include "u_console.h"
#include "u_altrasonic.h"
#include "u_oled.h"

#include <cstring>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

Console_Typedef console;
extern Oled_Typedef oled;

uint8_t fputc_src = 0;

int fputc(int ch, FILE *f)
{
  uint8_t tmp[1] = {ch};
  if(fputc_src == 0)
    HAL_UART_Transmit(console.uart, tmp, 1, 0xffff);
  else if(fputc_src == 1)
    // HAL_UART_Transmit(console.uart, tmp, 1, 0xffff);
    OLED_Show_Char(ch);
  return ch;
}

// int fgetc(FILE * f)
// {
//   uint8_t ch = 0;
//   HAL_UART_Receive(console.uart,&ch, 1, 0xffff);
//   return ch;
// }

void Console_Init(UART_HandleTypeDef *uart)
{
    console.length = 0;
    console.is_receiving = 0;
    console.uart = uart;
    // 打开串口接收中断
    HAL_UART_Receive_IT(uart, &console.buf, 1);
}

void Console_Log(const char *s, ...)
{
  va_list ap;
  printf(s, ap);
  printf("\n");
}

void Console_Pre_Handler(void)
{
  // 将cmd的后几个填充为"\0"
  for(int i=console.length; i<U_CONSOLE_CMD_MAX_LENGTH; i++)
  {
    console.cmd[i] = 0;
  }
  Console_Log("recieve: %s", console.cmd);
  // 指令处理
  Console_Cmd_Handler();
  // 处理完后清空指令
  console.length = 0;
}

void Console_Receive_Handler(void)
{
  if(!console.is_receiving)
  {
    // 若没有在接收
    if(console.buf == U_CONSOLE_CMD_BEGIN_TAG)
    {
      // 若收到了初始字符 说明收到的是指令
      // 进入接收指令状态
      console.is_receiving = 1;
      // 长度置零
      console.length = 0;
    }
  }
  else
  {
    // 若正在接收
    if(console.buf == U_CONSOLE_CMD_END_TAG)
    {
      // 收到结束字符
      console.is_receiving = 0;
      // 处理命令
      Console_Pre_Handler();
    }
    else
    {
      if(console.length < U_CONSOLE_CMD_MAX_LENGTH)
      {
        // 若未接收完成 则填入
        console.cmd[console.length] = console.buf;
        console.length ++;
      }
      else
      {
        // 若满 发回错误提示 并且清空
        Console_Log("error:cmd_too_long");
        console.length = 0;
        console.is_receiving = 0;
      }
    }
  }
  HAL_UART_Receive_IT(console.uart, &console.buf, 1);
}
 
/** @fn uint16_t hex_to_str(uint8_t *pAscii,uint8_t *pHex,uint16_t unStrLen)
 * @brief hex数组转字符串
 * @param pAscii：字符串
 * @param pHex：数组
 * @param unHexLen：数组长度
 * @return 转换结果	0：失败，其他：字符串长度
 * @note	转换后的字符串字符全部大写
 * 来自 https://blog.csdn.net/qq997758497/article/details/117755064
 */
uint16_t hex_to_str(uint8_t *pAscii, uint8_t *pHex, uint16_t unHexLen){
	uint8_t ucTmp;
	uint16_t i;
	uint8_t *pStr = pAscii;
	for(i = 0; i < unHexLen; i++){
		if(!pHex || !pStr)
			return 0;
 
		ucTmp = (pHex[i] & 0xF0) >> 4;
		if(ucTmp < 10){
			ucTmp += '0';
		} else {
			ucTmp = ucTmp - 10 + 'A';
		}
		*pStr++ = ucTmp;
 
		ucTmp = (pHex[i] & 0x0F);
		if(ucTmp < 10){
			ucTmp += '0';
		} else {
			ucTmp = ucTmp - 10 + 'A';
		}
		*pStr++ = ucTmp;
	}
	return (2 * unHexLen);
}
 
/** @fn uint16_t str_to_hex(uint8_t *pAscii,uint8_t *pHex,uint16_t unStrLen)
 * @brief 字符串转hex数组
 * @param pAscii：字符串
 * @param pHex：数组
 * @param unStrLen：字符串长度
 * @return 转换结果	0：失败，其他：数组长度
 * @note	字符串长度必须为2的倍数，不区分大小写
 * 来自 https://blog.csdn.net/qq997758497/article/details/117755064
 */
uint16_t str_to_hex(uint8_t *pAscii, uint8_t *pHex, uint16_t unStrLen){
	uint8_t ucByte;
	uint8_t ucTmp;
	uint16_t i;
 
	if(0 != unStrLen % 2)
		return 0;
 
	for(i = 0; i < unStrLen / 2; i++){
		ucTmp = *pAscii++;
		if(ucTmp >= '0' && ucTmp <= '9'){
			ucTmp -= '0';
		} else if(ucTmp >= 'A' && ucTmp <= 'F'){
			ucTmp = ucTmp + 10 - 'A';
		} else if(ucTmp >= 'a' && ucTmp <= 'f'){
			ucTmp = ucTmp + 10 - 'a';
		} else {
			return 0;
		}
		ucByte = (ucTmp << 4);
 
		ucTmp = *pAscii++;
		if(ucTmp >= '0' && ucTmp <= '9'){
			ucTmp -= '0';
		} else if(ucTmp >= 'A' && ucTmp <= 'F'){
			ucTmp = ucTmp + 10 - 'A';
		} else if(ucTmp >= 'a' && ucTmp <= 'f'){
			ucTmp = ucTmp + 10 - 'a';
		} else {
			return 0;
		}
		ucByte += ucTmp;
		pHex[i] = ucByte;
	}
	return (unStrLen / 2);
}

void Console_Cmd_Handler(void)
{
  char *head = strtok(console.cmd, " ");
  // 如果没有参数 arg会为null
  char *arg = strtok(NULL, " ");
  if(arg){

		if(strcmp(head, "line_1") == 0){
      oled.cache[1] = arg;
      OLED_Flush_Line(1);
      return;
    }
		if(strcmp(head, "collect") == 0){
      Astrasonic_Collect();
      return;
    }
		if(strcmp(head, "oled") == 0){
      OLED_Show_Char(arg[0]);
      return;
    }
		if(strcmp(head, "page") == 0){
      WriteCmd(0xb0 + atoi(arg));
      return;
    }
		if(strcmp(head, "fill") == 0){
      OLED_Fill(atoi(arg));
      return;
    }
		if(strcmp(head, "clear") == 0){
      OLED_Clear();
      return;
    }
		if(strcmp(head, "x") == 0){
      OLED_Set_Pos(atoi(arg), oled.y);
      return;
    }
		if(strcmp(head, "y") == 0){
      OLED_Set_Pos(oled.x, atoi(arg));
      return;
    }
  }
}
