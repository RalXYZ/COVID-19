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
#include "strlib.h"
#include "imgui.h"

#include "my_macro.h"
#include "my_callback.h"
#include "my_display.h"
#include "draw_chart.h"
#include "my_resource.h"

void DisplayClear();  // 定义在 graphics.c
int CurrentTheme = 3;  // 当前主题序号
theme MyThemes[THEME_NUM];  // 建立存储数量为 THEME_NUM 个主题的字符数组

bool EraseStatus = false;  // 记录文字擦除状态，目前作调试用，检测后来加上的组件是否会对回调函数产生干扰

extern bool PauseAllProcedure;  // 定义在 my_callback.c
extern bool DisplayLineChart;  // 定义在 draw_chart.c ，测试用，未来将移除

/*
 * 函数名: PauseDisplay
 * -------------------------------------
 * 封装了停止GUI行为时所需的一些步骤
 */
static void PauseDisplay() {
	display();                   // GUI刷新一次 
	PauseAllProcedure = true;    // 停止所有GUI行为 
}

/*
 * 函数名: ContinueDisplay
 * -------------------------------------
 * 封装了取消停止GUI行为时所需的一些步骤
 */
static void ContinueDisplay() {
	startTimer(TIME_ELAPSE_1, TIME_ELAPSE_1);     // 重置计时器 
	PauseAllProcedure = false;   // 取消禁止所有GUI行为 
}

/*
 * HexDefineColor
 * 参数1: name   指向你想处理的字符串的指针
 * 参数2: value  十六进制颜色码值
 * -------------------------------------
 * libgraphics 的新颜色定义中的红绿蓝值是0到1之间的实数，
 * 这不符合通用规范。
 * 本函数实现通过十六进制颜色码值定义新颜色。
 */
static void HexDefineColor(string name, int value) {
#define DEC_TO_REAL(input) ((input)/256.0)
	int DecValue[3];
	for (int i = 2; i >= 0; i--) {
		DecValue[i] = value % 0x100;           // 模256，相当于取16进制数的末两位
		value /= 0x100;                        // 舍去末两位
	}
	DefineColor(name, DEC_TO_REAL(DecValue[0]), DEC_TO_REAL(DecValue[1]), DEC_TO_REAL(DecValue[2]));
#undef DEC_TO_REAL                             // 解除宏定义，防止污染 
}

/*
 * InputMyColors
 * 参数1: position            该主题在结构数组中的存储位置
 * 参数2: BackgroundColorHex  背景色十六进制颜色码值
 * 参数3: ForegroundColorHex  前景色十六进制颜色码值
 * 参数4: AccentColorHex      强调色十六进制颜色码值
 * -------------------------------------
 * 输入一组（3个）有序的十六进制颜色码，将它们存储在给定的位置上
 */
static void InputMyColors(int position, char* name,
	int BackgroundColorHex, int ForegroundColorHex, int AccentColorHex) {
	MyThemes[position].name = name;

	MyThemes[position].background = IntegerToString(BackgroundColorHex);
	HexDefineColor(MyThemes[position].background, BackgroundColorHex);

	MyThemes[position].foreground = IntegerToString(ForegroundColorHex);
	HexDefineColor(MyThemes[position].foreground, ForegroundColorHex);

	MyThemes[position].accent = IntegerToString(AccentColorHex);
	HexDefineColor(MyThemes[position].accent, AccentColorHex);
}

// 注意：更改这个函数的同时也要更改 THEME_NUM 宏
void InitColor() {
	InputMyColors(0, "苍松", 0x203227, 0x637B6D, 0x9CC2AD);
	InputMyColors(1, "红豆", 0xB9B2B4, 0x1B121F, 0x7A0B0E);
	InputMyColors(2, "蔷薇", 0x312E31, 0xFFB6B9, 0xFF5F7F);
	InputMyColors(3, "万圣节", 0x1C162B, 0xB94E0A, 0xC38112);
	InputMyColors(4, "熊猫", 0x222222, 0x828282, 0xEAEAEA);
	InputMyColors(5, "蓝鹊", 0xC9D1D9, 0x4780BA, 0xBA5255);
}

void StretchDrawLine(double StartX, double StartY, double StretchX, double StretchY)
{
	MovePen(StartX, StartY);
	DrawLine(StretchX, StretchY);
}

void PointDrawLine(double StartX, double StartY, double EndX, double EndY)
{
	MovePen(StartX, StartY);
	DrawLine(EndX - StartX, EndY - StartY);
}

extern int MyMenuList(int id, double x, double y, double w, double wlist, double h, char* labels[], int n);
/*
 * 函数名: DrawMenu
 * -------------------------------------
 * 这个函数定义和绘制了菜单（目前未完成“绘制”）
 */
static void DrawMenu()
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

	static char* MenuListDisplay[] = { "视图",
		"切换主题" };

	static char* MenuListHelp[] = { "帮助",
		"使用帮助 无功能",
		"关于本软件 无功能" };


	const double MenuSelectionWidth = TextStringWidth(MenuListFile[0]) * 2;  // 菜单栏选项都是两个中文字
	const double MenuButtonHeight = GetFontHeight() * 1.5;  // 菜单栏每一个按钮的高度
	const double MenuBarVertical = GetWindowHeight() - MenuButtonHeight;  // 菜单栏的竖直位置
	static char temp[40] = { 0 };


	{
		const int MenuListFileSelection = MyMenuList(GenUIID(0), 0, MenuBarVertical,
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
		const int MenuListToolSelection = MyMenuList(GenUIID(0), MenuSelectionWidth, MenuBarVertical,
			TextStringWidth(MenuListTool[0]) * 2, TextStringWidth(MenuListTool[1]) * 1.2,
			MenuButtonHeight, MenuListTool, sizeof(MenuListTool) / sizeof(MenuListTool[0]));
	}

	{
		const int MenuListDrawSelection = MyMenuList(GenUIID(0), MenuSelectionWidth * 2, MenuBarVertical,
			TextStringWidth(MenuListDraw[0]) * 2, TextStringWidth(MenuListDraw[1]) * 1.2,
			MenuButtonHeight, MenuListDraw, sizeof(MenuListDraw) / sizeof(MenuListDraw[0]));
	}

	{
		const int MenuListDisplaySelection = MyMenuList(GenUIID(0), MenuSelectionWidth * 3, MenuBarVertical,
			TextStringWidth(MenuListDisplay[0]) * 2, TextStringWidth(MenuListDisplay[1]) * 1.2,
			MenuButtonHeight, MenuListDisplay, sizeof(MenuListDisplay) / sizeof(MenuListDisplay[0]));
		if (MenuListDisplaySelection == 1)
		{
			CurrentTheme = (CurrentTheme + 1) % THEME_NUM;
			sprintf(temp, "切换主题（当前：%s）", MyThemes[CurrentTheme].name);
			MenuListDisplay[1] = temp;
			display();
		}
	}

	{
		const int MenuListHelpSelection = MyMenuList(GenUIID(0), MenuSelectionWidth * 4, MenuBarVertical,
			TextStringWidth(MenuListHelp[0]) * 2, TextStringWidth(MenuListHelp[1]) * 1.4,
			MenuButtonHeight, MenuListHelp, sizeof(MenuListHelp) / sizeof(MenuListHelp[0]));
	}

}

void display()
{
	//DisplayClear();
	SetPenColor(MyThemes[CurrentTheme].background);
	drawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1);

	SetPenColor(MyThemes[CurrentTheme].foreground);

	/* 目前用于调试，检测后来加上的组件是否会对回调函数产生干扰 */
	MovePen(0, 0.05);
	if (EraseStatus)
		SetPenColor(MyThemes[CurrentTheme].accent);
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
