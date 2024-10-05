
#ifndef INC_FLY_DATA_H_
#define INC_FLY_DATA_H_
#include"main.h"
#include"stdio.h"
#include "nrf24L01.h"
#include"pid.h"
#define WAITTING_1  1
#define WAITTING_2  2
#define WAITTING_3  3
#define WAITTING_4  4


typedef struct  {
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
}_stRemote;

void DataRead(uint8_t *buf, uint8_t len);
void Rc_Unlock();
void PID_Control();
extern _stRemote remote;
extern uint8_t unlock_flag;
extern PidObject pidPitch;
extern PidObject pidRoll;
extern PidObject pidYaw;
extern PidObject pidRateX;
extern PidObject pidRateY;
extern PidObject pidRateZ;
#endif /* INC_FLY_DATA_H_ */
