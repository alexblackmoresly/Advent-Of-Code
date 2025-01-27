#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day6.h"
#include <cstdio>
#include<vector>
#include<string>
#include<unordered_set>
#include<unordered_map>

struct pair_hash {
	size_t operator()(const std::pair<int, int>& p) const {
		// A simple hash combining both integers
		return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
	}
};

struct GuardMatrixData {
	std::vector<std::vector<int>> guardMatrix;
	std::pair<int, int> guardPos;
	int xSize;
	int ySize;

	GuardMatrixData(std::vector<std::vector<int>> mat, std::pair<int, int> pos, int x, int y) : guardMatrix(mat), guardPos(pos), xSize(x), ySize(y) {}
};
struct Guard {
	int xPos{};
	int yPos{};
	int xVel{ 0 };
	int yVel{ -1 };
	int xMax{};
	int yMax{};
	bool guardOnMap{ true };
	std::vector<std::vector<int>> guardMatrix;

	Guard(GuardMatrixData matData) {
		xPos = matData.guardPos.first;
		yPos = matData.guardPos.second;
		xMax = matData.xSize;
		yMax = matData.ySize;
		guardMatrix = matData.guardMatrix;
	}

	void changeDirection() {
		if (xVel) { yVel = xVel; xVel = 0; }
		else { xVel = -yVel;  yVel = 0; }
	}
	void move() {
		std::pair<int, int> lookingAt;
		
		lookingAt.first = xPos + xVel;
		lookingAt.second = yPos + yVel;
		//std::cout << "\n at " <<xPos << " " << yPos << " looking at " << lookingAt.first << " " << lookingAt.second;
		if (lookingAt.first > xMax or lookingAt.first < 0 or lookingAt.second > yMax or lookingAt.second < 0) {
			guardOnMap = false;
		}
		else { 
			
			if (guardMatrix[lookingAt.first][lookingAt.second] == 1) {
				changeDirection();
			}
			else {
				xPos += xVel; yPos += yVel;
			}
		
		
		}

		

	}
	bool inBounds(int x, int y) {
		if (x > xMax or x < 0 or y > yMax or y < 0) {
			return false;
		}
		return true;

	}
	};



GuardMatrixData makeGuardMatrix(std::string fileURL){
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	std::vector<std::vector<int>> guardMatrix;
	int x{ 0 };
	int y{ 0 };
	int xSize{ -1 };
	std::pair<int,int> guardPos;
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else if (c != 10) {
			if (x > xSize) {
				xSize++;
				std::vector<int> emptVec;
				guardMatrix.push_back(emptVec);
			}
			if (c == '.'){ guardMatrix[x].push_back(0); }
			if (c == '#') { guardMatrix[x].push_back(1); }
			if (c == '^') { guardMatrix[x].push_back(0); guardPos.first = x; guardPos.second = y; }
			x++;

		}
		else {
			y++;
			x = 0;
		}
	}
	std::fclose(dataFile);
	//std::cout << guardPos.first << " " << guardPos.second << " ";
	GuardMatrixData data(guardMatrix, guardPos, xSize, y);
	
	return data;

}
void guardGallivant(std::string fileURL) {
	GuardMatrixData matrixData = makeGuardMatrix(fileURL);
	Guard guard(matrixData);
	std::unordered_set<std::pair<int, int>, pair_hash> guardPath;
	int total{ 0 };
	while (guard.guardOnMap) {
		std::pair<int, int> guardPos;
		guardPos.first = guard.xPos; guardPos.second = guard.yPos;
		guardPath.insert(guardPos);
		guard.move();

	}
	std::cout << guardPath.size();
	

}

bool isLoop(Guard guard, std::unordered_map<std::pair<int, int>, std::unordered_set<std::pair<int, int>, pair_hash>, pair_hash> guardPath) {
	while (guard.guardOnMap) {
		std::pair<int, int> guardDir{ guard.xVel, guard.yVel };
		std::pair<int, int> guardPos{ guard.xPos,guard.yPos };
		if (guardPath.count(guardPos)) {
			if (guardPath[guardPos].count(guardDir)) {
				return true;
			}
		}
		guardPath[guardPos].insert(guardDir);

		guard.move();

	}
	return false;
}



void guardGallivantPartTwo(std::string fileURL) {
	GuardMatrixData matrixData = makeGuardMatrix(fileURL);
	Guard guard(matrixData);
	std::unordered_map<std::pair<int, int>, std::unordered_set<std::pair<int, int>, pair_hash>, pair_hash> guardPath;
	int total{ 0 };
	while (guard.guardOnMap) {
		
		std::pair<int, int> lookingAt{ guard.xPos + guard.xVel, guard.yPos + guard.yVel };
		if (guard.inBounds(lookingAt.first, lookingAt.second)) {
			if (!guardPath.count(lookingAt)) {
				if (guard.guardMatrix[lookingAt.first][lookingAt.second] == 0) {
					Guard guardCopy = guard;
					//std::cout << " " << lookingAt.first << " " << lookingAt.second;
					guardCopy.guardMatrix[lookingAt.first][lookingAt.second] = 1;
					if (isLoop(guardCopy, guardPath)) {
						total += 1;
					}
				}
			}
		}
		std::pair<int, int> guardPos{ guard.xPos,guard.yPos };
		std::pair<int, int> guardDir{ guard.xVel,guard.yVel };
		guardPath[guardPos].insert(guardDir);
		
		guard.move();

	}
	std::cout << total << " ";


}