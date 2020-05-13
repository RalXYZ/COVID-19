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

extern int NeedMonth;//需求月份
extern int NeedDay;//需求日期
bool DisplayLineChart = false;  // 临时用作调试，在未来将会删除

extern int CurrentTheme;  // 当前主题序号，在 my_display.c 中定义
extern epidemic SentinelNode;  // 哨兵节点，在 my_resource.c 中声明
extern theme MyThemes[THEME_NUM];  // 存储主题的数组，在 my_display.c 中声明
extern DataProperty data;  // 链表相关属性值，在 my_resource.c 中声明

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

void DrawBrokenLine(int type)
{
	int start = 0, end = data.TotalDays - 1;  // 测试用，最终实现与此不同
	if (end - start <= 0)  // TODO 并未完全解决问题，应当妥善使用异常处理，而不是简单地退出函数
		return;

	const double step = (WINDOW_WIDTH - 2 * (SIDE_MARGIN + PADDING)) / (end - start);  // 步长，要求 start 大于 end
	const double LineChatHeight = BORDER_HEIGHT - 2 * PADDING;  // 折线图高度
	int count = 0;  // 计数器

	SetPenColor(MyThemes[CurrentTheme].accent);  // 暂时设为强调色，以后可能引入更多颜色
	for (epidemic* i = SentinelNode.next; i != nullptr && i->next != nullptr; i = i->next)  // 循环画割线
	{
		PointDrawLine(SIDE_MARGIN + PADDING + step * (count - start),
			BOTTOM_MARGIN + PADDING + LineChatHeight * (1.0 * i->properties[type] / data.MaxElement),
			SIDE_MARGIN + PADDING + step * (count + 1 - start),
			BOTTOM_MARGIN + PADDING + LineChatHeight * (1.0 * i->next->properties[type] / data.MaxElement));
		++count;
	}
}

double DataProportion(double x)//计算占比函数，仅饼状图使用
{
	double sum = (double)ReadEpidemicList(3, 2, Total) + (double)ReadEpidemicList(3, 2, Cured) + (double)ReadEpidemicList(3, 2, Dead);
	return x / sum * 360;
}

void FanChart()
{
	int radius = 2;//这个是半径，测试用预设为2
	double now[3] = { ReadEpidemicList(3, 2, Total), ReadEpidemicList(3, 2, Cured), ReadEpidemicList(3, 2, Dead) };
	double endX, endY;//这个记录每次画笔末尾坐标
	double centerX = GetWindowWidth() / 2;//圆心坐标
	double centerY = GetWindowHeight() / 2;
	double AngleSum = 0;//记录转过角度

	MovePen(centerX + 2, centerY);//绘图起点
	endX = centerX + 2;//绘图终点
	endY = centerY;

	int i;
	for (i = 0; i < 3; i++)//循环次数即显示参数数，每次执行一次画弧和一次画线
	{
		MovePen(endX, endY);
		DrawArc(radius, AngleSum, DataProportion(now[i]));
		AngleSum = AngleSum + DataProportion(now[i]);
		endX = GetCurrentX();
		endY = GetCurrentY();
		DrawLine(centerX - endX, centerY - endY);
	}
}

void BarChart(double x, double y, double w, double h, int month, int day, int n, int type, char* color)
{
	drawRectangle(x, y, GetWindowWidth() - 1, GetWindowHeight() - 1, 0);//测试用矩形区域
	SetPenColor(color);
	int i;

	for (i = 0; i < n; i++)
	{
		DateCalculate(month, day, i);
		drawRectangle(x+ w / (2 * n + 2), y, w / (2 * n + 2), ReadEpidemicList(NeedMonth, NeedDay, type) / 2, 1);
		x = x + w / (2 * n + 2);
	}
}
