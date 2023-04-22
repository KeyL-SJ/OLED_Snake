#ifndef __SNAKE_H
#define __SNAKE_H
#include "sys.h"

#include "stm32f1xx_hal.h"
#include "stdlib.h"

#pragma diag_suppress 177	//���α���������δʹ�þ���
#pragma diag_suppress 550


#define SNAKE_LENGTH_PIXELS		4		//�ߵ�����һ�ڵĳ������أ��ؽ��������εģ�
#define SNAKE_INIT_LENGTH		4		//����ͷ�������ʼ���Ĺؽ�����
#define SNAKE_MOVE_DISTANCE		SNAKE_LENGTH_PIXELS		//��ÿ��һ���ĳ��ȣ�����Ϊ��һ�ڳ��ȵ�������

#define SNAKE_UP				1						//��ͷ��������Ϊ��������
#define SNAKE_DOWN				2
#define SNAKE_LEFT				3
#define SNAKE_RIGHT				4


#define BORDER_LIMIT_UP			16							//�߽�����λ
#define BORDER_LIMIT_DOWN		0		//�߽�����λ����Ļ��-����320��
#define BORDER_LIMIT_LEFT		0							//�߽�����λ
#define BORDER_LIMIT_RIGHT		32		//�߽�����λ����Ļ��-����240��
#define BORDER_WIDTH			0							//�߽���

#define FOOD_LENGTH_PIXELS		SNAKE_LENGTH_PIXELS		//ʳ��Ŀ��


#define	Snake_FOOD				GENERAL_TIM_GetCapturex(GENERAL_TIM)		//ʳ������������ӣ�Ĭ������Ϊ����ȡ��ʱ����ֵ


//�ߵ�λ����Ϣ�ṹ��
typedef struct
{
	u8  sHeadDir;		//��ͷ����1�� 2�� 3�� 4�ң�
	u8  sEndDir;		//��β����1�� 2�� 3�� 4�ң�����ʱû�õ���
	u8  sLength;		//�ߵĹؽ�����
	u8  sLife;			//�ߵ�������0�� 1�
	u16 sHeadx;			//��ͷ������ֵ
	u16 sHeady;			//��ͷ������ֵ
	u16 sEndx;			//��β������ֵ
	u16 sEndy;			//��β������ֵ

}_snake_Info;
extern _snake_Info 	snake_Info;
extern _snake_Info 	snake_Info_leader;

//��Ϸ�߽�ṹ��
typedef struct
{
	u16 xUpLimit;		//����λ
	u16 xDownLimit;		//����λ
	u16 yLeftLimit;		//����λ
	u16 yRightLimit;	//����λ

}_border_limit;
extern _border_limit 	border_limit;



//ʳ��ṹ��
typedef struct
{
	u16 xFood;
	u16 yFood;
	u16 validity;		//ʳ������ĺϷ��ԣ�0���Ϸ��� 1�Ϸ���

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
