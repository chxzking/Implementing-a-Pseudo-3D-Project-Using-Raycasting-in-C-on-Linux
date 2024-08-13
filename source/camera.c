#include "../header/camera.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

//初始化相机的属性，包括移动速度、旋转速度、位置、方向和相机平面法向量。
//初始化相机结构体(确认)
void Camera_init(Camera *camera,vector camera_position,vector camera_forward){
    camera->moveSpeed = 0.1 * 5.0;//初始化移动速度
    camera->rotSpeed = 0.05 * 3.0;//初始化旋转速度

    camera->position = camera_position;//初始化相机位置
    camera->forward = camera_forward;//初始化相机方向
    camera->forward = vector_normalize(&camera_forward);//方向向量归一化处理

    //初始化平面法向量
    camera->planeX = 0;
    camera->planeY = 0.66;
}

//设置相机的位置。(确认)
void Camera_setPosition(Camera *camera, vector position) {
    camera->position = position;
}

//设置相机的方向。(确认)
void Camera_setDirection(Camera *camera, float dx, float dy) {
    camera->forward = vector_create(dx, dy);
}


//在屏幕上显示相机的视角内容。这个函数使用DDA算法（数字微分分析器）计算射线与墙壁的交点，并绘制墙壁的垂直线条。
void Camera_Show(Camera *camera, Screen *screen) {
    //初始化相机位置和方向：
    double camera_posX = camera->position.x,camera_posY = camera->position.y;
    double camera_dirX = camera->forward.x, camera_dirY = camera->forward.y;

    //在画布中按照列的形式输出线条
    for (int x = 0; x < WIDTH; x++) {
        
        double cameraX = 2 * x / (double)WIDTH - 1;//将相机水平视野缩小到[-1,1]中
        //偏转水平方向射线，发射纵向方向的视觉线
        double rayDirX = camera_dirX + camera->planeX * cameraX;
        double rayDirY = camera_dirY + camera->planeY * cameraX;

        //定位摄像机的位置
        int mapX = (int)camera_posX;
        int mapY = (int)camera_posY;

        //当前位置到下一个x,y边的长度
        double sideDistX;
        double sideDistY;

        //射线从一个x、y边到另个x、y边的距离
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

       
        //dda每一步的步进反向
        int stepX;
        int stepY;

        int hit = 0;//是否碰到墙壁
        int side;//碰撞的时南北（垂直与x的边）还是东西（垂直与y的边）边   


        //初始化 step和sideDist（步进方向和距离）
        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (camera_posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - camera_posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (camera_posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - camera_posY) * deltaDistY;
        }
        //执行 DDA算法
        while (hit == 0) {
            //如果 sideDistX 小于 sideDistY，则表示射线更接近 x 方向上的下一个方格。
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            //是否撞击
            if (worldMap[mapX][mapY] > 0) hit = 1;
        }


        //计算垂直墙壁的距离
        double perpWallDist;//参数的作用是计算光线与墙壁的垂直距离
        //因为进入了墙内所以回退一段距离
        if (side == 0) perpWallDist = (sideDistX - deltaDistX);
        else perpWallDist = (sideDistY - deltaDistY);

        //计算墙的高度和绘制范围
        //计算墙的高度
        int lineHeight = (int)(HEIGHT / perpWallDist);

        
        //墙的起始y值，以横向的中央分界线开始，起始值往上推移半个高度的距离获得起始点，如果突破了顶部就置0
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        //终点值，以横向的中央分界线开始，起始值往下推移半个高度的距离获得结束点，如果突破了底部置为最大值。
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;
        //因为是采用上下扩张的方式定位，所以显示的画面是以中线对称的。
       
        //绘制一列墙壁
        //上色
        int color = 255;
        if (side == 1)  color = 98;

        //宝箱
        if(worldMap[mapX][mapY] == 5 ){
            
            if(lineHeight >= 4){
                screen_draw_line(screen, x, drawStart, x, drawStart+(int)(((drawEnd - drawStart))/4), 190);
                screen_draw_line(screen, x, drawStart+(int)(((drawEnd - drawStart))/4), x, drawStart+ ((int)(((drawEnd - drawStart))/4))*2, 180);
                screen_draw_line(screen, x, drawStart+ ((int)(((drawEnd - drawStart))/4))*2, x, drawEnd, 190);
            }else{
                screen_draw_line(screen, x, drawStart, x, drawEnd, 190);
            }
            continue;
        }

        //画线
        screen_draw_line(screen, x, drawStart, x, drawEnd, color);
    }
}

//使相机向前移动。如果前方没有墙壁，则更新相机的位置。（确认）
void Camera_advance(Camera *camera) {
    if (worldMap[(int)(camera->position.x + camera->forward.x * camera->moveSpeed)][(int)(camera->position.y)] == 0)
        camera->position.x += camera->forward.x * camera->moveSpeed;
    if (worldMap[(int)(camera->position.x)][(int)(camera->position.y + camera->forward.y * camera->moveSpeed)] == 0)
        camera->position.y += camera->forward.y * camera->moveSpeed;
}

//使相机向后移动。如果后方没有墙壁，则更新相机的位置。（确认）
void Camera_recoil(Camera *camera) {
    if (worldMap[(int)(camera->position.x - camera->forward.x * camera->moveSpeed)][(int)(camera->position.y)] == 0)
        camera->position.x -= camera->forward.x * camera->moveSpeed;
    if (worldMap[(int)(camera->position.x)][(int)(camera->position.y - camera->forward.y * camera->moveSpeed)] == 0)
        camera->position.y -= camera->forward.y * camera->moveSpeed;
}


//使相机向左旋转。更新相机的方向和相机平面法向量。（确认）
void Camera_left_rotate(Camera *camera) {
    //保存当前方向向量的分量：
    double dirX = camera->forward.x, dirY = camera->forward.y;
    double oldDirX = dirX;
    //利用旋转矩阵计算更新方向向量：
    camera->forward.x = dirX * cos(camera->rotSpeed) - dirY * sin(camera->rotSpeed);
    camera->forward.y = oldDirX * sin(camera->rotSpeed) + dirY * cos(camera->rotSpeed);
    //保存当前平面法向量的 x 分量：
    double oldPlaneX = camera->planeX;
    //更新平面法向量：
    camera->planeX = camera->planeX * cos(camera->rotSpeed) - camera->planeY * sin(camera->rotSpeed);
    camera->planeY = oldPlaneX * sin(camera->rotSpeed) + camera->planeY * cos(camera->rotSpeed);
}

//使相机向右旋转。更新相机的方向和相机平面法向量。（确认）
void Camera_right_rotate(Camera *camera) {
    //保存当前方向向量的分量：
    double dirX = camera->forward.x, dirY = camera->forward.y;
    double oldDirX = dirX;
    //利用旋转矩阵计算更新方向向量：
    camera->forward.x = dirX * cos(-camera->rotSpeed) - dirY * sin(-camera->rotSpeed);
    camera->forward.y = oldDirX * sin(-camera->rotSpeed) + dirY * cos(-camera->rotSpeed);
    //保存当前平面法向量的 x 分量：
    double oldPlaneX = camera->planeX;
    //更新平面法向量：
    camera->planeX = camera->planeX * cos(-camera->rotSpeed) - camera->planeY * sin(-camera->rotSpeed);
    camera->planeY = oldPlaneX * sin(-camera->rotSpeed) + camera->planeY * cos(-camera->rotSpeed);
}


//使相机向左平移。如果左侧没有墙壁，则更新相机的位置。
void Camera_left_Move(Camera *camera) {
    if (worldMap[(int)(camera->position.x - camera->planeX * camera->moveSpeed)][(int)(camera->position.y)] == 0)
        camera->position.x -= camera->planeX * camera->moveSpeed;
    if (worldMap[(int)(camera->position.x)][(int)(camera->position.y - camera->planeY * camera->moveSpeed)] == 0)
        camera->position.y -= camera->planeY * camera->moveSpeed;
}

//使相机向右平移。如果右侧没有墙壁，则更新相机的位置。
void Camera_right_Move(Camera *camera) {
    if (worldMap[(int)(camera->position.x + camera->planeX * camera->moveSpeed)][(int)(camera->position.y)] == 0)
        camera->position.x += camera->planeX * camera->moveSpeed;
    if (worldMap[(int)(camera->position.x)][(int)(camera->position.y + camera->planeY * camera->moveSpeed)] == 0)
        camera->position.y += camera->planeY * camera->moveSpeed;
}


