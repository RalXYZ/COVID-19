/*
 * 文件名: my_files.c
 * -------------------------------------
 * 这个文件实现了一些文件处理相关的函数
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "graphics.h"

#include "my_macro.h"
#include "my_resource.h"

extern epidemic SentinelNode;

/*
 * 函数名: InitEpidemicList
 * 参数: node  一个指向你想初始化的哨兵节点的指针
 * ------------------------------------
 * 这个函数初始化了一个哨兵节点，把它的两个指针
 * 都赋成了空值，在功能上类似于构造函数。
 */
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

/*
 * 函数名: SafeFOpen
 * 参数1: fpp  指向文件指针的指针
 * 参数2: FileName  文件名
 * 参数3: mode  打开文件的模式
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
 * 参数1: reason  错误原因
 * 参数2: FirstNode  想要释放的链表的首动态节点值
 * 参数3: fp  文件指针
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

/*
 * 函数名: FileInputList
 * 参数1: FileName  资源文件的文件名
 * 参数2: begin  从哪天开始（目前从0开始）
 * 参数3: end  到哪天结束（目前功能不完备）
 * 返回值: 错误枚举量，定义见my_macro.h
 * ------------------------------------
 * 这个函数将资源文件里的数据输入到链表中，并自
 * 动处理输入时的一些常见异常。
 */
enum error FileInputList(char* FileName, int begin, int end)
{
	FILE* fp = nullptr;
	SafeFOpen(&fp, FileName, "r");

	epidemic* TempFirstNode = (epidemic*)malloc(sizeof(epidemic));
	epidemic* CurrentNode = TempFirstNode;

	for (int i = 0; ; i++)
	{
		const int SuccessInput = fscanf(fp, "%d-%d%d%d%d%d", &CurrentNode->time.mm,
			&CurrentNode->time.dd, &CurrentNode->confirmed.current,
			&CurrentNode->confirmed.total, &CurrentNode->cured, &CurrentNode->dead);
		CurrentNode->next = nullptr;  // 讲当前节点的指向后一个节点的指针默认设为空

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

	return Null;  // 无异常

	/*
	如果你想对这个函数进行测试，请在主函数中加入如下代码：
	if (!FileInputList("../myResourceFiles/statistics.txt", 1, 4))
	{
		for (struct epidemic* i = SentinelNode.next; i != nullptr; i = i->next)
			printf("%d-%d %d ", i->time.mm, i->time.dd, i->cured);
	}
	*/
}
