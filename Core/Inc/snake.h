#ifndef __SNAKE_H
#define __SNAKE_H
#include "sys.h"

#include "stm32f1xx_hal.h"
#include "stdlib.h"

#pragma diag_suppress 177	//屏蔽变量、函数未使用警告
#pragma diag_suppress 550


#define SNAKE_LENGTH_PIXELS		4		//蛇的其中一节的长度像素（关节是正方形的）
#define SNAKE_INIT_LENGTH		4		//除蛇头外蛇身初始化的关节数量
#define SNAKE_MOVE_DISTANCE		SNAKE_LENGTH_PIXELS		//蛇每走一步的长度，必须为蛇一节长度的整数倍

#define SNAKE_UP				1						//蛇头方向，依次为上下左右
#define SNAKE_DOWN				2
#define SNAKE_LEFT				3
#define SNAKE_RIGHT				4


#define BORDER_LIMIT_UP			16							//边界上限位
#define BORDER_LIMIT_DOWN		0		//边界下限位（屏幕长-）（320）
#define BORDER_LIMIT_LEFT		0							//边界左限位
#define BORDER_LIMIT_RIGHT		32		//边界右限位（屏幕宽-）（240）
#define BORDER_WIDTH			0							//边界宽度

#define FOOD_LENGTH_PIXELS		SNAKE_LENGTH_PIXELS		//食物的宽度


#define	Snake_FOOD				GENERAL_TIM_GetCapturex(GENERAL_TIM)		//食物坐标随机种子，默认种子为：读取定时器的值


//蛇的位置信息结构体
typedef struct
{
	u8  sHeadDir;		//蛇头朝向（1上 2下 3左 4右）
	u8  sEndDir;		//蛇尾朝向（1上 2下 3左 4右）【暂时没用到】
	u8  sLength;		//蛇的关节数量
	u8  sLife;			//蛇的生命（0死 1活）
	u16 sHeadx;			//蛇头的坐标值
	u16 sHeady;			//蛇头的坐标值
	u16 sEndx;			//蛇尾的坐标值
	u16 sEndy;			//蛇尾的坐标值

}_snake_Info;
extern _snake_Info 	snake_Info;
extern _snake_Info 	snake_Info_leader;

//游戏边界结构体
typedef struct
{
	u16 xUpLimit;		//上限位
	u16 xDownLimit;		//下限位
	u16 yLeftLimit;		//左限位
	u16 yRightLimit;	//右限位

}_border_limit;
extern _border_limit 	border_limit;



//食物结构体
typedef struct
{
	u16 xFood;
	u16 yFood;
	u16 validity;		//食物坐标的合法性（0不合法， 1合法）

}_food_Info;
extern _food_Info 	food_Info;





//static void Delay_ms( u32 ms );
void Snake_Draw_Erase_OneJoint( u16 x, u16 y,u8 t);
void Snake_Draw_Erase(u16 x,u16 y,u8 dir,u8 t);
void Snake_Update_HeadXY( u8 i );
void Snake_Update_EndXY( u8 i );
void Snake_Update_BodyXY(void);
void Snake_Display_Init(void);
//static void Border_Display_Init(void);

//static u8 Snake_HTW(void);
//static u8 Snake_TouchMyself(void);
u8 Snake_Move(void);
void Create_Food(void);

void Snake_Game(void);
void Snake_Move_Demo(void);
void Snake_Move_Encoder(int Snake_Dirs);


#endif
