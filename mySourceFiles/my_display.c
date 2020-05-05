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

bool EraseStatus = false;


/*
 * 函数名: DrawMenu
 * -------------------------------------
 * 这个函数定义和绘制了菜单（目前未完成“绘制”）
 */
void DrawMenu()
{
	static char* MenuListFile[] = { "文件",
		"新建 | Ctrl-N",
		"打开 | Ctrl-O",
		"保存 | Ctrl-S",
		"关闭 | Ctrl-W",
		"退出 | Ctrl-Q" };

	static char* EditListTool[] = { "编辑",
		"录入",
		"修改",
		"删除" };

	static char* DrawListTool[] = { "绘图",
		"绘制图表" };

	static char* HelpListTool[] = { "帮助",
		"使用帮助",
		"关于本软件" };
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
	MovePen(0, GetWindowHeight() / 2);
	SetEraseMode(EraseStatus);
	DrawTextString("Hello world");
}
