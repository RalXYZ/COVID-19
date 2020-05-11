﻿/*
 * 文件名: draw_chart.h
 * -------------------------------------
 *
 */

#ifndef DRAW_CHART_H_
#define DRAW_CHART_H_

 /* 名命逻辑参考HTML，实现绘图区域大小可调 */

#define SIDE_MARGIN 0.5
#define BOTTOM_MARGIN 2.0
#define PADDING 0.25
#define BORDER_HEIGHT 3.0

/*
 * 函数名: DrawLineChartFrame
 * -------------------------------------
 * 折线图绘制函数，绘制包括边框、坐标轴和折线
 */
void DrawLineChartFrame();

/*
 * 函数名: DrawBrokenLine
 * 参数1: type  选择想要显示哪一个属性的折线图，建议使用EpidemicProperty枚举量
 * -------------------------------------
 * 绘制折线，目前处于测试状态，并未完全实现
 */
void DrawBrokenLine(int type);

/*
 * 函数名: FanChart
 * ------------------------------------
 * 绘制饼状图，表示当天数据占比分布
 */
void FanChart();
void BarChart(int x, int y, int day);

#endif