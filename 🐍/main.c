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


/// 方向k控制
void getDirection()
{
    
}

/// 移动🐍
void moveSnake()
{
    
}

/// 刷新界面
void flushWindows()
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (i == 0 || i == 15 || j == 0 || j ==15)
                printf(" #");
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
    
    while (1)
    {
        system("clear");
        getDirection();
        
        moveSnake();
        
        flushWindows();
    }
    
    return 0;
}
