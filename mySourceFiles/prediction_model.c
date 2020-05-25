/*
 * 文件名: prediction_model.c
 * -------------------------------------
 * 这个文件实现了基于SEIR模型的疫情预测
 * 在使用数据画图时，务必将天数处减一，如: [day - 1]
 */
#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "my_macro.h"
#include "my_callback.h"
#include "draw_chart.h"
#include "my_display.h"
#include "my_resource.h"
#include "prediction_model.h"
 /*
  * SEIR相关函数集中注释
  * -------------------------------------
  * 参数1: i_infection_rate 感染者传染率
  * 参数2: e_infection_rate 潜伏者传染率
  * 参数3: e_turnto_i 潜伏者变为感染者的比率
  * 参数4: i_touch 感染者接触人数
  * 参数5: e_touch 潜伏者接触人数
  * 参数6: recovery_rate 康复概率
  * 参数7: mul_1 参数7*参数10
  * 参数8: mul_2 参数8*参数11
 */
double i_infection_rate;
double e_infection_rate;
double e_turnto_i;
int i_touch;
int e_touch;
double recovery_rate;
double mul_1, mul_2;
int population;

int SEIRmonth;
int SEIRday;
static int InflectionDay;//拐点日期
static int InflectionNumber;//拐点值
int NeedMonth;//需求月份
int NeedDay;//需求日期

double S[102] = { 0 };//易感者
double E[102] = { 0 };//潜伏者
double I[102] = { 0 };//感染者
double R[102] = { 0 };//康复者

void SEIREnterInt(int* variable, int value)
{
	*variable = value;
}

void SEIREnterDouble(double* variable, double value)
{
	*variable = value;
}

void SEIR(int SEIRmonth, int SEIRday)
{
	int K = 0;//循环变量
	double p = 10000;

	S[0] = population;
	E[0] = 0;
	I[0] = ReadEpidemicList(SEIRmonth, SEIRday, Current);
	R[0] = 0;

	for (K = 1; K < 100; K++)
	{
		mul_1 = i_infection_rate * i_touch * I[K - 1];
		mul_2 = e_infection_rate * e_touch * E[K - 1];

		S[K] = S[K - 1] - mul_1 * S[K - 1] / p - mul_2 * S[K - 1] / p;
		E[K] = E[K - 1] + mul_1 * S[K - 1] / p + mul_2 * S[K - 1] / p - e_turnto_i * E[K - 1];
		I[K] = I[K - 1] + e_turnto_i * E[K] - recovery_rate * I[K - 1];
		R[K] = R[K - 1] + recovery_rate * I[K - 1];
	}
}
/*
 * 函数名: EpidemicInflectionPoint
 * 参数1: Arr[] 输入数组 I 或 E
 * -------------------------------------
 * 本函数通过分别调用疫情数据，计算各项数据人数峰值(仅限感染人数与潜伏人数)
 * 插入排序计算疫情拐点,拐点天数为InflectionDay，值为InflectionNumber
 */
void EpidemicInflectionPoint(double Arr[])
{
	int t, k;
	int C[102];
	for (k = 0; k < 100; k++)
		C[k] = (int)Arr[k];
	int i, j;
	for (i = 1; i < 100; i++)
	{
		t = C[i];
		for (j = i; j > 0 && t > C[j - 1]; j--)
			C[j] = C[j - 1];
		C[j] = t;
	}
	for (k = 0; k < 100; k++)
	{
		if (C[0] == Arr[k])
		{
			InflectionDay = k + 1;
		}
	}
	C[0] = InflectionNumber;
}
/*
 * 函数名: DateCalculate
 * -------------------------------------
 * 本函数计算n天后的日期
 * NeedMonth:月份 NeedDay:日期
 */
void DateCalculate(int month, int day, int n)
{
	int mon[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };//各月天数
	int sum = 0, sum1 = 0;//计入本月总天数，本月前总天数
	int k;
	int t = 0;//记录上一个sum
	for (k = 0; k < month - 1; k++)
	{
		sum1 += mon[k];
	}
	for (k = 0; k < 12; k++)
	{
		sum += mon[k];
		if (sum >= day + sum1 + n)
		{
			NeedMonth = k + 1;
			break;
		}
		t = sum;
	}
	NeedDay = sum1 + day + n - t;
}

double StringDouble(char* p)
{
	double temp = 0;
	while (*p)
	{
		temp = temp * 10 + *p - '0';
		p++;
	}
	return temp;
}

int StringInt(char* p)
{
	int temp = 0;
	while (*p)
	{
		temp = temp * 10 + *p - '0';
		p++;
	}
	return temp;
}
