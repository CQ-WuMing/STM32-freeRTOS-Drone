

#ifndef INC_DATA_H_
#define INC_DATA_H_

#include "main.h"
#include"gpio.h"

#define Read_Key HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)//读校准的按键
#define Filter_Num 10//滑动窗口的长度
//四个微调
#define Read_Key_U  HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)
#define Read_Key_D  HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin)
#define Read_Key_L  HAL_GPIO_ReadPin(KEY5_GPIO_Port,KEY5_Pin)
#define Read_Key_R  HAL_GPIO_ReadPin(KEY6_GPIO_Port,KEY6_Pin)


struct  _Rc{
	int16_t THR;		//油门
	int16_t YAW;		//偏航
	int16_t ROL;		//横滚
	int16_t PIT;			//俯仰

	//预留六个通道
	int16_t AUX1;
	int16_t AUX2;
	int16_t AUX3;
	int16_t AUX4;
	int16_t AUX5;
	int16_t AUX6;
};

struct _Filter{

	uint32_t sum ;
	uint16_t old[Filter_Num];
};

//偏差值的结构体
struct _Offset{
	int16_t THR;		//油门
	int16_t YAW;		//偏航
	int16_t ROL;		//横滚
	int16_t PIT;			//俯仰
};

void Stick_Scan();
void Limit();
void Mid_Offset();
void Window_Filter();
void KeyPress();
void Mid_Limit();
void RemoteData();
extern struct _Rc rc;
extern struct _Filter Filter_Thr, Filter_Pitch, Filter_Roll, Filter_Yaw;
extern struct _Offset offset;
#endif /* INC_DATA_H_ */
