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
#include<regex>
#pragma once
std::vector<std::pair<int, int>> restroomRedoubtHelper(std::string fileURL, int howLong);

void restroomRedoubt(std::string fileURL) {

	std::vector<std::pair<int, int>>results = restroomRedoubtHelper(fileURL, 100);
	const int width{ 101 };
	const int height{ 103 };
	auto xMid = (width - 1) / 2;
	auto yMid = (height - 1) / 2;
	int q1{ 0 }, q2{ 0 }, q3{ 0 }, q4{ 0 };
	for (std::pair<int, int> robot : results) {
		int fx = robot.first, fy = robot.second;
		if (fx < xMid and fy < yMid) {
			q1++;
		}
		else if (fx > xMid and fy < yMid) {
			q2++;
		}
		else if (fx < xMid and fy >yMid) {
			q3++;
		}
		else if (fx > xMid and fy > yMid) {
			q4++;
		}
	}
	std::cout << q1 * q2 * q3 * q4;
}
void restroomRedoubtPartTwo(std::string fileURL) {

	int startCycle{ 1000 };
	int endCycle{ 1000000 };


	const int width{ 101 };
	const int height{ 103 };
	std::string emptyString{};
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			emptyString += '.';
		}
		emptyString += "\n";
	}
	for (int cycle = startCycle; cycle < endCycle; cycle++) {

		std::string bigString = emptyString;
		std::vector<std::pair<int, int>>results = restroomRedoubtHelper(fileURL, cycle);
		
		for (std::pair<int, int> pos : results) {
			int posInString = pos.first + (width + 1) * pos.second;
			bigString[posInString] = '0';
		}
		std::regex pattern(R"(0000000000000000+)");
		if (std::regex_search(bigString.begin(), bigString.end(), pattern)) {
			std::cout << bigString << "\n";
			std::cout << cycle << " continue y/n \n";
			char n;
			std::cin >> n;
			if (n == 'n') {
				break;
			}
		}
	}
	
	
}



std::vector<std::pair<int, int>> restroomRedoubtHelper(std::string fileURL, int howLong) {
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	std::string input{ "" };
	while (true) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			break;
		}
		input += c;
	}
	std::regex pattern(R"(\bp=(-?\d+),(-?\d+)\s+v=(-?\d+),(-?\d+)\b)");
	std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> initials;
	std::smatch match;
	auto itrtor = input.cbegin();
	while (std::regex_search(itrtor, input.cend(), match, pattern)) {
		int px = std::stoi(match[1]); int py = std::stoi(match[2]); int vx = std::stoi(match[3]); int vy = std::stoi(match[4]);
		initials.push_back({ {px,py},{vx,vy} });
		itrtor = match.suffix().first;

	}
	const int width{ 101 };
	const int height{ 103 };
	auto xMid = (width - 1) / 2;
	auto yMid = (height - 1) / 2;
	int q1{ 0 }, q2{ 0 }, q3{ 0 }, q4{ 0 };
	std::vector< std::pair<int, int>> results;
	for (std::pair<std::pair<int, int>, std::pair<int, int>> robot : initials) {
		int px = robot.first.first; int py = robot.first.second; int vx = robot.second.first; int vy = robot.second.second;
		int fx = (px + howLong * vx) % width; int fy = (py + howLong * vy) % height;
		if (fx < 0) {
			fx += width;
		}
		if (fy < 0) {
			fy += height;
		}
		results.push_back({ fx,fy });

	}
	std::fclose(dataFile);
	return results;
}