#include "encoder.h"
#include "snake.h"
u8 Snake_MoveDirs = 0;
u8 Encoder_Left_Num = 0,Encoder_Right_Num = 0;

int Read_Encoder(u8 TIMx){
    int Encoder_TIM;
    switch(TIMx){
        case 2:Encoder_TIM = (short)TIM2->CNT;TIM2->CNT = 0;break;
        case 3:Encoder_TIM = (short)TIM3->CNT;TIM3->CNT = 0;break;
        case 4:Encoder_TIM = (short)TIM4->CNT;TIM4->CNT = 0;break;
        default:Encoder_TIM = 0;
    }
    return Encoder_TIM;
}

int My_Abs(int a){
    int temp;
    if(a < 0){
        temp = -a;
    }else{
        temp = a;
    }
    return temp;
}
int Encoder_Choose(int encoder_left,int encoder_right){
    static int count_left;
    static int count_right = 360;
    if(encoder_left == 0 && encoder_right != 0){
        count_right += encoder_right;
//        OLED_ShowNumber(10,30,count_right,6,12);
//        OLED_Refresh_Gram();
        if(count_right > 10 && count_right < 360){
            Snake_MoveDirs = 2;
            Encoder_Right_Num = 2;
        }
        if(count_right > 370 && count_right < 720){
            Snake_MoveDirs  = 1;
            Encoder_Right_Num = 1;
        }
        if(count_right > 720 || count_right < 0){
            count_right = 0;
        }
    }
    if(encoder_right  == 10 && encoder_left != 0){
        count_left += My_Abs(encoder_left);
        if(count_left > 0 && count_left < 360){
            Snake_MoveDirs = 4;
            Encoder_Left_Num = 4;
        }
        if(count_left > 370 && count_left < 720){
            Snake_MoveDirs = 3;
            Encoder_Left_Num = 3;
        }
        if(count_left > 720 || count_left < 0){
            count_left = 0;
        }
    }
    OLED_ShowNumber(0,50,Encoder_Left_Num,2,12);
    OLED_ShowNumber(50,50,Snake_MoveDirs,2,12);
    OLED_ShowNumber(100,50,Encoder_Right_Num,2,12);
    OLED_Refresh_Gram();
    return Snake_MoveDirs;
}
