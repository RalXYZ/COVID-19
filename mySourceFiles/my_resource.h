/*
 * 文件名: my_files.h
 * -------------------------------------
 * 这个文件声明了一些文件处理相关的函数
 *
 */

#ifndef _MY_FILES_H_
#define _MY_FILES_H_

#ifndef __cplusplus
#define nullptr ((void *)0)
#endif

#define REGION_ARRAY_LENGTH 10

struct confirmed
{
	int current;
	int total;
};

struct epidemic
{
	char region[REGION_ARRAY_LENGTH];
	struct confirmed confirmed;
	int cured;
	int dead;

	struct epidemic* prev;
	struct epidemic* next;
};

void InitEpidemicList(struct epidemic** node);
enum error FileOpen(int begin, int end);

#endif
