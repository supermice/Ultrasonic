
/**
  ******************************************************************************
  * @file    delay.h
  * @author  wfh
  * @version V1.0
  * @date    2015.10.27
  * @brief   This file provides the delay function and system interrupt manage function
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
	

#include "LED.h"
#define Single_LED_DIS_TIME_ms 2 //display 2ms per led 

	
static LED_State_t LED_State = R_NUM_State;//_Num = R_NUM ,	LED_State_Angle = R_NUM;
static uint8_t L_NUM = 0, M_NUM = 0, R_NUM = 0;	

#ifdef __cplusplus
extern "C" {
#endif
	
	void LED_GPIO_Config(void);	
	void LED_Angle_Display(const int* AngleToDis);
	void LED_NUM_Display(const float* numToDis);
	
#ifdef __cplusplus
}
#endif



static void Update_NUM_ToDis(const float* numToDis, uint8_t *L_NUM, uint8_t *M_NUM, uint8_t *R_NUM);
static void Update_Angle_ToDis(const int* AngleToDis, uint8_t *L_NUM, uint8_t *M_NUM, uint8_t *R_NUM);
static void Display(const uint8_t *num);
static void update_LED_State(void);

	static void LED_DISPLAY0(void);
	static void LED_DISPLAY1(void);
	static void LED_DISPLAY2(void);
	static void LED_DISPLAY3(void);
	static void LED_DISPLAY4(void);
	static void LED_DISPLAY5(void);
	static void LED_DISPLAY6(void);
	static void LED_DISPLAY7(void);
	static void LED_DISPLAY8(void);
	static void LED_DISPLAY9(void);
	static void LED_DISPLAYA(void);
	static void LED_DISPLAYb(void);
	static void LED_DISPLAYC(void);
	static void LED_DISPLAYd(void);
	static void LED_DISPLAYE(void);
	static void LED_DISPLAYF(void);
	static void LED_DISPLAY_(void);
	static void LED_DISPLAYDOT(void);
	static void LED_DISPLAYNULL(void);//wfh add 15.12.05



static inline void ON_LEFT_LEDNum(void)
{
	GPIO_ResetBits  ( GPIOA,   GPIO_Pin_4);
  //GPIO_SetBits  ( GPIOA, GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15 ); 
//	GPIO_SetBits  ( GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 );
}
static inline void OFF_LEFT_LEDNum(void)
{
	GPIO_SetBits  ( GPIOA,   GPIO_Pin_4);
  //GPIO_SetBits  ( GPIOA, GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15 ); 
//	GPIO_SetBits  ( GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 );
}



static inline void ON_MIDDLE_LEDNum(void)
{
	GPIO_ResetBits  ( GPIOA,   GPIO_Pin_3);
  //GPIO_SetBits  ( GPIOA, GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15 ); 
//	GPIO_SetBits  ( GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 );
}
static inline void OFF_MIDDLE_LEDNum(void)
{
	GPIO_SetBits  ( GPIOA,   GPIO_Pin_3);
  //GPIO_SetBits  ( GPIOA, GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15 ); 
//	GPIO_SetBits  ( GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 );
}



static inline void ON_RIGHT_LEDNum(void)
{
	GPIO_ResetBits  ( GPIOA,   GPIO_Pin_2);
  //GPIO_SetBits  ( GPIOA, GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15 ); 
//	GPIO_SetBits  ( GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 );
}

static inline void OFF_RIGHT_LEDNum(void)
{
	GPIO_SetBits  ( GPIOA,   GPIO_Pin_2);
  //GPIO_SetBits  ( GPIOA, GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_15 ); 
//	GPIO_SetBits  ( GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 );
}


/*****************************************************************************
** 函数名称: SET_GPIO1
** 功能描述: 设置段1亮

*****************************************************************************/

static inline void ON_LED_CODE_1(void)
{
	GPIO_ResetBits  ( GPIOA,   GPIO_Pin_5);
}
static inline void OFF_LED_CODE_1(void)
{
	GPIO_SetBits  ( GPIOA,   GPIO_Pin_5);
}

/*****************************************************************************
** 函数名称: SET_GPIO2
** 功能描述: 设置段2亮

*****************************************************************************/

static inline void ON_LED_CODE_2(void)
{
	GPIO_ResetBits  ( GPIOA,   GPIO_Pin_8);
}
static inline void OFF_LED_CODE_2(void)
{
	GPIO_SetBits  ( GPIOA,   GPIO_Pin_8);
}

/*****************************************************************************
** 函数名称: SET_GPIO3
** 功能描述: 设置段3亮

*****************************************************************************/

 static inline void ON_LED_CODE_3(void)
{
	GPIO_ResetBits  ( GPIOA,   GPIO_Pin_11);
}
 static inline void OFF_LED_CODE_3(void)
{
	GPIO_SetBits  ( GPIOA,   GPIO_Pin_11);
}
/*****************************************************************************
** 函数名称: SET_GPIO4
** 功能描述: 设置段4亮

*****************************************************************************/

static inline void ON_LED_CODE_4(void)
{
	GPIO_ResetBits  ( GPIOA,   GPIO_Pin_12);
}
static inline void OFF_LED_CODE_4(void)
{
	GPIO_SetBits  ( GPIOA,   GPIO_Pin_12);
}

/*****************************************************************************
** 函数名称: SET_GPIO5
** 功能描述: 设置段5亮

*****************************************************************************/

static inline void ON_LED_CODE_5(void)
{
	GPIO_ResetBits  ( GPIOB,   GPIO_Pin_12);
}
static inline void OFF_LED_CODE_5(void)
{
	GPIO_SetBits  ( GPIOB,   GPIO_Pin_12);
}

/*****************************************************************************
** 函数名称: SET_GPIO7
** 功能描述: 设置段7亮

*****************************************************************************/

static inline void ON_LED_CODE_7(void)
{
	GPIO_ResetBits  ( GPIOB,   GPIO_Pin_13);
}
static inline void OFF_LED_CODE_7(void)
{
	GPIO_ResetBits  ( GPIOB,   GPIO_Pin_13);
}

/*****************************************************************************
** 函数名称: SET_GPIO10
** 功能描述: 设置段10亮

*****************************************************************************/

static inline void ON_LED_CODE_10(void)
{
	GPIO_ResetBits  ( GPIOB,   GPIO_Pin_14);
}
static inline void OFF_LED_CODE_10(void)
{
	GPIO_SetBits  ( GPIOB,   GPIO_Pin_14);
}

/*****************************************************************************
** 函数名称: SET_GPIO11
** 功能描述: 设置段11亮

*****************************************************************************/

static inline void ON_LED_CODE_11(void)
{
	GPIO_ResetBits  ( GPIOB,   GPIO_Pin_15);
}
static inline void OFF_LED_CODE_11(void)
{
	GPIO_SetBits  ( GPIOB,   GPIO_Pin_15);
}






/****************************************************************************
*****************************************************************************
*****************************************************************************
** 函数名称: LED_DISPLAY0
** 功能描述: LED显示数字0

*****************************************************************************/

static void LED_DISPLAY0(void)
{
	ON_LED_CODE_1();
	ON_LED_CODE_2();
//	SET_GPIO3();
	ON_LED_CODE_4();
//	SET_GPIO5();
	ON_LED_CODE_7();
	ON_LED_CODE_10();
	ON_LED_CODE_11();
	GPIO_SetBits( GPIOB, GPIO_Pin_12);
	GPIO_SetBits( GPIOA, GPIO_Pin_12);  
}

/*****************************************************************************
** 函数名称: LED_DISPLAY1
** 功能描述: LED显示数字1

*****************************************************************************/

static void LED_DISPLAY1(void)
{
//	SET_GPIO1();
//	SET_GPIO2();
//  SET_GPIO3();
	ON_LED_CODE_4();
//	SET_GPIO5();
	ON_LED_CODE_7();
//	SET_GPIO10();
//	SET_GPIO11();
	
	GPIO_SetBits( GPIOB, GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_SetBits( GPIOA, GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12); 
}
	
/*****************************************************************************
** 函数名称: LED_DISPLAY2
** 功能描述: LED显示数字2

*****************************************************************************/

static void LED_DISPLAY2(void)
{
	ON_LED_CODE_1();
	ON_LED_CODE_2();
//  SET_GPIO3();
//	SET_GPIO4();
	ON_LED_CODE_5();
	ON_LED_CODE_7();
//	SET_GPIO10();
	ON_LED_CODE_11();
	
	GPIO_SetBits( GPIOB, GPIO_Pin_14);
	GPIO_SetBits( GPIOA, GPIO_Pin_12 | GPIO_Pin_5 ); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAY3
** 功能描述: LED显示数字3

*****************************************************************************/

static void LED_DISPLAY3(void)
{
//	SET_GPIO1();
	ON_LED_CODE_2();
//  SET_GPIO3();
	ON_LED_CODE_4();
	ON_LED_CODE_5();
	ON_LED_CODE_7();
//	SET_GPIO10();
	ON_LED_CODE_11();
	
	GPIO_SetBits( GPIOB, GPIO_Pin_14);
	GPIO_SetBits( GPIOA, GPIO_Pin_8 | GPIO_Pin_12); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAY4
** 功能描述: LED显示数字4

*****************************************************************************/

static void LED_DISPLAY4(void)
{
//	SET_GPIO1();
//	SET_GPIO2();
//  SET_GPIO3();
	ON_LED_CODE_4();
	ON_LED_CODE_5();
	ON_LED_CODE_7();
	ON_LED_CODE_10();
//	SET_GPIO11();
	
	GPIO_SetBits( GPIOB, GPIO_Pin_15);
	GPIO_SetBits( GPIOA, GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAY5
** 功能描述: LED显示数字5

*****************************************************************************/

static void LED_DISPLAY5(void)
{
//	SET_GPIO1();
	ON_LED_CODE_2();
//  SET_GPIO3();
	ON_LED_CODE_4();
	ON_LED_CODE_5();
//	SET_GPIO7();
	ON_LED_CODE_10();
	ON_LED_CODE_11();
	
	GPIO_SetBits( GPIOB, GPIO_Pin_13 );
	GPIO_SetBits( GPIOA, GPIO_Pin_8 | GPIO_Pin_12); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAY6
** 功能描述: LED显示数字6

*****************************************************************************/

static void LED_DISPLAY6(void)
{
	ON_LED_CODE_1();
	ON_LED_CODE_2();
//  SET_GPIO3();
	ON_LED_CODE_4();
	ON_LED_CODE_5();
//	SET_GPIO7();
	ON_LED_CODE_10();
	ON_LED_CODE_11();
	
	GPIO_SetBits( GPIOB, GPIO_Pin_13);
	GPIO_SetBits( GPIOA, GPIO_Pin_12); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAY7
** 功能描述: LED显示数字7

*****************************************************************************/

static void LED_DISPLAY7(void)
{
//	SET_GPIO1();
//	SET_GPIO2();
//  SET_GPIO3();
	ON_LED_CODE_4();
//	SET_GPIO5();
	ON_LED_CODE_7();
//	SET_GPIO10();
	ON_LED_CODE_11();
	
	GPIO_SetBits( GPIOB, GPIO_Pin_14 );
	GPIO_SetBits( GPIOA, GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAY8
** 功能描述: LED显示数字8

*****************************************************************************/

static void LED_DISPLAY8(void)
{
	ON_LED_CODE_1();
	ON_LED_CODE_2();
//  SET_GPIO3();
	ON_LED_CODE_4();
	ON_LED_CODE_5();
	ON_LED_CODE_7();
	ON_LED_CODE_10();
	ON_LED_CODE_11();
	
	//GPIO_SetBits( GPIOB, GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_SetBits( GPIOA, GPIO_Pin_12); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAY9
** 功能描述: LED显示数字9

*****************************************************************************/

static void LED_DISPLAY9(void)
{
//	SET_GPIO1();
	ON_LED_CODE_2();
//  SET_GPIO3();
	ON_LED_CODE_4();
	ON_LED_CODE_5();
	ON_LED_CODE_7();
	ON_LED_CODE_10();
	ON_LED_CODE_11();
	
	//GPIO_SetBits( GPIOB, GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_SetBits( GPIOA, GPIO_Pin_8 | GPIO_Pin_12); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAYA
** 功能描述: LED显示字符 A

*****************************************************************************/

static void LED_DISPLAYA(void)
{
	ON_LED_CODE_1();
//	SET_GPIO2();
//  SET_GPIO3();
	ON_LED_CODE_4();
	ON_LED_CODE_5();
	ON_LED_CODE_7();
	ON_LED_CODE_10();
	ON_LED_CODE_11();
	
	//GPIO_SetBits( GPIOB, GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_SetBits( GPIOA,  GPIO_Pin_11 | GPIO_Pin_12); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAYb
** 功能描述: LED显示字符 b

*****************************************************************************/

static void LED_DISPLAYb(void)
{
	ON_LED_CODE_1();
	ON_LED_CODE_2();
//  SET_GPIO3();
	ON_LED_CODE_4();
	ON_LED_CODE_5();
//	SET_GPIO7();
	ON_LED_CODE_10();
//	SET_GPIO11();
	
	GPIO_SetBits( GPIOB, GPIO_Pin_13 | GPIO_Pin_15);
	GPIO_SetBits( GPIOA, GPIO_Pin_12); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAYC
** 功能描述: LED显示字符 C

*****************************************************************************/

static void LED_DISPLAYC(void)
{
	ON_LED_CODE_1();
  ON_LED_CODE_2();
//  SET_GPIO3();
//	SET_GPIO4();
//	SET_GPIO5();
//	SET_GPIO7();
	ON_LED_CODE_10();
	ON_LED_CODE_11();
	
	GPIO_SetBits( GPIOB, GPIO_Pin_12 | GPIO_Pin_13);
	GPIO_SetBits( GPIOA, GPIO_Pin_12 | GPIO_Pin_5); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAYd
** 功能描述: LED显示字符 d

*****************************************************************************/

static void LED_DISPLAYd(void)
{
	ON_LED_CODE_1();
  ON_LED_CODE_2();
//  SET_GPIO3();
	ON_LED_CODE_4();
	ON_LED_CODE_5();
	ON_LED_CODE_7();
//	SET_GPIO10();
//	SET_GPIO11();
	
	GPIO_SetBits( GPIOB, GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_SetBits( GPIOA, GPIO_Pin_12); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAYE
** 功能描述: LED显示字符 E

*****************************************************************************/

static void LED_DISPLAYE(void)
{
	ON_LED_CODE_1();
  ON_LED_CODE_2();
//  SET_GPIO3();
//	SET_GPIO4();
	ON_LED_CODE_5();
//	SET_GPIO7();
	ON_LED_CODE_10();
	ON_LED_CODE_11();
	
	GPIO_SetBits( GPIOB,GPIO_Pin_13);
	GPIO_SetBits( GPIOA, GPIO_Pin_12 | GPIO_Pin_5); 
}

/*****************************************************************************
** 函数名称: LED_DISPLAYF
** 功能描述: LED显示字符 F

*****************************************************************************/

static void LED_DISPLAYF(void)
{
	ON_LED_CODE_1();
//  SET_GPIO2();
//  SET_GPIO3();
//	SET_GPIO4();
	ON_LED_CODE_5();
//	SET_GPIO7();
	ON_LED_CODE_10();
	ON_LED_CODE_11();
	
	GPIO_SetBits( GPIOB,GPIO_Pin_13);
	GPIO_SetBits( GPIOA, GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_5);
}

/*****************************************************************************
** 函数名称: LED_DISPLAY-
** 功能描述: LED显示字符—

*****************************************************************************/

static void LED_DISPLAY_(void)
{
//	SET_GPIO1();
//  SET_GPIO2();
//  SET_GPIO3();
//	SET_GPIO4();
	ON_LED_CODE_5();
//	SET_GPIO7();
//	SET_GPIO10();
//	SET_GPIO11();
	
	GPIO_SetBits( GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	GPIO_SetBits( GPIOA, GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_5);
	
}

/*****************************************************************************
** 函数名称: LED_DISPLAYDOT
** 功能描述: LED显示字符 .

*****************************************************************************/
static void LED_DISPLAYDOT(void)
{
   ON_LED_CODE_3();
}

/*****************************************************************************
** 函数名称: LED_DISPLAYNULL
** 功能描述: LED不显示任何字符 .

*****************************************************************************/
static void LED_DISPLAYNULL(void)
{
	OFF_LED_CODE_1();
	OFF_LED_CODE_2();
	OFF_LED_CODE_3();
	OFF_LED_CODE_4();
	OFF_LED_CODE_5();
	OFF_LED_CODE_7();
	OFF_LED_CODE_10();
	OFF_LED_CODE_11();
}

	/*****************************************************************************
** 函数名称: LED_GPIO_Config
** 功能描述: 初始化GPIO口

*****************************************************************************/

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 使能GPIOA 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
 	/*设置位选IO口为 PA.2 PA.3 PA.4 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 ;
	/*GPIO设置为复用推挽输出*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_SetBits(GPIOA, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/*设置段选IO口为 PA.5 PA.8  PA.11  PA.12  */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8| GPIO_Pin_11 | GPIO_Pin_12;
	/*GPIO设置为复用开漏输出*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_8| GPIO_Pin_11 | GPIO_Pin_12);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*设置段选IO口为PB12  PB13  PB14  PB15*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	/*GPIO设置为复用开漏输出*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_12| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	
}


void LED_NUM_Display(const float* numToDis){
	
	static uint32_t last_time_ms = 0;
	static float _numToDis ;	_numToDis = *numToDis;//buffer to record the data in case we miss the data
	uint32_t now_time_ms = stm32_get_ms();
	
	
	if(last_time_ms == 0 || now_time_ms - last_time_ms >= Single_LED_DIS_TIME_ms){
		last_time_ms = now_time_ms;
		if(LED_State == R_NUM_State){//Only we have finished display the last num ,can we updata the num to display
			Update_NUM_ToDis(&_numToDis, &L_NUM, &M_NUM, &R_NUM);
		}		
		
		update_LED_State();
	}
	
	
	switch(LED_State){
		case 0:	ON_LEFT_LEDNum(); //Choose the Left num
						OFF_MIDDLE_LEDNum();
						OFF_RIGHT_LEDNum();
						Display(&L_NUM); 
						LED_DISPLAYDOT();break;//LED_DISPLAYDOT should be called after Display
		
		case 1:	ON_MIDDLE_LEDNum(); //Choose the Middle num
						OFF_LEFT_LEDNum();
						OFF_RIGHT_LEDNum();
						Display(&M_NUM);break;
		
		case 2: ON_RIGHT_LEDNum(); //Choose the Right num
						OFF_LEFT_LEDNum();
						OFF_MIDDLE_LEDNum();
						Display(&R_NUM);break;
		default : break;
	}
	
	
}

// static void update_LED_State_Num(void){
// 	
// 	if(LED_State_Num >= 0){
// 		if(LED_State_Num < 2){
// 			LED_State_Num++;
// 		}
// 		else{
// 			LED_State_Num = L_NUM;
// 		}
// 	}
// 	
// 	return;
// }

static void Update_NUM_ToDis(const float* numToDis, uint8_t *L_NUM, uint8_t *M_NUM, uint8_t *R_NUM){
	
		uint16_t NUM = 0;
		NUM = (uint16_t)(*numToDis * 100);
	
	/*Normal data display*/
		if(*numToDis >= 0.01 && *numToDis <= 9.99){	//Valid Ultrasonic data value
		NUM = (uint16_t)(*numToDis * 100);
		*R_NUM = NUM % 10; 
		*M_NUM = NUM / 10 % 10;
		*L_NUM = NUM /100 % 10;		
	}//if(*numToDis > 0.01 && *numToDis <= 9.99)
	
	/*Turn OFF the led*/
		if(*numToDis == -99 ){
		*R_NUM = 0xFF; //That means to display Nothing
		*M_NUM = 0xFF;
		*L_NUM = 0xFF;		
	}
	
	/*Error status display*/
	if(*numToDis == -1 ){//ERROR 1
		*R_NUM = 1; 
		*M_NUM = 0;
		*L_NUM = 0x0E;		
	}
	
	if(*numToDis == -2 ){//ERROR 2
		*R_NUM = 2; 
		*M_NUM = 0;
		*L_NUM = 0x0E;		
	}
	
	if(*numToDis == -3 ){//ERROR 3
		*R_NUM = 3; 
		*M_NUM = 0;
		*L_NUM = 0x0E;		
	}
	
	if(*numToDis == 0 ){//Outof range '-'
		*R_NUM = 0x10; //That means to display '-'
		*M_NUM = 0x10;
		*L_NUM = 0x10;		
	}
	

	
}

static void Display(const uint8_t *num){
	switch(*num){
		case 0: LED_DISPLAY0();break;
		case 1: LED_DISPLAY1();break;
		case 2: LED_DISPLAY2();break;
		case 3: LED_DISPLAY3();break;
		case 4: LED_DISPLAY4();break;
		case 5: LED_DISPLAY5();break;
		case 6: LED_DISPLAY6();break;
		case 7: LED_DISPLAY7();break;
		case 8: LED_DISPLAY8();break;
		case 9: LED_DISPLAY9();break;
		
		
		case 0x0A: LED_DISPLAYA();break;//'A'
		case 0x0b: LED_DISPLAYb();break;//'b'
		case 0x0C: LED_DISPLAYC();break;//'C'
		case 0x0d: LED_DISPLAYd();break;//'d'
		case 0x0E: LED_DISPLAYE();break;//'E'
		case 0x0F: LED_DISPLAYF();break;//'F'
		case 0x10: LED_DISPLAY_();break;//'-'
		
		case 0xFF: LED_DISPLAYNULL();break;//Display nothing
		
		default : break;
	}
}


void LED_Angle_Display(const int* AngleToDis){//Display Angle data resolution 1 degree
	//static uint8_t L_NUM = 0, M_NUM = 0, R_NUM = 0;	
	static uint32_t last_time_ms = 0;
	static int _AngleToDis = 0;	_AngleToDis = *AngleToDis;//buffer to record the data in case we miss the data
	uint32_t now_time_ms = stm32_get_ms();
	
	
	if(last_time_ms == 0 || now_time_ms - last_time_ms >= Single_LED_DIS_TIME_ms){
		last_time_ms = now_time_ms;
		if(LED_State == R_NUM_State){//Only we have finished display the last num ,can we updata the num to display
			Update_Angle_ToDis(&_AngleToDis, &L_NUM, &M_NUM, &R_NUM);
		}		
		
		update_LED_State();
	}
	
		switch(LED_State){
		case 0:	ON_LEFT_LEDNum(); //Choose the Left num
						OFF_MIDDLE_LEDNum();
						OFF_RIGHT_LEDNum();
						Display(&L_NUM); break;//We don't need call LED_DISPLAYDOT here for angle display
		
		case 1:	ON_MIDDLE_LEDNum(); //Choose the Middle num
						OFF_LEFT_LEDNum();
						OFF_RIGHT_LEDNum();
						Display(&M_NUM);break;
		
		case 2: ON_RIGHT_LEDNum(); //Choose the Right num
						OFF_LEFT_LEDNum();
						OFF_MIDDLE_LEDNum();
						Display(&R_NUM);break;
		default : break;
	}
		
	
}


static void Update_Angle_ToDis(const int* AngleToDis, uint8_t *L_NUM, uint8_t *M_NUM, uint8_t *R_NUM){
		/*Normal data display*/
		if(*AngleToDis >= -90 && *AngleToDis < 0){//negative angle value
			*R_NUM = *AngleToDis % 10; 
			*M_NUM = *AngleToDis / 10 % 10;
			*L_NUM = 0x10; //That means to display '-'
		}
		
		if(*AngleToDis >= 0 && *AngleToDis <= 90){//positive angle value
			*R_NUM = *AngleToDis % 10; 
			*M_NUM = *AngleToDis / 10 % 10;
			*L_NUM = 0xFF; //That means to display Nothing
		}
			
		/*Turn OFF the led*/
		if(*AngleToDis == -99 ){
			*R_NUM = 0xFF; //That means to display Nothing
			*M_NUM = 0xFF;
			*L_NUM = 0xFF;		
		}
}

static void update_LED_State(void){
	
	if(LED_State >= 0){
		if(LED_State < 2){
			LED_State++;
		}
		else{
			LED_State = L_NUM_State;
		}
	}
	
	return;
}

/*********************************
 *@name
 *@descript
 *@author wfh
 *@param  numToDis the float number to display 
 *				Frequence can be 1-10
 *				Dutycycle can be 0-100 for light_time
**********************************/
void LED_NUM_Blink(const float* numToDis, uint8_t Frequence, uint8_t Dutycycle){//Dutycycle can be 0-100 for light_time
	static uint32_t last_time_ms = 0;
	//static uint16_t ON_Time = 0;
	uint32_t now_time_ms = stm32_get_ms();
	
	const float Turn_OFF_LED = -99.0;//This vlaue will turn off LEDs
	if(Frequence < 1 || Frequence > 10 || Dutycycle < 0 || Dutycycle > 100){
		return ;
	}
	
	if(now_time_ms - last_time_ms > 1000/Frequence + 1){//We havn't call this function for sometime or We increase the Frequence 
		last_time_ms = now_time_ms;
	}
	
	if(now_time_ms - last_time_ms <= Dutycycle*0.01*(1000/Frequence)){		
		LED_NUM_Display(numToDis);
	}
	else if(now_time_ms - last_time_ms < (1000/Frequence)){
		LED_NUM_Display(&Turn_OFF_LED);
	}
	else {
		last_time_ms = stm32_get_ms();
	}
	
}

/*********************************
 *@name
 *@descript
 *@author wfh
 *@param  numToDis the angle degree number to display 
 *				Frequence can be 1-10
 *				Dutycycle can be 0-100 for light_time
**********************************/
void LED_Angle_Blink(const int* AngleToDis, uint8_t Frequence, uint8_t Dutycycle){//Dutycycle can be 0-100 for light_time
	static uint32_t last_time_ms = 0;
//	static uint16_t ON_Time = 0;
	uint32_t now_time_ms = stm32_get_ms();
	
	const int Turn_OFF_LED = -99;//This vlaue will turn off LEDs
	if(Frequence < 1 || Frequence > 10 || Dutycycle < 0 || Dutycycle > 100){
		return ;
	}
	
	if(now_time_ms - last_time_ms > 1000/Frequence + 1){//We havn't call this function for sometime
		last_time_ms = now_time_ms;
	}
	/*Display LED ,follow the Frequence and Dutycycle*/
	if(now_time_ms - last_time_ms <= Dutycycle*0.01*(1000/Frequence)){		
		LED_Angle_Display(AngleToDis);
	}
	else if(now_time_ms - last_time_ms < (1000/Frequence)){
		LED_Angle_Display(&Turn_OFF_LED);
	}
	else {
		last_time_ms = stm32_get_ms();
	}
	
}


