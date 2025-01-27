#define _CRT_SECURE_NO_DEPRECATE
#include "day3.h"
#include <iostream>
#include <cstdio>
#include<vector>
#include<string>
#include<cmath>
#include <stack>
#include<algorithm>
#pragma once

void makeTemplate(std::string str, std::stack<char>& stck) {
	std::reverse(str.begin(), str.end());
	for (char c : str) {
		stck.push(c);
	}
}

void printStack(std::stack<char> stck) {
	while (!stck.empty()) {
		char c = stck.top();
		stck.pop();
		std::cout << c;
	}
}

int mullItOver(std::string fileUrl) {
	FILE* dataFile = std::fopen(fileUrl.c_str(), "r+");

	bool reachedEnd{ false };
	bool lookingForInt{ false };
	std::stack<char> mulTemplate{};
	makeTemplate("mul(,)", mulTemplate);
	std::stack<char> curr = mulTemplate;
	std::string numString;
	int first{ 0 };
	int total{ 0 };


	while (not reachedEnd) {
		int num = std::fgetc(dataFile);
		std::cout << "\n" << static_cast<char>(num) << " ";
		printStack(curr);
		
		if (num == EOF) {
			reachedEnd = true;
			std::cout << "\n" << "total is " << total;
			std::fclose(dataFile);
			return 0;
		}
		else if (num == curr.top() and !lookingForInt) {
			if (curr.top() == '(' ) {
				lookingForInt = true;
			}
			curr.pop();
		}
		else if (lookingForInt) {
			if (num >= 48 and num <= 57) {
				numString += static_cast<char>(num);
				std::cout << " " << numString;
			} 
			else if (num == ',' and curr.top() == ',') {
				first = std::stoi(numString);
				numString.clear();
				curr.pop();
			}
			else if (num == ')' and curr.top() == ')') {
				int second = std::stoi(numString);
				total += (first * second);
				numString.clear();
				first = 0;
				lookingForInt = false;
				curr = mulTemplate;
			}
			else {
				numString.clear();
				first = 0;
				lookingForInt = false;
				curr = mulTemplate;
			}


		}
		else {
			curr = mulTemplate;
		}

		
	}

	//std::fclose(dataFile);
	//std::cout << "\n" << "total is " << total;
	return 0;
}

int mullItOverPartTwo(std::string fileUrl) {
	FILE* dataFile = std::fopen(fileUrl.c_str(), "r+");

	bool reachedEnd{ false };
	bool lookingForInt{ false };
	bool enabled{ true };


	std::stack<char> mulTemplate{};
	std::stack<char> doTemplate{};
	std::stack<char> dontTemplate{};
	makeTemplate("do()", doTemplate);
	makeTemplate("don't()", dontTemplate);
	makeTemplate("mul(,)", mulTemplate);
	std::stack<char> curr = mulTemplate;
	std::stack<char> currDo = doTemplate;
	std::stack<char> currDont = dontTemplate;
	std::string numString;
	int first{ 0 };
	int total{ 0 };


	while (not reachedEnd) {
		int num = std::fgetc(dataFile);
		//std::cout << "\n" << static_cast<char>(num) << " ";
		
		//printStack(curr);
		

		if (num == EOF) {
			std::fclose(dataFile);
			std::cout << "\n" << "total is " << total;
			reachedEnd = true;
			return total;
		}
		if (enabled) {
			if (num == currDont.top()) {
				//std::cout << " popped off currDont";
				currDont.pop();
				if (currDont.empty()) {
					enabled = false;
					currDont = dontTemplate;
					curr = mulTemplate;
					first = 0;
					lookingForInt = false;
					numString.clear();
				}
			}
			else {
				currDont = dontTemplate;
			}
		}
		else {
			if (num == currDo.top()) {
				currDo.pop();
				if (currDo.empty()) {
					enabled = true;
					currDo = doTemplate;
					curr = mulTemplate;
					first = 0;
					lookingForInt = false;
					numString.clear();
				}

			}
			else {
				currDo = doTemplate;
			}

		}




		if (enabled) {
			//std::cout << " enabled ";
			
			if (num == curr.top() and !lookingForInt) {
				if (curr.top() == '(') {
					lookingForInt = true;
				}
				curr.pop();
			}
			else if (lookingForInt) {
				if (num >= 48 and num <= 57) {
					numString += static_cast<char>(num);
					//std::cout << " " << numString;
				}
				else if (num == ',' and curr.top() == ',') {
					first = std::stoi(numString);
					numString.clear();
					curr.pop();
				}
				else if (num == ')' and curr.top() == ')') {
					int second = std::stoi(numString);
					total += (first * second);
					numString.clear();
					first = 0;
					lookingForInt = false;
					curr = mulTemplate;
				}
				else {
					numString.clear();
					first = 0;
					lookingForInt = false;
					curr = mulTemplate;
				}


			}
			else {
				curr = mulTemplate;
			}
		}
	}
	return 0;
}