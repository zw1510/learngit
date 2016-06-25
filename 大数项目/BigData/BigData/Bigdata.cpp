#define _CRT_SECURE_NO_WARNINGS 1

#include"Bigdata.h"


ostream& operator<<(ostream& os, const BigData& bigdata)
{
	//������������_value,���_pdata
	//���ж�_value�Ƿ񱣴��ֵ��Ч
	if (!bigdata.IsINT64Overflow())   //_value���������û�������ֱ�����_value
	{
		os << bigdata._value << endl;
	}
	else    
	{
		char* str = (char*)bigdata._pdata.c_str();
		if ('+' == bigdata._pdata[0])   //������λΪ'+'ֱ�������������
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
	//���������1��������δ���2����������һ�����
	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow())
	{
		//�������������������δ���
		//���������1��ͬ�� 2�����
		if (_pdata[0] != bigdata._pdata[0])  //���
		{
			return BigData(_value + bigdata._value);
		}
		else   //ͬ��
		{
			//���������1��ͬΪ�� 2��ͬΪ��
			if ('+' == _pdata[0])
			{
				if (MAX_INT64 - _value > bigdata._value)  //���δ���
				{
					return BigData(_value + bigdata._value);
				}

			}
			else
			{
				if (MIN_INT64 - _value < bigdata._value)  //���δ���
				{
					return BigData(_value + bigdata._value);
				}
			}
		}
	}
	else   		//����һ����������
	{
		return BigData((char*)Add(_pdata, bigdata._pdata).c_str());//ͬ����ž�ȡ����Ϊ����λ
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
	//���ʱ����ѭ���Ĵ������������нϳ����������ٿռ�Ҳ�ǿ��ٽϳ�����size+1
	int Lsize = left.size();
	int Rsize = right.size();

	if (Lsize < Rsize)
	{
		swap(left, right);
		swap(Lsize, Rsize);
	}
	string str;
	str.resize(Lsize + 1);

	//���
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
	//���������1��������δ���2������������һ�����

	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow())  
	{
		//���������1���������������2������������δ���
		//1��ͬ��   2.���
		if (_pdata[0] == bigdata._pdata[0])  //ͬ��
		{
			return BigData(_value - bigdata._value);
		}
		else  //���
		{
			if ('+' == _pdata[0] && _value < MAX_INT64 + bigdata._value
				|| '-' == _pdata[0] && _value > MIN_INT64 + bigdata._value)
				return BigData(_value - bigdata._value);
		}
	}
	else  //����������һ�����
	{
		if (_pdata[0] == bigdata._pdata[0])  //ͬ��
		{
			return BigData((char*)Sub(_pdata, bigdata._pdata).c_str());
		}
		else  //��ţ�ʹ�üӷ�
		{
		
			BigData bd(*this + bigdata); //��������
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

	//���
	int borrow = 0;
	for (int i = 1; i < Lsize; ++i)
	{
		char cRet = left[Lsize - i] - '0' + borrow;
		borrow = 0;
		if (Rsize - i > 0)
		{
			if (left[Lsize - i] >= right[Rsize - i])  //��ߴ��ڵ����ұ�ʱ�����λ
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
	//1����������û����� 2��������������һ�����
	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow())
	{
		//1��ͬ�� 2�����
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
	else   //���
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
	//��λ���
	for (int iLindex = 1; iLindex < Lsize; ++iLindex)
	{
		char cLeft = left[Lsize - iLindex]- '0';  //�Ӻ���ǰ��λȡֵ
		char cRet = 0;
		for (int iRindex = 1; iRindex < Rsize; ++iRindex)
		{
			if (cLeft == 0)
			{
				break;
			}
			cRet = cLeft * (right[Rsize - iRindex] - '0');
			cRet += str[DataLen - iRindex - offset] - '0';//ÿ�����֮ǰ���轫��һ����˵Ľ���ۼ�����
			cRet += cStep;
			cStep = 0;
			cStep = cRet / 10;
			str[DataLen-iRindex - offset] = cRet % 10 + '0';
		}
		str[DataLen - Rsize - offset] += cStep;//ÿ��ѭ������ǰ�����һ����˽�λ�����ۼ�����
		cStep = 0;
		++offset;
	}
	return str;
}
BigData BigData::operator/(const BigData bigdata)
{
	//1�������ͱ�������δ��� 2������һ�����
	if (!IsINT64Overflow() && !bigdata.IsINT64Overflow())
	{
		return BigData(_value / bigdata._value);
	}
	else
	{
		return BigData((char*)(Div(_pdata, bigdata._pdata).c_str()));
	}
}
string BigData::Div(string left, string right)//������
{
	int Lsize = left.size();
	int Rsize = right.size();
	if (Lsize < Rsize || (Lsize == Rsize && strcmp(left.c_str()+1, right.c_str()+1) < 0)  )//���С���ұߣ�����0
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
	if (strcmp(left.c_str() + 1, right.c_str() + 1) == 0)  //�ж��Ƿ����
	{
		string str("+1");
		if (right[0] != left[0])
		{
			str[0] = '-';
		}
		return str;
	}
	string str;
	str.append(1, '+'); //append ׷��һ���ַ��ռ䲢����ֵ
	if (left[0] != right[0])
	{
		str[0] = '-';
	}
	//����
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
		//	str.append(1, '0'); ������ʱ�ٲ�0
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
		//��λ���
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