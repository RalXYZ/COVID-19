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

extern int EpidemicListLength;  // 疫情链表长度，从1开始
extern int EpidemicElementMax;  // 疫情链表属性数据中的最大值，用于决定折线图的缩放
extern epidemic SentinelNode;  // 哨兵节点，在 my_resource.c 中声明

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
 * 参数1: type  选择想要显示哪一个属性的折线图，建议使用EpidemicProperty枚举量
 * -------------------------------------
 * 绘制折线，目前处于测试状态，并未完全实现
 */
void DrawBrokenLine(int type)
{
	int start = 0, end = EpidemicListLength - 1;  //测试用，最终实现与此不同
	if (end - start <= 0)  //TODO 并未完全解决问题，应当妥善使用异常处理，而不是简单地退出函数
		return;

	const double step = (WINDOW_WIDTH - 2 * (SIDE_MARGIN + PADDING)) / (end - start);  // 步长，要求 start 大于 end
	const double LineChatHeight = BORDER_HEIGHT - 2 * PADDING;  // 折线图高度
	int count = 0;  // 计数器
	for (epidemic* i = SentinelNode.next; i != nullptr && i->next != nullptr; i = i->next)  // 循环画割线
	{
		PointDrawLine(SIDE_MARGIN + PADDING + step * (count - start),
			BOTTOM_MARGIN + PADDING + LineChatHeight * (1.0 * i->properties[type] / EpidemicElementMax),
			SIDE_MARGIN + PADDING + step * (count + 1 - start),
			BOTTOM_MARGIN + PADDING + LineChatHeight * (1.0 * i->next->properties[type] / EpidemicElementMax));
		++count;
	}

	/*
	# 用 Python 快速获得一组符合标准的数据（直接把这段注释复制过去）：
	import math
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
