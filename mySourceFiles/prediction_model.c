/*
 * 文件名: prediction_model.c
 * -------------------------------------
 * 这个文件实现了基于SEIR模型的疫情预测
 * 注意: 这是个半成品，还没有完全实现，内部可能存在问题
 */
#include <stdio.h>
#include <math.h>

static int p;
static int s;
static int e;
static int i;
static int r;
static int day;

static double i_infection_rate;
static double e_infection_rate;
static double e_turnto_i;
static double i_touch;
static double e_touch;
static double recovery_rate;
static double mul_1, mul_2;

/*
 * 函数名1: SEIR_ENTER_INT
 * 函数名2: SEIR_ENTER_DOUBLE
 * 参数1: p 总人口
 * 参数2: s 易感者
 * 参数3: e 潜伏者
 * 参数4: i 感染者
 * 参数5: r 康复者
 * 参数6: day 疫情发生天数
 * 参数7: i_infection_rate 感染者传染率
 * 参数8: e_infection_rate 潜伏者传染率
 * 参数9: e_turnto_i 潜伏者变为感染者的比率
 * 参数10: i_touch 感染者接触人数
 * 参数11: e_touch 潜伏者接触人数
 * 参数12: recovery_rate 康复概率
 * 参数13: mul_1 参数7*参数10
 * 参数14: mul_2 参数8*参数11
 * -------------------------------------
 * 本函数录入疫情模型的基本参数
 */
void SEIR_ENTER_INT(int* variable, int value) 
{
	*variable = value;
}
void SEIR_ENTER_DOUBLE(double* variable, double value) 
{
	*variable = value;
}

 /*
  * 函数名: SEIR
  * 参数1: 函数名{S, E, I, R}
  * 参数2: day 疫情发生天数
  * -------------------------------------
  * 本函数通过分别调用疫情数据函数
  * 输入天数，并返回所需求的数据值
  */
int SEIR(int(*function)(int), int day)
{
	mul_1 = i_infection_rate * i_touch * I(day - 1);
	mul_2 = e_infection_rate * e_touch * E(day - 1);
	return function(day);
}

/*
 * 函数名: S
 * 参数: day
 * -------------------------------------
 * 截至今天易感人数 = 截至昨天易感人数 + 被感染者传染的易感者 + 被潜伏者传染的易感者
 */
int S(int day)
{
	return S(day - 1) - mul_1 * S(day - 1) - mul_2 * S(day - 1);
}

/*
 * 函数名: E
 * 参数: day
 * -------------------------------------
 * 截至今天潜伏人数 = 截至昨天潜伏人数 + 被感染者传染的潜伏者 + 被潜伏者传染的潜伏者
 */
int E(int day)
{
	return E(day - 1) + mul_1 * S(day - 1) + mul_2 * S(day - 1) - e_turnto_i * E(day - 1);
}

/*
 * 函数名: I
 * 参数: day
 * -------------------------------------
 * 截至今天感染人数 = 截至昨天感染人数 + 潜伏者转阳人数 - 昨天新康复人数
 */
int I(int day)
{
	return I(day - 1) + e_turnto_i * E(day - 1) - recovery_rate * I(day - 1);
}

/*
 * 函数名: R
 * 参数: day
 * -------------------------------------
 * 截至今天康复人数 = 截至昨天康复人数 + 昨天新康复人数
 */
int R(int day)
{
	return R(day - 1) + recovery_rate * I(day - 1);
}