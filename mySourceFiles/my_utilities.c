﻿/*
 * 文件名: my_utilities.c
 * -------------------------------------
 * 这个文件实现了杂项功能，大多为工具性质
 *
 */

#include <Windows.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "graphics.h"
#include "imgui.h"

#include "my_macro.h"
#include "my_callback.h"
#include "my_display.h"
#include "my_resource.h"

extern MyStatus status;  // 当前状态，在 my_resource.c 中定义

// 用于交换两个变量的宏函数
#define SWAP(x, y) (x)^=(y); (y)^=(x); (x)^=(y);

/*
 * 函数名: ArraySort
 * 参数1: array    待排序的数组
 * 参数2: partner  伴随数组，存储属性的编号，元素顺序与待排序数组一起变化
 * 参数3: length   数组长度
 * ------------------------------------
 * 将数组中的元素从小到大排序
 */
static void ArraySort(int* array, int* partner, int length)
{
	for (int i = 0; i < length - 1; i++)
	{
		for (int j = length - 1; j > i; j--)
		{
			if (array[j] < array[j - 1])
			{
				SWAP(array[j], array[j - 1]);
				SWAP(partner[j], partner[j - 1]);
			}
		}
	}
}

void MyExitConsole() {
	FreeConsole();                  // 该函数在 wincon.h 声明  
	freopen("NUL", "r+t", stdin);   // 将stdin重定向到默认位置 
	freopen("NUL", "w+t", stdout);  // 将stdout重定向到默认位置 
}

void PauseDisplay() {
	display();                   // GUI刷新一次
	status.PauseAllProcedure = true;    // 停止所有GUI行为
}

void ContinueDisplay() {
	startTimer(TIME_ELAPSE_1, TIME_ELAPSE_1);     // 重置计时器
	status.PauseAllProcedure = false;   // 取消禁止所有GUI行为
}

int SafeNNegIntInput(int digits) {
	// TODO: 当第一个字符即为空格时，goto到此函数开头
	int sum = 0;
	for (int i = 0; i <= digits; i++) {
		int input = getchar();
		if (input == '\n')
			return sum;
		else if (!(isdigit(input)))  // 如果不是数字
		{
			while (getchar() != '\n')  // 清空键盘缓冲区；不能使用 fflush() 代替，否则会出现UB
				;
			return -1;  // 出现非法符号
		}
		sum *= 10;
		sum += input - '0';
	}
	while (getchar() != '\n')  // 清空键盘缓冲区；不能使用 fflush() 代替，否则会出现UB
		;
	return -2;  // 超过位数限制
}

int UpSelectProperty()
{
	int output = -1;
	int PropertiesValue[EPIDEMIC_STATISTIC_NUM];
	int PropertiesPartner[EPIDEMIC_STATISTIC_NUM];
	for (int i = 0; i < EPIDEMIC_STATISTIC_NUM; i++)
		PropertiesValue[i] = status.HighlightNode->properties[i + EPIDEMIC_PROPERTY_START];

	for (int i = 0; i < EPIDEMIC_STATISTIC_NUM; i++)
		PropertiesPartner[i] = i + EPIDEMIC_PROPERTY_START;

	ArraySort(PropertiesValue, PropertiesPartner, EPIDEMIC_STATISTIC_NUM);

	if (status.HighlightProperty == PropertiesPartner[EPIDEMIC_STATISTIC_NUM - 1])
		output = PropertiesPartner[0];
	else
	{
		for (int i = 0; i < EPIDEMIC_STATISTIC_NUM - 1; i++)
		{
			if (status.HighlightProperty == PropertiesPartner[i])
				output = PropertiesPartner[i + 1];
		}
	}
	return output;
}

int DownSelectProperty()
{
	int output = -1;
	int PropertiesValue[EPIDEMIC_STATISTIC_NUM];
	int PropertiesPartner[EPIDEMIC_STATISTIC_NUM];
	for (int i = 0; i < EPIDEMIC_STATISTIC_NUM; i++)
		PropertiesValue[i] = status.HighlightNode->properties[i + EPIDEMIC_PROPERTY_START];

	for (int i = 0; i < EPIDEMIC_STATISTIC_NUM; i++)
		PropertiesPartner[i] = i + EPIDEMIC_PROPERTY_START;

	ArraySort(PropertiesValue, PropertiesPartner, EPIDEMIC_STATISTIC_NUM);

	if (status.HighlightProperty == PropertiesPartner[0])
		output = PropertiesPartner[EPIDEMIC_STATISTIC_NUM - 1];
	else
	{
		for (int i = 1; i < EPIDEMIC_STATISTIC_NUM; i++)
		{
			if (status.HighlightProperty == PropertiesPartner[i])
				output = PropertiesPartner[i - 1];
		}
	}
	return output;
}

char* PropertyMeaning(int property)
{
	switch (property)
	{
	case Month:
		return "月";
	case Day:
		return "日";
	case Current:
		return "当前确诊";
	case Total:
		return "累计确诊";
	case Cured:
		return "累计治愈";
	case Dead:
		return "累计死亡";
	}
}
