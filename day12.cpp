#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day12.h"
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

struct garden_shape_data {
	int area{ 0 };
	int perimeterLength{ 0 };
};


void findPerimeter(std::vector<std::vector<int>>& gardenMatrix, int x, int y, int& runningTotal, int& runningArea, int xMax, int yMax, int lookingFor,	//function is void but modifies runningArea, runningTotal, and perimeterSet
	std::unordered_map<std::pair<int, int>, std::unordered_set<char>, pair_hash>& perimeterSet) {
	gardenMatrix[x][y] = -lookingFor;
	runningArea += 1;

	if (x > 0) {																						//checks if in bounds
		if (gardenMatrix[x - 1][y] == lookingFor) {														//checks if block it's looking at is of the same type
			findPerimeter(gardenMatrix, x - 1, y, runningTotal, runningArea, xMax, yMax, lookingFor, perimeterSet);
		}
		else if (gardenMatrix[x-1][y] != -lookingFor) {													//if block is not of same type, checks if it is one already looked and if not then
			runningTotal++;																				//we must have a border facing the direction (here x-1, so left)
			perimeterSet[std::pair<int, int>{x, y}].insert('W');
		}
	}
	else {
		runningTotal++;
		perimeterSet[std::pair<int, int>{x, y}].insert('W');
	}
	if (y > 0) {
		if (gardenMatrix[x][y-1] == lookingFor) {
			findPerimeter(gardenMatrix, x, y-1, runningTotal, runningArea, xMax, yMax, lookingFor, perimeterSet);
		}
		else if (gardenMatrix[x][y - 1] != -lookingFor) {
			runningTotal++;
			perimeterSet[std::pair<int, int>{x, y}].insert('S');
		}
	}
	else {
		runningTotal++;
		perimeterSet[std::pair<int, int>{x, y}].insert('S');
	}
	if (x < xMax) {
		if (gardenMatrix[x + 1][y] == lookingFor) {
			findPerimeter(gardenMatrix, x + 1, y, runningTotal, runningArea, xMax, yMax, lookingFor, perimeterSet);
		}
		else if (gardenMatrix[x + 1][y] != -lookingFor) {
			runningTotal++;
			perimeterSet[std::pair<int, int>{x, y}].insert('E');
		}
	}
	else {
		runningTotal++;
		perimeterSet[std::pair<int, int>{x, y}].insert('E');

	}
	if (y < yMax) {
		if (gardenMatrix[x][y + 1] == lookingFor) {
			findPerimeter(gardenMatrix, x, y + 1, runningTotal, runningArea, xMax, yMax, lookingFor, perimeterSet);
		}
		else if (gardenMatrix[x][y + 1] != -lookingFor) {
			runningTotal++;
			perimeterSet[std::pair<int, int>{x, y}].insert('N');

		}
	}
	else {
		runningTotal++;
		perimeterSet[std::pair<int, int>{x, y}].insert('N');
	}
	return;
	}



int sidesFromPerimeterSet(std::unordered_map<std::pair<int, int>, std::unordered_set<char>, pair_hash>& perimeterSet) {
	int corners{ 0 };
	for (std::pair< std::pair<int, int>, std::unordered_set<char>> boxLookingAt : perimeterSet) {
		int x = boxLookingAt.first.first;
		int y = boxLookingAt.first.second;
		for (char direction : boxLookingAt.second) {
			if (direction == 'N') {
				if (boxLookingAt.second.count('E')) {
					corners++;
				}
				else {
					if (perimeterSet[std::pair<int, int> {x + 1, y + 1}].count('W')) {
						corners++;
					}
				}
			}
			if (direction == 'E') {
				if (boxLookingAt.second.count('S')) {
					corners++;
				}
				else {
					if (perimeterSet[std::pair<int, int> {x + 1, y - 1 }].count('N')) {
						corners++;
					}
				}
			}
			if (direction == 'S') {
				if (boxLookingAt.second.count('W')) {
					corners++;
				}
				else {
					if (perimeterSet[std::pair<int, int> {x - 1, y - 1 }].count('E')) {
						corners++;
					}
				}
			}
			if (direction == 'W') {
				if (boxLookingAt.second.count('N')) {
					corners++;
				}
				else {
					if (perimeterSet[std::pair<int, int> {x - 1, y + 1 }].count('S')) {
						corners++;
					}
				}
			}
		}
	}



	return corners;
}


void gardenGroups(std::string fileURL) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::vector<std::vector<int>> gardenMatrix = makeMatrix(fileURL);
	int xMax = gardenMatrix.size() - 1;
	int yMax = gardenMatrix[0].size() - 1;
	int total{ 0 };
	int total2{ 0 };
	//std::cout << findPerimeter(gardenMatrix, 3, 0, total, xMax, yMax, gardenMatrix[3][0]);
	for (int x = 0; x < xMax + 1; x++) {
		for (int y = 0; y < yMax + 1; y++) {
			if (gardenMatrix[x][y] > 0) {
				int runningTotal{ 0 }, runningArea{ 0 };
				std::unordered_map<std::pair<int, int>, std::unordered_set<char>, pair_hash> perimeterSet{};
				//garden_shape_data areaAndPerim = findPerimeter(gardenMatrix, x, y, runningTotal, runningArea, xMax, yMax, gardenMatrix[x][y], perimeterSet);
				findPerimeter(gardenMatrix, x, y, runningTotal, runningArea, xMax, yMax, gardenMatrix[x][y], perimeterSet);
				total += runningTotal * runningArea;
				total2 += runningArea * sidesFromPerimeterSet(perimeterSet);
			}
		}
	}

	std::cout << std::endl << total << " " << total2 << std::endl;
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;  //5ms and 4ms of that is making the matrix itself!
}