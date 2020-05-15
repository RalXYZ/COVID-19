/*
 * 文件名: my_utilities.h
 * -------------------------------------
 *
 */

#ifndef MY_UTILITIES_H_
#define MY_UTILITIES_H_

 /*
  * 函数名: UpSelectProperty
  * 返回值: 将改为的属性值
  * ------------------------------------
  * 这个函数决定了当按↑键时，属性值应当改为哪个
  */
EpidemicProperty UpSelectProperty();

/*
 * 函数名: DownSelectProperty
 * 返回值: 将改为的属性值
 * ------------------------------------
 * 这个函数决定了当按↓键时，属性值应当改为哪个
 */
EpidemicProperty DownSelectProperty();

#endif
