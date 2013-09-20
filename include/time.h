#ifndef _TIME_H
#define _TIME_H

#ifndef _TIME_T
#define _TIME_T
typedef long time_t;		// ��GMT 1970 ��1 ��1 �տ�ʼ�����������ʱ�䣨����ʱ�䣩��
#endif

#define CLOCKS_PER_SEC 100	// ϵͳʱ�ӵδ�Ƶ�ʣ�100HZ��

typedef long clock_t;		// �ӽ��̿�ʼϵͳ������ʱ�ӵδ�����

struct tm
{
  int tm_sec;			// ���� [0��59]��
  int tm_min;			// ������ [ 0��59]��
  int tm_hour;			// Сʱ�� [0��59]��
  int tm_mday;			// 1 ���µ����� [0��31]��
  int tm_mon;			// 1 �����·� [0��11]��
  int tm_year;			// ��1900 �꿪ʼ��������
  int tm_wday;			// 1 �����е�ĳ�� [0��6]�������� =0����
  int tm_yday;			// 1 ���е�ĳ�� [0��365]��
  int tm_isdst;			// ����ʱ��־��
};
long kernel_mktime (struct tm *tm);
 
#endif
