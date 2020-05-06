/*
 * 文件名: my_macro.h
 * -------------------------------------
 * 这个头文件定义了一些被其余所有文件依赖的宏
 * 所有其他文件都应该包含这个头文件
 * 这个头文件不应依赖其他文件
 */

#ifndef _MY_MACRO_H_
#define _MY_MACRO_H_

 /* 窗口宽度和高度 */
#define WINDOW_WIDTH
#define WINDOW_HEIGHT

/* 时间间隔1 */
#define TIME_ELAPSE_1 2000

/* 异常枚举 */
enum error { Null, ExceedDate, UnformattedData };

/* pass 是一个 Python 关键字，表示“空”，不执行任何功能 */
#define pass 0

#endif
