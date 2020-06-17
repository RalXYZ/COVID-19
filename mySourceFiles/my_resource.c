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
#include "my_display.h"

DataProperty data = { 0, 0, nullptr, false };  // 链表相关属性值
CompareDataProperty CompareData = { 0, nullptr };
epidemic SentinelNode;  // 哨兵节点
epidemic CompareSentinelNode;  // 辅助链表的哨兵节点，用于对比
MyStatus status = { false, nullptr, 0, 0 ,
				  false , false , false };  // 当前状态

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
	//SentinelNode.next = nullptr;  // 恢复为默认值
}

void InitHighlight()
{
	status.HighlightVisible = true;
	status.HighlightNode = SentinelNode.next;
	status.HighlightProperty = EPIDEMIC_PROPERTY_START + 1;  // 从“现有确诊”开始
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

int ReadEpidemicCompareList(int month, int date, EpidemicProperty type)
{
	if (0 <= type && type < EPIDEMIC_ELEMENT_NUM)
	{
		for (epidemic* i = CompareSentinelNode.next; i != nullptr; i = i->next)
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
 * 参数1: FileName  文件名
 * 参数2: mode      打开文件的模式
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

void GetDayNum()
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

/*
 * 函数名: GetCompareMaxElement
 * ------------------------------------
 * 取得主链表和辅助链表中 除日期外的最大元素
 */
void GetCompareMaxElement()
{
	CompareData.MaxAllElement = data.MaxElement;  // 至少是原链表的最大值
	for (EpidemicProperty i = EPIDEMIC_PROPERTY_START; i < EPIDEMIC_ELEMENT_NUM; i++)
	{
		for (epidemic* j = CompareSentinelNode.next; j != nullptr; j = j->next)
			CompareData.MaxAllElement < j->properties[i] ? CompareData.MaxAllElement = j->properties[i] : pass;
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
			&CurrentNode->properties[New], &CurrentNode->properties[Current],
			&CurrentNode->properties[Cured], &CurrentNode->properties[Dead]);
		CurrentNode->next = nullptr;  // 将当前节点的指向后一个节点的指针默认设为空

		if (SuccessInput >= 0 && SuccessInput < EPIDEMIC_ELEMENT_NUM)
		{
			GUIOutputMsg("文件格式有误");
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

	/* 检查总天数是否过小 */
	int TotalDays = 1;
	for (epidemic* i = TempFirstNode; i->next != nullptr; i = i->next, ++TotalDays)
		pass;
	if (TotalDays < MIN_LIST_LENGTH)
	{
		GUIOutputMsg("文件天数太少");
		EndFileInputTask("资源文件中的天数太少。", TempFirstNode, fp);
		return 2;
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

int FileInputCompareList(char* FileName)
{
	FILE* fp = SafeFOpen(FileName, "r");

	epidemic* TempFirstNode = (epidemic*)malloc(sizeof(epidemic));
	epidemic* CurrentNode = TempFirstNode;

	for (int i = 0; ; i++)
	{
		const int SuccessInput = fscanf(fp, "%d-%d%d%d%d%d",
			&CurrentNode->properties[Month], &CurrentNode->properties[Day],
			&CurrentNode->properties[New], &CurrentNode->properties[Current],
			&CurrentNode->properties[Cured], &CurrentNode->properties[Dead]);
		CurrentNode->next = nullptr;  // 将当前节点的指向后一个节点的指针默认设为空

		if (SuccessInput >= 0 && SuccessInput < EPIDEMIC_ELEMENT_NUM)
		{
			GUIOutputMsg("文件格式有误");
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

	/* 检查总天数是否过小 */
	int TotalDays = 1;
	for (epidemic* i = TempFirstNode; i->next != nullptr; i = i->next, ++TotalDays)
		pass;
	if (TotalDays < MIN_LIST_LENGTH)
	{
		GUIOutputMsg("文件天数太少");
		EndFileInputTask("资源文件中的天数太少。", TempFirstNode, fp);
		return 2;
	}

	FreeEpidemicList(CompareSentinelNode.next);  // 释放旧链表
	CompareSentinelNode.next = TempFirstNode;  // 哨兵节点与新链表连接
	CompareSentinelNode.next->prev = nullptr;  // 新链表无法返回哨兵节点
	fclose(fp);

	InitHighlight();
	CompareData.BaseDir = FileName;
	GetCompareMaxElement();

	return 0;  // 无异常
}

int FileSave(char* FileName)
{
	FILE* fp = SafeFOpen(FileName, "w");
	for (epidemic* i = SentinelNode.next; i != nullptr; i = i->next)
	{
		fprintf(fp, "%d-%d %d %d %d %d",
			i->properties[Month], i->properties[Day],
			i->properties[New], i->properties[Current],
			i->properties[Cured], i->properties[Dead]);

		if (i != nullptr && i->next != nullptr)  // 最后行末尾不换行
			fputc('\n', fp);
	}

	fclose(fp);
	return 0;
}
