#pragma once
#include<iostream>
#include<assert.h>
#include<string>

using namespace std;

#define UN_INIT 0xcccccccccccccccc
#define MAX_INT64 0x7FFFFFFFFFFFFFFF  //64位最大数
#define MIN_INT64 0x8000000000000000  //64位最小数

class BigData
{
public :
	BigData(long long value = UN_INIT)  //传入数字
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
	BigData(char* pdata):    //传入字符串
		_value(0)
	{
		assert(pdata);
		//处理第一位
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
		//处理符号位后的字符
		while (*pdata == '0')
		{
			++pdata;
		}
		_pdata.resize(strlen(pdata) + 1);//为字符串开辟空间，多开辟一位保存符号位
		_pdata[0] = symbol;//第一位保存符号位
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

	BigData operator+(const BigData bigdata);  //重载加
	string Add(string left, string right);
	BigData operator-(const BigData bigdata);  //重载减
	string Sub(string left, string right);
	BigData operator*(const BigData bigdata);  //重载乘
	string Mul(string left, string right);
	BigData operator/(const BigData bigdata);  //重载除
	string Div(string left, string right);
	char SubLoop(char* pleft,int Lsize, char* right,int Rsize);
	bool IsLeftStrBig(const char* left, int Lsize, const char* right, int Rsize);
private:
	bool IsINT64Overflow()const;
private:
	long long _value;  //保存小于MAX_INT64的数
	string  _pdata;     
};
