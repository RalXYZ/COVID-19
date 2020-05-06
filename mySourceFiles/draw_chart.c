/*
 * 文件名: draw_chart.c
 * -------------------------------------
 * 这个文件实现折线图、饼状图、柱状图的绘制
 * 需要借助链表，此处与链表的连接部分暂时留白
 */

#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "my_macro.h"
#include "my_callback.h"
#include "draw_chart.h"
#include "my_display.h"

bool DisplayLineChart = false;

/*
 * 函数名: LineChart
 * -------------------------------------
 * 折线图绘制函数，绘制包括边框、坐标轴和折线
 */
void LineChart()
{
#define DEBUG_ARRAY_LENGTH 40  // 测试用，未来将删除
	static int DebugArray[DEBUG_ARRAY_LENGTH] = { 134, 157, 178, 194, 204, 209, 208, 200,
		187, 169, 148, 124, 99, 74, 52, 34, 20, 12, 10, 14, 24, 39, 59, 82, 106, 131, 155,
		175, 192, 203, 209, 208, 202, 189, 172, 151, 127, 102, 78, 55, };  // 测试用，最终实现与此不同

	int max = -1;  // 由于统计数据数量一定非负，那么就把最大值初始化为负数
	for (int i = 0; i < DEBUG_ARRAY_LENGTH; i++)
		max < DebugArray[i] ? max = DebugArray[i] : pass;

	/* 画边框 */
	drawRectangle(SIDE_MARGIN, BOTTOM_MARGIN, WINDOW_WIDTH - SIDE_MARGIN * 2, BORDER_HEIGHT, 0);

	/* 画横轴、纵轴 */
	PointDrawLine(SIDE_MARGIN + PADDING, BOTTOM_MARGIN + PADDING,
		WINDOW_WIDTH - SIDE_MARGIN - PADDING, BOTTOM_MARGIN + PADDING);
	PointDrawLine(SIDE_MARGIN + PADDING, BOTTOM_MARGIN + PADDING,
		SIDE_MARGIN + PADDING, BOTTOM_MARGIN + BORDER_HEIGHT - PADDING);

	int start = 0, end = DEBUG_ARRAY_LENGTH - 1;  //测试用，最终实现与此不同

	double step = (WINDOW_WIDTH - 2 * (SIDE_MARGIN + PADDING)) / (end - start);  // 步长，要求 start 大于 end
	const double LineChatHeight = BORDER_HEIGHT - 2 * PADDING;  // 折线图高度
	for (int i = start; i < end; i++)  // 循环画割线
		PointDrawLine(SIDE_MARGIN + PADDING + step * i,
			BOTTOM_MARGIN + PADDING + LineChatHeight * (1.0 * DebugArray[i] / max),
			SIDE_MARGIN + PADDING + step * (i + 1),
			BOTTOM_MARGIN + PADDING + LineChatHeight * (1.0 * DebugArray[i + 1] / max));
}


void FanChart(int x, int y, int day)
{

}

void BarChart(int x, int y, int day)
{

}
