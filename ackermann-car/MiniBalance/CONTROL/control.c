#include "control.h"	
#include "filter.h"	

u8 Flag_Target,Flag_Change;  //相关标志位
float Voltage_Count,Voltage_All;  //电压采样相关变量
int j,sum;
#define T 0.245f
#define L 0.29f
#define K 14.00f
/**************************************************************************
函数功能：小车运动数学模型
入口参数：速度和转角
返回  值：无
**************************************************************************/
void Kinematic_Analysis(float velocity,float angle) //
{
	Servo=SERVO_INIT+angle*K; //舵机转向   angle*
	if(Servo > 1890){
		Servo = 1890;
		angle = (double)(Servo - SERVO_INIT)/K;
	}
	else if(Servo < 1280){
		Servo = 1280;
		angle = (double)(Servo - SERVO_INIT)/K;
	}
	
  Tand = tan(angle/57.3);;//(int)tan(angle);
	Target_Left=-velocity*(1-T*Tand/2/L); 
	Target_Right=velocity*(1+T*Tand/2/L);      //后轮差速
	Servo=SERVO_INIT+angle*K; //舵机转向   
}
/**************************************************************************
函数功能：所有的控制代码都在这里面
         定时中断触发
         严格保证采样和数据处理的时间同步				 
**************************************************************************/
void TIM6_IRQHandler(void)   //TIM6中断
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) 	
	{     	
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);          //清除中断标志位  	
			Flag_Target=!Flag_Target; //分频标志位
			if(delay_flag==1)
			{
					if(++delay_50==2)	 delay_50=0,delay_flag=0; //给主函数提供50ms的精准延时
			}
		  if(Flag_Target==1)
			{
					if(Usart_Flag==0&&Usart_ON_Flag==1)  memcpy(rxbuf,Urxbuf,8*sizeof(u8));	//如果解锁了串口控制标志位，读取相关的指令					 
			  	//Key();//扫描按键变化	
					Voltage_All+=Get_battery_volt();     //多次采样累积
			    if(++Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//求平均值 获取电池电压	   									                                        
			}else if(Flag_Target == 0){     
					Encoder_Right=Read_Encoder(3);  //===读取编码器的值
					Encoder_Left=Read_Encoder(2);    //===读取编码器的值
					Get_RC();   //===接收控制指令
		//			if(Turn_Off(Voltage)==0)   //===如果电池电压不存在异常
		//			{ 	
						 Kinematic_Analysis(Velocity,-Angle); 	//小车运动学分析   
						 Motor_Left=Incremental_PI_Left(Encoder_Left,Target_Left);  //===速度闭环控制计算左电机最终PWM   Motor_Left
						 Motor_Right=Incremental_PI_Right(Encoder_Right,Target_Right);  //===速度闭环控制计算右电机最终PWM  Motor_Right
						 Xianfu_Pwm(6900);                          //===PWM限幅
						 Set_Pwm(Motor_Left,Motor_Right,Servo);     //===赋值给PWM寄存器  
						 
		//			}
			//		else	Set_Pwm(0,0,SERVO_INIT);    //===赋值给PWM寄存器  	
			}
 }
	// return 0;	 //返回值                                                                                                                                                                                                   
} 
/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int servo)
{
	  	if(motor_a>0)			PWMA1=7200,PWMA2=7200-motor_a;
			else 	            PWMA2=7200,PWMA1=7200+motor_a;
		
		  if(motor_b>0)			PWMB1=7200,PWMB2=7200-motor_b;
			else 	            PWMB2=7200,PWMB1=7200+motor_b;
	   SERVO=servo;	
}
/**************************************************************************
函数功能：限制PWM赋值 
入口参数：幅值
返回  值：无
**************************************************************************/
void Xianfu_Pwm(int amplitude)
{	
    if(Motor_Left<-amplitude) Motor_Left=-amplitude;	//限制最小值
		if(Motor_Left>amplitude)  Motor_Left=amplitude;	  //限制最大值
	  if(Motor_Right<-amplitude) Motor_Right=-amplitude;//限制最小值	
		if(Motor_Right>amplitude)  Motor_Right=amplitude;	//限制最大值		
}
/************************************************************************
函数功能：按键修改小车运行状态 
入口参数：无
返回  值：无
**************************************************************************/
void Key(void)
{	
	u8 tmp,tmp2;
	tmp=click_N_Double(50); //双击，双击等待时间500ms
	if(tmp==1)Flag_Stop=!Flag_Stop;//单击控制小车的启停
	if(tmp==2)Flag_Show=!Flag_Show;//双击控制小车的显示状态
	tmp2=Long_Press();  //长按        
  if(tmp2==1)Flag_Show=!Flag_Show;//控制小车的显示状态                 
}
/**************************************************************************
函数功能：异常关闭电机
入口参数：电压
返回  值：1：异常  0：正常
**************************************************************************/
u8 Turn_Off( int voltage)
{
	    u8 temp;
			if(voltage<9500||Flag_Stop==1)//电池电压低于9.5V关闭电机
			{	                                                
      temp=1;      
     	PWMA1=0; //电机控制位清零                                           
			PWMB1=0; //电机控制位清零
			PWMA2=0; //电机控制位清零
			PWMB2=0; //电机控制位清零					
      }
			else
      temp=0;
      return temp;			
}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_Left (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_Right (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
/**************************************************************************
函数功能：通过指令对小车进行遥控
入口参数：串口指令
返回  值：无
**************************************************************************/
void Get_RC(void)
{
		int Yuzhi=2;  		//PS2控制防抖阈值
		float LY,RX;  //PS2手柄控制变量
		if(operationMode==0)//串口等控制方式
		{
			if(Usart_Flag==0&&Usart_ON_Flag==1){
				 Angle=-(rxbuf[2]-60);//角度获取
				 Velocity=rxbuf[3];	//速度
			 }
		}
		else	if(operationMode==1)//PS2控制
		{
				LY=PS2_LY - 128;     //计算偏差LY
				RX=PS2_RX - 128; 
				if(LY>-Yuzhi&&LY<Yuzhi)LY=0;   //小角度设为死区 防止抖动出现异常RX
				if(RX>-Yuzhi&&RX<Yuzhi)RX=0;
				Velocity=-(float)LY/4;	  //速度和摇杆的力度相关
				Angle=RX/4; 	
		}	
}

