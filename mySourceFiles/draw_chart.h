﻿/*
 * 文件名: draw_chart.h
 * -------------------------------------
 *
 */

#ifndef DRAW_CHART_H_
#define DRAW_CHART_H_

 /* 折线图绘图区域的宏定义 */
#define PADDING 0.2

/* 统一图表区域的宏定义 */
#include "my_macro.h"
#define GZ_SIDE_MARGIN 0.5
#define GZ_BOTTOM_MARGIN 2.0
#define GZ_TOP_MARGIN 1.0
#define GZ_X GZ_SIDE_MARGIN                                      // 起始的x坐标
#define GZ_Y GZ_BOTTOM_MARGIN                                    // 起始的y坐标
#define GZ_W (WINDOW_WIDTH - 2 * GZ_SIDE_MARGIN)                 // 宽度
#define GZ_H (WINDOW_HEIGHT - GZ_BOTTOM_MARGIN - GZ_TOP_MARGIN)  // 高度

/*
 * 函数名: DataProportion
 * ------------------------------------
 * 计算占比函数，饼状图使用
 */
double DataProportion(double x);
/*
 * 函数名: FanChart
 * ------------------------------------
 * 绘制饼状图，表示当天数据占比分布
 */
void FanChart();
/*
 * 函数名: BarChart
 * 参数1: x 矩形框x坐标
 * 参数2: y 矩形框y坐标
 * 参数3: w 矩形框宽度
 * 参数4: h 矩形框高度
 * 参数5: month 绘图起始月份
 * 参数6: day 绘图起始日期
 * 参数7: n 向后绘图天数
 * 参数8: type 绘图数据名
 * 参数9: color 绘图颜色，方便多图同时显示时减少干扰（不过大概还是会有一些重叠问题
 * ------------------------------------
 * 绘制柱状图，表示一段时间内数据的多少
 */
void BarChart(double x, double y, double w, double h, int month, int day, int n, int type, char* color);
/*
 * 函数名: DrawChart
 * 参数1: month 柱状图绘图起始月份
 * 参数2: day 柱状图绘图起始日期
 * 参数3: n 柱状图向后绘图天数
 * 参数4: type1 绘图数据名1用于折线图
 * 参数5: type2 绘图数据名2用于柱状图
 * 参数6: color 柱状图颜色
 * ------------------------------------
 * 在指定位置通过响应按钮画图
 */
void DrawChart(int month, int day, int n, int type1, int type2, char* color);
#endif
