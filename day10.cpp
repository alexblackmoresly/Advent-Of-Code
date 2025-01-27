#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day10.h"
#include <cstdio>
#include<vector>
#include<string>
#include<unordered_set>
#include <fstream>
#include <chrono>
#include "usefulFuncs.h"
#pragma once



void pathSearch(int x, int y, int val, std::vector<std::vector<int>>& topMap, std::unordered_set<std::pair<int,int>, pair_hash>& ninesReached , int border) {
	if (val == 9) {
		ninesReached.insert(std::pair<int, int> {x, y});
	}
	else {
		if (x + 1 <= border and topMap[x + 1][y] == val + 1) {
			pathSearch(x + 1, y, val + 1, topMap, ninesReached, border);
		}
		if (y + 1 <= border and topMap[x][y + 1] == val + 1) {
			pathSearch(x , y + 1, val + 1, topMap, ninesReached, border);
		}
		if  (x - 1 >= 0 and topMap[x - 1][y] == val + 1) {
			pathSearch(x-1, y, val + 1, topMap,	ninesReached, border);
		}
		if (y - 1 >= 0 and topMap[x][y - 1] == val + 1) {
			pathSearch(x, y - 1, val + 1, topMap, ninesReached, border);
		}
	}
	return;
}

void uniquePathSearch(int x, int y, int val, std::vector<std::vector<int>>& topMap, std::vector<std::pair<int, int>>& ninesReached, int border) {
	if (val == 9) {
		ninesReached.push_back(std::pair<int, int> {x, y});
	}
	else {
		if (x + 1 <= border and topMap[x + 1][y] == val + 1) {
			uniquePathSearch(x + 1, y, val + 1, topMap, ninesReached, border);
		}
		if (y + 1 <= border and topMap[x][y + 1] == val + 1) {
			uniquePathSearch(x, y + 1, val + 1, topMap, ninesReached, border);
		}
		if (x - 1 >= 0 and topMap[x - 1][y] == val + 1) {
			uniquePathSearch(x - 1, y, val + 1, topMap, ninesReached, border);
		}
		if (y - 1 >= 0 and topMap[x][y - 1] == val + 1) {
			uniquePathSearch(x, y - 1, val + 1, topMap, ninesReached, border);
		}
	}
	return;
}


void hoofIt(std::string fileURL, bool partTwo) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	std::vector<std::vector<int>> topMap;
	std::vector<std::pair<int, int>> zeros;
	int x{ 0 }, y{ 0 };
	int c{ 0 };
	while (!reachedEnd) {
		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			c = std::fgetc(dataFile);
			topMap.push_back(std::vector<int> {});
			while (c != 10 and c != EOF) {
				if (c == '0') {
					zeros.push_back(std::pair<int, int> {y, x});
				}
				topMap[y].push_back(c - 48);
				c = std::fgetc(dataFile);
				x++;
			}
			x = 0;
			y++;
		}
	}
	int total{ 0 };
	if (!partTwo) {
		for (std::pair<int, int> zeroPos : zeros) {
			std::unordered_set< std::pair<int, int>, pair_hash> ninesReached;
			pathSearch(zeroPos.first, zeroPos.second, 0, topMap, ninesReached, y - 1);
			total += ninesReached.size();

		}
	}
	else {
		for (std::pair<int, int> zeroPos : zeros) {
			std::vector<std::pair<int, int>> ninesReached;
			uniquePathSearch(zeroPos.first, zeroPos.second, 0, topMap, ninesReached, y - 1);
			total += ninesReached.size();
		}
	}
	std::cout << total << "\n";
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "us" << std::endl;

}