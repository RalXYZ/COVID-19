/*
 * 文件名: my_macro.h
 * -------------------------------------
 * 这个头文件定义了一些被其余所有文件依赖的宏
 * 所有其他文件都应该包含这个头文件
 * 这个头文件不应依赖其他文件
 */

#ifndef MY_MACRO_H_
#define MY_MACRO_H_

 /* 窗口宽度和高度 */
#define WINDOW_WIDTH 8
#define WINDOW_HEIGHT 6

/* 输入数据的十进制最大值，这个值如果超过32位int的十进制位数，可能导致溢出 */
#define MAX_DIGIT 7

/* 时间间隔1 */
#define TIME_ELAPSE_1 2000

/* pass 是一个 Python 关键字，表示“空”，不执行任何功能 */
#define pass 0

#endif
