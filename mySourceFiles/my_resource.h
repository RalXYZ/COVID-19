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

 /*
  * 结构名: DataProperty
  * ------------------------------------
  * 这个结构体里的变量是链表中数据的一些属性
  */
typedef struct DataProperty
{
	int TotalDays;  // 链表里所存储数据的总天数
	int MaxElement;  // 链表里初日期外的最大数据，用于缩放统计图
	char* BaseDir;  // 当前文件的绝对路径
	_Bool HasModified;  // 相比于输入的文件，链表是否被修改过
} DataProperty;

#define EPIDEMIC_ELEMENT_NUM 6
#define EPIDEMIC_PROPERTY_START 2
#define EPIDEMIC_STATISTIC_NUM (EPIDEMIC_ELEMENT_NUM - EPIDEMIC_PROPERTY_START)
/*
 * 枚举名: EpidemicProperty
 * 常量1: Month    月份
 * 常量2: Day     日期
 * 常量3: Current  当前感染人数
 * 常量4: Total    总感染人数
 * 常量5: Cured    治愈人数
 * 常量6: Dead     死亡人数
 * ------------------------------------
 * 这个结构对节点里数组的下标与存储内容构建了对应关系
 */
typedef enum EpidemicProperty
{
	Month, Day,
	Current, Total, Cured, Dead
} EpidemicProperty;


/*
 * 结构名: epidemic
 * ------------------------------------
 * 这个结构是存储疫情数据用的双向链表的一个节点
 */
typedef struct epidemic
{
	int properties[EPIDEMIC_ELEMENT_NUM];  // 存储所有的属性值，每个位置存储的内容由EpidemicProperty解释
	struct epidemic* prev;  // 指向上一个节点的指针
	struct epidemic* next;  // 指向下一个节点的指针
} epidemic;

/*
 * 结构名: MyStatus
 * ------------------------------------
 * 这个结构存储了程序当前的一些状态值
 */
typedef struct MyStatus
{
	_Bool HighlightVisible;  // 高亮点是否可见
	epidemic* HighlightNode;  // 当前高亮的节点
	EpidemicProperty HighlightProperty;  // 当前高亮的项目，注意，不从0开始！！！
	int HighlightNum;  // 目前高亮的是链表中的第几个
} MyStatus;


/*
 * 函数名: InitEpidemicList
 * 参数: node  一个指向你想初始化的哨兵节点的指针
 * ------------------------------------
 * 这个函数初始化了一个哨兵节点，把它的两个指针
 * 都赋成了空值，在功能上类似于构造函数。
 */
void InitEpidemicList(epidemic* node);

/*
 * 函数名: FreeEpidemicList
 * 参数: node  一个指向你想释放的链表的首节点的指针
 * ------------------------------------
 * 这个函数循环地释放当前节点到尾节点之间的所有
 * 节点，在功能上类似析构函数。
 */
void FreeEpidemicList(epidemic* node);

/*
 * 函数名: InitHighlight
 * ------------------------------------
 * 在当前有文件时，初始化这个高亮光标
 */
void InitHighlight();

/*
 * 函数名: DesHighlight
 * ------------------------------------
 * 当文件关闭时，恢复高亮光标相关值为默认值
 */
void DesHighlight();

/*
 * 函数名: ReadEpidemicList
 * 参数1: month  想获取的数据的月份
 * 参数2: date   想获得的数据的日期
 * 参数3: type   想获得的类别具体是哪一种，参见 my_resource.h 中的 EpidemicProperty 枚举
 * 返回值: 在该日期下的 想获取的类别 的值；如果返回值是 -1 ，代表未找到日期
 * ------------------------------------
 * 这个函数的目的是取一个特定日期中的特定值。
 */
int ReadEpidemicList(int month, int date, EpidemicProperty type);

/*
 * 函数名: FileInputList
 * 参数: FileName  资源文件的文件名
 * 返回值: 0代表无异常，1代表有异常
 * ------------------------------------
 * 这个函数将资源文件里的数据输入到链表中。
 */
int FileInputList(char* FileName);

/*
 * 函数名: FileSave
 * 参数: FileName  资源文件的文件名
 * 返回值: 0代表无异常，1代表有异常
 * ------------------------------------
 * 这个函数将链表中的数据保存到硬盘中。
 */
int FileSave(char* FileName);

#endif
