/*
 * 文件名: prediction_model.h
 * -------------------------------------
 *
 */

#ifndef PREDICTION_MODEL_H_
#define PREDICTION_MODEL_H_

 /*
  * 函数名1: SEIREnterInt
  * 函数名2: SEIREnterDouble
  * 参数1: i_infection_rate 感染者传染率
  * 参数2: e_infection_rate 潜伏者传染率
  * 参数3: e_turnto_i 潜伏者变为感染者的比率
  * 参数4: i_touch 感染者接触人数
  * 参数5: e_touch 潜伏者接触人数
  * 参数6: recovery_rate 康复概率
  * 参数7: mul_1 参数7*参数10
  * 参数8: mul_2 参数8*参数11
  * -------------------------------------
  * 以下两个函数录入疫情模型的基本参数
  * 参数已经有所预设
  */
void SEIREnterInt(int* variable, int value);
void SEIREnterDouble(double* variable, double value);
/*
 * 函数名: SEIR
 * -------------------------------------
 * 本函数通过分别调用疫情参数
 * 计算相关数据并存储在数组中
 * 调用数据直接使用数组名S，E，I，R
 * 数据范围不要超过100
 * 例如届时加入输入溢出提醒: 预测时间过长，模型无效
 * 在使用数据画图时，务必将天数处减一，如: [day - 1]
 */
void SEIR(int month, int day);
/*
 * 函数名: EpidemicInflectionPoint
 * 参数1: Arr[] 输入数组 I 或 E
 * -------------------------------------
 * 本函数通过分别调用疫情数据，计算各项数据人数峰值(仅限感染人数与潜伏人数)
 * 插入排序计算疫情拐点,拐点天数为InflectionDay，值为InflectionNumber
 */
double EpidemicInflectionPoint(double Arr[]);
/*
  * 函数名: DateCalculate
  * -------------------------------------
  * 本函数计算n天后的日期
  * NeedMonth:月份 NeedDay:日期
  */
void DateCalculate(int month, int day, int n);
/*
  * 函数名: StringDouble
  * -------------------------------------
  * 本函数将字符转化为double变量
  *
  */
double StringDouble(char* p);
/*
  * 函数名: StringInt
  * -------------------------------------
  * 本函数将字符转化为int变量
  *
  */
int StringInt(char* p);
/*
 * 函数名: PredictionInterface
 * -------------------------------------
 * 本函数显示进行疫情预测的输入界面
 *
 */
void PredictionInterface();
/*
 * 函数名: PredictionChart
 * -------------------------------------
 * 本函数显示进行疫情预测的图表
 *
 */
void PredictionChart();
#endif
