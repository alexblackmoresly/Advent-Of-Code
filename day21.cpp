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
#include <algorithm>
#pragma once

bool validKeyPadMoves(std::unordered_map<char, std::pair<int, int>>& keyPadMap, char startChar, std::string path) {
	std::pair<int, int> start = keyPadMap[startChar];
	std::pair<int, int> invalid = keyPadMap['-'];
	for (char c : path) {
		if (c == '^') {
			start.second++;
		}
		else if (c == '>') {
			start.first++;
		}
		else if (c == '<') {
			start.first--;
		}
		else if (c == 'v') {
			start.second--;
		}
		if (start == invalid) {
			return false;
		}
	}
	return true;	
}

std::string buttonToButtonCode(char start, char end, std::unordered_map<char, std::pair<int, int>>& keyPadMap) {
	std::string newCode{ "" };
	std::pair<int, int> startCoords = keyPadMap[start];
	std::pair<int, int> endCoords = keyPadMap[end];
	std::pair<int, int> move {endCoords.first - startCoords.first, endCoords.second - startCoords.second};
	std::string newMove{ "" };
	newMove = stringMult("<", -move.first) + stringMult("v", -move.second) + stringMult("^", move.second) + stringMult(">", move.first);
	if (!validKeyPadMoves(keyPadMap, start, newMove)) {
		std::reverse(newMove.begin(), newMove.end());
	}
	return newMove + "A";
}
//501512 3
//1240556 4
//3059222 5
//7609244 6



long long moveAtDepth(std::unordered_map<std::string, std::unordered_map<int, long long>>& sequenceDict, std::string sequence, int depth, std::unordered_map<std::string, std::string>& graph) {
	if (depth == 0) {
		return sequence.length();
	}

	if (sequenceDict[sequence].count(depth)) {
		return sequenceDict[sequence][depth];
	}
	else {
		long long res = 0;
		std::string prev = "A";
		for (char next : sequence) {
			res += moveAtDepth(sequenceDict, graph[prev + next], depth - 1, graph);
			prev = next;
		}
		sequenceDict[sequence][depth] = res;
		return res;
	}

}

void keyboardConundrum(std::string fileURL, int depth) {
	std::string fileString = fileToString(fileURL);
	std::regex pattern(R"((\d+A))");
	std::vector<std::string> initialCodes, codes = regexRepeatedData(fileString, pattern);
	initialCodes = codes;

	std::unordered_map<char, std::pair<int, int>> keyPadMap, movePadMap;
	keyPadMap['7'] = { 0,3 }; keyPadMap['8'] = { 1,3 };  keyPadMap['9'] = { 2,3 };
	keyPadMap['4'] = { 0,2 }; keyPadMap['5'] = { 1,2 }; keyPadMap['6'] = { 2,2 };
	keyPadMap['1'] = { 0,1 }; keyPadMap['2'] = { 1,1 }; keyPadMap['3'] = { 2,1 };
	keyPadMap['0'] = { 1,0 }; keyPadMap['A'] = { 2,0 };	keyPadMap['-'] = { 0,0 };

	movePadMap['<'] = { 0,0 }; 	movePadMap['v'] = { 1,0 }; 	movePadMap['>'] = { 2,0 }; 	
	movePadMap['^'] = { 1,1 }; 	movePadMap['A'] = { 2,1 };	movePadMap['-'] = { 0,1 };

	std::vector<char> allMoveKeys{'^', '<', 'v', '>', 'A'};
	 
	std::unordered_map<std::string, std::string> graph;
	std::unordered_map<std::string, std::unordered_map<int, long long>> sequenceDict;
	for (char key1 : allMoveKeys) {
		for (char key2 : allMoveKeys) {
			std::string sequence = "";
			sequence += key1; sequence += key2;
			graph[sequence] = buttonToButtonCode(key1, key2, movePadMap);
		}
	}
		


	
	long long total{ 0 };
	for (std::string code : codes) {
		int codeVal = std::stoi(std::string{ code.begin(), code.begin() + 3 });
		code = "A" + code;
		std::unordered_map<std::pair<int, int>, int, pair_hash> fin;
		std::string newCode{ "" };
		for (int i = 0; i < code.length() - 1; i++) {
			newCode += buttonToButtonCode(code[i], code[i + 1], keyPadMap);
		}
		long long codeLen = moveAtDepth(sequenceDict, newCode, depth, graph);

		total += codeLen * codeVal;
	}
	std::cout << total << "\n";

	
}
