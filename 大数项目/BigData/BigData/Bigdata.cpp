#define _CRT_SECURE_NO_WARNINGS 1

#include"Bigdata.h"


ostream& operator<<(ostream& os, const BigData& bigdata)
{
	//两种情况，输出_value,输出_pdata
	//需判断_value是否保存的值有效
	if (!bigdata.IsINT64Overflow())   //_value保存的数据没有溢出，直接输出_value
	{
		os << bigdata._value << endl;
	}
	else    
	{
		char* str = (char*)bigdata._pdata.c_str();
		if ('+' == bigdata._pdata[0])   //若符号位为'+'直接跳过，不输出
		{
			++str;
		}
		cout << str;
	}
	return os;
}

bool BigData::IsINT64Overflow()const 
{
	string tmp("+9223372036854775807");
	if ('-' == _pdata[0])
	{
		tmp = "-9223372036854775806";
	}
	if (_pdata.size() < tmp.size())
	{
		return false;
	}
	else if (_pdata.size() == tmp.size() && _pdata <= tmp)
	{
		return false;
	}
	return true;
}
BigData BigData::operator + (const BigData bigdata)
{
	//两种情况。1、两数均未溢出2、两数至少一个溢出
	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow())
	{
		//两数相加溢出，两数相加未溢出
		//两种情况。1、同号 2、异号
		if (_pdata[0] != bigdata._pdata[0])  //异号
		{
			return BigData(_value + bigdata._value);
		}
		else   //同号
		{
			//两种情况。1、同为正 2、同为负
			if ('+' == _pdata[0])
			{
				if (MAX_INT64 - _value > bigdata._value)  //相加未溢出
				{
					return BigData(_value + bigdata._value);
				}

			}
			else
			{
				if (MIN_INT64 - _value < bigdata._value)  //相加未溢出
				{
					return BigData(_value + bigdata._value);
				}
			}
		}
	}
	else   		//至少一个相加数溢出
	{
		return BigData((char*)Add(_pdata, bigdata._pdata).c_str());//同号异号均取长的为符号位
		//if (_pdata[0] == bigdata._pdata[0])
		//{
		//	return BigData((char*)Add(_pdata, bigdata._pdata).c_str());		
		//}
		//else
		//{
		//	return BigData(*this-bigdata);
		//}
	}
	return BigData("");
}
string BigData::Add(string left, string right)
{
	//相加时控制循环的次数依靠二者中较长的数，开辟空间也是开辟较长数的size+1
	int Lsize = left.size();
	int Rsize = right.size();

	if (Lsize < Rsize)
	{
		swap(left, right);
		swap(Lsize, Rsize);
	}
	string str;
	str.resize(Lsize + 1);

	//相加
	str[0] = left[0];
	char step = 0;
	for (int index = 1; index < Lsize; ++index)
	{
		char cRet = left[Lsize - index] - '0' + step;
		if (index < Rsize)
		{
			cRet += right[Rsize-index] - '0';
		}
		str[Lsize + 1 - index] = cRet%10 + '0';
		step = cRet / 10;
	}
	str[1] = step + '0';
	return str;
}
BigData BigData::operator-(const BigData bigdata)
{
	//两种情况，1、两数均未溢出2、两数中至少一个溢出

	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow())  
	{
		//两种情况，1、两数相减结果溢出2、两数相减结果未溢出
		//1、同号   2.异号
		if (_pdata[0] == bigdata._pdata[0])  //同号
		{
			return BigData(_value - bigdata._value);
		}
		else  //异号
		{
			if ('+' == _pdata[0] && _value < MAX_INT64 + bigdata._value
				|| '-' == _pdata[0] && _value > MIN_INT64 + bigdata._value)
				return BigData(_value - bigdata._value);
		}
	}
	else  //两数中至少一个溢出
	{
		if (_pdata[0] == bigdata._pdata[0])  //同号
		{
			return BigData((char*)Sub(_pdata, bigdata._pdata).c_str());
		}
		else  //异号，使用加法
		{
		
			BigData bd(*this + bigdata); //符号问题
			if (bigdata._pdata[0] == '+')
			{
				bd._pdata[0] = '-';
			}
			return bd;  
		}
	}
	return BigData("");
}
string BigData::Sub(string left, string right)
{
	int Lsize = left.size();
	int Rsize = right.size();
	bool flag = false;
	if (Rsize > Lsize || (Lsize == Rsize && left<right))   //
	{
		swap(left, right);
		swap(Lsize, Rsize);
		flag = true;
	}

	string str;
	str.resize(Lsize);
	str[0] = left[0];
	if (flag)
	{
		str[0] = '-';
	}

	//相减
	int borrow = 0;
	for (int i = 1; i < Lsize; ++i)
	{
		char cRet = left[Lsize - i] - '0' + borrow;
		borrow = 0;
		if (Rsize - i > 0)
		{
			if (left[Lsize - i] >= right[Rsize - i])  //左边大于等于右边时不需借位
			{
				cRet -= right[Rsize - i] - '0';
			}
			else
			{
				borrow = -1;
				cRet += 10;
				cRet -= right[Rsize - i] - '0';
			}
		}
		if (cRet < 0)
		{
			borrow = -1;
			cRet += 10;
		}
		str[Lsize - i] = cRet + '0';
	}
	return str;
}
BigData BigData::operator*(const BigData bigdata)
{
	//1、两个乘数没有溢出 2、两个乘数至少一个溢出
	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow())
	{
		//1、同号 2、异号
		if (_pdata[0] == bigdata._pdata[0])
		{
			if ((_pdata[0] == '+' && MAX_INT64 / _value > bigdata._value) ||
				(_pdata[0] == '-' && MAX_INT64 / _value < bigdata._value))
			{
				return BigData(_value * bigdata._value);
			}
		}
		else
		{
			if ((_pdata[0] == '+' && MIN_INT64 / _value < bigdata._value) ||
				(_pdata[0] == '-' && MIN_INT64 / _value > bigdata._value))
			{
				return BigData(_value * bigdata._value);
			}
		}
	}
	else   //溢出
	{
		return BigData((char*)(Mul(_pdata, bigdata._pdata).c_str()));
	}
}
string BigData::Mul(string left, string right)
{
	int Lsize = left.size();
	int Rsize = right.size();
	if (Lsize < Rsize)
	{
		swap(left, right);
		swap(Lsize, Rsize);
	}
	string str;
	str.resize(Lsize + Rsize  -1,'0');
	str[0] = '+';
	if (left[0] != right[0])
	{
		str[0] = '-';
	}
	char cStep = 0;
	int offset = 0;
	int DataLen = str.size();
	//逐位相乘
	for (int iLindex = 1; iLindex < Lsize; ++iLindex)
	{
		char cLeft = left[Lsize - iLindex]- '0';  //从后向前逐位取值
		char cRet = 0;
		for (int iRindex = 1; iRindex < Rsize; ++iRindex)
		{
			if (cLeft == 0)
			{
				break;
			}
			cRet = cLeft * (right[Rsize - iRindex] - '0');
			cRet += str[DataLen - iRindex - offset] - '0';//每次相加之前都需将上一轮相乘的结果累加起来
			cRet += cStep;
			cStep = 0;
			cStep = cRet / 10;
			str[DataLen-iRindex - offset] = cRet % 10 + '0';
		}
		str[DataLen - Rsize - offset] += cStep;//每次循环结束前的最后一次相乘进位必须累加起来
		cStep = 0;
		++offset;
	}
	return str;
}
BigData BigData::operator/(const BigData bigdata)
{
	//1、减数和被减数均未溢出 2、至少一个溢出
	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow())
	{
		return BigData(_value / bigdata._value);
	}
	else
	{
		return BigData((char*)(Div(_pdata, bigdata._pdata).c_str()));
	}
}
string BigData::Div(string left, string right)//有问题
{
	int Lsize = left.size();
	int Rsize = right.size();
	if (Lsize < Rsize || (Lsize == Rsize && strcmp(left.c_str()+1, right.c_str()+1) < 0)  )//左边小于右边，返回0
	{
		return "0";
	}
	if (right == "+1" || right == "-1")
	{
		left[0] = '+';
		if (right[0] != left[0])
		{
			left[0] = '-';
		}
		return left;
	}
	if (strcmp(left.c_str() + 1, right.c_str() + 1) == 0)  //判断是否相等
	{
		string str("+1");
		if (right[0] != left[0])
		{
			str[0] = '-';
		}
		return str;
	}
	string str;
	str.append(1, '+'); //append 追加一个字符空间并赋初值
	if (left[0] != right[0])
	{
		str[0] = '-';
	}
	//除法
	char* pLeft = (char* )(left.c_str() + 1);
	char* pRight = (char*)(right.c_str() + 1);
	int DataLen = right.size() - 1;

	for (int iLindex = 0; iLindex < Lsize - 1;)
	{
		if (*pLeft == '0')
		{
			str.append(1, '0');
			++pLeft;
			++iLindex;
			continue;
		}
		if (!IsLeftStrBig(pLeft, DataLen, pRight, right.size()-1))	
		{
			if (DataLen + iLindex >= Lsize-1)
			{
				break;
			}
		//	str.append(1, '0'); 除不过时再补0
			++DataLen;
			if (!IsLeftStrBig(pLeft, DataLen, pRight, right.size() - 1))
			{
				str.append(1, '0');
			}
		}
		else
		{
			str.append(1, SubLoop(pLeft, DataLen, pRight, right.size()-1));
			while (*pLeft == '0' && DataLen >0)
			{
				--DataLen;
				++pLeft;
				++iLindex;
			}
		}
	}
	return str;
}
char BigData::SubLoop(char* pleft, int Lsize, char* pright, int Rsize)
{
	assert(pleft != NULL && pright != NULL);
	char cRet = 0;
	while (true)
	{
		if (!IsLeftStrBig(pleft, Lsize, pright, Rsize))
		{
			break;
		}
		//逐位相减
		for (int i = 1; i <= Rsize; ++i)
		{
			char ret = pleft[Lsize - i] - '0';
			ret -= pright[Rsize - i] - '0';
			if (ret < 0)
			{
				pleft[Lsize - i - 1] -= 1;
				ret += 10;
			}
			pleft[Lsize - i] = ret + '0';
		}
		while (*pleft == '0' && Lsize > 0)
		{
			++pleft;
			--Lsize;
		}
		cRet++;
	}
	return cRet + '0';
}
bool BigData::IsLeftStrBig(const char* left, int Lsize, const char* right, int Rsize)
{
	if (Lsize>Rsize || (Lsize == Rsize &&strcmp(left, right) >= 0))
	{
		return true;
	}
	return false;
}