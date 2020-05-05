/*
 * 文件名: main.c
 * -------------------------------------
 * 入口文件
 *
 */

#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"

#include "my_macro.h"
#include "my_resource.h"
#include "my_callback.h"
#include "my_display.h"

 /*哨兵节点*/
struct epidemic SentinelNode;

void Main()
{
	SetWindowSize(8, 6);
	SetWindowTitle("COVID-19 统计数据");
	InitGraphics();

	/*计时器消息注册*/
	registerTimerEvent(TimerEventProcess);
	startTimer(TIME_ELAPSE_1, TIME_ELAPSE_1);

	/*鼠标、键盘与字符消息注册*/
	registerKeyboardEvent(KeyboardEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerCharEvent(CharEventProcess);

	/*链表初始化*/
	InitEpidemicList(&SentinelNode);

}
