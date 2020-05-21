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
#include "prediction_model.h"

extern int NeedMonth;//需求月份
extern int NeedDay;//需求日期
static int line = 0;//折线图标志
static int fan = 0;//饼状图标志
static int bar = 0;//柱状图标志

bool DisplayLineChart = false;  // 临时用作调试，在未来将会删除

extern int CurrentTheme;  // 当前主题序号，在 my_display.c 中定义
extern epidemic SentinelNode;  // 哨兵节点，在 my_resource.c 中声明
extern theme MyThemes[THEME_NUM];  // 存储主题的数组，在 my_display.c 中声明
extern DataProperty data;  // 链表相关属性值，在 my_resource.c 中声明

/*
 * 函数名: DrawLineChartFrame
 * -------------------------------------
 * 折线图绘制函数，绘制包括边框、坐标轴和折线
 */
static void DrawLineChartFrame(double x, double y, double w, double h)
{
	SetPenColor(MyThemes[CurrentTheme].foreground);

	/* 画边框 */
	drawRectangle(x, y, w, h, 0);

	/* 画横轴、纵轴 */
	PointDrawLine(x + PADDING, y + PADDING,
		x + w - PADDING, y + PADDING);
	PointDrawLine(x + PADDING, y + PADDING,
		x + PADDING, y + h - PADDING);
}

/*
 * 函数名: DrawBrokenLine
 * 参数1: type  选择想要显示哪一个属性的折线图，建议使用EpidemicProperty枚举量
 * -------------------------------------
 * 绘制折线，目前处于测试状态，并未完全实现
 */
static void DrawBrokenLine(double x, double y, double w, double h, int type)
{
	int start = 0, end = data.TotalDays - 1;  // 测试用，最终实现与此不同
	if (end - start <= 0)  // TODO 并未完全解决问题，应当妥善使用异常处理，而不是简单地退出函数
		return;

	const double step = (w - 2 * PADDING) / (end - start);  // 步长，要求 start 大于 end
	const double LineChatHeight = h - 2 * PADDING;  // 折线图高度
	int count = 0;  // 计数器

	SetPenColor(MyThemes[CurrentTheme].accent);  // 暂时设为强调色，以后可能引入更多颜色
	for (epidemic* i = SentinelNode.next; i != nullptr && i->next != nullptr; i = i->next)  // 循环画割线
	{
		PointDrawLine(x + PADDING + step * (count - start),
			y + PADDING + LineChatHeight * (1.0 * i->properties[type] / data.MaxElement),
			x + PADDING + step * (count + 1 - start),
			y + PADDING + LineChatHeight * (1.0 * i->next->properties[type] / data.MaxElement));
		++count;
	}
}

void LineChart(double x, double y, double w, double h)
{
	DrawBrokenLine(x, y, w, h, Current);
	DrawBrokenLine(x, y, w, h, Total);
	DrawBrokenLine(x, y, w, h, Cured);
	DrawBrokenLine(x, y, w, h, Dead);
	DrawLineChartFrame(x, y, w, h);
}

double DataProportion(double x)//计算占比函数，饼状图使用
{
	double sum = (double)ReadEpidemicList(3, 2, Total) + (double)ReadEpidemicList(3, 2, Cured) + (double)ReadEpidemicList(3, 2, Dead);
	return x / sum * 360;
}

void FanChart(double centerX, double centerY, double radius)
{
	double now[3] = { ReadEpidemicList(3, 2, Total), ReadEpidemicList(3, 2, Cured), ReadEpidemicList(3, 2, Dead) };
	double endX, endY;//这个记录每次画笔末尾坐标

	double AngleSum = 0;//记录转过角度

	MovePen(centerX + radius, centerY);//绘图起点
	endX = centerX + radius;//绘图终点
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
	drawRectangle(x, y, w, h, 0);//测试用矩形区域
	SetPenColor(color);
	int i;

	for (i = 0; i < n; i++)
	{
		DateCalculate(month, day, i);
		drawRectangle(x + w / (2 * n + 2), y, w / (2 * n + 2), ReadEpidemicList(NeedMonth, NeedDay, type) / (1.0 * data.MaxElement), 1);
		x += 2 * w / (2 * n + 2);
	}
}

void DrawChart(int month, int day, int n, int type1, int type2, char* color)
{
	double wid = GetWindowWidth();
	double hei = GetWindowHeight();
	if (button(GenUIID(0), wid / 8, hei / 16, wid / 6, 3 * hei / 32, "柱状图"))
	{
		bar = !bar;
	}
	if (button(GenUIID(1), 10 * wid / 24, hei / 16, wid / 6, 3 * hei / 32, "饼状图"))
	{
		fan = !fan;
	}
	if (button(GenUIID(2), 17 * wid / 24, hei / 16, wid / 6, 3 * hei / 32, "折线图"))
	{
		line = !line;
	}
	if (bar == 1 && fan == 0 && line == 0)
	{
		BarChart(wid / 12, hei / 4, 5 * wid / 6, 5 * hei / 8, month, day, n, type2, color);
	}
	if (bar == 0 && fan == 1 && line == 0)
	{
		drawRectangle(7 * wid / 24, hei / 4, 5 * wid / 12, 5 * hei / 8, 0);
		FanChart(wid / 2, 9 * hei / 16, wid / 6);
	}
	if (bar == 0 && fan == 0 && line == 1)
	{
		LineChart(wid / 12, 9 * hei / 16, 5 * wid / 6, 5 * hei / 16);
	}
	if (bar == 1 && fan == 1 && line == 0)
	{
		BarChart(wid / 12, hei / 4, 7 * wid / 12, 5 * hei / 8, month, day, n, type2, color);
		drawRectangle(16 * wid / 24, hei / 4, wid / 4, 5 * hei / 8, 0);
		FanChart(19 * wid / 24, 9 * hei / 16, 5 * wid / 48);
	}
	if (bar == 1 && fan == 0 && line == 1)
	{
		BarChart(wid / 12, hei / 4, 5 * wid / 6, 5 * hei / 16, month, day, n, type2, color);
		LineChart(wid / 12, 9 * hei / 16, 5 * wid / 6, 5 * hei / 16);
	}
	if (bar == 0 && fan == 1 && line == 1)
	{
		drawRectangle(16 * wid / 24, hei / 4, wid / 4, 5 * hei / 8, 0);
		FanChart(19 * wid / 24, 9 * hei / 16, 5 * wid / 48);
		LineChart(wid / 12, hei / 4, 7 * wid / 12, 5 * hei / 8);
	}
	if (bar == 1 && fan == 1 && line == 1)
	{
		BarChart(wid / 12, hei / 4, 7 * wid / 12, 5 * hei / 16, month, day, n, type2, color);
		drawRectangle(16 * wid / 24, hei / 4, wid / 4, 5 * hei / 8, 0);
		FanChart(19 * wid / 24, 9 * hei / 16, 5 * wid / 48);
		LineChart(wid / 12, 9 * hei / 16, 7 * wid / 12, 5 * hei / 16);
	}
}
