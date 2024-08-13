#ifndef __SCREEN_H__
#define __SCREEN_H__

extern int WIDTH;   //屏幕宽度
extern int HEIGHT;  //屏幕高度

typedef unsigned char pixel;

//屏幕结构体
typedef struct Screen{
    pixel *canvas;
} Screen;

//初始化屏幕设置，初始化屏幕设置，包括获取终端大小、计算窗口宽度和高度，并分配画布内存。
void screen_init(Screen *screen);
//屏幕清空，将画布的上半部分设置为亮度0，下半部分设置为亮度15。
void screen_clear(Screen *screen);
//在指定位置绘制一个亮度为 luminuns 的点。
void screen_draw_point(Screen *screen, int x, int y, pixel luminuns);
//使用 Bresenham 算法绘制一条线段。
void screen_draw_line(Screen *screen, int x0, int y0, int x1, int y1, pixel luminuns);
//将画布内容渲染到屏幕上。
void screen_show(Screen *screen);
//将字符串内容填充到屏幕上。
void screen_fill_screen_with_string(const char *frame);
//根据亮度值返回相应的字符。
char screen_brightness(pixel n);


#endif