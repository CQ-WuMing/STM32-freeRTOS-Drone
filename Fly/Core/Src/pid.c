#include "main.h"
#include"pid.h"
#include"Fly_Data.h"

void PidUpdata(PidObject *pid,float dt){

	//本次偏差
	float temp_err;
	float dri;
	//计算偏差值
	temp_err = pid->desired - pid->measured;
	//计算积分。偏差累计和
	pid->integ += temp_err * dt;
	//计算微分，偏差的变化率
	dri = (temp_err - pid->prevError ) / dt;
	//结果保存到out里
	pid->out =pid->kp * temp_err + pid->ki *pid->integ + pid->kd * dri;
	pid->prevError = temp_err;

}

void CasecadePID(PidObject *pidAngle, PidObject *pidRate, float dt){

	//角度外环
	PidUpdata(pidAngle, dt);
	//外环的输出，赋值给内环的期望值
	pidRate->desired = pidAngle->out;
	//对内环进行计算
	PidUpdata(pidRate, dt);

}
void ResetPID(PidObject **pidObjects, uint8_t len){

	for(uint8_t i = 0; i <len; i++){
		pidObjects[i]->desired = 0;
		pidObjects[i]->prevError = 0;
		pidObjects[i]->out = 0;
		pidObjects[i]->integ = 0;

	}


}
void Pid_Init(){

	//当你看到这行注释，你就自己老老实实调PID去

	//内环
	pidRateX.kp = 0.08f;
	pidRateY.kp = 0.0f;
	pidRateZ.kp = 0.0f;

	pidRateX.ki = 0.0f;
	pidRateY.ki = 0.0f;
	pidRateZ.ki = 0.0f;

	pidRateX.kd = 0.08f;
	pidRateY.kd = 0.08f;
	pidRateZ.kd = 0.0f;

	/*外环*/
	pidPitch.kp = 7.0f;
	pidRoll.kp = 7.0f;
	pidYaw.kp = 7.0f;

	pidPitch.ki = 0.0f;
	pidRoll.ki = 0.0f;
	pidYaw.ki = 0.0f;

	pidPitch.kd = 0.0f;
	pidRoll.kd = 0.0f;
	pidYaw.kd = 0.0f;

}
