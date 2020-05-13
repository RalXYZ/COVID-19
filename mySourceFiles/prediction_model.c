/*
 * �ļ���: prediction_model.c
 * -------------------------------------
 * ����ļ�ʵ���˻���SEIRģ�͵�����Ԥ��
 * ��ʹ�����ݻ�ͼʱ����ؽ���������һ����: [day - 1]
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

static int InflectionDay;//�յ�����
static int InflectionNumber;//�յ�ֵ
int NeedMonth;//�����·�
int NeedDay;//��������

double S[102] = { 0 };//�׸���
double E[102] = { 0 };//Ǳ����
double I[102] = { 0 };//��Ⱦ��
double R[102] = { 0 };//������
/*
 * ������1: SEIREnterInt
 * ������2: SEIREnterDouble
 * ����1: i_infection_rate ��Ⱦ�ߴ�Ⱦ��
 * ����2: e_infection_rate Ǳ���ߴ�Ⱦ��                                                                                                                                                          
 * ����3: e_turnto_i Ǳ���߱�Ϊ��Ⱦ�ߵı���
 * ����4: i_touch ��Ⱦ�߽Ӵ�����
 * ����5: e_touch Ǳ���߽Ӵ�����
 * ����6: recovery_rate ��������
 * ����7: mul_1 ����7*����10
 * ����8: mul_2 ����8*����11
 * -------------------------------------
 * ������������¼������ģ�͵Ļ�������
 * �����Ѿ�����Ԥ��
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
 * ������: SEIR
 * -------------------------------------
 * ������ͨ���ֱ�����������
 * ����������ݲ��洢��������
 * ��������ֱ��ʹ��������S��E��I��R
 * ���ݷ�Χ��Ҫ����100
 * �����ʱ���������������: Ԥ��ʱ�������ģ����Ч
 * ��ʹ�����ݻ�ͼʱ����ؽ���������һ����: [day - 1]
 */
void SEIR()
{	
	int K = 0;//ѭ������
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
 * ������: EpidemicInflectionPoint
 * ����1: Arr[] �������� I �� E
 * -------------------------------------
 * ������ͨ���ֱ�����������ݣ������������������ֵ(���޸�Ⱦ������Ǳ������)
 * ���������������յ�,�յ�����ΪInflectionDay��ֵΪInflectionNumber
 */
int EpidemicInflectionPoint(double Arr[])
{
	int t,k;
	int C[102];
	for (k = 0; k < 100; k++)
		C[k] = Arr[k];
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
 * ������: DateCalculate
 * -------------------------------------
 * ����������n��������
 * NeedMonth:�·� NeedDay:����
 */
void DateCalculate(int month, int day, int n)
{
	int mon[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };//��������
	int sum = 0, sum1 = 0;//���뱾��������������ǰ������
	int k;
	int t = 0;//��¼��һ��sum
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