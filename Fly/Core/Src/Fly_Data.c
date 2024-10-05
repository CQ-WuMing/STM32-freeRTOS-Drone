#include"Fly_Data.h"
#include"main.h"
#include"stdio.h"
#include "nrf24L01.h"
#include"pid.h"
#include"mpu6050.h"

PidObject pidPitch;
PidObject pidRoll;
PidObject pidYaw;
PidObject pidRateX;
PidObject pidRateY;
PidObject pidRateZ;

PidObject *pids[] = {&pidPitch,&pidRoll, &pidYaw, &pidRateX, &pidRateY, &pidRateZ};
_stRemote remote;
uint8_t unlock_flag = 0;
//解析接收到的数据
void DataRead(uint8_t *buf, uint8_t len){
	connect_flag ++;
	if(connect_flag == 1){

	uint32_t rc_sum = 0;
	uint32_t fly_sum = 0;
	//_stRemote remote;

	//校验帧头
	if(!((*buf == 0xAA) && (*(buf+1) == 0xAF))){
		return;
	}

	//校验和检验
	rc_sum = *(buf+len - 4) <<24 |*(buf +len - 3) <<16 |*(buf+len -2) << 8 | *(buf + len -1);

	for(uint8_t i = 0; i < len -4; i++){
		fly_sum += buf[i];

	}

	if(fly_sum != rc_sum){
		return;
	}
	//判断功能字
	if(buf[2] == 0x03){
		//取出数据
		remote.THR  = (uint16_t)( *(buf+4) << 8 | *(buf+5));
		remote.YAW = (uint16_t)( *(buf+6) << 8 | *(buf+7));
		remote.ROL  = (uint16_t)( *(buf+8) << 8 | *(buf+9));
		remote.PIT    = (uint16_t)( *(buf+10) << 8 | *(buf+11));

		//辅助通道数据
		remote.AUX1    = (uint16_t)( *(buf+12) << 8 | *(buf+13));
		remote.AUX2    = (uint16_t)( *(buf+14) << 8 | *(buf+15));
		remote.AUX3    = (uint16_t)( *(buf+16) << 8 | *(buf+17));
		remote.AUX4    = (uint16_t)( *(buf+18) << 8 | *(buf+19));
		remote.AUX5    = (uint16_t)( *(buf+20) << 8 | *(buf+21));
		remote.AUX6    = (uint16_t)( *(buf+22) << 8 | *(buf+23));
	}
	}
	if(connect_flag > 500){

		connect_flag = 1;

	}

}

//解锁飞机，状态机
void Rc_Unlock(){

	static uint8_t status = WAITTING_1;
	static uint8_t time_count = 0;
	switch(status){
		case WAITTING_1:
			if(remote.THR < 1000){
				status = WAITTING_2;
			}
			break;

		case WAITTING_2:
			if(remote.THR > 1900){

				time_count++;
				if(time_count > 50){
				status = WAITTING_3;
				time_count = 0;
				}

			}
			break;

		case WAITTING_3:
			if(remote.THR < 1000){
				status = WAITTING_4;
			}
			break;


		case WAITTING_4:
			unlock_flag = 1;//解锁

			//如果长时间不使用就关锁
			if(remote.THR <1030){
			if(time_count > 1000){
				unlock_flag = 1;//关
				status = WAITTING_1;
				time_count = 0;

			}

			}

			break;
	}

}
//失联后的处理逻辑
void Rc_Analysis(){
	static uint16_t thr_count = 0;
	static uint16_t disconnect_count = 0;
	if(connect_flag  == 1){

		Rc_Unlock();
	}
	else{
		//失联3秒
		if(disconnect_count++ > 300){



		//慢慢降低油门
		//除了油门全部归中
		remote.PIT = 1500;
		remote.ROL= 1500;
		remote.YAW = 1500;
		//如果油门很低，直接清
		if(remote.THR < 1200){

			remote.THR = 1000;
			//尝试重连2.4G
			while(NRF24L01_Check());
			NRF24L01_RX_Mode();

		}
		else{
			if(thr_count++ > 100){//假设10ms执行一次，100次等于1秒
				remote.THR -= 50;
				thr_count = 0;

			}

		}
		}


	}
}

void PID_Control(){
	static uint8_t status = WAITTING_1;
	switch(status){
			case WAITTING_1:
				if(unlock_flag == 1){
					status = WAITTING_2;
				}
				break;

			case WAITTING_2:
				ResetPID(pids, 6);
				status = WAITTING_3;
				break;

			case WAITTING_3:
				//赋值测量值
				pidPitch.measured =pitch;
				pidRoll.measured = roll;
				pidYaw.measured = yaw;

				pidRateX.measured = roll_g;
				pidRateY.measured = pitch_g;
				pidRateZ.measured = yaw_g;

				CasecadePID(&pidPitch, &pidRateY,0.002f);
				CasecadePID(&pidRoll, &pidRateX,0.002f);
				CasecadePID(&pidYaw, &pidRateZ,0.002f);
				break;

  }
}





