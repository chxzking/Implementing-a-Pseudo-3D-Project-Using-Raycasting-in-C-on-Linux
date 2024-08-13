#include "../header/screen.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <string.h>
#include <math.h>

int WIDTH;    //默认
int HEIGHT;    //默认

//初始化屏幕设置，初始化屏幕设置，包括获取终端大小、计算窗口宽度和高度，并分配画布内存。
void screen_init(Screen *screen) {
    //获取终端的高度和宽度
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    WIDTH = w.ws_col;   //设置显示宽度
    HEIGHT = w.ws_row;  //设置显示高度
    //创建画布空间
    screen->canvas = (pixel *)malloc(WIDTH * HEIGHT * sizeof(pixel));
    screen_clear(screen);
}

//屏幕清空，将画布的上半部分设置为亮度0，下半部分设置为亮度15。
void screen_clear(Screen *screen) {
    //按照行式遍历画布（从上往下）
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            //判断当前位置是否处在画布的下半部分
            if(i >= HEIGHT / 2) screen->canvas[i * WIDTH + j] = 15; //如果i的值超过了高度的一半，那么意味着到达了画布下半部分
            else screen->canvas[i * WIDTH + j] = 0; //如果i值没有超过高度的一半，那么就意味着在画布上半部分
        }
    }
}


//在指定位置绘制一个亮度为 luminuns 的点。
void screen_draw_point(Screen *screen, int x, int y, pixel luminuns) {
    //以二维的方式解读一维度数组。
    //判断是否超出了画布范围
    if(x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) {
        return;
    } else {
        //定位指定位置：设置亮度值
        screen->canvas[y * WIDTH + x] = luminuns;
    }
}

//使用 Bresenham 算法绘制一条线段。
void screen_draw_line(Screen *screen, int x0, int y0, int x1, int y1, pixel luminuns) {
    //检查线段顶点是否超出了画布范围
    if(x0 < 0 || y0 < 0 || x0 >= WIDTH || y0 >= HEIGHT) return;
    if(x1 < 0 || y1 < 0 || x1 >= WIDTH || y1 >= HEIGHT) return;

    //计算dx的差值绝对值,计算线段x坐标增长方向
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    //计算dy的差值绝对值,计算线段y坐标增长方向
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;


    //根据经验设计的初始值
    int erro = (dx > dy ? dx : -dy) / 2;
    //循环操作，写入画布的（x0,y0坐标），判断条件：左端点移动到了右端点的位置跳出循环
    while(screen->canvas[y0 * WIDTH + x0] = luminuns, x0 != x1 || y0 != y1) {
        int e2 = erro;
        if(e2 > -dx) { erro -= dy; x0 += sx; }
        if(e2 <  dy) { erro += dx; y0 += sy; }
    }
}

//将画布内容渲染到屏幕上。
void screen_show(Screen *screen) {
    //申请一帧的内存
    char *frame = (char *)malloc(WIDTH * HEIGHT * sizeof(char));
    //将一维空间的帧逻辑化为二维
    for(int i = 0; i < HEIGHT - 1; i++) {
        frame[WIDTH * i + WIDTH - 1] = '\n';
    }
    //添加帧的终点
    frame[WIDTH * HEIGHT - 1] = '\0';

    //根据画布的内容写入帧中
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH - 1; j++) {
            frame[i * WIDTH + j] = screen_brightness(screen->canvas[WIDTH * i + j]);
        }
    }

    //添加边框
    for (int i = 0; i < HEIGHT; ++i) {
        frame[WIDTH * i] = '@';
        frame[WIDTH * i + WIDTH - 2] = '@';
    }
    for (int j = 0; j < WIDTH - 1; ++j) {
        frame[j] = '@';
        frame[WIDTH * (HEIGHT - 1) + j] = '@';
    }
    //输出屏幕
    screen_fill_screen_with_string(frame);
    //释放帧空间
    free(frame);
}

//将字符串内容填充到屏幕上。
void screen_fill_screen_with_string(const char *frame) {
    printf("\033[H\033[J"); // 清屏
    //输出画布缓存
    printf("%s", frame);
}

//根据亮度值返回相应的字符。
char screen_brightness(pixel n) {
    char s[] = " .,^:-+abcdw?$&%#@";
    //线性映射，相较于n%18来说，该思路将概率输入值平均映射到0-17的范围
    return s[n * 18 / 256];
}

