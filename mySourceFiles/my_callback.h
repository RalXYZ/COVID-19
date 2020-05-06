/*
 * 文件名: my_callback.h
 * -------------------------------------
 * 这个头文件声明了回调函数
 *
 */

#ifndef MY_CALLBACK_H_
#define MY_CALLBACK_H_

void KeyboardEventProcess(int key, int event);

void MouseEventProcess(int x, int y, int button, int event);

void CharEventProcess(char key);

void TimerEventProcess(int timerID);

#endif
