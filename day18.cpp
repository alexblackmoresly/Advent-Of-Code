#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day13.h"
#include <cstdio>
#include<vector>
#include<string>
#include <fstream>
#include <chrono>
#include "usefulFuncs.h"
#include<regex>
#include <cmath>
#include <unordered_set>
#include <unordered_map>
#pragma once


std::vector<std::vector<int>> ramEmptyMatrix(int size) {
	std::vector<int> column;
	std::vector<std::vector<int>> mat;

	for (int i = 0; i < size; i++) {
		column.push_back(1);
	}
	for (int i = 0; i < size; i++) {
		mat.push_back(column);
	}
	return mat;
	
}

bool updateMaxRowCts(std::vector<std::vector<int>>& ramMat, std::vector<int>& maxRowCts, std::pair<int, int> coords, std::vector<int>& maxColCts, bool swapped) {
	if (!swapped) {
		//std::cout << "looking at " << coords.first << "," << coords.second << "\n";
	}
	else {
		//std::cout << "looking at " << coords.second << "," << coords.first << "\n";
	}
	int currRowDist = maxRowCts[coords.first];
	bool added = false;
	if (coords.second > currRowDist) {																					//updates current row distance if needed
		for (int i : {-1, 0, 1}) {
			int newX = coords.first + i;
			if (newX >= 0 and newX < maxRowCts.size()) {
				if (-1 <= maxRowCts[newX] - coords.second and 1 >= maxRowCts[newX] - coords.second) {
					maxRowCts[coords.first] = coords.second;
					added = true;
					if (!swapped) {
						//std::cout << "updated col " << coords.first << " to distance " << coords.second << "\n";
					}
					else {
						//std::cout << "updated row " << coords.first << " to distance " << coords.second << "\n";
					}
					break;
				}
			}
		}
	}
	if (!added) {
		return false;
	}
	/*if (maxColCts[coords.second] >= coords.first) {													//this is the hard part - actually checking if what we have done will block off all paths
		if (swapped) {
			std::cout << "matched row " << coords.second << " with col " << coords.first << "\n";
		}
		else {
			std::cout << "matched row " << coords.first << " with col " << coords.second << "\n";
		}
		return true;
	}*/
	if (coords.second == maxRowCts.size() - 1) {
		return true;
	}

	for (int i : {-1, 0, 1}) {
		for (int j : {-1, 0, 1}) {
			if (i == 0 and j == 0) {
				continue;
			}
			int newX = coords.first + i; int newY = coords.second + j;
			if (newX >= 0 and newY >= 0 and newX < maxRowCts.size() and newY < maxRowCts.size()) {
				if ((!swapped and ramMat[newX][newY] == 0) or (swapped and ramMat[newY][newX] == 0)) {
					if (maxRowCts[newX] < newY) {
						if (updateMaxRowCts(ramMat, maxRowCts, { newX, newY }, maxColCts, swapped)) {
							return true;
						}
					}
					
				}
				
			}

		}
	}
	return false;
	
}

bool ramConUpdate(std::pair<int, int> coords, std::unordered_set<std::pair<int, int>, pair_hash>& conSide, std::unordered_set<std::pair<int, int>, pair_hash>& conTop, int matSize, std::vector<std::vector<int>>& ramMatrix, int depth) {
	int x = coords.first; int y = coords.second;
	bool added = false;
	if (depth > 200) {   
		//std::cout << "looking at " << x << "," << y << " \n";
	}
	if (depth > 300) {   //all this tells me is that somewhere the code is broken but it did end up giving me the right answer... code frankly might not even be broken just extremely memory inefficient
		return false;
	}
	if (x == 0) {
		conSide.insert(coords);
		added = true;
	}
	else {
		for (int i : {-1, 0, 1}) {
			for (int j : {-1, 0, 1}) {
				if (i == 0 and j == 0) {
					continue;
				}
				else {
					std::pair<int, int> newCoords = { x + i, y + j };
					if (conSide.count(newCoords)) {
						conSide.insert(coords);
						added = true;
						if (x == matSize - 1) {
							//std::cout << "a";
							return true;
						}
						break;
					}
				}
			}
		}
	}

	if (!added) {
		return false;
	}
	for (int i : {-1, 0, 1}) {
		for (int j : {-1, 0, 1}) {
			if (conTop.count({y+j, x+i})) {
				//std::cout << "b";
				return true;
			}
		}
	}

	for (int i : {-1, 0, 1}) {
		for (int j : {-1, 0, 1}) {
			if (i == 0 and j == 0){
				continue;
			}
			int newX = x + i; int newY = y + j;
			if (newX >= 0 and newX < matSize and newY >= 0 and newY < matSize and ramMatrix[newX][newY] == 0) {
				if (!conSide.count({ newX,newY })) {
					if (ramConUpdate({ newX,newY }, conSide, conTop, matSize, ramMatrix, depth + 1)) {
						//std::cout << "c";
						return true;
					}
				}
			}
		}
	}






	return false;
}





void ramRunPartTwo(std::string fileURL, int matSize) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::vector<std::vector<int>> ramMatrix = ramEmptyMatrix(matSize);
	std::vector<std::vector<int>> invRamMatrix = ramMatrix;
	std::vector<std::vector<int>> flippedMatrix = ramMatrix;
	std::vector<std::vector<int>> invFlippedMatrix = ramMatrix;
	int maxDist = matSize * matSize + 1;
	std::pair<int, int> startCoords{ 0,0 };
	std::pair<int, int> endCoords = { matSize - 1, matSize - 1 };
	std::string fileString = fileToString(fileURL);
	std::regex coordPattern(R"((\d+),(\d+))");
	std::smatch match;
	auto start = fileString.cbegin();
	

	std::vector<int> maxRowCts;  
	std::vector<int> maxColCts;
	std::unordered_set<std::pair<int, int>, pair_hash> conTop, conSide, flipConTop, flipConSide;

	//for (int i = 0; i < matSize; i++) {
	//	maxRowCts.push_back(-1);
	//	maxColCts.push_back(-1);
	//}
	int counter{ 0 };

	while (std::regex_search(start, fileString.cend(), match, coordPattern)) {
		int x = std::stoi(match[1]); int y = std::stoi(match[2]);
		ramMatrix[x][y] = 0;
		invRamMatrix[y][x] = 0;
		flippedMatrix[matSize - 1 - x][matSize - 1 - y] = 0;
		invFlippedMatrix[matSize - 1 - y][matSize - 1 - x] = 0;
		//std::cout << counter << "\n";

		if (ramConUpdate({ x,y }, conSide, conTop, matSize, ramMatrix, 0)) {
			std::cout << "answer is " << x << "," << y;
			break;
		}
		if (ramConUpdate({ y,x }, conTop, conSide, matSize, invRamMatrix, 0)) {
			std::cout << "answer is " << x << "," << y;
			break;
		}
		if (ramConUpdate({ matSize - 1 - x, matSize - 1 - y }, flipConSide, flipConTop, matSize, flippedMatrix, 0)) {
			std::cout << "answer is " << x << "," << y;
			break;
		}
		if (ramConUpdate({ matSize - 1 - y, matSize - 1 - x }, flipConTop, flipConSide, matSize, invFlippedMatrix, 0)) {
			std::cout << "answer is " << x << "," << y;
			break;
		}
		counter++;



		/*if (updateMaxRowCts(ramMatrix, maxRowCts, { x,y }, maxColCts, false)) {
			std::cout << "answer is " << x << "," << y;
			break;
		}

		

		if (updateMaxRowCts(ramMatrix, maxColCts, { y,x }, maxRowCts, true)) {
			std::cout << "answer is " << x << "," << y;
			break;
		}*/
		
		start = match.suffix().first;
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "\nTime difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;  //slooooooooow

	
}




void ramRun(std::string fileURL, int matSize, int noOfBytes) {
	std::vector<std::vector<int>> ramMatrix = ramEmptyMatrix(matSize);
	int maxDist = matSize * matSize + 1;
	std::pair<int, int> startCoords{ 0,0 };
	std::pair<int, int> endCoords = { matSize - 1, matSize - 1 };
	std::string fileString = fileToString(fileURL);
	std::regex coordPattern(R"((\d+),(\d+))");
	std::smatch match;
	auto start = fileString.cbegin();
	int count{ noOfBytes };
	while (std::regex_search(start, fileString.cend(), match, coordPattern) and noOfBytes > 0) {
		int x = std::stoi(match[1]); int y = std::stoi( match[2]);
		ramMatrix[x][y] = 0;
		start = match.suffix().first;
		noOfBytes--;
	}

	std::unordered_map < std::vector<int>, minTreeNode*, vec_hash> unvisited, visited;
	minBinTree tree;
	

	for (int x = 0; x < matSize; x++) {
		for (int y = 0; y < matSize; y++) {
			if (ramMatrix[x][y] == 1) {
				minTreeNode* node = new minTreeNode(maxDist, std::vector<int> {x, y});
				unvisited[node->data] = node;
				tree.push(node);
			}

		}
	}
	std::cout << "  ";
	for (int x = 0; x < matSize; x++) {
		std::cout << x << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < matSize; i++) {
		std::cout << i << " ";

		for (int j = 0; j < matSize; j++) {

			std::cout << ramMatrix[j][i] << " ";
		}
		std::cout << std::endl;
	}
	int finalDist{ -1 };
	tree.decreasePriority(unvisited[{0, 0}], 0);
	while (tree.size > 0) {
		std::vector<int> coords = tree.peek().data;
		int dist = tree.peek().value;
		tree.pop();
		unvisited.erase(coords);
		if (coords[0] == endCoords.first and coords[1] == endCoords.second) {
			if (dist < finalDist or finalDist == -1) {
				finalDist = dist;
			}
		}

		for (int i : std::vector<int>{ -1,1 }) {
			if (unvisited.count({coords[0] + i, coords[1]})) {
				tree.decreasePriority(unvisited[{coords[0] + i, coords[1]}], dist + 1);
			}
			if (unvisited.count({ coords[0], coords[1] + i })) {
				tree.decreasePriority(unvisited[{coords[0], coords[1]+ i}], dist + 1);
			}
			
		}
	}
	std::cout << finalDist;
	
	
}