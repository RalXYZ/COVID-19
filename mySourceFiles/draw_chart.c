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
#include "my_resource.h"

bool DisplayLineChart = false;

extern epidemic SentinelNode;

/*
 * 函数名: DrawLineChartFrame
 * -------------------------------------
 * 折线图绘制函数，绘制包括边框、坐标轴和折线
 */
void DrawLineChartFrame()
{
	/* 画边框 */
	drawRectangle(SIDE_MARGIN, BOTTOM_MARGIN, WINDOW_WIDTH - SIDE_MARGIN * 2, BORDER_HEIGHT, 0);

	/* 画横轴、纵轴 */
	PointDrawLine(SIDE_MARGIN + PADDING, BOTTOM_MARGIN + PADDING,
		WINDOW_WIDTH - SIDE_MARGIN - PADDING, BOTTOM_MARGIN + PADDING);
	PointDrawLine(SIDE_MARGIN + PADDING, BOTTOM_MARGIN + PADDING,
		SIDE_MARGIN + PADDING, BOTTOM_MARGIN + BORDER_HEIGHT - PADDING);
}

/*
 * 函数名: DrawBrokenLine
 * -------------------------------------
 * 绘制折线，目前处于测试状态，并未完全实现
 */
void DrawBrokenLine()
{
	int max = -1;  // 由于统计数据一定非负，那么就把最大值初始化为负数
	for (epidemic* i = SentinelNode.next; i != nullptr; i = i->next)
		max < i->confirmed.total ? max = i->confirmed.total : pass;

	int start = 0, end = 29;  //测试用，最终实现与此不同
	const double step = (WINDOW_WIDTH - 2 * (SIDE_MARGIN + PADDING)) / (end - start);  // 步长，要求 start 大于 end
	const double LineChatHeight = BORDER_HEIGHT - 2 * PADDING;  // 折线图高度
	int count = 0;
	for (epidemic* i = SentinelNode.next; i != nullptr && i->next != nullptr; i = i->next)  // 循环画割线
	{
		PointDrawLine(SIDE_MARGIN + PADDING + step * (count - start),
			BOTTOM_MARGIN + PADDING + LineChatHeight * (1.0 * i->confirmed.total / max),
			SIDE_MARGIN + PADDING + step * (count + 1 - start),
			BOTTOM_MARGIN + PADDING + LineChatHeight * (1.0 * i->next->confirmed.total / max));
		++count;
	}


	/*
	用 Python 快速获得一组符合标准的数据：
	for i in range(0, 30):
		print("3-" + str(i + 1), end = ' ')
		print(int(50 * (math.sin(0.1 * i) + 1)), end = ' ')
		print(int(50 * (math.cos(0.3 * i) + 1)), end = ' ')
		print(int(100 * math.log(0.1 * i + 1)), end = ' ' )
		print(int(20 * math.sqrt(i)), end = '\n' )
	 */
}


void FanChart(int x, int y, int day)
{

}

void BarChart(int x, int y, int day)
{

}
