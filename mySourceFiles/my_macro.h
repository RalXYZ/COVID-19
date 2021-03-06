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

/* “新建”功能的文件目录 */
#define NEW_FILE_DIR ".\\new.covid19"

/* 对比模式下，对比数据的配色 */
#define COMPARE_COLOR "15763017"  //警告：请保证 0xF08649 颜色已被定义。该颜色为饱和度稍低的橙色。

/* 文件名最大长度 */
#define MAX_FILE_NAME_LENGTH 20

/* 输入数据的十进制最大值，这个值如果超过32位int的十进制位数，可能导致溢出 */
#define MAX_DIGIT 7

/* 链表的最短长度 */
#define MIN_LIST_LENGTH 3

/* 时间间隔1 */
#define TIME_ELAPSE_1 2000

/* pass 是一个 Python 关键字，表示“空”，不执行任何功能 */
#define pass 0

#endif
