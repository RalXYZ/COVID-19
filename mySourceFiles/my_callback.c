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

extern bool EraseStatus;

void KeyboardEventProcess(int key, int event)
{
	if (event == KEY_DOWN)
		EraseStatus = !EraseStatus;

	uiGetKeyboard(key, event);
	display();
}

void MouseEventProcess(int x, int y, int button, int event)
{
	if (event == BUTTON_DOWN)
		EraseStatus = !EraseStatus;

	uiGetMouse(x, y, button, event);
	display();
}

void CharEventProcess(char key)
{

}

void TimerEventProcess(int timerID)
{
	if (timerID == TIME_ELAPSE_1)
		EraseStatus = !EraseStatus;
	display();
}
