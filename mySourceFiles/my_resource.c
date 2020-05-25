/*
 * 文件名: my_resource.c
 * -------------------------------------
 * 这个文件实现了一些文件处理相关的函数
 *
 */

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>

#include "graphics.h"

#include "my_macro.h"
#include "my_resource.h"

DataProperty data = { 0, 0, nullptr, false };  // 链表相关属性值
epidemic SentinelNode;  // 哨兵节点
MyStatus status = { false, nullptr, 0, 0 , false , false };  // 当前状态

void InitEpidemicList(epidemic* node)
{
	node->prev = nullptr;
	node->next = nullptr;
}

void FreeEpidemicList(epidemic* node)
{
	epidemic* CurrentNode = node;
	while (CurrentNode != nullptr)
	{
		epidemic* TempNode = CurrentNode->next;
		free(CurrentNode);
		CurrentNode = TempNode;
	}
	SentinelNode.next = nullptr;  // 恢复为默认值
}

void InitHighlight()
{
	status.HighlightVisible = true;
	status.HighlightNode = SentinelNode.next;
	status.HighlightProperty = EPIDEMIC_PROPERTY_START;
	status.HighlightNum = 0;
}

void DesHighlight()
{
	status.HighlightVisible = false;
	status.HighlightNode = nullptr;
	status.HighlightProperty = 0;
	status.HighlightNum = 0;
}

int ReadEpidemicList(int month, int date, EpidemicProperty type)
{
	if (0 <= type && type < EPIDEMIC_ELEMENT_NUM)
	{
		for (epidemic* i = SentinelNode.next; i != nullptr; i = i->next)
		{
			if (date == i->properties[Day] && month == i->properties[Month])
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
static FILE* SafeFOpen(char* FileName, char* mode)
{
	FILE* fp = nullptr;
	if ((fp = fopen(FileName, mode)) == nullptr)
	{
		InitConsole();
		printf("文件打开错误！ 位于 %s ，在\"%s\"模式下\n", FileName, mode);
		return nullptr;
	}
	return fp;
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
static void EndFileInputTask(char* reason, epidemic* FirstNode, FILE* fp)
{
	extern HWND graphicsWindow;  // GUI窗口句柄，在 libgraphics 里声明
	FreeEpidemicList(FirstNode);
	fclose(fp);
	MessageBox(graphicsWindow, TEXT(reason), TEXT("错误"), MB_OK | MB_ICONERROR);
}

/*
 * 函数名: GetDayNum
 * ------------------------------------
 * 取得疫情链表中的总天数
 */
static void GetDayNum()
{
	data.TotalDays = 0;
	for (epidemic* i = &SentinelNode; i->next != nullptr; i = i->next, ++data.TotalDays)
		pass;
}

void GetMaxElement()
{
	data.MaxElement = -1;  // 由于统计数据一定非负，那么就把最大值初始化为负数
	for (EpidemicProperty i = EPIDEMIC_PROPERTY_START; i < EPIDEMIC_ELEMENT_NUM; i++)
	{
		for (epidemic* j = SentinelNode.next; j != nullptr; j = j->next)
			data.MaxElement < j->properties[i] ? data.MaxElement = j->properties[i] : pass;
	}
}

int FileInputList(char* FileName)
{
	FILE* fp = SafeFOpen(FileName, "r");

	epidemic* TempFirstNode = (epidemic*)malloc(sizeof(epidemic));
	epidemic* CurrentNode = TempFirstNode;

	for (int i = 0; ; i++)
	{
		const int SuccessInput = fscanf(fp, "%d-%d%d%d%d%d",
			&CurrentNode->properties[Month], &CurrentNode->properties[Day],
			&CurrentNode->properties[Current], &CurrentNode->properties[Total],
			&CurrentNode->properties[Cured], &CurrentNode->properties[Dead]);
		CurrentNode->next = nullptr;  // 将当前节点的指向后一个节点的指针默认设为空

		if (SuccessInput >= 0 && SuccessInput < EPIDEMIC_ELEMENT_NUM)
		{
			EndFileInputTask("资源文件格式可能有误，请校对格式。", TempFirstNode, fp);
			return 1;
		}
		if (feof(fp))  // 资源文件已经到达了末尾
			break;

		epidemic* TempNode = (epidemic*)malloc(sizeof(epidemic));
		CurrentNode->next = TempNode;
		TempNode->prev = CurrentNode;
		CurrentNode = TempNode;
	}

	DesHighlight();
	FreeEpidemicList(SentinelNode.next);  // 释放旧链表
	SentinelNode.next = TempFirstNode;  // 哨兵节点与新链表连接
	SentinelNode.next->prev = nullptr;  // 新链表无法返回哨兵节点
	fclose(fp);

	InitHighlight();
	data.BaseDir = FileName;
	data.HasModified = false;
	GetDayNum();
	GetMaxElement();

	return 0;  // 无异常
}

int FileSave(char* FileName)
{
	FILE* fp = SafeFOpen(FileName, "w");
	for (epidemic* i = SentinelNode.next; i != nullptr; i = i->next)
	{
		fprintf(fp, "%d-%d %d %d %d %d",
			i->properties[Month], i->properties[Day],
			i->properties[Current], i->properties[Total],
			i->properties[Cured], i->properties[Dead]);

		if (i != nullptr && i->next != nullptr)  // 最后行末尾不换行
			fputc('\n', fp);
	}

	fclose(fp);
	return 0;
}
