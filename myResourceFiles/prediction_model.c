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
 * 参数1:
 * 参数2:
 * 参数3:
 * 参数4:
 * 参数5:
 * 参数6:
 * 参数7:
 * 参数8:
 * 参数9:
 * 参数10:
 * 参数11:
 * 参数12:
 * 参数13:
 * 参数14:
 */