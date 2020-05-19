/*
 * 文件名: my_utilities.h
 * -------------------------------------
 *
 */

#ifndef MY_UTILITIES_H_
#define MY_UTILITIES_H_

 /*
  * 函数名: MyExitConsole
  * -------------------------------------
  * libgraphics 并没有实现关闭终端窗口的函数
  * 这个函数模仿 InitConsole()，实现了关闭终端窗口
  */
void MyExitConsole();

/*
 * 函数名: PauseDisplay
 * -------------------------------------
 * 封装了停止GUI行为时所需的一些步骤
 */
void PauseDisplay();

/*
 * 函数名: ContinueDisplay
 * -------------------------------------
 * 封装了取消停止GUI行为时所需的一些步骤
 */
void ContinueDisplay();

/*
 * 函数名: SafeNNegIntInput
 * 参数:   digits  待输入数据的十进制位数
 * 返回值: 如果为非负数，则是输入结果；如果是负数，代表出现异常，含义详见函数实现
 * ------------------------------------
 * 安全的非负整数输入
 */
int SafeNNegIntInput(int digits);

/*
 * 函数名: UpSelectProperty
 * 返回值: 将改为的属性值
 * ------------------------------------
 * 这个函数决定了当按↑键时，属性值应当改为哪个
 */
int UpSelectProperty();

/*
 * 函数名: DownSelectProperty
 * 返回值: 将改为的属性值
 * ------------------------------------
 * 这个函数决定了当按↓键时，属性值应当改为哪个
 */
int DownSelectProperty();

/*
 * 函数名: PropertyMeaning
 * 参数:   property  枚举量
 * 返回值: 枚举量的含义
 * ------------------------------------
 * 解释枚举量的含义
 */
char* PropertyMeaning(int property);

#endif
