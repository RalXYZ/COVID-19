/*
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
 * 函数名: LineChart
 * 参数1: x 矩形框x坐标
 * 参数2: y 矩形框y坐标
 * 参数3: w 矩形框宽度
 * 参数4: h 矩形框高度
 * ------------------------------------
 * 绘制折线图
 */
void LineChart(double x, double y, double w, double h);

/*
 * 函数名: DataProportion
 * ------------------------------------
 * 计算占比函数，饼状图使用
 */
double DataProportion(double x);

/*
 * 函数名: DrawChart
 * 参数1: month 柱状图绘图起始月份
 * 参数2: day 柱状图绘图起始日期
 * 参数3: n 柱状图向后绘图天数
 * ------------------------------------
 * 在指定位置通过响应按钮画图
 */
void DrawChart(int month, int day, int n);
/*
 * 函数名: DrawPredictionInterface
 * -------------------------------------
 * 本函数提供进行疫情预测的界面
 *
 */
void DrawPredictionInterface();
#endif
