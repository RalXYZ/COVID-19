/*
 * 文件名: main.c
 * -------------------------------------
 * 入口文件
 *
 */

#include <Windows.h>
#include <stdio.h>

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "my_macro.h"
#include "my_resource.h"
#include "my_callback.h"
#include "my_display.h"

 /*哨兵节点*/
struct epidemic* SentinelNode;

void Main()
{
	SetWindowSize(4, 2);
	SetWindowTitle("一个程序");
	InitGraphics();

	/*GUI初始化*/
	MovePen(0, GetWindowHeight() / 2);

	/*计时器消息注册*/
	registerTimerEvent(TimerEventProcess);
	startTimer(TIME_ELAPSE_1, TIME_ELAPSE_1);

	/*鼠标、键盘与字符消息注册*/
	registerKeyboardEvent(KeyboardEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerCharEvent(CharEventProcess);

	/*链表初始化*/
	InitEpidemicList(&SentinelNode);
	FileOpen(0, 1);

	for (struct epidemic* i = SentinelNode->next; i != nullptr; i = i->next)
	{
		printf("%s %d ", i->region, i->cured);
	}

	//ExitGraphics();
}
