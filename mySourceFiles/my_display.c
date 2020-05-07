/*
 * 文件名: my_display.c
 * -------------------------------------
 * 这个文件实现了GUI显示相关函数
 *
 */

#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "my_macro.h"
#include "my_callback.h"
#include "my_display.h"
#include "draw_chart.h"
#include "my_resource.h"

void DisplayClear();  // 定义在 graphics.c

bool EraseStatus = false;  // 记录文字擦除状态，目前作调试用，检测后来加上的组件是否会对回调函数产生干扰

extern bool PauseAllProcedure;  // 定义在 my_callback.c
extern bool DisplayLineChart;  // 定义在 draw_chart.c ，测试用，未来将移除

/*
 * 函数名: PauseDisplay
 * -------------------------------------
 * 封装了停止GUI行为时所需的一些步骤
 */
void PauseDisplay() {
	display();                   // GUI刷新一次 
	PauseAllProcedure = true;    // 停止所有GUI行为 
}

/*
 * 函数名: ContinueDisplay
 * -------------------------------------
 * 封装了取消停止GUI行为时所需的一些步骤
 */
void ContinueDisplay() {
	startTimer(TIME_ELAPSE_1, TIME_ELAPSE_1);     // 重置计时器 
	PauseAllProcedure = false;   // 取消禁止所有GUI行为 
}

/*
 * 函数名: StretchDrawLine
 * 参数1: StartX    有向线段的起始位置的横坐标
 * 参数2: StartY    有向线段的起始位置的纵坐标
 * 参数3: StretchX  有向线段的横向份量
 * 参数4: StretchY  有向线段的纵向份量
 * -------------------------------------
 * 按有向线段的逻辑绘制线段，需要给出有向线段的
 * 起始点坐标和两个方向上的分量
 */
void StretchDrawLine(double StartX, double StartY, double StretchX, double StretchY)
{
	MovePen(StartX, StartY);
	DrawLine(StretchX, StretchY);
}

/*
 * 函数名: PointDrawLine
 * 参数1: StartX  线段的起始位置的横坐标
 * 参数2: StartY  线段的起始位置的纵坐标
 * 参数3: EndX    线段的终止位置的横坐标
 * 参数4: EndY    线段的终止位置的纵坐标
 * -------------------------------------
 * 按有向线段的逻辑绘制线段，需要给出有向线段的
 * 起始点坐标和两个方向上的分量
 */
void PointDrawLine(double StartX, double StartY, double EndX, double EndY)
{
	MovePen(StartX, StartY);
	DrawLine(EndX - StartX, EndY - StartY);
}

/*
 * 函数名: DrawMenu
 * -------------------------------------
 * 这个函数定义和绘制了菜单（目前未完成“绘制”）
 */
void DrawMenu()
{
	static char* MenuListFile[] = { "文件",
		"新建 无功能 | Ctrl-N",
		"打开 无功能 | Ctrl-O",
		"保存 无功能 | Ctrl-S",
		"关闭 无功能 | Ctrl-W",
		"退出 | Ctrl-Q" };

	static char* MenuListTool[] = { "编辑",
		"录入 无功能",
		"修改 无功能",
		"删除 无功能" };

	static char* MenuListDraw[] = { "绘图",
		"绘制图表 无功能" };

	static char* MenuListHelp[] = { "帮助",
		"使用帮助 无功能",
		"关于本软件 无功能" };


	const double MenuSelectionWidth = TextStringWidth(MenuListFile[0]) * 2;  // 菜单栏选项都是两个中文字
	const double MenuButtonHeight = GetFontHeight() * 1.5;  // 菜单栏每一个按钮的高度
	const double MenuBarVertical = GetWindowHeight() - MenuButtonHeight;  // 菜单栏的竖直位置


	{
		const int MenuListFileSelection = menuList(GenUIID(0), 0, MenuBarVertical,
			MenuSelectionWidth, TextStringWidth(MenuListFile[1]) * 1.2,
			MenuButtonHeight, MenuListFile, sizeof(MenuListFile) / sizeof(MenuListFile[0]));
		if (MenuListFileSelection == 5)
		{
			PauseDisplay();  // 暂停图形界面所有功能 
			const int selection = MessageBox(NULL, TEXT("您确定要退出吗？"),
				TEXT("提示"), MB_OKCANCEL | MB_ICONINFORMATION);
			if (selection == IDOK)
				exit(0);
			ContinueDisplay();  // 继续图形界面所有功能 
		}
	}

	{
		const int MenuListToolSelection = menuList(GenUIID(0), MenuSelectionWidth, MenuBarVertical,
			TextStringWidth(MenuListTool[0]) * 2, TextStringWidth(MenuListTool[1]) * 1.4,
			MenuButtonHeight, MenuListTool, sizeof(MenuListTool) / sizeof(MenuListTool[0]));
	}

	{
		const int MenuListDrawSelection = menuList(GenUIID(0), MenuSelectionWidth * 2, MenuBarVertical,
			TextStringWidth(MenuListDraw[0]) * 2, TextStringWidth(MenuListDraw[1]) * 1.4,
			MenuButtonHeight, MenuListDraw, sizeof(MenuListDraw) / sizeof(MenuListDraw[0]));
	}

	{
		const int MenuListHelpSelection = menuList(GenUIID(0), MenuSelectionWidth * 3, MenuBarVertical,
			TextStringWidth(MenuListHelp[0]) * 2, TextStringWidth(MenuListHelp[1]) * 1.4,
			MenuButtonHeight, MenuListHelp, sizeof(MenuListHelp) / sizeof(MenuListHelp[0]));
	}

}


/*
 * 函数名: display
 * -------------------------------------
 * 这个函数是可视化显示最核心的函数。
 * 几乎每一个 Windows 消息被捕获后，
 * 这个函数都会被回调函数调用。
 * 每一次调用，相关依赖的数据有变化，
 * 对应地，绘图窗口产生了变化。
 */
void display()
{
	DisplayClear();

	/* 目前用于调试，检测后来加上的组件是否会对回调函数产生干扰 */
	MovePen(0, 0.05);
	SetEraseMode(EraseStatus);
	DrawTextString("每隔2秒 或 鼠标右击 或 键盘输入时闪烁，\
		用于测试回调函数的行为是否符合预期。目前不要删除这个功能。");
	SetEraseMode(false);

	DrawMenu();  // 绘制菜单组件

	/* 目前用于测试折线图，后期将会删除 */
	MovePen(6, WINDOW_HEIGHT - 0.2);
	DrawTextString("F1键显示折线图");
	if (DisplayLineChart)  // 折线图功能测试函数
	{
		DrawLineChartFrame();
		DrawBrokenLine(Current);
		DrawBrokenLine(Total);
		DrawBrokenLine(Cured);
		DrawBrokenLine(Dead);
	}
}
