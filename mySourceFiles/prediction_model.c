/*
 * 文件名: prediction_model.c
 * -------------------------------------
 * 这个文件实现了基于SEIR模型的疫情预测
 * 在使用数据画图时，务必将天数处减一，如: [day - 1]
 */
#include <stdio.h>
#include <math.h>

static double i_infection_rate;
static double e_infection_rate;
static double e_turnto_i;
static double i_touch;
static double e_touch;
static double recovery_rate;
static double mul_1, mul_2;

static int InflectionDay;//拐点日期
static int InflectionNumber;//拐点值

double S[102] = { 0 };//易感者
double E[102] = { 0 };//潜伏者
double I[102] = { 0 };//感染者
double R[102] = { 0 };//康复者
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
void SEIREnterInt(int* variable, int value) 
{
	*variable = value;
}
void SEIREnterDouble(double* variable, double value) 
{
	*variable = value;
}
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
void SEIR()
{	
	int K = 0;//循环变量
	double p = 10000;

	S[0] = 10000;
	E[0] = 0;
	I[0] = 1;
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
 * -------------------------------------
 * 本函数通过分别调用疫情数据，计算感染人数峰值
 * 插入排序计算疫情拐点,拐点天数为InflectionDay，值为InflectionNumber
 */
int EpidemicInflectionPoint()
{
	int t,k;
	int C[102];
	for (k = 0; k < 100; k++)
		C[k] = I[k];
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
		if (C[0] == I[k])
		{
			InflectionDay = k + 1;
		}
	}
	C[0] = InflectionNumber;
}