﻿/*
 * 文件名: my_display.
 * -------------------------------------
 * 这个头文件声明了GUI显示相关函数
 *
 */

#ifndef MY_DISPLAY_H_
#define MY_DISPLAY_H_

#define THEME_NUM 6
 /*
  * 结构体名: theme
  * -------------------------------------
  * 这个结构体由三个字符指针构成。
  * 每一个字符指针应该指向一个已被定义的颜色的颜色名。
  * 把三个字符指针组合在一起，形成了一套颜色，即一个主题 (theme)
  */
typedef struct theme {
	char* name;
	char* background;  // 背景色
	char* foreground;  // 前景色
	char* accent;      // 强调色
} theme;

/*
 * 函数名：InitColor
 * -------------------------------------
 * 初始化已在代码中写好的主题。
 * 将这些主题的颜色信息通过 libgraphics 的函数进行定义。
 */
void InitColor();

/*
 * 函数名: StretchDrawLine
 * 参数1: StartX    有向线段的起始位置的横坐标
 * 参数2: StartY    有向线段的起始位置的纵坐标
 * 参数3: StretchX  有向线段的横向份量
 * 参数4: StretchY  有向线段的纵向份量
 * -------------------------------------
 * 按有向线段的逻辑绘制线段，需要给出有向线段的
 * 起始点坐标和两个方向上的分量
 */
void StretchDrawLine(double StartX, double StartY, double StretchX, double StretchY);

/*
 * 函数名: PointDrawLine
 * 参数1: StartX  线段的起始位置的横坐标
 * 参数2: StartY  线段的起始位置的纵坐标
 * 参数3: EndX    线段的终止位置的横坐标
 * 参数4: EndY    线段的终止位置的纵坐标
 * -------------------------------------
 * 按有向线段的逻辑绘制线段，需要给出有向线段的
 * 起始点坐标和两个方向上的分量
 */
void PointDrawLine(double StartX, double StartY, double EndX, double EndY);

/*
 * 函数名: display
 * -------------------------------------
 * 这个函数是可视化显示最核心的函数。
 * 几乎每一个 Windows 消息被捕获后，
 * 这个函数都会被回调函数调用。
 * 每一次调用，相关依赖的数据有变化，
 * 对应地，绘图窗口产生了变化。
 */
void display();

#endif
