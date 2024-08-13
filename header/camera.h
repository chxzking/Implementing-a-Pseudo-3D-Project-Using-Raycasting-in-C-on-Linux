#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "vector.h" 
#include "screen.h"
#include "worldMap.h"


//摄像机结构体
typedef struct Camera{
    double moveSpeed; // 移动速度
    double rotSpeed;  // 旋转速度 每次旋转nPI的值

    vector position;    // 相机位置
    vector forward;     // 相机方向

    double planeX;     // 相机平面法向量X
    double planeY;     // 相机平面法向量Y
} Camera;



//初始化相机的属性，包括移动速度、旋转速度、位置、方向和相机平面法向量。
void Camera_init(Camera *camera,vector camera_position,vector camera_forward);
//设置相机的位置。
void Camera_setPosition(Camera *camera, vector position);
//设置相机的方向。
void Camera_setDirection(Camera *camera, float dx, float dy) ;
//在屏幕上显示相机的视角内容。这个函数使用DDA算法（数字微分分析器）计算射线与墙壁的交点，并绘制墙壁的垂直线条。
void Camera_Show(Camera *camera, Screen *screen);
//使相机向前移动。如果前方没有墙壁，则更新相机的位置。
void Camera_advance(Camera *camera) ;
//使相机向后移动。如果后方没有墙壁，则更新相机的位置。
void Camera_recoil(Camera *camera);
//使相机向左旋转。更新相机的方向和相机平面法向量。
void Camera_left_rotate(Camera *camera);
//使相机向右旋转。更新相机的方向和相机平面法向量。
void Camera_right_rotate(Camera *camera) ;
//相机向左平移移动
void Camera_left_Move(Camera *camera);
//相机向右平移移动
void Camera_right_Move(Camera *camera);
#endif