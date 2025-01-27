#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day13.h"
#include <cstdio>
#include<vector>
#include<string>
#include<unordered_set>
#include <fstream>
#include <chrono>
#include "usefulFuncs.h"
#include <deque>
#include <unordered_map>
#include<array>
#pragma once


struct clawPair {
	std::pair<int, int> A{ 0,0 }, B{ 0,0 };
	std::pair<long long, long long>  goal{ 0,0 };
};

long long intUntilSpecialChar(FILE* dataFile, char specialChar) {
	int c = std::fgetc(dataFile);
	std::string curr{ "" };
	while (c != specialChar and c != EOF) {
		curr += c;
		c = std::fgetc(dataFile);
	}
	return std::stoll(curr);
}


void clawMachine(std::string fileURL) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	std::vector<clawPair> clawSet{};
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			clawPair newClawPair;
			bool finished{ false };
			while (!finished) {
				c = std::fgetc(dataFile);
				if (c == '+') {
					newClawPair.A.first = intUntilSpecialChar(dataFile, ',');
					std::fgetc(dataFile);   //goes from comma to space
					std::fgetc(dataFile);	// goes from space to Y
					std::fgetc(dataFile);	// goes from y to +
					newClawPair.A.second = intUntilSpecialChar(dataFile, 10);
					finished = true;
				}
			}
			finished = false;
			while (!finished) {
				c = std::fgetc(dataFile);
				if (c == '+') {
					newClawPair.B.first = intUntilSpecialChar(dataFile, ',');
					std::fgetc(dataFile);   //goes from comma to space
					std::fgetc(dataFile);	// goes from space to Y
					std::fgetc(dataFile);	// goes from y to +
					newClawPair.B.second = intUntilSpecialChar(dataFile, 10);
					finished = true;
				}
			}
			finished = false;
			while (!finished) {
				c = std::fgetc(dataFile);
				if (c == '=') {
					newClawPair.goal.first = intUntilSpecialChar(dataFile, ',');
					std::fgetc(dataFile);   //goes from comma to space
					std::fgetc(dataFile);	// goes from space to Y
					std::fgetc(dataFile);	// goes from y to +
					newClawPair.goal.second = intUntilSpecialChar(dataFile, 10);
					finished = true;
				}
			}
			clawSet.push_back(newClawPair);
		}
	}

	long long total{ 0 };

	for (clawPair& claw : clawSet) {
		std::pair<int, std::pair<int, int>> xEucl = extendedEuclidAlgor(claw.A.first, claw.B.first);
		std::pair<int, std::pair<int, int>> yEucl = extendedEuclidAlgor(claw.A.second, claw.B.second);
		//std::cout << "goal " << claw.goal.first << " " << claw.goal.second << " \n";
		if (claw.goal.first % xEucl.first != 0 or claw.goal.second % yEucl.first != 0) {
			continue;
		}
		bool solFound{ false };
		long long ACount{ 0 }, BCount{ 0 };
		int clawAX = claw.A.first;
		int clawBX = claw.B.first;

		while (claw.goal.first > 0) {
			long long goalX = claw.goal.first;
			if (goalX % clawBX == 0) {
				if ((long long(goalX / clawBX) * claw.B.second + ACount * claw.A.second) == claw.goal.second) {
					BCount = long long(goalX / clawBX);
					solFound = true;
					break;
				}
			}
			claw.goal.first -= clawAX;
			ACount++;
		}
		if (solFound) {
			total += 3 * ACount + BCount;
			//std::cout << ACount << " " << BCount << " ";
		}

	}

	std::cout << "\n" << total << "\n";
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}



void clawMachinePartTwo(std::string fileURL) { //immensely funny and very over the top solution
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	bool reachedEnd{ false };
	std::vector<clawPair> clawSet{};
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			clawPair newClawPair;
			bool finished{ false };
			while (!finished) {
				c = std::fgetc(dataFile);
				if (c == '+') {
					newClawPair.A.first = intUntilSpecialChar(dataFile, ',');
					std::fgetc(dataFile);   //goes from comma to space
					std::fgetc(dataFile);	// goes from space to Y
					std::fgetc(dataFile);	// goes from y to +
					newClawPair.A.second = intUntilSpecialChar(dataFile, 10);
					finished = true;
				}
			}
			finished = false;
			while (!finished) {
				c = std::fgetc(dataFile);
				if (c == '+') {
					newClawPair.B.first = intUntilSpecialChar(dataFile, ',');
					std::fgetc(dataFile);   //goes from comma to space
					std::fgetc(dataFile);	// goes from space to Y
					std::fgetc(dataFile);	// goes from y to +
					newClawPair.B.second = intUntilSpecialChar(dataFile, 10);
					finished = true;
				}
			}
			finished = false;
			while (!finished) {
				c = std::fgetc(dataFile);
				if (c == '=') {
					newClawPair.goal.first = intUntilSpecialChar(dataFile, ',');
					std::fgetc(dataFile);   //goes from comma to space
					std::fgetc(dataFile);	// goes from space to Y
					std::fgetc(dataFile);	// goes from y to +
					newClawPair.goal.second = intUntilSpecialChar(dataFile, 10);
					finished = true;
				}
			}
			clawSet.push_back(newClawPair);
		}
	}

	long long total{ 0 };


	for (clawPair& claw : clawSet) {            
		claw.goal.first += 10000000000000; claw.goal.second += 10000000000000;
		std::pair<int, std::pair<int, int>> xEucl = extendedEuclidAlgor(claw.A.first, claw.B.first);		//contains d, and then n,m
		bool swapped{ false };
		if (xEucl.second.first <= 0) {																		//swaps A and B such that n is always positive (hence m is negative). we remember this for the end
			std::swap(claw.A, claw.B);
			swapped = true;
		}
		xEucl = extendedEuclidAlgor(claw.A.first, claw.B.first);
		std::pair<int, std::pair<int, int>> yEucl = extendedEuclidAlgor(claw.A.second, claw.B.second);
		if (claw.goal.first % xEucl.first != 0 or claw.goal.second % yEucl.first != 0) {				//tells us when a solution is invalid
			continue;
		}
		int n = xEucl.second.first;
		int m = xEucl.second.second;
		int d = xEucl.first;		
		
																	//gcd of A.first (x val of a) and B.first(x val of b)
		long long k = long long(claw.goal.first/d);														//value of goal x val over d
		long long upperLim = k * n;					//upper limit and lower limit of i
		long long lowerLim{ 0 };
		lowerLim = long long(-(m * claw.B.first * k) / (claw.A.first)) + 1; 
		long long iMultiple = long long(claw.B.first / d);		
		long long a{ 0 }, b{ 0 };
		long long iStartPoint = long long((lowerLim / iMultiple + 1) * iMultiple);
		int x = claw.A.first;
		int y = claw.B.first;
		long long z = claw.goal.first;
		int X = claw.A.second;
		int Y = claw.B.second;
		long long Z = claw.goal.second;
		long long ZPrime = (n * k - iStartPoint) * X + (m * k + (iStartPoint * x) / y) * Y;
		long long ZIncrement = (y * X - x * Y) / d;
		if ((Z - ZPrime) % ZIncrement == 0) {
			long long incrementAdd = (Z - ZPrime) / ZIncrement;
			long long a = n * k - iStartPoint + incrementAdd*iMultiple;
			long long b = m * k + ((iStartPoint - incrementAdd*iMultiple) * x) / y;
			if (a >= 0 and b >= 0) {
				if (swapped) {
					total += a + 3 * b;
				}
				else {
					total += 3 * a + b;
				}
				
			}
		}


		


	}
std::cout << "\n" << total << "\n";
std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}
