#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<unordered_map>
using namespace std;

class QInt {
	int data[4];
public:
//tao lap QInt = 0
	QInt(); // 128 bit gan bang 0
//nhap QInt. chuoi nhap vao theo he 2, 10 hoac 16 (type)
//--neu he 2 thi luu vao data
//--neu he 10 thi chuyen thanh he 2 roi luu vao data
//--neu he 16 thi chuyen thanh he 2 roi luu vao data
friend	istream& ScanQInt(istream& inDev, QInt& x, int type);
//xuat QInt theo he 2, 10, 16(bien type)
friend	ostream& PrintQInt(ostream& outDev, QInt& x, int type);
//phep toan cong
friend	QInt& operator+(QInt x, QInt y);
//lay so bu 2 cua x
friend  QInt& twoComplement(QInt x);
//phep toan tru x - y
friend  QInt& operator-(QInt x, QInt y);
//phep toan x * y
friend  QInt& operator*(QInt x, QInt y);
//phep toan x/y
friend QInt& operator/(QInt x, QInt y);
//phep toan dich trai
friend QInt& operator<<(QInt x, int n);
//phep toan dich phai
friend QInt& operator>>(QInt x, int n);
//phep xoay trai
friend QInt& rol(QInt x, int n);
//phep xoay ohai
friend QInt& ror(QInt x, int n);
//phep AND
friend QInt& operator&(QInt x, QInt y);
//phep OR
friend QInt& operator|(QInt x, QInt y);
//phep xor
friend QInt& operator^(QInt x, QInt y);
//phep not
friend QInt& operator~(QInt x);
//so sanh <
friend bool operator<(QInt x, QInt y);
//so sanh >
friend bool operator>(QInt x, QInt y);
//so sanh <=
friend bool operator<=(QInt x, QInt y);
//so sanh >=
friend bool operator>=(QInt x, QInt y);
//so sanh ==
friend bool operator==(QInt x, QInt y);
};
int getBit(int x, int i);// lay bit thu i cua x
int setBit1(int x, int i); // gan bit thu i cua x bang 1
int setBit0(int x, int n); // gan bit thu i cua x bang 0
void setHexToBin(unordered_map<char, string>& map);//bang chuyen doi hex to bin
void setBinToHex(unordered_map<string, char>& map);//bang chuyen doi bin to hex
string addTwoString(string a, string b); //string a+b 
string subtTwoString(string a, string b);//string b - a (a > b) result negative
string dev2(string a);// so chan chia cho 2
string pow2(int a); //string 2^a