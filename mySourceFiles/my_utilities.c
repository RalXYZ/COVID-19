/*
 * 文件名: my_utilities.c
 * -------------------------------------
 * 这个文件实现了杂项功能
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

EpidemicProperty UpSelectProperty()
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

EpidemicProperty DownSelectProperty()
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
