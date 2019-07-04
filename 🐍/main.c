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
#include <unistd.h>


int kbhit()
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
    }
    int bytesWaiting;
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

void initSanke(Snake ** sk,Food * fd)
{
    /// 判断初始化的食物蛇头 不在食物上
    *sk = (Snake *)malloc(sizeof(Snake));
    (*sk)->type = HEAD;
    (*sk)->next = NULL;
    do{
        (*sk)->pos.x_ = rand() % 10 + 3;
        (*sk)->pos.y_ = rand() % 10 + 3;
    }while((*sk)->pos.x_ == fd->pos.x_
           && (*sk)->pos.y_ == fd->pos.y_);
}

int isSnakePlay(Snake * sk,int x,int y,SnakeType * type)
{
    while (sk)
    {
        if (sk->pos.x_ == x && sk->pos.y_ == y)
        {
            *type = sk->type;
            return 1;
        }
        sk = sk->next;
    }
    return 0;
}

/// 方向控制
void getDirection(Direct * dirct)
{
    while (kbhit())
    {
        int a = getchar();
        *dirct = a;
    }
}

/// 移动🐍
void moveSnake(Snake * sk,Food * fd,Direct dirct)
{
    Pos pos = sk->pos;
    Snake * t = sk->next;
    /// 撞墙
    if (pos.x_ == 0|| pos.x_ == 15 || pos.y_ == 15 || pos.y_ == 0)
        exit(0);
    ///吃到自己
    while (t)
    {
        if (t->pos.x_ == sk->pos.x_ && t->pos.y_ == sk->pos.y_)
        {
            exit(0);
        }
        t = t->next;
    }
    
    /// 吃食物
    if (sk->pos.x_ == fd->pos.x_ && sk->pos.y_ == fd->pos.y_)
    {
        /// 吃到食物加一个节点
        Snake * cur = (Snake *)malloc(sizeof(Snake));
        t = sk;
        while (t->next)
        {
            t = t->next;
            t->type = BODY;
        }
        t->next = cur;
        cur->next = NULL;
        cur->type = TAIL;
        
        ///随机食物的位置 不可以出现在蛇的身体上
        int isPos;
        t = sk;
        do {
            isPos = 0;
            fd->level = LEVELONE;
            fd->pos.x_ = rand() % 14 + 1;
            fd->pos.y_ = rand() % 14 + 1;
            while (t)
            {
                if (t->pos.x_ == fd->pos.x_ && t->pos.x_ == fd->pos.y_)
                {
                    isPos = 1;
                    break;
                }
                t = t->next;
            }
        } while (isPos);
    }
    
    /// 移动
    if(sk->type == HEAD)
    {
        switch (dirct) {
            case UP:
                sk->pos.x_--;
                if (sk->next && sk->pos.x_ == sk->next->pos.x_)
                {
                    sk->pos.x_++;
                }
                break;
            case DOWN:
                sk->pos.x_++;
                break;
            case LEFT:
                sk->pos.y_--;
                break;
            case RIGHT:
                sk->pos.y_++;
                break;
            }
        
    
        t = sk->next;
        Pos cpos;
        while (t)
        {
            cpos = t->pos;
            t->pos = pos;
            pos = cpos;
            t = t->next;
        }
    }
}

/// 刷新界面
void flushWindows(Snake * sk,Food * fd)
{
    SnakeType  type;
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (i == 0 || i == 15 || j == 0 || j ==15)
                printf(" @");
            else if (fd->pos.x_ == i && fd->pos.y_ == j)
                printf("🔥");
            else if (isSnakePlay(sk, i, j, &type))
            {
                switch (type) {
                    case HEAD:
                        printf("👽");
                        break;
                    case BODY:
                        printf(" #");
                        break;
                    case TAIL:
                        printf(" *");
                        break;
                }
            }
            else
                printf("  ");
        }
        putchar(10);
    }
}

int main(int argc, const char * argv[]) {
    
    /// 关闭缓存机制 不用回车 直接输出字符
    system("stty -icanon");
    
    srand((unsigned int)time(NULL));
    Food fd;
    initFood(&fd);
    Snake * sk;
    initSanke(&sk, &fd);
    
    Direct dirct;
    
    while (1)
    {
        system("clear");
        
        getDirection(&dirct);
        
        moveSnake(sk,&fd,dirct);
        
        flushWindows(sk,&fd);
        usleep(1000 * 500);
        
        
    }
    
    return 0;
}

