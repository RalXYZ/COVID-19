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
#include "prediction_model.h"

void DisplayClear();  // 定义在 graphics.c
int CurrentTheme = 3;  // 当前主题序号
theme MyThemes[THEME_NUM];  // 建立存储数量为 THEME_NUM 个主题的字符数组
char* DisplayMessage = "";

bool EraseStatus = false;  // 记录文字擦除状态，目前作调试用，检测后来加上的组件是否会对回调函数产生干扰

extern bool PauseAllProcedure;  // 定义在 my_callback.c
extern HWND graphicsWindow;     // GUI窗口句柄，在 libgraphics 中声明
extern DataProperty data;  // 链表相关属性值，在 my_resource.c 中声明
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
 * 参数5: CurrentColorHex     当前感染色十六进制颜色码值
 * 参数6: TotalColorHex       总感染色十六进制颜色码值
 * 参数7: CuredColorHex       治愈色十六进制颜色码值
 * 参数8: DeadColorHex        死亡色十六进制颜色码值
 * -------------------------------------
 * 输入一组（3个）有序的十六进制颜色码，将它们存储在给定的位置上
 */
static void InputMyColors(int position, char* name,
	int BackgroundColorHex, int ForegroundColorHex, int AccentColorHex,
	int CurrentColorHex, int TotalColorHex, int CuredColorHex, int DeadColorHex) {
	MyThemes[position].name = name;

	MyThemes[position].background = IntegerToString(BackgroundColorHex);
	HexDefineColor(MyThemes[position].background, BackgroundColorHex);

	MyThemes[position].foreground = IntegerToString(ForegroundColorHex);
	HexDefineColor(MyThemes[position].foreground, ForegroundColorHex);

	MyThemes[position].accent = IntegerToString(AccentColorHex);
	HexDefineColor(MyThemes[position].accent, AccentColorHex);

	MyThemes[position].current = IntegerToString(CurrentColorHex);
	HexDefineColor(MyThemes[position].current, CurrentColorHex);

	MyThemes[position].total = IntegerToString(TotalColorHex);
	HexDefineColor(MyThemes[position].total, TotalColorHex);

	MyThemes[position].cured = IntegerToString(CuredColorHex);
	HexDefineColor(MyThemes[position].cured, CuredColorHex);

	MyThemes[position].dead = IntegerToString(DeadColorHex);
	HexDefineColor(MyThemes[position].dead, DeadColorHex);
}

// 注意：更改这个函数的同时也要更改 THEME_NUM 宏
void InitColor() {
	InputMyColors(0, "苍松", 0x203227, 0x637B6D, 0x9CC2AD, 0x8EAC4A, 0xCFDB35, 0x10D296, 0xC0C0C0);
	InputMyColors(1, "红豆", 0xB9B2B4, 0x1B121F, 0x7A0B0E, 0x9A5C5C, 0xCF4141, 0x6AA40C, 0x4B4B4B);
	InputMyColors(2, "蔷薇", 0x312E31, 0xFFB6B9, 0xFF5F7F, 0xEF8D8D, 0xC20A0A, 0xAFCF0F, 0xB4B4B4);
	InputMyColors(3, "万圣节", 0x1C162B, 0xB94E0A, 0xC38112, 0xC46C6C, 0xFC5534, 0xA5A836, 0x737373);
	InputMyColors(4, "熊猫", 0x222222, 0x828282, 0xEAEAEA, 0xE0948C, 0xEF2D2D, 0x0FCF66, 0x686868);
	InputMyColors(5, "蓝鹊", 0xC9D1D9, 0x4780BA, 0xBA5255, 0xDB8177, 0xA61616, 0x10E26F, 0x4F4F4F);
}

char* GetEpidemicColor(int property)
{
	switch (property)
	{
	case Current:
		return MyThemes[CurrentTheme].current;
	case Total:
		return MyThemes[CurrentTheme].total;
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

/*
 * 函数名: DrawMenu
 * -------------------------------------
 * 这个函数定义和绘制了菜单（目前未完成“绘制”）
 */
static void DrawMenu()
{
	static char ChangeThemeLabel[40] = { 0 };
	static char Highlight[20] = "隐藏高亮光标";
	static char MenuDrawPrediction[20] = "显示预测";

	static char* MenuFile[] = { "文件",
		"新建 | Ctrl-N",
		"打开 | Ctrl-O",
		"保存 | Ctrl-S",
		"另存为",
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
		MenuDrawPrediction,
		Highlight,
		ChangeThemeLabel };

	static char* MenuHelp[] = { "帮助",
		"使用帮助",
		"关于本软件" };


	const double MenuSelectionWidth = TextStringWidth(MenuFile[0]) * 2;  // 菜单栏选项都是两个中文字
	const double MenuButtonHeight = GetFontHeight() * 1.5;  // 菜单栏每一个按钮的高度
	const double MenuBarVertical = GetWindowHeight() - MenuButtonHeight;  // 菜单栏的竖直位置

	extern int MyMenuList(int id, double x, double y, double w, double wlist, double h, char* labels[], int n);  // 修改过的菜单绘制函数

	sprintf(ChangeThemeLabel, "切换主题（当前：%s）", MyThemes[CurrentTheme].name);

	// 文件
	{
		const int MenuFileSelection = MyMenuList(GenUIID(0), 0, MenuBarVertical,
			MenuSelectionWidth, TextStringWidth(MenuFile[1]) * 1.2,
			MenuButtonHeight, MenuFile, sizeof(MenuFile) / sizeof(MenuFile[0]));

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
		case 5:  // 关闭
			MenuFileClose();
			break;
		case 6:  // 退出
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
			TextStringWidth(MenuDraw[0]) * 2, TextStringWidth(MenuDraw[4]) * 1.02,
			MenuButtonHeight, MenuDraw, sizeof(MenuDraw) / sizeof(MenuDraw[0]));
		if (MenuDrawSelection == 1)
		{
			extern _Bool DisplayLineChart, DisplayFanChart, DisplayBarChart;
			if (data.BaseDir == nullptr)
			{
				MessageBox(graphicsWindow,
					TEXT("您尚未打开文件。请先打开文件。"),
					TEXT("提示"), MB_OK | MB_ICONWARNING);
			}
			else
				DisplayLineChart = DisplayFanChart = DisplayBarChart = true;
		}
		if (MenuDrawSelection == 2)
		{
			if (data.BaseDir == nullptr)
			{
				MessageBox(graphicsWindow,
					TEXT("您尚未打开文件。请先打开文件。"),
					TEXT("提示"), MB_OK | MB_ICONWARNING);
			}
			else
			{
				status.DisplayPrediction = !(status.DisplayPrediction);

				if (status.DisplayPrediction)
					sprintf(MenuDrawPrediction, "隐藏预测");
				else
					sprintf(MenuDrawPrediction, "显示预测");

				display();
			}
		}
		else if (MenuDrawSelection == 3)
		{
			if (data.BaseDir == nullptr)
			{
				MessageBox(graphicsWindow,
					TEXT("您尚未打开文件。请先打开文件。"),
					TEXT("提示"), MB_OK | MB_ICONWARNING);
			}

			else if (!status.HighlightVisible)
			{
				sprintf(Highlight, "隐藏高亮光标");
				status.HighlightVisible = true;
			}
			else if (status.HighlightVisible)
			{
				sprintf(Highlight, "显示高亮光标");
				status.HighlightVisible = false;
			}
		}
		else if (MenuDrawSelection == 4)
		{
			CurrentTheme = (CurrentTheme + 1) % THEME_NUM;
			display();
		}
	}

	// 帮助
	{
		const int MenuHelpSelection = MyMenuList(GenUIID(0), MenuSelectionWidth * 3, MenuBarVertical,
			TextStringWidth(MenuHelp[0]) * 2, TextStringWidth(MenuHelp[2]) * 1.15,
			MenuButtonHeight, MenuHelp, sizeof(MenuHelp) / sizeof(MenuHelp[0]));
		if (MenuHelpSelection == 1)  // 使用帮助
		{
			// system("start ..\\xxx");  // 将指令传给shell；由于目前还没有帮助文档，这行代码被注释掉
			MessageBox(graphicsWindow, TEXT("目前帮助文档还不存在，但用于打开帮助文档的代码已写好。"),
				TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		}
		if (MenuHelpSelection == 2)  // 关于本软件
		{
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

	DrawMenu();  // 绘制菜单组件

	if (data.BaseDir != nullptr && !status.DisplayPrediction)
		DrawChart(SentinelNode.next->properties[Month], SentinelNode.next->properties[Day], data.TotalDays);

	if (status.DisplayPrediction)
	{
		PredictionInterface();  //调试用
		PredictionInterface();
		PredictionChart();//调试用
	}
}
