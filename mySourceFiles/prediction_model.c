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
static double i_infection_rate;
static double e_infection_rate;
static double e_turnto_i;
static int i_touch;
static int e_touch;
static double recovery_rate;
static double mul_1, mul_2;
static int population;

static int SEIRmonth;
static int SEIRday;
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

void PredictionInterface()
{
	static char a[10] = "10000";
	static char b[10] = "3";
	static char c[10] = "3";
	static char d[10] = "1";
	static char e[10] = "20";
	static char f[10] = "20";
	static char g[10] = "1";

	static char mon[10] = "1";
	static char day[10] = "1";

	const double wid = GetWindowWidth();
	const double hei = GetWindowHeight();

	double fH = GetFontHeight();
	double h = fH * 2;
	double w = wid / 6;
	double x = wid / 5;
	double y = 7 * hei / 8;


	SetPenColor("blue");
	drawLabel(x - fH / 2 - TextStringWidth("人口"), (y -= h * 1.5) + fH * 0.7, "人口");
	if (textbox(GenUIID(0), x, y, w, h, a, sizeof(a)))
	{
		SEIREnterInt(&population, StringInt(a));
	};

	SetPenColor("red");
	drawLabel(x - fH / 2 - TextStringWidth("感染者传染率(百分)"), (y -= h * 1.5) + fH * 0.7, "感染者传染率(百分)");
	if (textbox(GenUIID(0), x, y, w, h, b, sizeof(b)))
	{
		SEIREnterDouble(&i_infection_rate, StringDouble(b) / 100);
	};

	SetPenColor("Gray");
	drawLabel(x - fH / 2 - TextStringWidth("潜伏者传染率(百分)"), (y -= h * 1.5) + fH * 0.7, "潜伏者传染率(百分)");
	if (textbox(GenUIID(0), x, y, w, h, c, sizeof(c)))
	{
		SEIREnterDouble(&e_infection_rate, StringDouble(c) / 100);
	}

	SetPenColor("Dark Gray");
	drawLabel(x - fH / 2 - TextStringWidth("潜感转化率(百分)"), (y -= h * 1.5) + fH * 0.7, "潜感转化率(百分)");
	if (textbox(GenUIID(0), x, y, w, h, d, sizeof(d)))
	{
		SEIREnterDouble(&e_turnto_i, StringDouble(d) / 100);
	};

	SetPenColor("Magenta");
	drawLabel(x - fH / 2 - TextStringWidth("感染者接触人数"), (y -= h * 1.5) + fH * 0.7, "感染者接触人数");
	if (textbox(GenUIID(0), x, y, w, h, e, sizeof(e)))
	{
		SEIREnterInt(&i_touch, StringInt(e));
	};

	SetPenColor("Cyan");
	drawLabel(x - fH / 2 - TextStringWidth("潜伏者接触人数"), (y -= h * 1.5) + fH * 0.7, "潜伏者接触人数");
	if (textbox(GenUIID(0), x, y, w, h, f, sizeof(f)))
	{
		SEIREnterInt(&e_touch, StringInt(f));
	};

	SetPenColor("green");
	drawLabel(x - fH / 2 - TextStringWidth("康复率(百分)"), (y -= h * 1.5) + fH * 0.7, "康复率(百分)");
	if (textbox(GenUIID(0), x, y, w, h, g, sizeof(g)))
	{
		SEIREnterDouble(&recovery_rate, StringDouble(g) / 100);
	};

	SetPenColor("Black");
	drawLabel(x - fH / 2 - TextStringWidth("起始月份"), (y -= h * 1.5) + fH * 0.7, "起始月份");
	if (textbox(GenUIID(0), x, y, w, h, mon, sizeof(mon)))
	{
		SEIREnterInt(&SEIRmonth, StringInt(mon));
	};

	SetPenColor("Black");
	drawLabel(x - fH / 2 - TextStringWidth("起始日期"), (y -= h * 1.5) + fH * 0.7, "起始日期");
	if (textbox(GenUIID(0), x, y, w, h, day, sizeof(day)))
	{
		SEIREnterInt(&SEIRday, StringInt(day));
	};

}
