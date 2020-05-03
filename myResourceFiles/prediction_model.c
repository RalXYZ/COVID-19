/*
 * 文件名: prediction_model.c
 * -------------------------------------
 * 这个文件实现了基于SEIR模型的疫情预测
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
static double mul_1, mul_2;

/*
 * 函数名: SEIR
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
 * 参数12: mul_1 参数7*参数10
 * 参数13: mul_2 参数8*参数11
 * 参数14: 保留
 */