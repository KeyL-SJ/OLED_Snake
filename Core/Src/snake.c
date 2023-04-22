#include "snake.h"
#include "encoder.h"

_border_limit border_limit;
_food_Info food_Info;
_snake_Info snake_Info; //蛇的身体信息
_snake_Info snake_Info_next; //蛇下一坐标信息

u16 speed = 50;

struct List* snake_list;
//链表结构体
struct Node
{
    u16 x;
    u16 y;
    struct Node* left;
    struct Node* right;
};

//创建新结点
struct Node* createNode(u16 x,u16 y)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->x = x;
    newNode->y = y;
    newNode->left = newNode->right = NULL;
    return newNode;
}

//链表，蛇头蛇尾
struct List
{
    int size;
    struct Node* firstNode; //第一个结点
    struct Node* lastNode;  //最后一个结点
};

//创造双向链表
static struct List* createList()
{
    //申请内存
    struct List* list = (struct List*)malloc(sizeof(struct List));
    //初始化变量
    list->size = 0;
    list->firstNode = list->lastNode = NULL;
    return list;
}

//判断链表是否为空
static u8 listSize(struct List* list)
{
    return list->size;
}

static u8 emptyList(struct List* list)
{
    if(list->size == 0)
        return 0;
    else
        return 1;
}

//检查链表中数据是否有与传入数据相同
//返回值：0：无
//      1：有
static u8 listCheck(struct List* list,u16 x,u16 y)
{
    if(list->size == 0) {
//        Snake_printf("链表为空\n");
    }
    else
    {
        struct Node* pMove = list->firstNode;//创建链表移动指针
        list->lastNode->right = NULL;//断开双向循环列表
        while(pMove)
        {
            if(pMove->x == x && pMove->y == y)
            {
                list->lastNode->right = list->firstNode;
                return 1;
            }
            else
                pMove = pMove->right;
        }
        list->lastNode->right = list->firstNode;
        return 0;
    }
    return NULL;
}

//销毁双向链表
static void ListDestory(struct List* list)
{
    if(list->size == 0) {
//        Snake_printf("链表为空\n");
    }
    else{
        struct Node* pMove = list->firstNode;
        list->lastNode->right = NULL;
        while(pMove){
//            Snake_printf("%d,%d 销毁\n",pMove->x,pMove->y);
            free(pMove);
            pMove = pMove->right;
        }
//        Snake_printf("销毁完成\n");
    }
}

//头插
static void InsertNodeByHead(struct List* list,u16 x,u16 y){
    struct Node* newNode = createNode(x,y);
    if(list->firstNode == NULL){
        list->lastNode = newNode;
    }
    else{
        list->firstNode->left = newNode;
        newNode->right = list->firstNode;
    }
    list->firstNode = newNode;
    list->firstNode->left = list->lastNode;
    list->lastNode->right = list->firstNode;
    list->size++;
}

//尾插
static void InsertNodeByTail(struct List* list,u16 x,u16 y){
    struct Node* newNode = createNode(x,y);
    if(list->lastNode == NULL){
        list->lastNode = newNode;
    }
    else{
        list->lastNode->right = newNode;
        newNode->left = list->lastNode;
    }
    list->lastNode = newNode;
    list->firstNode->left = list->lastNode;
    list->lastNode->right = list->firstNode;
    list->size++;
}

//双向链表移动，原表尾移动为新表头，并重新赋值
static void ListTailMoveToHead(struct List* list,u16 x,u16 y){
    list->firstNode = list->lastNode;
    list->lastNode = list->lastNode->left;
    list->firstNode->x = x;
    list->firstNode->y = y;
}

static void Delay_ms(u32 ms){
    ms=ms*6500;
    for (u32 i = 0; i < ms; i++) ;
}

/**
 * 函数名：Snake_Draw_Erase_OneJoint
 * 功能：绘制蛇的一个关节/食物，擦除一个关节，食物
 * 参数：x：起点x坐标，y：起点y坐标
 * 参数：t：1：绘制，0：擦除
 * 返回值：无
 */
void Snake_Draw_Erase_OneJoint(u16 x,u16 y,u8 t){
    u16 x1,y1;
    x1 = 4*x;
    y1 = 4*y;
    for (int i = x1; i < x1 + 4; i++) {
        for (int j = y1; j < y1 + 4; j++) {
            OLED_DrawPoint(i,j,t);
        }
    }
 }

 /**
  * 函数名：Snake_Draw_Erase
  * 功能：绘画，擦除蛇的一个关节的一个像素
  * 参数：x,y：坐标
  *      dir：蛇移动方向
  *      t:1：绘画
  *        0：擦除
  * 返回值：无
  */
void Snake_Draw_Erase(u16 x,u16 y,u8 dir,u8 t){
    u16 x1 = x*4,y1 = y*4;
    if(dir == 3 || dir == 4)
        y1 = y + 4;
    else
        x1 = x + 4;
    for (int i = x1; i < x1 + 4; i++) {
        for (int j = y1; j < y1 + 4; j++) {
            OLED_DrawPoint(i,j,t);
        }
    }
 }

 /***
  * 函数名：Snake_Update_HeadXY
  * 功能：更新蛇头显示
  * 参数：u8 i 显示蛇头关节的第几个像素块
  * 返回值：无
  */
void Snake_Update_HeadXY(u8 i) {
    if (snake_Info.sHeadDir == 4) //蛇头向右
        Snake_Draw_Erase_OneJoint(snake_Info_next.sHeadx + i, snake_Info.sHeady, 1);
    else if (snake_Info.sHeadDir == 3) //蛇头向左
        Snake_Draw_Erase_OneJoint(snake_Info_next.sHeadx + 4 - i,snake_Info.sHeady,1);
    else if (snake_Info.sHeadDir == 2)
        Snake_Draw_Erase_OneJoint(snake_Info_next.sHeadx,snake_Info.sHeady + i,1);
    else if (snake_Info.sHeadDir == 1)
        Snake_Draw_Erase_OneJoint(snake_Info_next.sHeadx,snake_Info.sHeady + 4 - i,1);
}

  /***
   * 函数名：Snake_Update_EndXY
   * 功能：更新蛇尾显示，擦除蛇尾
   * 参数：u8 i    擦除蛇尾关节的第几个像素块
   * 返回值：无
   */
void Snake_Update_EndXY(u8 i){
//蛇尾如果和上一节蛇身在同一y轴，则往左右走
    if (snake_list->lastNode->y == snake_list->lastNode->left->y) {
        //往左
        if (snake_list->lastNode->left->x - snake_list->lastNode->x > 0) {
          Snake_Draw_Erase_OneJoint(snake_Info.sEndx + i, snake_Info.sEndy,0);//擦除
        }
        else{
          Snake_Draw_Erase_OneJoint(snake_Info.sEndx + 4 - i, snake_Info.sEndy,0);
        }
    }
    else{
        if(snake_list->lastNode->left->y - snake_list->lastNode->y > 0){
          Snake_Draw_Erase_OneJoint(snake_Info.sEndx,snake_Info.sEndy + i,0);
        }
        else{
          Snake_Draw_Erase_OneJoint(snake_Info.sEndx,snake_Info.sEndy + 4 - i,0);
        }
    }
}

  /**
   * 函数名：Snake_Update_BodyXY
   * 功能：更新蛇身坐标
   * 参数：无
   * 返回值：无
   */
void Snake_Update_BodyXY(void){
    if (snake_list->size < snake_Info.sLength)
        InsertNodeByHead(snake_list,snake_Info_next.sHeadx,snake_Info_next.sHeady);
    else
        ListTailMoveToHead(snake_list,snake_Info_next.sHeadx,snake_Info_next.sHeady);
    snake_Info.sHeadx = snake_Info_next.sHeadx;
    snake_Info.sHeady = snake_Info_next.sHeady;

    snake_Info.sEndx = snake_list->lastNode->x;
    snake_Info.sEndy = snake_list->lastNode->y;
}

  /**
   * 函数名：Snake_Display_Init
   * 功能：蛇的信息及显示初始化
   * 参数：无
   * 返回值：无
   */
void Snake_Display_Init(void) {
      snake_Info.sHeadDir = 4;//蛇头朝右
      snake_Info.sEndDir = 4;//蛇尾朝右
      snake_Info.sLife = 1;//初始化蛇的生命
      food_Info.validity = 0;//食物初始化

      snake_Info.sLength = 4;

      //蛇头蛇尾坐标初始化
      //       snake_Info.sHeadx = border_limit.yLeftLimit + SNAKE_LENGTH_PIXELS*(1+SNAKE_INIT_LENGTH) + SNAKE_LENGTH_PIXELS + 1;
      //       snake_Info.sHeady = border_limit.xUpLimit + SNAKE_LENGTH_PIXELS + 1;
      snake_Info.sHeadx = 0;
      snake_Info.sHeady = 4;
      snake_Info.sEndx = 0;
      snake_Info.sEndy = 4;

      snake_Info_next.sHeadx = snake_Info.sHeadx;
      snake_Info_next.sHeadx = snake_Info.sHeady;

//      snake_list = createList();
//      InsertNodeByHead(snake_list, snake_Info.sHeadx, snake_Info.sHeady);
//      //增加蛇头
//      Snake_Draw_Erase_OneJoint(snake_Info.sHeadx, snake_Info.sHeady, 1);
//      for (int count = 1; count < snake_Info.sLength; count++) {
//          snake_Info.sHeadx += 1;
//          InsertNodeByHead(snake_list, snake_Info.sHeadx, snake_Info.sHeady);        //表尾循环插入蛇身数据
//          Snake_Draw_Erase_OneJoint(snake_Info.sHeadx, snake_Info.sHeady, 1);    //添加蛇身
//      }
      if(snake_Info.sHeadx  < snake_Info.sLength) {
          snake_list = createList();
          InsertNodeByHead(snake_list, snake_Info.sHeadx, snake_Info.sHeady);
          //增加蛇头
          Snake_Draw_Erase_OneJoint(snake_Info.sHeadx, snake_Info.sHeady, 1);
          for (int count = 1; count < snake_Info.sLength; count++) {
              snake_Info.sHeadx += 1;
              InsertNodeByHead(snake_list, snake_Info.sHeadx, snake_Info.sHeady);        //表尾循环插入蛇身数据
              Snake_Draw_Erase_OneJoint(snake_Info.sHeadx, snake_Info.sHeady, 1);    //添加蛇身

              HAL_Delay(100);
              OLED_Refresh_Gram();
          }
      }
}

/**
 * 函数名：Snake_TouchSelf
 * 功能：判断蛇是否吃到自己
 * 参数：无
 * 返回值：0：死亡，1：正常
 */
static u8 Snake_TouchSelf(void){
    u8 snakeSta = 1, row, col;
    //遍历链表数据，检查蛇头是否碰到蛇身。
    snakeSta = 1 - listCheck(snake_list, snake_Info_next.sHeadx, snake_Info_next.sHeady);
    if (snakeSta == 0){

        snake_Info.sLife = 0;
    }
    return snake_Info.sLife;
}
/**
 * 函数名：Snake_Move
 * 功能：蛇身移动
 * 参数：无
 * 返回值：0：死亡，1：正常
 */
u8 Snake_Move(void){
    u8 upDateSnakeEnd = 0;  //更新尾巴标志位(0：更新，1：不更新)
    u8 SnakeSta = Snake_TouchSelf();
    if(SnakeSta == 0){
        return 0;
    }

    //判断蛇下一个坐标是否与食物一样，若吃到食物，则此次不删除尾巴
    if(snake_Info_next.sHeadx == food_Info.xFood && snake_Info_next.sHeady == food_Info.yFood){
        upDateSnakeEnd = 1;
    }

    for (int i=0; i<SNAKE_MOVE_DISTANCE; i++){
        Snake_Update_HeadXY(i);
        if(upDateSnakeEnd == 0){
            Snake_Update_EndXY(i);
        }
    }
    if(upDateSnakeEnd == 1){
        snake_Info.sLength++;
        food_Info.validity = 0;//更新食物为被吃掉状态
    }
    Snake_Update_BodyXY();
    return 1;
}


void Snake_Move_Demo(void){
//    Snake_Display_Init();
    for (int i = 0; i < BORDER_LIMIT_RIGHT; i++){
        Snake_Update_HeadXY(i);
        Snake_Update_EndXY(i);
        HAL_Delay(100);
        OLED_Refresh_Gram();
    }
}

void Snake_Move_Encoder(int Snake_Dirs){
    u8 snakeSta = 1;
    switch(Snake_Dirs){
        case 1:
            snake_Info.sHeadx += 1;
            Snake_Update_BodyXY();
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
    }
    OLED_Refresh_Gram();
}
