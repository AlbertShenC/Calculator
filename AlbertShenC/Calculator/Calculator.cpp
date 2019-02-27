// Calculator.cpp: 定义控制台应用程序的入口点。
//



#include "stdafx.h"
#include <stack>
#include <vector>
#include <iostream>
#include "stdlib.h"
#include <ctime>
#include <string>
#include <fstream>
#include "Calculator.h"

#define random(a,b) (rand()%(b-a+1)+a)

using namespace std;

Calculator::Calculator() {}

string Calculator::MakeFormula() {
	string formula = "";
	// 运算符数量范围错误
	int count = random(2, 3);
	int start = 0;
	// 数字取值范围错误
	int number1 = random(0, 100);
	int result = number1;
	formula += to_string(number1);
	while (start <= count) {
		int operation = random(0, 3);
		int number2 = random(1, 100);
		// 计算过程中不能出现小数，也就是必须整除
		if(operation == 2){ // 乘法
			result *= number2;
		} else if(operation == 3){ // 除法
			while(number2 == 0 || result % number2 != 0){
				number2 = random(0, 100);
			}
			result /= number2;
		} else{
			result = number2;
		}
		formula += op[operation] + to_string(number2);
		start++;
	}
	return formula;
};

string Calculator::Solve(string formula) {
	vector<string>* tempStack = new vector<string>(); // 储存后缀表达式
	stack<char>* operatorStack = new stack<char>(); // 操作符栈
	int len = formula.length();
	int k = 0;
	for (int j = 0; j < len; j++) {
		char formulaChar = formula[j];
		if (j == len - 1 || formulaChar == '+' || formulaChar == '-' ||
			formulaChar == '*' || formulaChar == '/') {
			if (j == len - 1) {
				tempStack->push_back(formula.substr(k));
			}
			else {
				if (k < j) {
					tempStack->push_back(formula.substr(k, j - k));
				}
				if (operatorStack->empty()) {
					operatorStack->push(formulaChar);
				}
				else {
					char stackChar = operatorStack->top();
					if ((stackChar == '+' || stackChar == '-')
						&& (formulaChar == '*' || formulaChar == '/')) {
						operatorStack->push(formulaChar);
					}
					else {
						// 这个错误有点坑，to_string函数是将数字转化为string的函数
						tempStack->push_back(string(1, operatorStack->top()));
						operatorStack->pop();
						operatorStack->push(formulaChar);
					}
				}
			}
			k = j + 1;
		}
	}
	while (!operatorStack->empty()) {
		tempStack->push_back(string(1, operatorStack->top()));
		operatorStack->pop();
	}
	stack<string>* calcStack = new stack<string>();
	for (int i = 0; i < tempStack->size(); i++) {
		string peekChar = tempStack->at(i);
		if (peekChar != "+" && peekChar != "-"
			&& peekChar != "/" && peekChar != "*") {
			calcStack->push(peekChar);
		}
		else {
			int a1 = 0;
			int b1 = 0;
			if (!calcStack->empty()) {
				b1 = stoi(calcStack->top());
				calcStack->pop();
			}
			if (!calcStack->empty()) {
				a1 = stoi(calcStack->top());
				calcStack->pop();
			}
			if (peekChar == "+") {
				calcStack->push(to_string(a1 + b1));
			}
			else if (peekChar == "-") {
				calcStack->push(to_string(a1 - b1));
			}
			else if (peekChar == "*") {
				calcStack->push(to_string(a1 * b1));
			}
			else if (peekChar == "/") {
				calcStack->push(to_string(a1 / b1));
			}
		}
	}
	return formula + "=" + calcStack->top();
}

int main()
{
	srand((unsigned int)time(NULL));
	Calculator* calc = new Calculator();
	ofstream SubjectFile("subject.txt");

	int n;
	cin >> n;
	
	for (int i = 0; i < n; i++) {
		string question = calc->MakeFormula();
		string ret = calc->Solve(question);
		cout << ret << endl;
		SubjectFile << ret << endl;
	}
	SubjectFile.close();

	getchar();
}



