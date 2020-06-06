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
#include "my_utilities.h"
#include "prediction_model.h"

extern int NeedMonth;//需求月份
extern int NeedDay;//需求日期
_Bool DisplayLineChart = false;  //折线图标志
_Bool DisplayFanChart = false;  //饼状图标志
_Bool DisplayBarChart = false;  //柱状图标志

extern int CurrentTheme;  // 当前主题序号，在 my_display.c 中定义
extern epidemic SentinelNode;  // 哨兵节点，在 my_resource.c 中声明
extern theme MyThemes[THEME_NUM];  // 存储主题的数组，在 my_display.c 中声明
extern DataProperty data;  // 链表相关属性值，在 my_resource.c 中声明
extern MyStatus status;  // 当前状态，在 my_resource.c 中定义

/*
 * 函数名: DrawLineChartFrame
 * -------------------------------------
 * 绘制折线图边框和横轴、纵轴
 * 参数1: x 矩形框x坐标
 * 参数2: y 矩形框y坐标
 * 参数3: w 矩形框宽度
 * 参数4: h 矩形框高度
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
 * 函数名: DisplayStatistics
 * -------------------------------------
 * 显示当日的具体数据
 */
static void DisplayStatistics()
{
	char date[20] = "";
	SetPenColor(MyThemes[CurrentTheme].accent);
	sprintf(date, "%d月%d日：",
		status.HighlightNode->properties[Month],
		status.HighlightNode->properties[Day]);

	MovePen(2.5, 0.05);
	DrawTextString(date);
	for (int i = EPIDEMIC_PROPERTY_START; i < EPIDEMIC_ELEMENT_NUM; i++)
	{
		char property[20] = "";
		SetPenColor(GetEpidemicColor(i));
		if (i == status.HighlightProperty)  // 如果该项目为当前高亮项目
			SetStyle(1);  // 粗体
		switch (i)
		{
		case New:
			sprintf(property, "%s%d人 ", PropertyMeaning(New),
				status.HighlightNode->properties[New]);
			break;
		case Current:
			sprintf(property, "%s%d人 ", PropertyMeaning(Current),
				status.HighlightNode->properties[Current]);
			break;
		case Cured:
			sprintf(property, "%s%d人 ", PropertyMeaning(Cured),
				status.HighlightNode->properties[Cured]);
			break;
		case Dead:
			sprintf(property, "%s%d人", PropertyMeaning(Dead),
				status.HighlightNode->properties[Dead]);
			break;
		}
		DrawTextString(property);
		SetStyle(0);  // 恢复字体样式为正常状态
	}
}

/*
 * 函数名: Highlight
 * -------------------------------------
 * 显示高亮光标
 */
static void Highlight(double x, double y, double w, double h)
{
	const double LineChatHeight = h - 2 * PADDING;
	const double HeightInGraph = 1.0 * status.HighlightNode->properties[status.HighlightProperty];
	double WidthInGraph = 1.0 * status.HighlightNum * (w - 2 * PADDING) / (data.TotalDays * 1.0 - 1);

	SetPenSize(1);
	SetPenColor(MyThemes[CurrentTheme].accent);

	StretchDrawLine(x + PADDING,  // 画横向
		y + PADDING + LineChatHeight * (HeightInGraph / data.MaxElement),
		w - 2 * PADDING, 0);
	StretchDrawLine(x + PADDING + WidthInGraph,  // 画纵向
		y + PADDING, 0, h - 2 * PADDING);
}

/*
 * 函数名: DrawBrokenLine
 * 参数1: x 矩形框x坐标
 * 参数2: y 矩形框y坐标
 * 参数3: w 矩形框宽度
 * 参数4: h 矩形框高度
 * 参数5: type  选择想要显示哪一个属性的折线图，建议使用EpidemicProperty枚举量
 * -------------------------------------
 * 绘制折线
 */
static void DrawBrokenLine(double x, double y, double w, double h, int month, int day, int n, int type)
{
	int start = 0;
	int end;  // 测试用，最终实现与此不同
	if (status.ZoomIn)
		end = n;
	else
		end = n - 1;

	if (end - start <= 0)  // TODO 并未完全解决问题，应当妥善使用异常处理，而不是简单地退出函数
		return;

	const double step = (w - 2 * PADDING) / (end - start);  // 步长，要求 start 大于 end
	const double LineChatHeight = h - 2 * PADDING;  // 折线图高度
	int count = 0;  // 计数器

	epidemic* StartNode = SentinelNode.next;
	while (StartNode->properties[Day] != day || StartNode->properties[Month] != month)
		StartNode = StartNode->next;

	if (type == status.HighlightProperty)
		SetPenSize(2);
	SetPenColor(GetEpidemicColor(type));
	for (epidemic* i = StartNode; count < n && i->next != nullptr; i = i->next)  // 循环画割线
	{
		PointDrawLine(x + PADDING + step * (count - start),
			y + PADDING + LineChatHeight * (1.0 * i->properties[type] / data.MaxElement),
			x + PADDING + step * (count + 1 - start),
			y + PADDING + LineChatHeight * (1.0 * i->next->properties[type] / data.MaxElement));
		++count;
	}
	SetPenSize(1);  // 将笔触宽度恢复到初始状态
}

void LineChart(double x, double y, double w, double h, int month, int day, int n)
{
	DrawBrokenLine(x, y, w, h, month, day, n, Current);
	DrawBrokenLine(x, y, w, h, month, day, n, New);
	DrawBrokenLine(x, y, w, h, month, day, n, Cured);
	DrawBrokenLine(x, y, w, h, month, day, n, Dead);

	DrawLineChartFrame(x, y, w, h);

	if (status.HighlightVisible)
		Highlight(x, y, w, h);
}

double DataProportion(double x)//计算占比函数，饼状图使用
{
	const int month = status.HighlightNode->properties[Month];
	const int day = status.HighlightNode->properties[Day];
	double sum = (double)ReadEpidemicList(month, day, Current) + (double)ReadEpidemicList(month, day, Cured) + (double)ReadEpidemicList(month, day, Dead);
	return x / sum * 360;
}

/*
 * 函数名: FanChart
 * ------------------------------------
 * 绘制饼状图，表示当天数据占比分布
 */
static void FanChart(double centerX, double centerY, double radius)
{
	const int month = status.HighlightNode->properties[Month];
	const int day = status.HighlightNode->properties[Day];
	double now[3] = { ReadEpidemicList(month, day, Dead), ReadEpidemicList(month, day, Cured), ReadEpidemicList(month, day, Current) };

	double AngleSum = 0;//记录转过角度

	MovePen(centerX + radius, centerY);//绘图起点
	double VectorX = radius;//绘图终点
	double VectorY = 0;

	SetPenColor(MyThemes[CurrentTheme].accent);

	if (!now[0] && !now[1] && !now[2])
	{
		DrawArc(radius, 0, 360);
		return;
	}

	//循环次数即显示参数数，每次执行一次画弧和一次画线
	for (int i = 0; i < 3; i++)
	{
		MovePen(centerX, centerY);
		switch (i)
		{
		case 0:
			SetPenColor(GetEpidemicColor(Dead));
			break;
		case 1:
			SetPenColor(GetEpidemicColor(Cured));
			break;
		case 2:
			SetPenColor(GetEpidemicColor(Current));
			break;
		}
		StartFilledRegion(1);
		DrawLine(VectorX, VectorY);
		DrawArc(radius, AngleSum, DataProportion(now[i]));
		AngleSum = AngleSum + DataProportion(now[i]);
		VectorX = GetCurrentX() - centerX;
		VectorY = GetCurrentY() - centerY;
		DrawLine(-VectorX, -VectorY);
		EndFilledRegion();
	}
}

/*
 * 函数名: BarChart
 * 参数1: x 矩形框x坐标
 * 参数2: y 矩形框y坐标
 * 参数3: w 矩形框宽度
 * 参数4: h 矩形框高度
 * 参数5: month 绘图起始月份
 * 参数6: day 绘图起始日期
 * 参数7: n 向后绘图天数
 * ------------------------------------
 * 绘制柱状图，表示一段时间内数据的多少
 */
static void BarChart(double x, double y, double w, double h, int month, int day, int n)
{
	/*坐标变换*/
	double xt = x + PADDING;
	double yt = y + PADDING;
	double wt = w - 2 * PADDING;
	double ht = h - 2 * PADDING;

	SetPenColor(MyThemes[CurrentTheme].foreground);
	drawRectangle(xt, yt, wt, ht, 0);  // 测试用矩形区域
	int i;
	const int CurrentMonth = status.HighlightNode->properties[Month];
	const int CurrentDay = status.HighlightNode->properties[Day];

	//采用逐段画矩形的方式绘制柱状图
	for (i = 0; i < n; i++)
	{
		DateCalculate(month, day, i);
		if (NeedDay == CurrentDay && NeedMonth == CurrentMonth)
			SetPenColor(MyThemes[CurrentTheme].accent);  // 临时
		else
			SetPenColor(GetEpidemicColor(status.HighlightProperty));
		double pro = ReadEpidemicList(NeedMonth, NeedDay, status.HighlightProperty) / (1.0 * data.MaxElement);
		drawRectangle(xt + wt / (2.0 * n + 1), yt, wt / (2.0 * n + 1), 1.0 * ht * pro, 1);
		xt += 2 * wt / (2.0 * n + 1);
	}

	DrawLineChartFrame(x, y, w, h);
}

void DrawChart(int month, int day, int n)
{
	const double width = GetWindowWidth();
	const double height = GetWindowHeight();
	extern int MyButton(int id, double x, double y, double w, double h, char* label);  // 在 imgui.c 中定义

	//绘制按钮部分
	if (MyButton(GenUIID(0), width / 8, 0.55, width / 6, 3 * height / 32, "折线图"))
	{
		DisplayLineChart = !DisplayLineChart;
		GUIOutputMsg("折线图");
	}
	if (MyButton(GenUIID(1), 10 * width / 24, 0.55, width / 6, 3 * height / 32, "柱状图"))
	{
		DisplayBarChart = !DisplayBarChart;
		GUIOutputMsg("柱状图");
	}
	if (MyButton(GenUIID(2), 17 * width / 24, 0.55, width / 6, 3 * height / 32, "饼状图"))
	{
		DisplayFanChart = !DisplayFanChart;
		GUIOutputMsg("饼状图");
	}

	//穷举所有按钮情况，并给出对应显示
	if (DisplayBarChart == 1 && DisplayFanChart == 0 && DisplayLineChart == 0)
	{
		BarChart(width / 12, height / 4, 5 * width / 6, 5 * height / 8, month, day, n);
	}
	else if (DisplayBarChart == 0 && DisplayFanChart == 1 && DisplayLineChart == 0)
	{
		SetPenColor(MyThemes[CurrentTheme].foreground);
		drawRectangle(7 * width / 24, height / 4, 5 * width / 12, 5 * height / 8, 0);
		FanChart(width / 2, 9 * height / 16, width / 6);
	}
	else if (DisplayBarChart == 0 && DisplayFanChart == 0 && DisplayLineChart == 1)
	{
		LineChart(width / 12, height / 4, 5 * width / 6, 5 * height / 8, month, day, n);
	}
	else if (DisplayBarChart == 1 && DisplayFanChart == 1 && DisplayLineChart == 0)
	{
		BarChart(width / 12, height / 4, 7 * width / 12, 5 * height / 8, month, day, n);
		SetPenColor(MyThemes[CurrentTheme].foreground);
		drawRectangle(16 * width / 24, height / 4, width / 4, 5 * height / 8, 0);
		FanChart(19 * width / 24, 9 * height / 16, 5 * width / 48);
	}
	else if (DisplayBarChart == 1 && DisplayFanChart == 0 && DisplayLineChart == 1)
	{
		BarChart(width / 12, height / 4, 5 * width / 6, 5 * height / 16, month, day, n);
		LineChart(width / 12, 9 * height / 16, 5 * width / 6, 5 * height / 16, month, day, n);
	}
	else if (DisplayBarChart == 0 && DisplayFanChart == 1 && DisplayLineChart == 1)
	{
		SetPenColor(MyThemes[CurrentTheme].foreground);
		drawRectangle(16 * width / 24, height / 4, width / 4, 5 * height / 8, 0);
		FanChart(19 * width / 24, 9 * height / 16, 5 * width / 48);
		LineChart(width / 12, height / 4, 7 * width / 12, 5 * height / 8, month, day, n);
	}
	else if (DisplayBarChart == 1 && DisplayFanChart == 1 && DisplayLineChart == 1)
	{
		BarChart(width / 12, height / 4, 7 * width / 12, 5 * height / 16, month, day, n);
		SetPenColor(MyThemes[CurrentTheme].foreground);
		drawRectangle(16 * width / 24, height / 4, width / 4, 5 * height / 8, 0);
		FanChart(19 * width / 24, 9 * height / 16, 5 * width / 48);
		LineChart(width / 12, 9 * height / 16, 7 * width / 12, 5 * height / 16, month, day, n);
	}

	DisplayStatistics();  // 绘制右下角的统计数据
}
