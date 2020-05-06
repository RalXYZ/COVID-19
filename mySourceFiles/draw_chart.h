/*
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
 * 函数名: LineChart
 * -------------------------------------
 * 折线图绘制函数，绘制包括边框、坐标轴和折线
 */
void LineChart();



void FanChart(int x, int y, int day);
void BarChart(int x, int y, int day);

#endif
