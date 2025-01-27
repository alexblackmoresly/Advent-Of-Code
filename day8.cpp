#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day8.h"
#include <cstdio>
#include<vector>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include <chrono>
#include "usefulFuncs.h"

#pragma once



std::pair<int, int> vectorBetween(std::pair<int, int> vec1, std::pair<int, int>vec2) {
	std::pair<int, int> returnVec;
	returnVec.first = vec2.first - vec1.first;
	returnVec.second = vec2.second - vec1.second;
	return returnVec;
}

std::pair<int, int> vectorAdd(std::pair<int, int> vec1, std::pair<int, int>vec2) {
	std::pair<int, int> returnVec;
	returnVec.first = vec2.first + vec1.first;
	returnVec.second = vec2.second + vec1.second;
	return returnVec;
}



void resonantFrequency(std::string fileURL) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	int xPos{ 0 };
	int yPos{ 0 };
	std::unordered_map<int, std::vector<std::pair<int, int>>> antennaMap;
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);

		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			if (c != '.' and c != 10) {
				antennaMap[c].push_back({ xPos, yPos });
				xPos++;
			}
			else if (c == '.') {
				xPos++;
			}
			else if (c == 10){
				yPos += 1;
				xPos = 0;
			}
		}
	}
	std::unordered_set<std::pair<int, int>, pair_hash> intfMap;
	xPos -= 1;

	for (std::pair<int, std::vector<std::pair<int, int>>> antennaType : antennaMap) {
		for (int i = 0; i < antennaType.second.size(); i++) {
			for (int j = i + 1; j < antennaType.second.size(); j++) {
				std::pair<int, int> diffVec = vectorBetween(antennaType.second[i], antennaType.second[j]); //from i vector to j vector
				std::pair<int, int> jSide = vectorAdd(antennaType.second[j], diffVec); //so we add the diff to j vec and take it away from i vec
				std::pair<int, int> iSide = vectorBetween(diffVec, antennaType.second[i]);
				if (jSide.first >= 0  and jSide.first <= xPos and jSide.second >= 0 and jSide.second <= yPos) {
					intfMap.insert({ jSide.first, jSide.second });
				}
				if (iSide.first >= 0 and iSide.first <= xPos and iSide.second >= 0 and iSide.second <= yPos) {
					intfMap.insert({ iSide.first, iSide.second });
				}
			}
		}

	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
	std::cout<< std::endl<< intfMap.size();

}


void resonantFrequencyPartTwo(std::string fileURL) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	int xPos{ 0 };
	int yPos{ 0 };
	std::unordered_map<int, std::vector<std::pair<int, int>>> antennaMap;
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);

		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			if (c != '.' and c != 10) {
				antennaMap[c].push_back({ xPos, yPos });
				xPos++;
			}
			else if (c == '.') {
				xPos++;
			}
			else if (c == 10) {
				yPos += 1;
				xPos = 0;
			}
		}
	}
	std::unordered_set<std::pair<int, int>, pair_hash> intfMap;
	xPos -= 1;

	for (std::pair<int, std::vector<std::pair<int, int>>> antennaType : antennaMap) {
		for (int i = 0; i < antennaType.second.size(); i++) {
			for (int j = i + 1; j < antennaType.second.size(); j++) {
				std::pair<int, int> diffVec = vectorBetween(antennaType.second[i], antennaType.second[j]); //from i vector to j vector
				int gcd = euclidAlgor(diffVec.first, diffVec.second);
				diffVec.first = diffVec.first / gcd;
				diffVec.second = diffVec.second / gcd;
				std::pair<int, int> iSide = antennaType.second[i];
				std::pair<int, int> jSide = antennaType.second[j];
				

				while (jSide.first >= 0 and jSide.first <= xPos and jSide.second >= 0 and jSide.second <= yPos) {
					intfMap.insert({ jSide.first,jSide.second });
					jSide = vectorAdd(jSide, diffVec);
					
				}
				while (iSide.first >= 0 and iSide.first <= xPos and iSide.second >= 0 and iSide.second <= yPos) {

					intfMap.insert({ iSide.first,iSide.second });
					iSide = vectorBetween(diffVec, iSide);
					
				}
			}
		}

	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
	std::cout << std::endl << intfMap.size();

}



