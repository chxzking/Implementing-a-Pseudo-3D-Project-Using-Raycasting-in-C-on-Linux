#include "../header/camera.h"
#include "../header/input.h"
#include "../header/screen.h"
#include "../header/vector.h"
#include "../header/worldMap.h"

#include <stdio.h>

int main(){

  Camera camera;
  Screen screen;
  //初始化相机位置和相机方向向量
  vector camera_position = vector_create(22,12);
  vector camera_forward = vector_create(-1,0);
  //初始化相机
  Camera_init(&camera,camera_position,camera_forward);
  //初始化屏幕
  screen_init(&screen);

  //生成初始画面
  screen_clear(&screen);
  Camera_Show(&camera, &screen);
  screen_show(&screen);

  while(1) {
    int ch = key_getch(); 
    screen_clear(&screen);
    switch(ch)
    {
      case 'e':
          Camera_right_rotate(&camera);//右旋
        break;
      case 's':
          Camera_recoil(&camera); //后退
          break;
      case 'q':
          Camera_left_rotate(&camera);//左旋
          break;
      case 'w':
          Camera_advance(&camera); //前进
          break;
      case 'a':
          Camera_left_Move(&camera);//左移动
          break;
      case 'd':
          Camera_right_Move(&camera);//右移动
          break;
    }
    
    //判断是否到达终点，如果周围3*3的范围出现终点，那么就意味着到达终点
    for(int i = 0;i<3;i++){
      for(int j = 0;j<3;j++){
        if(worldMap[(int)camera.position.x -1 + i][(int)camera.position.y -1 + j] == 5){
          my_clear();
          printf("通关！！！");
          return 0;
        }
      }
    }

    //生成摄像机视野范围，并输入到画布上
    Camera_Show(&camera, &screen);
    //将画布输出到屏幕上
    screen_show(&screen);
    
  }
}