#pragma once
#include"QInt.h"

class QFloat {
	int data[4];
public:
	QFloat();
//nhap chuoi float theo he 2 hoac 10 (type)
	friend istream& ScanQFloat(istream&, QFloat&, int type);
	friend ostream& PrintQFloat(ostream&, QFloat&, int type);
};
string mult2(string);
string mult5(int);//tinh toan phan thap phan theo chuoi
string addSignificant(string, string);//cong phan thap phan 5 + 25 -> 75