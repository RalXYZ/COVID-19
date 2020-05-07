/*
 * 文件名: my_resource.h
 * -------------------------------------
 * 这个文件声明了一些文件处理相关的函数
 *
 */

#ifndef MY_RESOURCE_H_
#define MY_RESOURCE_H_

#ifndef __cplusplus  // 此语句避免了宏定义与C++发生冲突
#define nullptr NULL  // nullptr 是一个C++关键字，表示一个特定的空指针
#endif

#define EPIDEMIC_ELEMENT_NUM 6
#define EPIDEMIC_PROPERTY_START 2
 /*
  * 枚举名: EpidemicProperty
  * 常量1: Month    月份
  * 常量2: Date     日期
  * 常量3: Current  当前感染人数
  * 常量4: Total    总感染人数
  * 常量5: Cured    治愈人数
  * 常量6: Dead     死亡人数
  * ------------------------------------
  * 这个结构是存储疫情数据用的双向链表的一个节点。
  */
typedef enum EpidemicProperty
{
	Month, Date,
	Current, Total, Cured, Dead
} EpidemicProperty;


/*
 * 结构名: epidemic
 * 成员1: properties  存储所有的属性值，每个位置存储的内容由EpidemicProperty解释
 * 成员2: prev        指向上一个节点的指针
 * 成员3: next        指向下一个节点的指针
 * ------------------------------------
 * 这个结构是存储疫情数据用的双向链表的一个节点。
 */
typedef struct epidemic
{
	int properties[EPIDEMIC_ELEMENT_NUM];
	struct epidemic* prev;
	struct epidemic* next;
} epidemic;


/*
 * 函数名: InitEpidemicList
 * 参数: node  一个指向你想初始化的哨兵节点的指针
 * ------------------------------------
 * 这个函数初始化了一个哨兵节点，把它的两个指针
 * 都赋成了空值，在功能上类似于构造函数。
 */
void InitEpidemicList(epidemic* node);

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
