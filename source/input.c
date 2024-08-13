#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
//无回显立即获取一个字符
char key_getch(){
    struct termios old_tio, new_tio;
    unsigned char c;

    /* 获取当前终端设置 */
    tcgetattr(STDIN_FILENO, &old_tio);

    /* 复制当前设置到新设置中 */
    new_tio = old_tio;

    /* 修改新设置 */
    new_tio.c_lflag &=(~ICANON & ~ECHO);

    /* 应用新设置 */
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    c = getchar();

    /* 恢复旧设置 */
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    return c;
}
//刷新终端
void my_clear(){
  printf("\033[H\033[2J");//刷新终端
}

