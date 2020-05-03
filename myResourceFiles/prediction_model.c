/*
 * �ļ���: prediction_model.c
 * -------------------------------------
 * ����ļ�ʵ���˻���SEIRģ�͵�����Ԥ��
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
 * ������: SEIR
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
 * ����12: mul_1 ����7*����10
 * ����13: mul_2 ����8*����11
 * ����14: ����
 */