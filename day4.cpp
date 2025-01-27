#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day4.h"
#include <cstdio>
#include<vector>
#include<string>

#pragma once

void printVec(std::vector<int> vec);

struct rawMatrixData {
	std::vector<std::vector<int>> rawMatrix;
	std::vector < std::pair<int, int>> XMap;
	int XSize{ 0 };
	int YSize{ 0 };
};

rawMatrixData makeRawMatrix(std::string fileUrl, char startChar) {
	FILE* dataFile = std::fopen(fileUrl.c_str(), "r+");
	bool reachedEnd{ false };
	std::vector<std::vector<int>> rawMatrix;
	std::vector<std::pair<int, int>> XMap;
	int x{ 0 };
	int y{ 0 };
	int xSize{ -1 };
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else if (c != 10) {
			if (x > xSize) {
				xSize++;
				std::vector<int> emptVec;
				rawMatrix.push_back(emptVec);
			}
			rawMatrix[x].push_back(c);
			if (c == startChar) {
				XMap.push_back({ x, y });
			}
			x++;

		}
		else {
			y++;
			x = 0;
		}

	}
	rawMatrixData matData;
	matData.rawMatrix = rawMatrix;
	matData.XMap = XMap;
	matData.XSize = xSize;
	matData.YSize = y;
	return matData;
}

bool XYInBounds(int x, int y, int xMax, int yMax, int xMin, int yMin) {
	if (x <= xMax and x >= xMin and y <= yMax and y >= yMin) {
		return true;
	}
	return false;
}

bool wordFromX(int x, int y, int xInc, int yInc, const std::vector<std::vector<int>>& rawMatrix, int XMax, int YMax) {
	int nextX = x + xInc;
	int nextY = y + yInc;
	if (XYInBounds(nextX,nextY, XMax, YMax, 0, 0) and rawMatrix[nextX][nextY] == 'M') {
		nextX += xInc;
		nextY += yInc;
		if (XYInBounds(nextX, nextY, XMax, YMax, 0, 0) and rawMatrix[nextX][nextY] == 'A') {
			nextX += xInc;
			nextY += yInc;
			if (XYInBounds(nextX, nextY, XMax, YMax, 0, 0) and rawMatrix[nextX][nextY] == 'S') {
				return true;
			}
		}

	}
	return false;
}

bool wordFromA(int x, int y, const std::vector<std::vector<int>>& rawMatrix, int XMax, int YMax) {
	if (!XYInBounds(x + 1, y + 1, XMax, YMax, 0, 0) or !XYInBounds(x - 1, y - 1, XMax, YMax, 0, 0)) {
		return false;
	}
	int topRight = rawMatrix[x + 1][y + 1];
	int botRight = rawMatrix[x + 1][y - 1];
	int topLeft = rawMatrix[x - 1][y + 1];
	int botLeft = rawMatrix[x - 1][y - 1];
	if ((topRight == 'M' and botLeft == 'S') or (topRight == 'S' and botLeft == 'M')) {
		if ((topLeft == 'M' and botRight == 'S') or (topLeft == 'S' and botRight == 'M')) {
			return true;
		}

	}

	return false;
}
	

void ceresSearch(std::string fileUrl) {
	
	rawMatrixData matrixData = makeRawMatrix(fileUrl, 'X');
	const std::vector< std::vector<int> > rawMatrix = matrixData.rawMatrix;
	std::vector<std::pair<int, int>> XMap = matrixData.XMap;
	int XSize = matrixData.XSize;
	int YSize = matrixData.YSize;
	int total{ 0 };
	FILE* dataFile = std::fopen(fileUrl.c_str(), "r+");
	for (std::pair<int, int> XYPair : XMap) {
		//std::cout << "\n pair looked at";
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				//std::cout << "\n" << i << " " << j;
				if (wordFromX(XYPair.first, XYPair.second, i, j, rawMatrix, XSize, YSize)) {
					
					total += 1;
				}
			}
		}
		
	}
	std::cout << total;

	return;
}


void ceresSearchPartTwo(std::string fileUrl) {

	rawMatrixData matrixData = makeRawMatrix(fileUrl, 'A');
	const std::vector< std::vector<int> > rawMatrix = matrixData.rawMatrix;
	std::vector<std::pair<int, int>> XMap = matrixData.XMap;
	int XSize = matrixData.XSize;
	int YSize = matrixData.YSize;
	int total{ 0 };
	FILE* dataFile = std::fopen(fileUrl.c_str(), "r+");
	for (std::pair<int, int> XYPair : XMap) {
		//std::cout << "\n pair looked at";
		if (wordFromA(XYPair.first, XYPair.second, rawMatrix, XSize, YSize)) {
			total += 1;
		}

	}
	std::cout << total;

	return;
}

//plan: turn wordsearch into array of arrays, note all locations of x's, for each x we look in each direction (all 8, unfortunately) and check if it gives a valid word. if it contains 
// and x we can probably yeet than from our list of x locations but that might be hard etc (and might be impossible anyways really)
