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


std::pair<std::pair<int,int>, std::vector<std::vector<int>>> warehouseMapMaker(std::string mapFile) {
	FILE* mapData = std::fopen(mapFile.c_str(), "r+");
	std::vector < std::vector<int>> warehouseMap{ std::vector<int>{} };
	int x{ 0 }, y{ 0 };
	std::pair<int, int> botPos;
	while (true) {
		int c = std::fgetc(mapData);
		if (c == EOF) {
			break;
		}
		if (c == 10) {
			x = 0;
			y++;
			warehouseMap.push_back(std::vector<int>{});
		}
		else {
			warehouseMap[y].push_back(c);
			if (c == '@') {
				botPos = { y,x };
			}
			x++;
		}
		
	}

	std::fclose(mapData);
	return { botPos,warehouseMap };
}

std::pair<std::pair<int, int>, std::vector<std::vector<int>>> wideWarehouseMapMaker(std::string mapFile) {
	FILE* mapData = std::fopen(mapFile.c_str(), "r+");
	std::vector < std::vector<int>> warehouseMap{ std::vector<int>{} };
	int x{ 0 }, y{ 0 };
	std::pair<int, int> botPos;
	while (true) {
		int c = std::fgetc(mapData);
		if (c == EOF) {
			break;
		}
		if (c == 10) {
			x = 0;
			y++;
			warehouseMap.push_back(std::vector<int>{});
		}
		else {
			if (c == '@') {
				warehouseMap[y].push_back(c);
				warehouseMap[y].push_back('.');
				botPos = { y,x };
			}
			else if (c == '.'){
				warehouseMap[y].push_back('.');
				warehouseMap[y].push_back('.');
			}
			else if (c == 'O') {
				warehouseMap[y].push_back('[');
				warehouseMap[y].push_back(']');
			}
			else if (c == '#') {
				warehouseMap[y].push_back('#');
				warehouseMap[y].push_back('#');
			}
			x+=2;
		}

	}

	std::fclose(mapData);
	return { botPos,warehouseMap };
}

int warehouseScore(std::vector<std::vector<int>> map) {
	int x{ 0 }, y{ 0 };
	int total{ 0 };
	for (std::vector<int> row : map) {
		for (int i : row) {
			if (i == 'O' or i == '[') {
				total += 100 * y + x;
			}
			x++;
		}
		x = 0; y++;
	}
	return total;
}

void warehouseWoes(std::string mapFile, std::string movesFile) {
	std::pair<std::pair<int, int>, std::vector<std::vector<int>>> mapData = warehouseMapMaker(mapFile);
	std::vector<std::vector<int>> map = mapData.second;
	std::pair<int, int> botPos = mapData.first;
	FILE* movesData = std::fopen(movesFile.c_str(), "r+");
	std::string moves{ "" };
	while (true) {
		int c = std::fgetc(movesData);
		if (c == EOF) {
			break;
		}
		moves += char(c);
	}
	std::fclose(movesData);
	for (char move : moves) {
		int vx{ 0 }, vy{ 0 };
		if (move == 'v') {
			vx = 0; vy = 1;
		}
		else if (move == '^') {
			vx = 0; vy = -1;
		}
		else if (move == '>') {
			vx = 1; vy = 0;
		}
		else if (move == '<') {
			vx = -1; vy = 0;
		}
		int x = botPos.second + vx; int y = botPos.first + vy;
		if (map[y][x] != '#') {
			while (map[y][x] == 'O') {
				y += vy; x += vx;
			}
		}
		if (map[y][x] != '#') {
			map[y][x] = 'O';
			map[botPos.first][botPos.second] = '.';
			botPos.first += vy; botPos.second += vx;
			map[botPos.first][botPos.second] = '@';
		}
	

	}
	for (std::vector<int> row : map) {
		std::cout << std::endl;
		for (int i : row) {
			std::cout << char(i);
		}
	}
	std::cout << warehouseScore(map);
}

void warehouseWoes2(std::string mapFile, std::string movesFile) {
	std::pair<std::pair<int, int>, std::vector<std::vector<int>>> mapData = wideWarehouseMapMaker(mapFile);
	std::vector<std::vector<int>> map = mapData.second;
	std::pair<int, int> botPos = mapData.first;
	FILE* movesData = std::fopen(movesFile.c_str(), "r+");
	std::string moves{ "" };
	while (true) {
		int c = std::fgetc(movesData);
		if (c == EOF) {
			break;
		}
		moves += char(c);
	}
	std::fclose(movesData);
	for (char move : moves) {
		int vx{ 0 }, vy{ 0 };
		if (move == 'v') {
			vx = 0; vy = 1;
		}
		else if (move == '^') {
			vx = 0; vy = -1;
		}
		else if (move == '>') {
			vx = 1; vy = 0;
		}
		else if (move == '<') {
			vx = -1; vy = 0;
		}
		int x = botPos.second + vx; int y = botPos.first + vy;
		if (vx) {
			while (map[y][x] == ']' or map[y][x] == '[') {
				x += vx;
			}
			if (map[y][x] != '#') {
				if (vx == 1) {
					for (int i = x; i > botPos.second; i--) {
						map[y][i] = map[y][i - 1];
					}
				}
				else {
					for (int i = x; i < botPos.second; i++) {
						map[y][i] = map[y][i + 1];
					}
				}
				map[botPos.first][botPos.second] = '.';
				botPos.first = y; botPos.second += vx;
				map[botPos.first][botPos.second] = '@';
			}
		}
		if (vy) {
			std::unordered_set<std::pair<int,int>, pair_hash> boxLayer;
			std::vector<std::pair<int, int>> toBeMoved;
			bool failed{ false };
			if (map[y][x] == '[') {
				boxLayer.insert({ y,x });

			}
			else if (map[y][x] == ']') {
				boxLayer.insert({ y, x - 1 });

			}
			else if (map[y][x] == '#') {
				failed = true;
			}
			while (!boxLayer.empty()) {
				std::unordered_set<std::pair<int, int>, pair_hash> tempLayer{};
				for (std::pair<int, int> box : boxLayer) {
					x = box.second; y = box.first;
					if (map[y + vy][x] == '#' or map[y + vy][x + 1] == '#') {
						boxLayer.clear();
						failed = true;
						break;
					}
					if (map[y + vy][x] == ']') {
						tempLayer.insert({ y + vy, x - 1 });
					}
					else if (map[y + vy][x] == '[') {
						tempLayer.insert({ y + vy, x });
					}
					if (map[y + vy][x + 1] == '[') {
						tempLayer.insert({ y + vy, x + 1 });
					}

					toBeMoved.push_back({ y,x });
					toBeMoved.push_back({ y,x + 1 });
				}
				boxLayer = tempLayer;
			}
			if (!failed) {
				for (int i = toBeMoved.size() - 1; i >= 0; i -- ) {
					std::pair<int,int> box = toBeMoved[i];
					map[box.first + vy][box.second] = map[box.first][box.second];
					map[box.first][box.second] = '.';
				}
				map[botPos.first][botPos.second] = '.';
				botPos.first += vy;
				map[botPos.first][botPos.second] = '@';
			}
		}


	}
	for (std::vector<int> row : map) {
		std::cout << std::endl;
		for (int i : row) {
			std::cout << char(i);
		}
	}
	std::cout << warehouseScore(map);
}