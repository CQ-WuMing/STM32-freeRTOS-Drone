
#include "data.h"

struct _Rc rc;
struct _Filter Filter_Thr, Filter_Pitch, Filter_Roll, Filter_Yaw;
struct _Offset offset;
extern uint16_t ADC_Value[5];

//滑动窗口滤波
void Window_Filter(struct _Rc *rc){

	static uint16_t count = 0;

	//丢掉旧数据
	Filter_Thr.sum -= Filter_Thr.old[count];
	Filter_Pitch.sum -= Filter_Pitch.old[count];
	Filter_Roll.sum -= Filter_Roll.old[count];
	Filter_Yaw.sum -= Filter_Yaw.old[count];


	//加上新数据,新数据要加到sum里，也要存到old里
	Filter_Thr.old[count] = rc->THR;
	Filter_Pitch.old[count] = rc->PIT;
	Filter_Roll.old[count] = rc->ROL;
	Filter_Yaw.old[count] = rc->YAW;


	Filter_Thr.sum += Filter_Thr.old[count];
	Filter_Pitch.sum += Filter_Pitch.old[count];
	Filter_Roll.sum += Filter_Roll.old[count];
	Filter_Yaw.sum += Filter_Yaw.old[count];

	//求平均值
	rc->THR = Filter_Thr.sum  / Filter_Num;
	rc->PIT = Filter_Pitch.sum  / Filter_Num;
	rc->ROL = Filter_Roll.sum  / Filter_Num;
	rc->YAW = Filter_Yaw.sum  / Filter_Num;

	//判断索引值的范围
	count++;
	if(count >= Filter_Num){
		count = 0;
	}
}

//长按key1 中点校准函数
void Mid_Offset(struct _Rc *rc){
	static uint16_t sum_thr = 0,  sum_pit = 0,  sum_roll = 0,  sum_yaw = 0, count = 0;
	//判断按键按下,内部上拉模式
	if(Read_Key == GPIO_PIN_RESET){
		HAL_Delay(1000);//长按1秒
		if(Read_Key == GPIO_PIN_RESET){
			//长按1秒，开始校准中点。偏差值=50次的和 / 50  - 1500
			if(count == 0){
				sum_thr = 0;
				sum_pit = 0;
				sum_roll = 0;
				sum_yaw = 0;
				offset.PIT = 0;
				offset.THR = 0;
				offset.ROL = 0;
				offset.YAW = 0;

				count = 1;
			}else{
				//累加、计数
				count ++;
				sum_thr += rc->THR;
				sum_pit += rc->PIT;
				sum_roll +=rc->ROL;
				sum_yaw += rc->YAW;
			}
			if(count ==51){
				count --;
				offset.THR = sum_thr / count  -1000;
				offset.PIT   = sum_pit / count  -1500;
				offset.ROL = sum_roll / count  -1500;
				offset.YAW = sum_yaw / count  -1500;
				count = 0;
			}
		}

	}

}
//限幅的函数
void Limit(struct _Rc *rc){
	rc->THR = (rc ->THR < 1000) ? 1000 : rc->THR;
	rc->THR = (rc ->THR > 2000) ? 2000 : rc->THR;

	rc->PIT = (rc ->PIT < 1000) ? 1000 : rc->PIT;
	rc->PIT = (rc ->PIT > 2000) ? 2000 : rc->PIT;

	rc->ROL = (rc ->ROL < 1000) ? 1000 : rc->ROL;
	rc->ROL = (rc ->ROL > 2000) ? 2000 : rc->ROL;

	rc->YAW = (rc ->YAW < 1000) ? 1000 : rc->YAW;
	rc->YAW = (rc ->YAW > 2000) ? 2000 : rc->YAW;

}
//中点限幅
void Mid_Limit(struct _Rc *rc){
	if(rc->THR >1450 && rc->THR <1550){

		rc->THR = 1500;
	}

	if(rc->PIT >1450 && rc->PIT <1550){

		rc->PIT = 1500;
	}

	if(rc->ROL >1450 && rc->ROL <1550){

		rc->ROL = 1500;
	}

	if(rc->YAW >1450 && rc->YAW <1550){

		rc->YAW = 1500;
	}

}
//按键微调
void KeyPress(){

	if(Read_Key_U  == GPIO_PIN_RESET ){
		//微调PIT,前进
		offset.PIT = offset.PIT  - 10;
	}
	if(Read_Key_D  == GPIO_PIN_RESET){
		//微调PIT,前进
		offset.PIT = offset.PIT  + 10;
	}
	if(Read_Key_L ==GPIO_PIN_RESET ){
		//左移，加
		offset.ROL = offset.ROL + 10;
	}
	if(Read_Key_R ==GPIO_PIN_RESET ){
		//右移，加
		offset.ROL = offset.ROL - 10;
	}
}

//封装数据
void RemoteData(uint8_t *remote_send){
	uint8_t index = 0;
	uint32_t check_sum = 0;
	//帧头
	remote_send[index++] = 0xAA;
	remote_send[index++] = 0xAF;
	//功能字
	remote_send[index++] = 0x03;

	//长度
	remote_send[index++] = 0x0;

	//数据
	remote_send[index++] = (uint8_t)(rc.THR >>8);
	remote_send[index++] = (uint8_t)rc.THR;

	remote_send[index++] = (uint8_t)(rc.YAW >>8);
	remote_send[index++] = (uint8_t)rc.YAW;

	remote_send[index++] = (uint8_t)(rc.ROL >>8);
	remote_send[index++] = (uint8_t)rc.ROL;

	remote_send[index++] = (uint8_t)(rc.PIT >>8);
	remote_send[index++] = (uint8_t)rc.PIT;

	//辅助通道，12个字节
	remote_send[index++] = 0x0;
	remote_send[index++] = 0x0;
	remote_send[index++] = 0x0;
	remote_send[index++] = 0x0;
	remote_send[index++] = 0x0;
	remote_send[index++] = 0x0;
	remote_send[index++] = 0x0;
	remote_send[index++] = 0x0;
	remote_send[index++] = 0x0;
	remote_send[index++] = 0x0;
	remote_send[index++] = 0x0;
	remote_send[index++] = 0x0;

	//重新计算数据长度
	remote_send[3] = index - 8;

	//校验和
	for(uint8_t i = 0; i < index; i++){
		check_sum +=remote_send[i];
	}
	remote_send[index++] =(uint8_t) (check_sum >> 24);
	remote_send[index++] =(uint8_t) (check_sum >> 16);
	remote_send[index++] =(uint8_t) (check_sum >> 8);
	remote_send[index++] =(uint8_t) (check_sum);



}
//摇杆扫描
void Stick_Scan(){

	//将ADC的值转为 1000-2000的范围，处理正负

	rc.THR = 2000 - (uint16_t)0.25 *ADC_Value[1] - offset.THR;
	rc.PIT = 2000 - (uint16_t)0.25 *ADC_Value[3] - offset.PIT;
	rc.ROL = 2000 - (uint16_t)0.25 *ADC_Value[2] -offset.ROL;
	rc.YAW = 2000 - (uint16_t)0.25 *ADC_Value[0] - offset.YAW;

	Window_Filter(&rc);//调用滤波函数

	Mid_Offset(&rc);	//中点校准
	Limit(&rc);//最大最小限幅
	Mid_Limit(&rc);//中点限幅
}
