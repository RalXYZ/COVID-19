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

extern epidemic SentinelNode;  // 哨兵节点，在 my_resource.c 中声明

void Main()
{
	SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetWindowTitle("COVID-19 统计数据");
	InitGraphics();

	/*链表初始化*/
	InitEpidemicList(&SentinelNode);

	/*主题初始化*/
	InitColor();

	/*计时器消息注册*/
	registerTimerEvent(TimerEventProcess);
	startTimer(TIME_ELAPSE_1, TIME_ELAPSE_1);

	/*鼠标、键盘与字符消息注册*/
	registerKeyboardEvent(KeyboardEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerCharEvent(CharEventProcess);

	FileInputList("../myResourceFiles/statistics.txt", 0, 48);  // 测试用，未来将删除

}
