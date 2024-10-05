#include "main.h"
#include "motor.h"
#include"Fly_Data.h"
#include "tim.h"
/*四个电机的pwm*/
int16_t motor1 = 0;
int16_t motor2 = 0;
int16_t motor3 = 0;
int16_t motor4 = 0;

void Motor_Control(){
	uint8_t statue = WAITTING_1;
	//阶段一，判断解锁标志位
	switch(statue){

		case WAITTING_1:
		motor1 = motor2= motor3= motor4 =0;
		if(unlock_flag == 1){
			statue = WAITTING_2;
		}
			break;
	//阶段二，判断油门动了>1000，进入正式控制
		case WAITTING_2:
			if(remote.THR > 1000){
				statue = WAITTING_3;
			}
		break;
	//正式控制，摇杆减1000，如果油门小于1050 pwm=0；限制到900+3个pid
		case WAITTING_3:
		motor1 =    remote.THR - 1000;
		motor2 = 	remote.PIT - 1000;
		motor3 = 	remote.ROL - 1000;
		motor4 = 	remote.YAW - 1000;
		//如果油门低，直接不转
		if(remote.THR < 1500){
			motor1 = motor2= motor3= motor4 =0;
		}
		//预留100给pid
		if(motor1 >900){
			motor1 = 900;
		}
		if(motor2 >900){
			motor2 = 900;
		}
		if(motor3 >900){
			motor3 = 900;
		}
		if(motor4 >900){
			motor4 = 900;
		}

		motor1 += pidRateX.out+pidRateY.out+pidRateZ.out;
		motor2 += pidRateX.out- pidRateY.out- pidRateZ.out;
		motor3 += -pidRateX.out- pidRateY.out+pidRateZ.out;
		motor4 += -pidRateX.out+pidRateY.out - pidRateZ.out;
			break;
	}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,motor3);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,motor2);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,motor4);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,motor1);


}


