#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day13.h"
#include <cstdio>
#include<vector>
#include<string>
#include <fstream>
#include <chrono>
#include "usefulFuncs.h"
#include<regex>;
#include <cmath>;
#pragma once


struct chronoComp {
	long long registerA;
	long long registerB;
	long long registerC;
	std::vector<int> program;
	int pointer{ 0 };
	int programSize;
	std::string finalOutput{ "" };
	long long operand{ 0 };
	int opCode{ 0 };
	std::vector<int> outputstring;
	int l{ 0 };
	long long bestSol = -1;
	bool foundSol = false;
	
	chronoComp(std::string fileURL) {
		std::string fileString = fileToString(fileURL);
		std::regex regAPattern(R"(Register\sA:\s(-?\d+))");
		std::regex regBPattern(R"(Register\sB:\s(-?\d+))");
		std::regex regCPattern(R"(Register\sC:\s(-?\d+))");
		std::smatch match;
		std::regex_search(fileString.cbegin(), fileString.cend(), match, regAPattern);
		registerA = std::stoi(match[1]);
		std::regex_search(fileString.cbegin(), fileString.cend(), match, regBPattern);
		registerB = std::stoi(match[1]);
		std::regex_search(fileString.cbegin(), fileString.cend(), match, regCPattern);
		registerC = std::stoi(match[1]);
		auto start = match.suffix().first;
		std::regex programPattern(R"((\d+))");
		program;
		while (std::regex_search(start, fileString.cend(), match, programPattern)) {
			program.push_back(std::stoi(match[0]));
			start = match.suffix().first;
		}
		

		for (int i : program) {
			outputstring.push_back(i);
		}
		l = outputstring.size();
		programSize = program.size();

	}
	long long combo(int i) {
		switch (i) {
		case 0:
			return 0;
		case 1:
			return 1;
		case 2:
			return 2;
		case 3:
			return 3;
		case 4:
			return registerA;
		case 5:
			return registerB;
		case 6:
			return registerC;
		case 7: 
			std::cout << "ERROR COMBO";
		}
	}
	int runLineOnce(long long regA) {
		registerA = regA;
		registerB = 0;
		registerC = 0;
		pointer = 0;
		finalOutput.clear();
		while (pointer < programSize) {
			opCode = program[pointer];
			operand = program[pointer + 1];
			switch (opCode) {
			case 0: {
				adv();
				break;
			}
			case 1: {
				bxl();
				break;
			}
			case 2: {
				bst();
				break;
			}
			case 4: {
				bxc();
				break;
			}
			case 5: {
				out();
				break;
			}
			case 6: {
				bdv();
				break;
			}
			case 7: {
				cdv();
				break;
			}

			}

			pointer += 2;
		}
		return finalOutput[0] - 48;
	}

	void startProgram() {
		std::cout << registerA << " " << registerB << " " << registerC << "\n";

		while (pointer < programSize) {
			opCode = program[pointer];
			operand = program[pointer + 1];
			switch (opCode) {
			case 0: {
				adv();
				break;
			}
			case 1: {
				bxl();
				break;
			}
			case 2: {
				bst();
				break;
			}
			case 3: {
				jnz();
				break;
			}
			case 4: {
				bxc();
				break;
			}
			case 5: {
				out();
				break;
			}
			case 6: {
				bdv();
				break;
			}
			case 7: {
				cdv();
				break;
			}

			}
			std::cout << registerA << " " << registerB << " " << registerC << "\n";

			pointer += 2;
		}
		std::cout << finalOutput;

	}

	void adv() {
		long long num = registerA;
		long long denom = std::pow(2, combo(operand));
		registerA = num / denom;

		return;
	}

	void bxl() {
		registerB ^= operand;
	}

	void bst() {
		registerB = combo(operand) % 8;
	}
	
	void jnz() {
		if (registerA != 0) {
			pointer = operand - 2;
		}
	}

	void bxc() {
		registerB ^= registerC;
	}

	void out() {
		finalOutput += intToString(combo(operand) % 8) + ",";
	}

	void bdv() {
		long long num = registerA;
		long long denom = std::pow(2, combo(operand));
		registerB = num / denom;

		return;
	}
	
	void cdv() {
		long long num = registerA;
		long long denom = std::pow(2, combo(operand));
		registerC = num / denom;

		return;
	}

	void findOrigin(int count = 0, long long curr = 0) {
		if (count == l) {
			if (curr < bestSol or bestSol == -1) {
				bestSol = curr;
				foundSol = true;							//because it starts searching the lowest possible solution we can just be greedy and end the loop as soon as we find a solution (eg if we had j = 8, j-- then we could not do this and the last sol would be the best one)
				return;
			}
		}
		else if (!foundSol) {
			long long nextLowerBound = curr * 8;
			int expected = outputstring[l - count - 1];
			for (int j = 0; j < 8; j++) {
				int output = runLineOnce(nextLowerBound + j);
				if (output == expected) {
					findOrigin(count + 1, nextLowerBound + j);
				}
			}
		}
	}

	
};


void chronspatialComputer(std::string fileURL) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	chronoComp computer(fileURL);

	computer.findOrigin();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;  

	std::cout << computer.bestSol;
		
	

	
	


}