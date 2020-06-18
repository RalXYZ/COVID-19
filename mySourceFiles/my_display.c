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
#include "menu_functions.h"
#include "my_utilities.h"
#include "prediction_model.h"

void DisplayClear();  // 定义在 graphics.c
int CurrentTheme = 4;  // 当前主题序号
theme MyThemes[THEME_NUM];  // 建立存储数量为 THEME_NUM 个主题的字符数组
char* DisplayMessage = "";

bool EraseStatus = false;  // 记录文字擦除状态，目前作调试用，检测后来加上的组件是否会对回调函数产生干扰

extern bool PauseAllProcedure;  // 定义在 my_callback.c
extern HWND graphicsWindow;     // GUI窗口句柄，在 libgraphics 中声明
extern DataProperty data;  // 链表相关属性值，在 my_resource.c 中声明
extern CompareDataProperty CompareData;
extern epidemic SentinelNode;  // 哨兵节点，在 my_resource.c 中声明
extern MyStatus status;  // 当前状态，在 my_resource.c 中定义

/*
 * HexDefineColor
 * 参数1: name   指向你想处理的字符串的指针
 * 参数2: value  十六进制颜色码值
 * -------------------------------------
 * libgraphics 的新颜色定义中的红绿蓝值是0到1之间的实数，
 * 这不符合通用规范。
 * 本函数实现通过十六进制颜色码值定义新颜色。
 */
static void HexDefineColor(string name, int value)
{
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
 * 参数5: NewColorHex     当前感染色十六进制颜色码值
 * 参数6: CurrentColorHex       总感染色十六进制颜色码值
 * 参数7: CuredColorHex       治愈色十六进制颜色码值
 * 参数8: DeadColorHex        死亡色十六进制颜色码值
 * -------------------------------------
 * 输入一组（8个）有序的十六进制颜色码，将它们存储在给定的位置上
 */
static void InputMyColors(int position, char* name,
	int BackgroundColorHex, int ForegroundColorHex, int AccentColorHex,
	int NewColorHex, int CurrentColorHex, int CuredColorHex, int DeadColorHex)
{
	MyThemes[position].name = name;

	MyThemes[position].background = IntegerToString(BackgroundColorHex);
	HexDefineColor(MyThemes[position].background, BackgroundColorHex);

	MyThemes[position].foreground = IntegerToString(ForegroundColorHex);
	HexDefineColor(MyThemes[position].foreground, ForegroundColorHex);

	MyThemes[position].accent = IntegerToString(AccentColorHex);
	HexDefineColor(MyThemes[position].accent, AccentColorHex);

	MyThemes[position].new = IntegerToString(NewColorHex);
	HexDefineColor(MyThemes[position].new, NewColorHex);

	MyThemes[position].current = IntegerToString(CurrentColorHex);
	HexDefineColor(MyThemes[position].current, CurrentColorHex);

	MyThemes[position].cured = IntegerToString(CuredColorHex);
	HexDefineColor(MyThemes[position].cured, CuredColorHex);

	MyThemes[position].dead = IntegerToString(DeadColorHex);
	HexDefineColor(MyThemes[position].dead, DeadColorHex);
}

// 注意：更改这个函数的同时也要更改 THEME_NUM 宏
void InitColor()
{
	InputMyColors(0, "苍松", 0x203227, 0x637B6D, 0x9CC2AD, 0x8EAC4A, 0xCFDB35, 0x10D296, 0xC0C0C0);
	InputMyColors(1, "红豆", 0xB9B2B4, 0x1B121F, 0x7A0B0E, 0x9A5C5C, 0xCF4141, 0x6AA40C, 0x4B4B4B);
	InputMyColors(2, "蔷薇", 0x312E31, 0xFFB6B9, 0xFF5F7F, 0xEF8D8D, 0xC20A0A, 0xAFCF0F, 0xB4B4B4);
	InputMyColors(3, "万圣节", 0x1C162B, 0xB94E0A, 0xC38112, 0xC46C6C, 0xFC5534, 0xA5A836, 0x737373);
	InputMyColors(4, "熊猫", 0x222222, 0x828282, 0xEAEAEA, 0xE0948C, 0xEF2D2D, 0x0FCF66, 0x686868);
	InputMyColors(5, "蓝鹊", 0xC9D1D9, 0x4780BA, 0xBA5255, 0xDB8177, 0xA61616, 0x54852f, 0x4F4F4F);
}

char* GetEpidemicColor(int property)
{
	switch (property)
	{
	case New:
		return MyThemes[CurrentTheme].new;
	case Current:
		return MyThemes[CurrentTheme].current;
	case Cured:
		return MyThemes[CurrentTheme].cured;
	case Dead:
		return MyThemes[CurrentTheme].dead;
	default:
		return nullptr;
	}
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

void GUIOutputMsg(char* msg)
{
	DisplayMessage = msg;
	display();
}

/***********************************************************/

static void IconOpen(double x, double y, double n)//打开图标，x,y从最左边左上点开始 
{
	MovePen(x, y);
	DrawLine(n / 3, 0);
	DrawLine(0, n / 3);
	DrawLine(n / 3, 0);
	DrawLine(0, -n / 3);
	DrawLine(n / 3, 0);
	DrawLine(0, -n / 3);
	DrawLine(-n / 3, 0);
	DrawLine(0, -n / 3);
	DrawLine(-n / 3, 0);
	DrawLine(0, n / 3);
	DrawLine(-n / 3, 0);
	DrawLine(0, n / 3);
}

static void IconClose(double x, double y, double n)//关闭图标，x,y从最左上点开始 
{
	MovePen(x, y);
	DrawLine(2 * n, -2 * n);
	MovePen(x, y - 2 * n);
	DrawLine(2 * n, 2 * n);
}

static void IconDrawGraph(double x, double y, double n)//绘制图表图标,x,y从底线最左开始
{
	MovePen(x, y);
	DrawLine(n, 0);
	MovePen(x + n / 7, y);
	drawRectangle(x + n / 7, y, n / 7, 3 * n / 7, 0);
	drawRectangle(x + 3 * n / 7, y, n / 7, 4 * n / 7, 0);
	drawRectangle(x + 5 * n / 7, y, n / 7, 2 * n / 7, 0);
}

static void IconEdit(double x, double y, double n)//编辑图标,x,y从笔杆右下开始
{
	MovePen(x, y);
	DrawLine(-n, -1.166 * n);
	DrawLine(0, 1.5 * n);
	DrawLine(n, 3 * n);
	DrawLine(n, -0.333 * n);
	DrawLine(-n, -3 * n);
	DrawLine(-n, 0.333 * n);
	DrawLine(0.333 * n, -0.111 * n);
	DrawLine(n, 3 * n);
	DrawLine(0.333 * n, -0.111 * n);
	DrawLine(-n, -3 * n);
	DrawLine(-0.666 * n, 0.222 * n);
	DrawLine(0, -n);
	DrawLine(n / 3, -n / 9);
}

static void IconPredict(double x, double y, double n)//预测图标，x,y从最左侧开始
{
	MovePen(x, y);
	DrawLine(2.55 * n, 0);
	MovePen(x + 0.5 * n, y - 0.5 * n);
	DrawLine(0, 2.5 * n);
	MovePen(x + 0.5 * n, y);
	DrawLine(n / 4, n / 2);
	DrawLine(n / 2, 5.5 * n / 4);
	DrawLine(n / 4, n / 8);
	DrawLine(n / 4, -n / 8);
	DrawLine(n / 2, -5.5 * n / 4);
	DrawLine(n / 4, -n / 2);
}

static void IconColor(double x, double y, double n)//调色图标，x+n,y为圆心
{
	MovePen(x + n, y);
	DrawArc(n, 0, 360);
	MovePen(x - 5 * n / 16, y);
	DrawArc(n / 4, 0, 360);
	MovePen(x + n / 8, y + 5 * n / 8);
	DrawArc(n / 4, 0, 360);
	MovePen(x + 7 * n / 8, y + n / 8);
	DrawArc(n / 4, 0, 360);
}

/*
 * 函数名: DrawToolBar
 * -------------------------------------
 * 绘制图标工具栏
 */
static void DrawToolBar()
{
#define ICON_BLOCK .4

	extern int MyButton(int id, double x, double y, double w, double h, char* label);  // 在 imgui.c 中定义

	SetPenColor(MyThemes[CurrentTheme].foreground);

	IconOpen(5.7, 5.83, .2);
	IconClose(6.1, 5.89, .1);
	IconDrawGraph(6.48, 5.73, .25);
	IconEdit(7.01, 5.75, .05);
	IconPredict(7.28, 5.73, .1);
	IconColor(7.803, 5.792, .12);

	if (MyButton(GenUIID(0), WINDOW_WIDTH - 6 * ICON_BLOCK, WINDOW_HEIGHT - ICON_BLOCK, ICON_BLOCK, ICON_BLOCK, ""))
		MenuFileOpen();
	if (MyButton(GenUIID(0), WINDOW_WIDTH - 5 * ICON_BLOCK, WINDOW_HEIGHT - ICON_BLOCK, ICON_BLOCK, ICON_BLOCK, ""))
		MenuFileClose();
	if (MyButton(GenUIID(0), WINDOW_WIDTH - 4 * ICON_BLOCK, WINDOW_HEIGHT - ICON_BLOCK, ICON_BLOCK, ICON_BLOCK, ""))
		MenuDrawGraph();
	if (MyButton(GenUIID(0), WINDOW_WIDTH - 3 * ICON_BLOCK, WINDOW_HEIGHT - ICON_BLOCK, ICON_BLOCK, ICON_BLOCK, ""))
		MenuEditChange();
	if (MyButton(GenUIID(0), WINDOW_WIDTH - 2 * ICON_BLOCK, WINDOW_HEIGHT - ICON_BLOCK, ICON_BLOCK, ICON_BLOCK, ""))
		MenuDrawPrediction();
	if (MyButton(GenUIID(0), WINDOW_WIDTH - 1 * ICON_BLOCK, WINDOW_HEIGHT - ICON_BLOCK, ICON_BLOCK, ICON_BLOCK, ""))
		MenuDrawChangeTheme();
}

/*************************************************************/

char compare[20] = "对比...";
char Highlight[20] = "隐藏高亮光标";
char MenuDrawPredictionString[20] = "显示预测";
/*
 * 函数名: DrawMenu
 * -------------------------------------
 * 这个函数定义和绘制了菜单栏
 */
static void DrawMenu()
{
	static char ChangeThemeLabel[40] = { 0 };

	static char* MenuFile[] = { "文件",
		"新建 | Ctrl-N",
		"打开 | Ctrl-O",
		"保存 | Ctrl-S",
		"另存为",
		compare,
		"关闭 | Ctrl-W",
		"退出 | Ctrl-Q" };

	static char* MenuEdit[] = { "编辑",
		"修改",
		"前置录入",
		"后置录入",
		"前置删除",
		"后置删除" };

	static char* MenuDraw[] = { "绘图",
		"绘制图表",
		"按M键放大",
		MenuDrawPredictionString,
		Highlight,
		ChangeThemeLabel };

	static char* MenuHelp[] = { "帮助",
		"使用方法",
		"关于本软件",
		"致哀" };


	const double MenuSelectionWidth = TextStringWidth(MenuFile[0]) * 2;  // 菜单栏选项都是两个中文字
	const double MenuButtonHeight = GetFontHeight() * 1.5;  // 菜单栏每一个按钮的高度
	const double MenuBarVertical = GetWindowHeight() - MenuButtonHeight;  // 菜单栏的竖直位置

	extern int MyMenuList(int id, double x, double y, double w, double wlist, double h, char* labels[], int n);  // 修改过的菜单绘制函数

	sprintf(ChangeThemeLabel, "切换主题（当前：%s）", MyThemes[CurrentTheme].name);

	// 文件
	{
		const int MenuFileSelection = MyMenuList(GenUIID(0), 0, MenuBarVertical,
			MenuSelectionWidth, TextStringWidth(MenuFile[1]) * 1.4,
			MenuButtonHeight, MenuFile, sizeof(MenuFile) / sizeof(MenuFile[0]));

		status.CompareMode ? sprintf(compare, "退出对比模式") : sprintf(compare, "对比...");

		switch (MenuFileSelection)
		{
		case 1:  // 新建
			MenuFileNew();
			break;
		case 2:  // 打开
			MenuFileOpen();
			break;
		case 3:  // 保存
			MenuFileSave();
			break;
		case 4:  // 另存为
			MenuFileSaveAs();
			break;
		case 5:  // 对比
			status.CompareMode ? MenuFileCompareClose() : MenuFileCompareOpen();
			break;
		case 6:  // 关闭
			MenuFileClose();
			break;
		case 7:  // 退出
			MenuFileExit();
			break;
		}
	}

	// 编辑
	{
		const int MenuEditSelection = MyMenuList(GenUIID(0), MenuSelectionWidth, MenuBarVertical,
			TextStringWidth(MenuEdit[0]) * 2, TextStringWidth(MenuEdit[4]) * 1.2,
			MenuButtonHeight, MenuEdit, sizeof(MenuEdit) / sizeof(MenuEdit[0]));
		switch (MenuEditSelection)
		{
		case 1:  // 修改
			MenuEditChange();
			break;
		case 2:  // 前置录入
			MenuEditFrontInsert();
			break;
		case 3:  // 后置录入
			MenuEditBackInsert();
			break;
		case 4:  // 前置删除
			MenuEditFrontDelete();
			break;
		case 5:  // 后置删除
			MenuEditBackDelete();
			break;
		}
	}

	// 绘图
	{
		const int MenuDrawSelection = MyMenuList(GenUIID(0), MenuSelectionWidth * 2, MenuBarVertical,
			TextStringWidth(MenuDraw[0]) * 2, TextStringWidth(MenuDraw[5]) * 1.02,
			MenuButtonHeight, MenuDraw, sizeof(MenuDraw) / sizeof(MenuDraw[0]));
		switch (MenuDrawSelection)
		{
		case 1:
			MenuDrawGraph();
			break;
		case 2:
			MenuDrawZoom();
			break;
		case 3:
			MenuDrawPrediction();
			break;
		case 4:
			MenuDrawHighlight();
			break;
		case 5:
			MenuDrawChangeTheme();
			break;
		}
	}

	// 帮助
	{
		const int MenuHelpSelection = MyMenuList(GenUIID(0), MenuSelectionWidth * 3, MenuBarVertical,
			TextStringWidth(MenuHelp[0]) * 2, TextStringWidth(MenuHelp[2]) * 1.15,
			MenuButtonHeight, MenuHelp, sizeof(MenuHelp) / sizeof(MenuHelp[0]));
		if (MenuHelpSelection == 1)  // 使用帮助
		{
			display();
			MessageBox(graphicsWindow, TEXT("详情请见大程报告 4.3使用操作"),
				TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		}
		if (MenuHelpSelection == 2)  // 关于本软件
		{
			display();
			MessageBox(graphicsWindow, TEXT("本软件不是开源软件。\n\
本软件是2019学年春夏学期“程序设计专题”大作业。\n\
本软件之权利与使用范畴受到课程规则的约束。\n\
本软件使用 libgraphics 和 simpleGUI 两个库。\n\
以上两个库均无开源许可证。\n\
为了避免法律纠纷，对库进行的修改，已在原处注明。\n\
本软件的所有开发工作完全由组内成员完成。\n\
凡借助外部资料完成的工作均已在原处注明资料来源。"),
TEXT("关于本软件"), MB_OK | MB_ICONINFORMATION);
		}
		if (MenuHelpSelection == 3)  // 致哀
		{
			display();
			MessageBox(graphicsWindow, TEXT("在此，我们向新冠疫情中的逝者致哀。\n\
我们不会遗忘逝去的、鲜活的生命，它们永远不是冰冷的数字；\
而在困厄中深省、从悲痛中奋起，就是对他们最好的告慰。\n\
本程序默认使用暗色主题，因为死亡的色彩从不是明快的。\n\
愿疫情早日消逝。愿逝者安息。"),
TEXT("致哀"), MB_OK | MB_ICONINFORMATION);
		}
	}

}

void display()
{
	SetPenColor(MyThemes[CurrentTheme].background);
	drawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1);

	SetPenColor(MyThemes[CurrentTheme].foreground);

	MovePen(0, 0.05);
	if (EraseStatus)  // 闪烁
		SetPenColor(MyThemes[CurrentTheme].accent);
	DrawTextString(DisplayMessage);  // 画操作信息
	SetEraseMode(false);

	/* 显示当前地区 */
	if (status.CompareMode)
	{
		char FileName[MAX_FILE_NAME_LENGTH] = { 0 };
		char CompareFileName[MAX_FILE_NAME_LENGTH] = { 0 };
		GetFileName(FileName, data.BaseDir);
		GetFileName(CompareFileName, CompareData.BaseDir);

		MovePen((WINDOW_WIDTH - TextStringWidth(FileName) - TextStringWidth(CompareFileName)) / 2.0, WINDOW_HEIGHT - 0.4);
		SetPenColor(MyThemes[CurrentTheme].accent);
		DrawTextString(FileName);
		DrawTextString(" ");
		SetPenColor(COMPARE_COLOR);
		DrawTextString(CompareFileName);
	}
	else if (data.BaseDir != nullptr)
	{
		char FileName[MAX_FILE_NAME_LENGTH] = { 0 };
		GetFileName(FileName, data.BaseDir);

		MovePen((WINDOW_WIDTH - TextStringWidth(FileName)) / 2.0, WINDOW_HEIGHT - 0.44);
		SetPenColor(MyThemes[CurrentTheme].accent);
		DrawTextString(FileName);
	}

	if (data.BaseDir != nullptr && !status.DisplayPrediction)
	{
		if (!status.ZoomIn)
			DrawChart(SentinelNode.next->properties[Month], SentinelNode.next->properties[Day], data.TotalDays);
		else
		{
			int month = status.HighlightNode->properties[Month];
			int day = status.HighlightNode->properties[Day];
			const int step = CalculateZoomDate(&month, &day);
			DrawChart(month, day, step);
		}
	}

	if (status.DisplayPrediction)
	{
		PredictionInterface();
		PredictionChart();
	}

	DrawMenu();  // 绘制菜单组件

	DrawToolBar();

}
