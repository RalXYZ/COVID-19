﻿/*
 * 文件名: my_resource.c
 * -------------------------------------
 * 这个文件实现了一些文件处理相关的函数
 *
 */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "graphics.h"

#include "my_macro.h"
#include "my_resource.h"

int EpidemicListLength;  // 疫情链表长度，从1开始
int EpidemicElementMax;  // 疫情链表属性数据中的最大值，用于决定折线图的缩放
epidemic SentinelNode;  // 哨兵节点

void InitEpidemicList(epidemic* node)
{
	node->prev = nullptr;
	node->next = nullptr;
}

/*
 * 函数名: FreeEpidemicList
 * 参数: node  一个指向你想释放的链表的首节点的指针
 * ------------------------------------
 * 这个函数循环地释放当前节点到尾节点之间的所有
 * 节点，再功能上类似析构函数。
 */
void FreeEpidemicList(epidemic* node)
{
	epidemic* CurrentNode = node;
	while (CurrentNode != nullptr)
	{
		epidemic* TempNode = CurrentNode->next;
		free(CurrentNode);
		CurrentNode = TempNode;
	}
}

int ReadEpidemicList(int month, int date, EpidemicProperty type)
{
	if (0 <= type && type < EPIDEMIC_ELEMENT_NUM)
	{
		for (epidemic* i = SentinelNode.next; i != nullptr; i = i->next)
		{
			if (date == i->properties[Date] && month == i->properties[Month])
				return i->properties[type];
		}
	}
	else
	{
		InitConsole();
		printf("在 ReadEpidemicList 函数中，错误参数 type: %d\n\
它应该大于等于0，小于宏 EPIDEMIC_ELEMENT_NUM 。此错误与用户行为无关，只与开发者有关。", type);
		_getch();  // 阻塞
		// 这个分支的返回值未定义。如果程序进入了这个分支，一定是开发者的代码出了问题，请立即debug。
	}
	return -1;  // 如果找不到日期，则返回-1
}

/*
 * 函数名: SafeFOpen
 * 参数1: fpp       指向文件指针的指针
 * 参数2: FileName  文件名
 * 参数3: mode      打开文件的模式
 * ------------------------------------
 * 这个函数给文件指针正确赋值；如果发生异常，在
 * 终端显示错误信息。
 */
void SafeFOpen(FILE** fpp, char* FileName, char* mode)
{
	if ((*fpp = fopen(FileName, mode)) == nullptr)
	{
		InitConsole();
		printf("文件打开错误！ 位于 %s ，在\"%s\"模式下\n", FileName, mode);
		//exit(0);
	}
}

/*
 * 函数名: EndFileInputTask
 * 参数1: reason     错误原因
 * 参数2: FirstNode  想要释放的链表的首动态节点值
 * 参数3: fp         文件指针
 * ------------------------------------
 * 这个函数当且仅当FileInputList函数出现异常
 * 的时候被调用，主要用于做清理未输入完成的链表、
 * 关闭文件 和 用对话框输出错误信息。
 */
void EndFileInputTask(char* reason, epidemic* FirstNode, FILE* fp)
{
	FreeEpidemicList(FirstNode);
	fclose(fp);
	MessageBox(NULL, TEXT(reason), TEXT("错误"), MB_OK | MB_ICONERROR);
}

enum error FileInputList(char* FileName, int begin, int end)
{
	FILE* fp = nullptr;
	SafeFOpen(&fp, FileName, "r");

	epidemic* TempFirstNode = (epidemic*)malloc(sizeof(epidemic));
	epidemic* CurrentNode = TempFirstNode;

	for (int i = 0; ; i++)
	{
		const int SuccessInput = fscanf(fp, "%d-%d%d%d%d%d",
			&CurrentNode->properties[Month], &CurrentNode->properties[Date],
			&CurrentNode->properties[Current], &CurrentNode->properties[Total],
			&CurrentNode->properties[Cured], &CurrentNode->properties[Dead]);
		CurrentNode->next = nullptr;  // 将当前节点的指向后一个节点的指针默认设为空

		if (SuccessInput >= 0 && SuccessInput < EPIDEMIC_ELEMENT_NUM)
		{
			EndFileInputTask("资源文件格式可能有误，请校对格式。", TempFirstNode, fp);
			return UnformattedData;
		}
		if (i != end && feof(fp))
		{
			EndFileInputTask("请求的日期超出了资源文件的存储范围。", TempFirstNode, fp);
			return ExceedDate;
		}
		if (i == end || feof(fp))  // 到了结束日期，或资源文件已经到达了末尾
			break;
		if (i >= begin)
		{
			epidemic* TempNode = (epidemic*)malloc(sizeof(epidemic));
			CurrentNode->next = TempNode;
			TempNode->prev = CurrentNode;
			CurrentNode = TempNode;
		}
	}

	FreeEpidemicList(SentinelNode.next);  // 释放旧链表
	SentinelNode.next = TempFirstNode;  // 哨兵节点与新链表连接
	fclose(fp);

	EpidemicListLength = 0;
	for (epidemic* i = &SentinelNode; i->next != nullptr; i = i->next, ++EpidemicListLength)
		pass;

	EpidemicElementMax = -1;  // 由于统计数据一定非负，那么就把最大值初始化为负数
	for (EpidemicProperty i = EPIDEMIC_PROPERTY_START; i < EPIDEMIC_ELEMENT_NUM; i++)
	{
		for (epidemic* j = SentinelNode.next; j != nullptr; j = j->next)
			EpidemicElementMax < j->properties[i] ? EpidemicElementMax = j->properties[i] : pass;
	}

	return Null;  // 无异常
}
