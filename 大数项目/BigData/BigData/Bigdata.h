#pragma once
#include<iostream>
#include<assert.h>
#include<string>

using namespace std;

#define UN_INIT 0xcccccccccccccccc
#define MAX_INT64 0x7FFFFFFFFFFFFFFF  //64λ�����
#define MIN_INT64 0x8000000000000000  //64λ��С��

class BigData
{
public :
	BigData(long long value = UN_INIT)  //��������
	{
		_value = value;
		long long tmp = _value;
		int count = 0;
		while (tmp != 0)
		{
			++count;
			tmp /= 10;
		}
		_pdata.resize(count+1);
		_pdata[0] = '+';
		tmp = _value;
		if (tmp < 0)
		{
			_pdata[0] = '-';
			tmp *= -1;
		}
		for (int index = 0; index < count; ++index)
		{
			_pdata[count - index] = tmp % 10 + '0';
			int i = tmp % 10;
			tmp /= 10;
		}
	}
	BigData(char* pdata):    //�����ַ���
		_value(0)
	{
		assert(pdata);
		//�����һλ
		char symbol = pdata[0];
		if (pdata[0] >= '0'&& pdata[0] <= '9')
		{
			symbol = '+';
		}
		else if (pdata[0] == '+' || pdata[0] == '-')
		{
			++pdata;
		}
		else
		{
			_value = UN_INIT;
			return;
		}
		//�������λ����ַ�
		while (*pdata == '0')
		{
			++pdata;
		}
		_pdata.resize(strlen(pdata) + 1);//Ϊ�ַ������ٿռ䣬�࿪��һλ�������λ
		_pdata[0] = symbol;//��һλ�������λ
		int index = 1;
		while (*pdata >= '0'&&*pdata <= '9')
		{
			_value = _value*10+*pdata - '0';
			_pdata[index++] = *pdata++;
		}
		if (symbol == '-')
		{
			_value = 0 - _value;
		}
		_pdata.resize(index);
	}
	~BigData()
	{}
public:
	friend ostream& operator<<(ostream& os,const BigData& bigdata);

	BigData operator+(const BigData bigdata);  //���ؼ�
	string Add(string left, string right);
	BigData operator-(const BigData bigdata);  //���ؼ�
	string Sub(string left, string right);
	BigData operator*(const BigData bigdata);  //���س�
	string Mul(string left, string right);
	BigData operator/(const BigData bigdata);  //���س�
	string Div(string left, string right);
	char SubLoop(char* pleft,int Lsize, char* right,int Rsize);
	bool IsLeftStrBig(const char* left, int Lsize, const char* right, int Rsize);
private:
	bool IsINT64Overflow()const;
private:
	long long _value;  //����С��MAX_INT64����
	string  _pdata;     
};
