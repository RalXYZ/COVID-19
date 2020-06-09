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

#include "menu_functions.h"
#include "my_display.h"
#include "my_resource.h"
#include "my_utilities.h"

extern bool EraseStatus;  // 定义在 my_display.c
extern MyStatus status;  // 当前状态，在 my_resource.c 中定义

void KeyboardEventProcess(int key, int event)
{
	if (event == KEY_DOWN && key == 'M')
	{
		MenuDrawZoom();
	}
	if (status.ZoomIn)
		return;

	if (event == KEY_DOWN)
		EraseStatus = !EraseStatus;

	if (event == KEY_DOWN)
	{
		if (key == VK_RIGHT)
			if (status.HighlightVisible && status.HighlightNode != nullptr && status.HighlightNode->next != nullptr)
			{
				status.HighlightNode = status.HighlightNode->next;
				++status.HighlightNum;
				GUIOutputMsg("光标右移");
			}
		if (key == VK_LEFT)
			if (status.HighlightVisible && status.HighlightNode->prev != nullptr)
			{
				status.HighlightNode = status.HighlightNode->prev;
				--status.HighlightNum;
				GUIOutputMsg("光标左移");
			}
		if (key == VK_UP)
			if (status.HighlightVisible)
			{
				status.HighlightProperty = UpSelectProperty();
				GUIOutputMsg("光标上移");
			}
		if (key == VK_DOWN)
			if (status.HighlightVisible)
			{
				status.HighlightProperty = DownSelectProperty();
				GUIOutputMsg("光标下移");
			}
	}

	uiGetKeyboard(key, event);
	display();
}

void MouseEventProcess(int x, int y, int button, int event)
{
	if (status.ZoomIn)
		return;

	uiGetMouse(x, y, button, event);
	display();
}

void CharEventProcess(char key)
{
	if (status.ZoomIn)
		return;
	uiGetChar(key); // GUI字符输入
	display();
}

void TimerEventProcess(int timerID)
{
	if (status.ZoomIn)
		return;

	if (timerID == TIME_ELAPSE_1)
		EraseStatus = !EraseStatus;
	display();
}
