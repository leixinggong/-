//
//  main.c
//  🐍
//
//  Created by 龚磊星 on 2019/7/3.
//  Copyright © 2019 龚磊星. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <time.h>


int _kbhit()
{
    static const int STDIN = 0;
    static int initialized = 0;
    if (! initialized)
    {
        // Use termios to turn off line buffering
        struct termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = 1;
    } int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}

typedef enum direction{
    UP      = 119  ,
    DOWN    = 115  ,
    LEFT    = 97   ,
    RIGHT   = 100
} Direct;

typedef enum _level
{
    LEVELONE,LEVELTOW
}Level;

typedef struct _pos
{
    int x_;
    int y_;
}Pos;

typedef struct _food
{
    Level level;
    Pos pos;
}Food;

void initFood(Food * food)
{
    food->level = LEVELONE;
    food->pos.x_ = rand() % 14 + 1;
    food->pos.y_ = rand() % 14 + 1;
}

typedef enum _snaketype
{
    HEAD,
    BODY,
    TAIL
}SnakeType;

typedef struct _snake
{
    Pos pos;
    SnakeType type;
    struct _snake * next;
    
}Snake;

void initSanke(Snake * sk)
{
    /// 判断初始化的食物蛇头 不在食物上
}

/// 方向k控制
void getDirection()
{
    
}

/// 移动🐍
void moveSnake()
{
    
}

/// 刷新界面
void flushWindows(Food * fd)
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (i == 0 || i == 15 || j == 0 || j ==15)
                printf(" @");
            else if (fd->pos.x_ == i && fd->pos.y_ == j)
                printf("🔥");
            else
                printf("  ");
        }
        putchar(10);
    }
}

int main(int argc, const char * argv[]) {
    
//    int a;
//    /// 关闭缓存机制 不用回车 直接输出字符
//    system("stty -icanon");
//    a = getchar();
    

    
    srand((unsigned int)time(NULL));
    Food fd;
    initFood(&fd);
    
    while (1)
    {
        system("clear");
        getDirection();
        
        moveSnake();
        
        flushWindows(&fd);
    }
    
    return 0;
}
