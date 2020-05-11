#include"QInt.h"
#include"QFloat.h"
#include<conio.h>
#include<fstream>
#include<unordered_map>

int main(int argc, char *argv[]) {
	ifstream inFile(argv[1], ifstream::in);
	ofstream outFile(argv[2], ofstream::out);
	for (int i = 0; i < 4; i++) {//chuyen doi he so
		int p1, p2;
		inFile >> p1 >> p2;
		QInt x;
		ScanQInt(inFile, x, p1);
		PrintQInt(outFile, x, p2);
		outFile << endl;
	}
	for (int i = 0; i < 4; i++) {//tinh toan + - * /
		int p1;
		char command;
		QInt x, y, z;
		inFile >> p1;
		ScanQInt(inFile, x, p1);
		inFile >> command;
		ScanQInt(inFile, y, p1);
		switch (command)
		{
		case('+'): {
			z = x + y;
			break;
		}
		case('-'): {
			z = x - y;
			break;
		}
		case('*'): {
			z = x * y;
			break;
		}
		case('/'): {
			z = x / y;
			break;
		}
		default:
			break;
		}
		PrintQInt(outFile, z, p1);
		outFile << endl;
	}
	for (int i = 0; i < 5; i++) {//so sanh
		int p1;
		QInt x, y;
		string command;
		bool result;
		inFile >> p1;
		ScanQInt(inFile, x, p1);
		inFile >> command;
		ScanQInt(inFile, y, p1);
		int compare;
		if (command == "<")
			compare = 1;
		if (command == ">")
			compare = 2;
		if (command == "==")
			compare = 3;
		if (command == "<=")
			compare = 4;
		if (command == ">=")
			compare = 5;
		switch (compare)
		{
		case(1): {
			result = x < y;
			break;
		}
		case(2): {
			result = x > y;
			break;
		}
		case(3): {
			result = x == y;
			break;
		}
		case(4): {
			result = x <= y;
			break;
		}
		case(5): {
			result = x >= y;
			break;
		}
		default:
			break;
		}
		outFile << result;
		outFile << endl;
	}
	for (int i = 0; i < 3; i++) {//phep and or xor
		int p1;
		QInt x, y, z;
		char command;
		inFile >> p1;
		ScanQInt(inFile, x, p1);
		inFile >> command;
		ScanQInt(inFile, y, p1);
		switch (command)
		{
		case('&'): {
			z = x & y;
			break;
		}
		case('|'): {
			z = x | y;
			break;
		}
		case('^'): {
			z = x ^ y;
			break;
		}
		default:
			break;
		}
		PrintQInt(outFile, z, p1);
		outFile << endl;
	}
	//phep not
	int p1;
	inFile >> p1;
	char command;
	inFile >> command;
	QInt x, y;
	ScanQInt(inFile, x, p1);
	y = ~x;
	PrintQInt(outFile, y, p1);
	outFile << endl;
	for (int i = 0; i < 4; i++) {//phep dich, xoay bit
		int p1;
		QInt x, y;
		string command;
		int opt;
		int nums;
		inFile >> p1;
		ScanQInt(inFile, x, p1);
		inFile >> command;
		inFile >> nums;
		if (command == "<<")
			opt = 1;
		if (command == ">>")
			opt = 2;
		if (command == "ror")
			opt = 3;
		if (command == "rol")
			opt = 4;
		switch (opt)
		{
		case(1): {
			y = x << nums;
			break;
		}
		case(2): {
			y = x >> nums;
			break;
		}
		case(3): {
			y = ror(x, nums);
			break;
		}
		case(4): {
			y = rol(x, nums);
			break;
		}
		default:
			break;
		}
		PrintQInt(outFile, y, p1);
		outFile << endl;
	}
	return 1;
}