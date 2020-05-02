/*
 * 文件名: my_files.c
 * -------------------------------------
 * 这个文件实现了一些文件处理相关的函数
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graphics.h"

#include "my_macro.h"
#include "my_resource.h"

extern struct epidemic* SentinelNode;

void InitEpidemicList(struct epidemic** node)
{
	*node = (struct epidemic*)malloc(sizeof(struct epidemic));
	(*node)->prev = nullptr;
	(*node)->next = nullptr;
}

void FreeEpidemicList(struct epidemic* node)
{
	struct epidemic* CurrentNode = node;
	while (CurrentNode != nullptr)
	{
		struct epidemic* TempNode = CurrentNode->next;
		free(CurrentNode);
		CurrentNode = TempNode;
	}
}

void SafeFOpen(FILE** fp, char* FileName, char* mode)
{
	if ((*fp = fopen(FileName, mode)) == NULL)
	{
		InitConsole();
		printf("File open error! at %s in mode \"%s\"\n", FileName, mode);
		exit(0);
	}
}

enum error FileOpen(int begin, int end)
{
	FILE* fp = nullptr;
	SafeFOpen(&fp, "../myResourceFiles/test.txt", "r");

	InitConsole();  ///////////////////////////////////

	struct epidemic* TempSentinelNode = (struct epidemic*)malloc(sizeof(struct epidemic));
	struct epidemic* CurrentNode = TempSentinelNode;

	for (int i = 0; ; i++)
	{
		fscanf(fp, "%s%d%d%d%d", CurrentNode->region, &CurrentNode->confirmed.current,
			&CurrentNode->confirmed.total, &CurrentNode->cured, &CurrentNode->dead);
		if (feof(fp))
		{
			CurrentNode->next = nullptr;
			break;
		}
		struct epidemic* TempNode = (struct epidemic*)malloc(sizeof(struct epidemic));
		CurrentNode->next = TempNode;
		TempNode->prev = CurrentNode;
		CurrentNode = TempNode;
	}

	FreeEpidemicList(SentinelNode->next);
	SentinelNode->next = TempSentinelNode;

	return Null;
}
