/*
 * �ļ���: prediction_model.c
 * -------------------------------------
 * ����ļ�ʵ���˻���SEIRģ�͵�����Ԥ��
 * ע��: ���Ǹ����Ʒ����û����ȫʵ�֣��ڲ����ܴ�������
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
 * ������1: SEIR_ENTER_INT
 * ������2: SEIR_ENTER_DOUBLE
 * ����1: p ���˿�
 * ����2: s �׸���
 * ����3: e Ǳ����
 * ����4: i ��Ⱦ��
 * ����5: r ������
 * ����6: day ���鷢������
 * ����7: i_infection_rate ��Ⱦ�ߴ�Ⱦ��
 * ����8: e_infection_rate Ǳ���ߴ�Ⱦ��
 * ����9: e_turnto_i Ǳ���߱�Ϊ��Ⱦ�ߵı���
 * ����10: i_touch ��Ⱦ�߽Ӵ�����
 * ����11: e_touch Ǳ���߽Ӵ�����
 * ����12: recovery_rate ��������
 * ����13: mul_1 ����7*����10
 * ����14: mul_2 ����8*����11
 * -------------------------------------
 * ������¼������ģ�͵Ļ�������
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
  * ������: SEIR
  * ����1: ������{S, E, I, R}
  * ����2: day ���鷢������
  * -------------------------------------
  * ������ͨ���ֱ�����������ݺ���
  * ���������������������������ֵ
  */
int SEIR(int(*function)(int), int day)
{
	mul_1 = i_infection_rate * i_touch * I(day - 1);
	mul_2 = e_infection_rate * e_touch * E(day - 1);
	return function(day);
}

/*
 * ������: S
 * ����: day
 * -------------------------------------
 * ���������׸����� = ���������׸����� + ����Ⱦ�ߴ�Ⱦ���׸��� + ��Ǳ���ߴ�Ⱦ���׸���
 */
int S(int day)
{
	return S(day - 1) - mul_1 * S(day - 1) - mul_2 * S(day - 1);
}

/*
 * ������: E
 * ����: day
 * -------------------------------------
 * ��������Ǳ������ = ��������Ǳ������ + ����Ⱦ�ߴ�Ⱦ��Ǳ���� + ��Ǳ���ߴ�Ⱦ��Ǳ����
 */
int E(int day)
{
	return E(day - 1) + mul_1 * S(day - 1) + mul_2 * S(day - 1) - e_turnto_i * E(day - 1);
}

/*
 * ������: I
 * ����: day
 * -------------------------------------
 * ���������Ⱦ���� = ���������Ⱦ���� + Ǳ����ת������ - �����¿�������
 */
int I(int day)
{
	return I(day - 1) + e_turnto_i * E(day - 1) - recovery_rate * I(day - 1);
}

/*
 * ������: R
 * ����: day
 * -------------------------------------
 * �������쿵������ = �������쿵������ + �����¿�������
 */
int R(int day)
{
	return R(day - 1) + recovery_rate * I(day - 1);
}