#include"QFloat.h"

QFloat::QFloat() {
	for (int i = 0; i < 4; i++)
		data[i] = 0;
}

istream& ScanQFloat(istream& inDev, QFloat& x, int type) {
	string str;
	inDev >> str;
	if (type == 10) {
		int data[8];//4 luu bit nguyen, 4 luu phan thap phan
		for (int i = 0; i < 8; i++)
			data[i] = 0;
		if (str[0] == '-') {//set bit dau
			x.data[3 - 127 / 32] = setBit1(x.data[3 - 127 / 32], 127 % 32);
			str.erase(0, 1);
		}
		string int_str, float_str;//lay phan nguyen, phan thap phan ra
		float_str.push_back('0');
		int point_position = str.size();
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == '.')
				point_position = i;
			else if (i < point_position)
				int_str.push_back(str[i]);
			else
				float_str.push_back(str[i]);
		}
		int indexBit = 128; //luu bit phan nguyen vao data[3] -> data[0]
		while (int_str != "") {
			int temp = int_str.back() - '0';
			if (temp % 2) {
				data[7 - indexBit / 32] = setBit1(data[7 - indexBit / 32], indexBit % 32);
				int_str.back()--;
			} else
				data[7 - indexBit / 32] = setBit0(data[7 - indexBit / 32], indexBit % 32);
			int_str = dev2(int_str);
			indexBit++;
		}
		indexBit = 127;//luu bit phan thap phan vao data[4] -> data[7]
		do {
			float_str = mult2(float_str);
			if (float_str[0] == '1') {
				data[7 - indexBit / 32] = setBit1(data[7 - indexBit / 32], indexBit % 32);
				float_str[0] = '0';
			}
			else
				data[7 - indexBit / 32] = setBit0(data[7 - indexBit / 32], indexBit % 32);
			indexBit--;
		} while ((float_str.size() > 1) && (indexBit >= 0));
		int first_bit_1 = 128 - 16383;
		for (int i = 255; i >= 0; i--)
			if (getBit(data[7 - i / 32], i % 32)) {//first bit 1
				first_bit_1 = i;
				i = -1;
			}
		int exponent = first_bit_1 - 128;//set bit exponent
		exponent += (pow(2, 15 - 1) - 1);//16383
		for (int i = 112; i <= 126; i++) {
			int temp = i - 112;
			x.data[3 - i / 32] = getBit(exponent, temp) == 1 ?
				setBit1(x.data[3 - i / 32], i % 32) : setBit0(x.data[3 - i / 32], i % 32);
		}
		int significantBit = first_bit_1 - 1;//first significant bit
		for (int i = 111; i >= 0; i--) {//set bit significant
			if (significantBit >= 0)
				x.data[3 - i / 32] = 
				getBit(data[7 - significantBit / 32], significantBit % 32) == 1 ?
				setBit1(x.data[3 - i / 32], i % 32) : setBit0(x.data[3 - i / 32], i % 32);
			else
				x.data[3 - i / 32] = setBit0(x.data[3 - i / 32], i % 32);
			significantBit--;
		}
	}
	if (type == 2) {
		for (int i = 127; i >= 0; i--) {
			int temp = 127 - i;
			x.data[3 - i / 32] = str[temp] == '1' ?
				setBit1(x.data[3 - i / 32], i % 32) :
				setBit0(x.data[3 - i / 32], i % 32);
		}
	}
	return inDev;
}

ostream& PrintQFloat(ostream& outDev, QFloat& x, int type) {
	string result;
	if (type == 2) {
		result.resize(128);
		for (int i = 127; i >= 0; i--) {
			int temp = 127 - i;
			result[temp] = getBit(x.data[3 - i / 32], i % 32) == 1 ? '1' : '0';
		}
	}
	if (type == 10) {
		if(getBit(x.data[3 - 127 / 32], 127 % 32))//xet dau
			result.push_back('-');
		int exponent = 0;//tinh phan nguyen
		for (int i = 112; i <= 126; i++) {
			int temp = i - 112;
			exponent = getBit(x.data[3 - i / 32], i % 32) == 1 ?
				setBit1(exponent, temp) : setBit0(exponent, temp);
		}
		exponent -= (pow(2, 15 - 1) - 1);
		int firest_bit_1 = 128 + exponent;
		int data[8];
		for (int i = 0; i < 8; i++)
			data[i] = 0;
		data[7 - firest_bit_1 / 32] = setBit1(data[7 - firest_bit_1 / 32], firest_bit_1 % 32);//
		int significant = firest_bit_1 - 1;
		int x_significant = 111;
		while (significant >= 0 && x_significant >= 0) {
			data[7 - significant / 32] = getBit(x.data[3 - x_significant / 32], x_significant % 32) == 1 ?
				setBit1(data[7 - significant / 32], significant % 32) :
				setBit0(data[7 - significant / 32], significant % 32);
			significant--;
			x_significant--;
		}
		string int_str = "0";
		for (int i = 128; i < 255; i++) {
			if (getBit(data[7 - i / 32], i % 32))
				int_str = addTwoString(int_str, pow2(i - 128));
		}
		result += int_str;
		result.push_back('.');
		string float_str = "";
		for (int i = 127; i >= 0; i--) {
			if (getBit(data[7 - i / 32], i % 32)) {
				float_str = addSignificant(float_str, mult5(128 - i));
			}
		}
		result += float_str;
	}
	outDev << result;
	return outDev;
}

string mult2(string a) {
	string result;
	result.resize(a.size());
	bool remember = false;
	for (int i = a.size() - 1; i >= 0; i--) {
		int temp = a[i] - '0';
		temp *= 2;
		if (remember)
			temp++;
		if (temp > 9)
			remember = true;
		else
			remember = false;
		temp %= 10;
		result[i] = (char)(temp + '0');
	}
	while (result.back() == '0')
		result.pop_back();
	return result;
}

string mult5(int n) {
	string result;
	if (n == 1) {
		result = "5";
		return result;
	}
	result = mult5(n - 1);
	string temp = result;
	for (int i = 0; i < 4; i++)
		result = addTwoString(result, temp);
	int fill = n - result.size();
	if(fill > 0)
		result.insert(0, fill, '0');
	return result;
}

string addSignificant(string a, string b) {
	string result;
	int a_len = a.size(), b_len = b.size();
	result.resize(b_len);
	bool remember = false;
	for (int i = b_len - 1; i >= a_len; i--) {
		result[i] = b[i];
	}
	for (int i = a_len - 1; i >= 0; i--) {
		int temp = a[i] - '0' + b[i] - '0';
		if (remember)
			temp++;
		if (temp > 9)
			remember = true;
		else
			remember = false;
		temp %= 10;
		result[i] = (char)(temp + '0');
	}
	return result;
}