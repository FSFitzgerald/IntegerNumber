#include"QInt.h"

QInt::QInt() {
	for (int i = 0; i < 4; i++)
	{
		data[i] = 0;
	}
}

istream& ScanQInt(istream& inDev, QInt& x, int type) {
	string str;
	inDev >> str;
	if (type == 2) {
		for (int i = str.size() - 1; i >= 0; i--) {
			int temp = str.size() - 1 - i;
			x.data[3 - temp / 32] = (str[i] == '1') ?
				setBit1(x.data[3 - temp / 32], temp % 32) : 
				setBit0(x.data[3 - temp / 32], temp % 32);
		}
	}
	if (type == 10) {
		int indexBit = 0;
		bool negative = false;//check if mumber is negative
		if (str.front() == '-') {
			negative = true;
			str.erase(0, 1);
		}
		while (str != "") {
			int temp = str.back() - '0';
			if (temp % 2) {
				x.data[3 - indexBit / 32] = setBit1(x.data[3 - indexBit / 32], indexBit % 32);
				str.back()--;
			}
			else {
				x.data[3 - indexBit / 32] = setBit0(x.data[3 - indexBit / 32], indexBit % 32);
			}		
			str = dev2(str);
			indexBit++;
		}
		if (negative)
			x = twoComplement(x);
	}
	if (type == 16) {
		unordered_map<char, string> HexBin;
		setHexToBin(HexBin);
		int indexBit = 0;
		for (int i = str.length() - 1; i >= 0; i--) {
			for (int j = 3; j >= 0; j--) {
				x.data[3 - indexBit / 32] = HexBin[str[i]][j] == '1' ?
					setBit1(x.data[3 - indexBit / 32], indexBit % 32) :
					setBit0(x.data[3 - indexBit / 32], indexBit % 32);
				indexBit++;
			}
		}
	}
	return inDev;
}

ostream& PrintQInt(ostream& outDev, QInt& x, int type) {
	string result;
	if (type == 10) {
		result = "0";
		for (int i = 0; i < 128; i++) {
			if (getBit(x.data[3 - i / 32], i % 32) && i == 127)
				result = subtTwoString(pow2(i), result);
			if (getBit(x.data[3 - i / 32], i % 32) && i != 127)
				result = addTwoString(result, pow2(i));
		}
	}
	if (type == 2) {
		result.resize(128);
		for (int i = 0; i < 128; i++) {
			int temp = 127 - i;
			result[i] = (char)(getBit(x.data[3 - temp / 32], temp % 32) + '0');
		}
		while (result[0] == '0') {
			result.erase(0, 1);
		}
	}
	if (type == 16) {
		result.resize(32);
		unordered_map<string, char> BinHex;
		setBinToHex(BinHex);
		int indexBit = 127;
		for (int i = 0; i < 32; i++) {
			string str;
			for (int j = 0; j < 4; j++) {
				str.push_back((char)(getBit(x.data[3 - indexBit / 32], indexBit % 32) + '0'));
				indexBit--;
			}
			result[i] = BinHex[str];
		}
		while (result[0] == '0') {
			result.erase(0, 1);
		}
	}
	outDev << result;
	return outDev;
}

QInt& operator+(QInt x, QInt y) {
	QInt result;
	bool remember = false;
	for (int i = 0; i < 128; i++) {
		int temp = getBit(x.data[3 - i / 32], i % 32) + getBit(y.data[3 - i / 32], i % 32);
		if (remember)
			temp++;
		if (temp == 2) {
			temp = 0;
			remember = true;
		}
		else if (temp == 3) {
			temp = 1;
			remember = true;
		}
		else
			remember = false;
		result.data[3 - i / 32] = temp == 1 ? setBit1(result.data[3 - i / 32], i % 32) :
			setBit0(result.data[3 - i / 32], i % 32);
	}
	return result;
}

QInt& twoComplement(QInt x) {
	QInt result;
	for (int i = 0; i < 128; i++) {
		result.data[3 - i/32] = getBit(x.data[3 - i / 32], i % 32) == 1 ?
			setBit0(result.data[3 - i / 32], i % 32) :
			setBit1(result.data[3 - i / 32], i % 32);
	}
	bool remember = false;
	for (int i = 0; i < 128; i++) {
		int temp;
		if (i == 0)
			temp = 1 + getBit(result.data[3 - i / 32], i % 32);
		else
			temp = getBit(result.data[3 - i / 32], i % 32);
		if (remember)
			temp++;
		if (temp == 2) {
			temp = 0;
			remember = true;
		}
		else
			remember = false;
		result.data[3 - i / 32] = temp == 1 ?
			setBit1(result.data[3 - i / 32], i % 32) :
			setBit0(result.data[3 - i / 32], i % 32);
	}
	return result;
}

QInt& operator-(QInt x, QInt y) {
	return x + twoComplement(y);
}

QInt& operator*(QInt x, QInt y) {
	QInt result;
	for (int i = 0; i < 128; i++) {
		if (getBit(y.data[3 - i / 32], i % 32) == 1) {
			QInt temp = x << i;
			result = result + temp;
		}
	}
	return result;
}

QInt& operator/(QInt Q, QInt M) {
	QInt A;//128 bit 0
	QInt zero;//so 0
	bool sign;
	if (getBit(Q.data[3 - 127 / 32], 127 % 32) != getBit(M.data[3 - 127 / 32], 127 % 32))
		sign = true;//negative
	else
		sign = false;
	if (Q < zero)
		Q = twoComplement(Q);
	if (M < zero)
		M = twoComplement(M);
	int k = 128;
	while (k > 0) {
		A = A << 1;
		if (getBit(Q.data[3 - 127 / 32], 127 % 32) == 1)
			A.data[3 - 0 / 32] = setBit1(A.data[3 - 0 / 32], 0 % 32);
		Q = Q << 1;
		A = A - M;
		if (A < zero) {
			Q.data[3 - 0 / 32] = setBit0(Q.data[3 - 0 / 32], 0 % 32);
			A = A + M;
		}else
			Q.data[3 - 0 / 32] = setBit1(Q.data[3 - 0 / 32], 0 % 32);
		k--;
	}
	if (sign)
		Q = twoComplement(Q);
	return Q;
}

QInt& operator<<(QInt x, int n) {
	QInt result;
	for (int i = 127; i >= n; i--) {
		int temp = i - n;
		result.data[3 - i / 32] = getBit(x.data[3 - temp / 32], temp % 32) == 1 ?
			setBit1(result.data[3 - i / 32], i % 32) :
			setBit0(result.data[3 - i / 32], i % 32);
	}
	return result;
}

QInt& operator>>(QInt x, int n) {
	QInt result;
	for (int i = 0; i <= 127 - n; i++) {
		int temp = i + n;
		result.data[3 - i / 32] = getBit(x.data[3 - temp / 32], temp % 32) == 1 ?
			setBit1(result.data[3 - i / 32], i % 32) :
			setBit0(result.data[3 - i / 32], i % 32);
	}
	return result;
}

QInt& rol(QInt x, int n) {
	QInt result;
	for (int i = 0; i < n; i++) {
		int temp1 = 127 - i; //x index
		int temp2 = n - 1 - i;//result index
		result.data[3 - temp2 / 32] = getBit(x.data[3 - temp1 / 32], temp1 % 32) == 1 ?
			setBit1(result.data[3 - temp2 / 32], temp2 % 32) :
			setBit0(result.data[3 - temp2 / 32], temp2 % 32);
	}
	for (int i = 127; i >= n; i--) {
		int temp = i - n;
		result.data[3 - i / 32] = getBit(x.data[3 - temp / 32], temp % 32) == 1 ?
			setBit1(result.data[3 - i / 32], i % 32) :
			setBit0(result.data[3 - i / 32], i % 32);
	}
	return result;
}

QInt& ror(QInt x, int n) {
	QInt result;
	for (int i = 0; i < n; i++) {
		int temp = 127 - (n - 1 - i);
		result.data[3 - temp / 32] = getBit(x.data[3 - i / 32], i % 32) == 1 ?
			setBit1(result.data[3 - temp / 32], temp % 32) :
			setBit0(result.data[3 - temp / 32], temp % 32);
	}
	for (int i = 0; i <= 127 - n; i++) {
		int temp = i + n;
		result.data[3 - i / 32] = getBit(x.data[3 - temp / 32], temp % 32) == 1 ?
			setBit1(result.data[3 - i / 32], i % 32) :
			setBit0(result.data[3 - i / 32], i % 32);
	}
	return result;
}

QInt& operator&(QInt x, QInt y) {
	QInt result;
	for (int i = 0; i < 128; i++) {
		int temp = getBit(x.data[3 - i / 32], i % 32) & getBit(y.data[3 - i / 32], i % 32);
		result.data[3 - i / 32] = temp == 1 ? setBit1(result.data[3 - i / 32], i % 32) :
			setBit0(result.data[3 - i / 32], i % 32);
	}
	return result;
}

QInt& operator|(QInt x, QInt y) {
	QInt result;
	for (int i = 0; i < 128; i++) {
		int temp = getBit(x.data[3 - i / 32], i % 32) | getBit(y.data[3 - i / 32], i % 32);
		result.data[3 - i / 32] = temp == 1 ? setBit1(result.data[3 - i / 32], i % 32) :
			setBit0(result.data[3 - i / 32], i % 32);
	}
	return result;
}

QInt& operator^(QInt x, QInt y) {
	QInt result;
	for (int i = 0; i < 128; i++) {
		int temp = getBit(x.data[3 - i / 32], i % 32) ^ getBit(y.data[3 - i / 32], i % 32);
		result.data[3 - i / 32] = temp == 1 ? setBit1(result.data[3 - i / 32], i % 32) :
			setBit0(result.data[3 - i / 32], i % 32);
	}
	return result;
}

QInt& operator~(QInt x) {
	QInt result;
	for (int i = 0; i < 128; i++) {
		int temp = getBit(x.data[3 - i / 32], i % 32);
		result.data[3 - i / 32] = temp == 1 ? setBit0(result.data[3 - i / 32], i % 32) :
			setBit1(result.data[3 - i / 32], i % 32);
	}
	return result;
}

bool operator<(QInt x, QInt y) {
	int x_sign = getBit(x.data[3 - 127 / 32], 127 % 32);
	int y_sign = getBit(y.data[3 - 127 / 32], 127 % 32);
	if (x_sign > y_sign)//1: negative 0:positive
		return true;
	else if (x_sign < y_sign)
		return false;
	else {
		for (int i = 126; i >= 0; i--) {
			int x_bit = getBit(x.data[3 - i / 32], i % 32);
			int y_bit = getBit(y.data[3 - i / 32], i % 32);
			if (x_bit < y_bit)
				return true;
			else if (x_bit > y_bit)
				return false;
		}
	}
	return false;
}

bool operator>(QInt x, QInt y) {
	int x_sign = getBit(x.data[3 - 127 / 32], 127 % 32);
	int y_sign = getBit(y.data[3 - 127 / 32], 127 % 32);
	if (x_sign < y_sign)//1: negative 0:positive
		return true;
	else if (x_sign > y_sign)
		return false;
	else {
		for (int i = 126; i >= 0; i--) {
			int x_bit = getBit(x.data[3 - i / 32], i % 32);
			int y_bit = getBit(y.data[3 - i / 32], i % 32);
			if (x_bit > y_bit)
				return true;
			else if (x_bit < y_bit)
				return false;
		}
	}
	return false;
}

bool operator<=(QInt x, QInt y) {
	return (x < y) || (x == y);
}

bool operator>=(QInt x, QInt y) {
	return (x > y) || (x == y);
}

bool operator==(QInt x, QInt y) {
	for (int i = 0; i < 128; i++)
		if (getBit(x.data[3 - i / 32], i % 32) != getBit(y.data[3 - i / 32], i % 32))
			return false;
	return true;
}

int getBit(int x, int i) { 
	return (x >> i) & 1;
}

int setBit1(int x, int i) { // 31 30 ... 1 0 
	return x | (1 << (i));
}

int setBit0(int x, int i) {
	return ~(1 << i) & x;
}

void setHexToBin(unordered_map<char, string>& map) {
	map['0'] = "0000";
	map['1'] = "0001";
	map['2'] = "0010";
	map['3'] = "0011";
	map['4'] = "0100";
	map['5'] = "0101";
	map['6'] = "0110";
	map['7'] = "0111";
	map['8'] = "1000";
	map['9'] = "1001";
	map['A'] = "1010";
	map['B'] = "1011";
	map['C'] = "1100";
	map['D'] = "1101";
	map['E'] = "1110";
	map['F'] = "1111";
}

void setBinToHex(unordered_map<string, char>& map) {
	map["0000"] = '0';
	map["0001"] = '1';
	map["0010"] = '2';
	map["0011"] = '3';
	map["0100"] = '4';
	map["0101"] = '5';
	map["0110"] = '6';
	map["0111"] = '7';
	map["1000"] = '8';
	map["1001"] = '9';
	map["1010"] = 'A';
	map["1011"] = 'B';
	map["1100"] = 'C';
	map["1101"] = 'D';
	map["1110"] = 'E';
	map["1111"] = 'F';
}

string addTwoString(string a, string b) {
	if (a.length() < b.length()) {
		string temp = a;
		a = b;
		b = temp;
	}
	bool remember = false;
	int a_len = a.length();
	int b_len = b.length();
	string result;
	result.resize(a_len);
	for (int i = 0; i < b_len; i++) {
		int temp = a[a_len - 1 - i] - '0' + b[b_len - 1 - i] - '0';
		if (remember)
			temp++;
		if (temp > 9)
			remember = true;
		else
			remember = false;
		temp %= 10;
		result[a_len - 1 - i] = (char)(temp + '0');
 	}
	for (int i = 0; i < a_len - b_len; i++) {
		int temp = a[a_len - b_len - 1 - i] - '0';
		if (remember)
			temp++;
		if (temp > 9)
			remember = true;
		else
			remember = false;
		temp %= 10;
		result[a_len - b_len - 1 - i] = (char)(temp + '0');//
	}
	if (remember)
	{
		result.resize(a_len + 1);
		for (int i = result.length() - 1; i >= 1; i--)
			result[i] = result[i - 1];
		result[0] = '1';
	}
	return result;
}

string subtTwoString(string a, string b) {
	string result;
	int a_len = a.length();
	int b_len = b.length();
	bool remember = false;
	result.resize(a_len);
	for (int i = 0; i < b_len; i++) {
		int temp = a[a_len - 1 - i] - b[b_len - 1 - i];
		if (remember)
			temp--;
		if (temp < 0) {
			temp += 10;
			remember = true;
		}
		else
			remember = false;
		result[a_len - 1 - i] = (char)(temp + '0');
	}
	for (int i = 0; i < a_len - b_len; i++) {
		int temp = a[a_len - b_len - 1 - i] - '0';
		if (remember)
			temp--;
		if (temp < 0) {
			temp += 10;
			remember = true;
		}
		else
			remember = false;
		result[a_len - b_len - 1 - i] = (char)(temp + '0');
	}
	while (result[0] == '0') {
		result.erase(0, 1);
	}
	result.resize(result.size() + 1);
	for (int i = result.size() - 1; i >= 1; i--)
		result[i] = result[i - 1];
	result[0] = '-';
	return result;
}

string dev2(string a) {
	string result;
	int a_len = a.length();
	result.resize(a_len);
	bool remember = false;
	for (int i = 0; i < a_len; i++) {
		int temp = a[i] - '0';
		if (remember)
			temp += 10;
		if (temp % 2)
			remember = true;
		else
			remember = false;
		result[i] = (char)(temp / 2 + '0');
	}
	while (result[0] == '0') {
		result.erase(0, 1);
	}
	return result;
}

string pow2(int a) {
	string result = "1";
	for (int i = 1; i <= a; i++) {
		result = addTwoString(result, result);
	}
	return result;
}