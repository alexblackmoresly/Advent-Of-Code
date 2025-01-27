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


void raceCondition(std::string fileURL, int cheatSize) {
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	int y{ 0 }, x{ 0 };
	std::vector<int> start, end;
	std::vector<std::vector<int>> raceMatrix{ std::vector<int> {} };
	std::unordered_map<std::vector<int>, int, vec_hash> visited;
	std::unordered_set<std::vector<int>, vec_hash> unvisited, checked;
	while (!reachedEnd) {
		//std::cout << "a";
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			if (c != 10) {
				raceMatrix[y].push_back(c);
				if (c == '.') {
					unvisited.insert({ x,y });
				}
				if (c == 'S') {
					start = { x,y };
				}
				if (c == 'E') {
					end = { x,y };
					unvisited.insert({ x,y });
				}
				x++;
			}
			else {
				raceMatrix.push_back(std::vector<int> {});
				y++;
				x = 0;
			}
		}
	}
	std::fclose(dataFile);
	reachedEnd = false;
	std::vector<int> curr = start;
	std::vector<int> next;
	std::vector<std::pair<int, int>> oneMove{ {0,1},{1,0},{-1,0},{0 ,-1} };
	std::vector<std::pair<int, int>> twoMoves{ {1,1},{1,-1},{-1,1},{-1 ,-1},{0,2},{2,0},{-2,0},{0 ,-2} };
	std::vector<std::vector<int>> path;
	int prevDist = -1;
	int total{ 0 };

	while (!reachedEnd) {
		path.push_back(curr);
		int currX = curr[0];
		int currY = curr[1];

		int newDist = prevDist + 1;
		//std::cout << currX << currY << newDist << "\n";

		prevDist = newDist;
		unvisited.erase(curr);
		visited.insert({ curr, newDist });
		for (std::pair<int, int> move : oneMove) {
			if (unvisited.count({ currX + move.first, currY + move.second })) {
				next = { currX + move.first, currY + move.second };
				break;
			}
		}
		if (curr == end){
			reachedEnd = true;
		}
		else {
			curr = next;
		}
	}
	reachedEnd = false;
	int currDist{ 0 };
	int targetSaved{ 100 };
	
	for (std::vector<int> curr : path) {
		int currDist = visited[{curr[0], curr[1]}];
		int currX = curr[0]; int currY = curr[1];
		visited.erase(curr);

		for (int i = -cheatSize; i <= cheatSize; i ++) {
			for (int j = 0; j + std::abs(i) <= cheatSize; j++) {
				int newX = currX + i;
				int newY = currY + j;
				if (visited.count({ newX, newY })) {
					int lookingAtDist = visited[{newX, newY}];
					int distBetween = std::abs(i) + j;
					if (lookingAtDist < currDist) {
						std::swap(lookingAtDist, currDist);
					}
					int savedDist = lookingAtDist - (currDist + distBetween);
					if (savedDist >= targetSaved) {
						total += 1;
					}
				}
			}
			for (int j = -1; -j + std::abs(i) <= cheatSize; j--) {
				int newX = currX + i;
				int newY = currY + j;
				if (visited.count({ newX, newY })) {
					int lookingAtDist = visited[{newX, newY}];
					int distBetween = std::abs(i) - j;
					if (lookingAtDist < currDist) {
						std::swap(lookingAtDist, currDist);
					}
					int savedDist = lookingAtDist - (currDist + distBetween);
					if (savedDist >= targetSaved) {
						total += 1;
					}
				}
			}
		}

		currDist++;
	}
	std::cout << total;


}