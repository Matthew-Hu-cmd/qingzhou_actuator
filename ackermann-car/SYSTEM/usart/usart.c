#include "usart.h"	  
//#include "stm32f10x_usart.h"
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      

	while((USART3->SR&0X40)==0);
	USART3->DR = (u8) ch;      
  return ch;
}
#endif 
u8 Usart3_Receive=0X5A;
void usart1_init(u32 bound)
	{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  
   //USART 初始化设置
//UsartNVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}
	
/**************************************************************************
函数功能：串口1接收中断
入口参数：无
返回  值：无
**************************************************************************/
int USART1_IRQHandler(void)
{	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收到数据
		//if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{	  	
       	
				u8 temp;
				static u8 count,last_data,last_last_data,Usart_ON_Count;
		   
      USART_ClearITPendingBit(USART1,USART_IT_RXNE);	

		    if(Usart_ON_Flag==0)
				{	
		    if(++Usart_ON_Count>10)Usart_ON_Flag=1;
				}
				temp=USART1->DR;
				 if(Usart_Flag==0)
					{	
						if(last_data==0x5a&&last_last_data==0xa5) 
							Usart_Flag=1,count=0;	
					}
							
				 if(Usart_Flag==1)
					{	
						Urxbuf[count]=temp;     
						count++;                
						if(count==8)Usart_Flag=0;
					}
					last_last_data=last_data;
					last_data=temp;
					
					if(USART_GetFlagStatus(USART1,USART_FLAG_ORE) == SET)
					{

						USART_ClearFlag(USART1,USART_FLAG_ORE);
             USART_ReceiveData(USART1);
          }			
		
						
						
   }
return 0;	
}
//////////////////////////////////////////////////////////////////
/**************************实现函数**********************************************
*功    能:		usart3发送一个字节
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}
/**************************************************************************
函数功能：串口3初始化
入口参数： bound:波特率
返回  值：无
**************************************************************************/
void usart3_init(u32 bound)
{  	 
	  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);// 需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能GPIO时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART时钟
		GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);//引脚重映射
	//USART_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //C10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);   
  //USART_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PC11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  //UsartNVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure);     //初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口3 
}

/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
	{	  
	static u8 Flag_PID,i,j,Receive[50];
			static float Data;
  	  Usart3_Receive=USART3->DR; 
			if(Usart3_Receive>=0x41&&Usart3_Receive<=0x48)  
			Flag_Direction=Usart3_Receive-0x40;
			else 	  
			Flag_Direction=0;	
			
				//以下是与APP调试界面通讯
		if(Usart3_Receive==0x7B) Flag_PID=1;   //APP参数指令起始位
		if(Usart3_Receive==0x7D) Flag_PID=2;   //APP参数指令停止位

		 if(Flag_PID==1)  //采集数据
		 {
			Receive[i]=Usart3_Receive;
			i++;
		 }
		 if(Flag_PID==2)  //分析数据
		 {
					 if(Receive[3]==0x50) 	 PID_Send=1;
					 else  if(Receive[1]!=0x23) 
					 {								
						for(j=i;j>=4;j--)
						{
						  Data+=(Receive[j-1]-48)*pow(10,i-j);
						}
						switch(Receive[1])
						 {
							 case 0x30:  RC_Velocity=Data;break;
							 case 0x31:  Velocity_KP=Data;break;
							 case 0x32:  Velocity_KI=Data;break;
							 case 0x33:  break;
							 case 0x34:  break;
							 case 0x35:  break;
							 case 0x36:  break;
							 case 0x37:  break; //预留
							 case 0x38:  break; //预留
						 }
					 }				 
					 Flag_PID=0;//相关标志位清零
					 i=0;
					 j=0;
					 Data=0;
					 memset(Receive, 0, sizeof(u8)*50);//数组清零
		 } 	 			
	}  											 
} 


