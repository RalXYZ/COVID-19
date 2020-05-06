/*
 * 文件名: my_callback.c
 * -------------------------------------
 * 这个文件实现了回调函数
 *
 */

#include <Windows.h>
#include <ctype.h>
#include <stdbool.h>

#include "graphics.h"
#include "imgui.h"

#include "my_macro.h"
#include "my_callback.h"
#include "my_display.h"

bool PauseAllProcedure = false;  // 记录是否要暂停所有回调函数的功能，用于弹出对话框时的阻塞

extern bool EraseStatus;  // 定义在 my_display.c
extern bool DisplayLineChart;  // 定义在 draw_chart.c ，测试用，未来将移除

void KeyboardEventProcess(int key, int event)
{
	if (PauseAllProcedure)
		return;

	if (event == KEY_DOWN)  // 目前作调试用，检测后来加上的组件是否会对回调函数产生干扰
		EraseStatus = !EraseStatus;

	if (event == KEY_DOWN)  // 测试用，未来将移除
	{
		if (key == VK_F1)
			DisplayLineChart = !DisplayLineChart;
	}

	uiGetKeyboard(key, event);
	display();
}

void MouseEventProcess(int x, int y, int button, int event)
{
	if (PauseAllProcedure)
		return;

	if (event == BUTTON_DOWN)  // 目前作调试用，检测后来加上的组件是否会对回调函数产生干扰
	{
		if (button == RIGHT_BUTTON)
			EraseStatus = !EraseStatus;
	}

	uiGetMouse(x, y, button, event);
	display();
}

void CharEventProcess(char key)
{
	if (PauseAllProcedure)
		return;
}

void TimerEventProcess(int timerID)
{
	if (PauseAllProcedure)
		return;

	if (timerID == TIME_ELAPSE_1)  // 目前作调试用，检测后来加上的组件是否会对回调函数产生干扰
		EraseStatus = !EraseStatus;
	display();
}
