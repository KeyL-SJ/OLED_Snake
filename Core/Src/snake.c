#include "snake.h"
#include "encoder.h"

_border_limit border_limit;
_food_Info food_Info;
_snake_Info snake_Info; //�ߵ�������Ϣ
_snake_Info snake_Info_next; //����һ������Ϣ

u16 speed = 50;

struct List* snake_list;
//����ṹ��
struct Node
{
    u16 x;
    u16 y;
    struct Node* left;
    struct Node* right;
};

//�����½��
struct Node* createNode(u16 x,u16 y)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->x = x;
    newNode->y = y;
    newNode->left = newNode->right = NULL;
    return newNode;
}

//������ͷ��β
struct List
{
    int size;
    struct Node* firstNode; //��һ�����
    struct Node* lastNode;  //���һ�����
};

//����˫������
static struct List* createList()
{
    //�����ڴ�
    struct List* list = (struct List*)malloc(sizeof(struct List));
    //��ʼ������
    list->size = 0;
    list->firstNode = list->lastNode = NULL;
    return list;
}

//�ж������Ƿ�Ϊ��
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

//��������������Ƿ����봫��������ͬ
//����ֵ��0����
//      1����
static u8 listCheck(struct List* list,u16 x,u16 y)
{
    if(list->size == 0) {
//        Snake_printf("����Ϊ��\n");
    }
    else
    {
        struct Node* pMove = list->firstNode;//���������ƶ�ָ��
        list->lastNode->right = NULL;//�Ͽ�˫��ѭ���б�
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

//����˫������
static void ListDestory(struct List* list)
{
    if(list->size == 0) {
//        Snake_printf("����Ϊ��\n");
    }
    else{
        struct Node* pMove = list->firstNode;
        list->lastNode->right = NULL;
        while(pMove){
//            Snake_printf("%d,%d ����\n",pMove->x,pMove->y);
            free(pMove);
            pMove = pMove->right;
        }
//        Snake_printf("�������\n");
    }
}

//ͷ��
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

//β��
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

//˫�������ƶ���ԭ��β�ƶ�Ϊ�±�ͷ�������¸�ֵ
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
 * ��������Snake_Draw_Erase_OneJoint
 * ���ܣ������ߵ�һ���ؽ�/ʳ�����һ���ؽڣ�ʳ��
 * ������x�����x���꣬y�����y����
 * ������t��1�����ƣ�0������
 * ����ֵ����
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
  * ��������Snake_Draw_Erase
  * ���ܣ��滭�������ߵ�һ���ؽڵ�һ������
  * ������x,y������
  *      dir�����ƶ�����
  *      t:1���滭
  *        0������
  * ����ֵ����
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
  * ��������Snake_Update_HeadXY
  * ���ܣ�������ͷ��ʾ
  * ������u8 i ��ʾ��ͷ�ؽڵĵڼ������ؿ�
  * ����ֵ����
  */
void Snake_Update_HeadXY(u8 i) {
    if (snake_Info.sHeadDir == 4) //��ͷ����
        Snake_Draw_Erase_OneJoint(snake_Info_next.sHeadx + i, snake_Info.sHeady, 1);
    else if (snake_Info.sHeadDir == 3) //��ͷ����
        Snake_Draw_Erase_OneJoint(snake_Info_next.sHeadx + 4 - i,snake_Info.sHeady,1);
    else if (snake_Info.sHeadDir == 2)
        Snake_Draw_Erase_OneJoint(snake_Info_next.sHeadx,snake_Info.sHeady + i,1);
    else if (snake_Info.sHeadDir == 1)
        Snake_Draw_Erase_OneJoint(snake_Info_next.sHeadx,snake_Info.sHeady + 4 - i,1);
}

  /***
   * ��������Snake_Update_EndXY
   * ���ܣ�������β��ʾ��������β
   * ������u8 i    ������β�ؽڵĵڼ������ؿ�
   * ����ֵ����
   */
void Snake_Update_EndXY(u8 i){
//��β�������һ��������ͬһy�ᣬ����������
    if (snake_list->lastNode->y == snake_list->lastNode->left->y) {
        //����
        if (snake_list->lastNode->left->x - snake_list->lastNode->x > 0) {
          Snake_Draw_Erase_OneJoint(snake_Info.sEndx + i, snake_Info.sEndy,0);//����
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
   * ��������Snake_Update_BodyXY
   * ���ܣ�������������
   * ��������
   * ����ֵ����
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
   * ��������Snake_Display_Init
   * ���ܣ��ߵ���Ϣ����ʾ��ʼ��
   * ��������
   * ����ֵ����
   */
void Snake_Display_Init(void) {
      snake_Info.sHeadDir = 4;//��ͷ����
      snake_Info.sEndDir = 4;//��β����
      snake_Info.sLife = 1;//��ʼ���ߵ�����
      food_Info.validity = 0;//ʳ���ʼ��

      snake_Info.sLength = 4;

      //��ͷ��β�����ʼ��
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
//      //������ͷ
//      Snake_Draw_Erase_OneJoint(snake_Info.sHeadx, snake_Info.sHeady, 1);
//      for (int count = 1; count < snake_Info.sLength; count++) {
//          snake_Info.sHeadx += 1;
//          InsertNodeByHead(snake_list, snake_Info.sHeadx, snake_Info.sHeady);        //��βѭ��������������
//          Snake_Draw_Erase_OneJoint(snake_Info.sHeadx, snake_Info.sHeady, 1);    //�������
//      }
      if(snake_Info.sHeadx  < snake_Info.sLength) {
          snake_list = createList();
          InsertNodeByHead(snake_list, snake_Info.sHeadx, snake_Info.sHeady);
          //������ͷ
          Snake_Draw_Erase_OneJoint(snake_Info.sHeadx, snake_Info.sHeady, 1);
          for (int count = 1; count < snake_Info.sLength; count++) {
              snake_Info.sHeadx += 1;
              InsertNodeByHead(snake_list, snake_Info.sHeadx, snake_Info.sHeady);        //��βѭ��������������
              Snake_Draw_Erase_OneJoint(snake_Info.sHeadx, snake_Info.sHeady, 1);    //�������

              HAL_Delay(100);
              OLED_Refresh_Gram();
          }
      }
}

/**
 * ��������Snake_TouchSelf
 * ���ܣ��ж����Ƿ�Ե��Լ�
 * ��������
 * ����ֵ��0��������1������
 */
static u8 Snake_TouchSelf(void){
    u8 snakeSta = 1, row, col;
    //�����������ݣ������ͷ�Ƿ���������
    snakeSta = 1 - listCheck(snake_list, snake_Info_next.sHeadx, snake_Info_next.sHeady);
    if (snakeSta == 0){

        snake_Info.sLife = 0;
    }
    return snake_Info.sLife;
}
/**
 * ��������Snake_Move
 * ���ܣ������ƶ�
 * ��������
 * ����ֵ��0��������1������
 */
u8 Snake_Move(void){
    u8 upDateSnakeEnd = 0;  //����β�ͱ�־λ(0�����£�1��������)
    u8 SnakeSta = Snake_TouchSelf();
    if(SnakeSta == 0){
        return 0;
    }

    //�ж�����һ�������Ƿ���ʳ��һ�������Ե�ʳ���˴β�ɾ��β��
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
        food_Info.validity = 0;//����ʳ��Ϊ���Ե�״̬
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
