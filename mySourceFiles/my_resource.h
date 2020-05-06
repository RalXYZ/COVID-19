/*
 * 文件名: my_files.h
 * -------------------------------------
 * 这个文件声明了一些文件处理相关的函数
 *
 */

#ifndef MY_FILES_H_
#define MY_FILES_H_

#ifndef __cplusplus  // 此语句避免了宏定义与C++发生冲突
#define nullptr NULL  // nullptr 是一个C++关键字，表示一个特定的空指针
#endif



 /*
  * 结构名: confirmed
  * 成员1: current  当前确诊人数
  * 成员2: total  总确诊人数
  * ------------------------------------
  * 这个结构需要与epidemic搭配使用。
  */
struct confirmed
{
	int current;
	int total;
};


/*
 * 结构名: time
 * 成员1: mm  月份
 * 成员2: dd  日期
 * ------------------------------------
 * 这个结构需要与epidemic搭配使用。
 */
struct time
{
	int mm;
	int dd;
};

#define EPIDEMIC_ELEMENT_NUM 6
/*
 * 结构名: epidemic
 * 成员1: time  时间
 * 成员2: confirmed  确诊人数
 * 成员3: cured  治愈人数
 * 成员4: dead  死亡人数
 * 成员5: prev  指向上一个节点的指针
 * 成员6: next  指向下一个节点的指针
 * ------------------------------------
 * 这个结构是存储疫情数据用的双向链表的一个节点。
 */
struct epidemic
{
	struct time time;
	struct confirmed confirmed;
	int cured;
	int dead;

	struct epidemic* prev;
	struct epidemic* next;
};

/*
 * 函数名: InitEpidemicList
 * 参数: node  一个指向你想初始化的哨兵节点的指针
 * ------------------------------------
 * 这个函数初始化了一个哨兵节点，把它的两个指针
 * 都赋成了空值，在功能上类似于构造函数。
 */
void InitEpidemicList(struct epidemic* node);

/*
 * 函数名: FileInputList
 * 参数1: FileName  资源文件的文件名
 * 参数2: begin  从哪天开始（目前功能不完备）
 * 参数3: end  到哪天结束（目前功能不完备）
 * 返回值: 错误枚举量，定义见my_macro.h
 * ------------------------------------
 * 这个函数将资源文件里的数据输入到链表中，并自
 * 动处理输入时的一些常见异常。
 */
enum error FileInputList(char* FileName, int begin, int end);

#endif
