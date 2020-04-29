/*
 * 文件名: my_display.c
 * -------------------------------------
 * 这个文件实现了GUI显示相关函数
 *
 */

#include <Windows.h>
#include <stdio.h>

#include "graphics.h"
#include "extgraph.h"
#include "strlib.h"
#include "imgui.h"

#include "my_macro.h"
#include "my_callback.h"
#include "my_display.h"



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
	DrawTextString("Hello world ");
}
