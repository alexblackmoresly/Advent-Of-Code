#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include<string>
#include <chrono>
#include <vector>
#include "usefulFuncs.h"
#include <regex>
#pragma once



std::vector<std::string> regexRepeatedData(std::string input, std::regex pattern) {
	std::smatch match;
	auto start = input.cbegin();
	std::vector<std::string> outputVec;
	while (std::regex_search(start, input.cend(), match, pattern)) {
		for (int i = 1; i < match.size(); i++) {
			outputVec.push_back(match[i]);
		}
		start = match.suffix().first;
	}
	return outputVec;
}

std::string stringMult(std::string s, int mult) {
	std::string ret = "";
	for (int i = 0; i < mult; i++) {
		ret += s;
	}
	return ret;
}

std::vector <std::string> permutations(std::string base) {
	std::string s = base;
	std::vector<std::string> perms;
	for (long long i = 0;i < std::tgamma(base.size() + 1); i++) {
		s = base;
		int k = i;
		for (int j = 1; j < s.size(); ++j)
		{
			std::swap(s[k % (j + 1)], s[j]);
			k = k / (j + 1);
		}
		perms.push_back(s);
	}
	return perms;
}


void allCombinedStrings(std::vector<std::vector<std::string>>& possibilities, std::vector<std::string>& result, std::string curr, int selectingFrom, int counter) {
	//return a vector of all possible combined strings ie from {{"a", "b}, {"c", "d"}} we need {"ac", "ad", "ac", "ad"}}
	if (selectingFrom == possibilities.size()) {
		result.push_back(curr);
		return;
	}
	allCombinedStrings(possibilities, result, curr + possibilities[selectingFrom][counter], selectingFrom + 1, 0);
	if (counter + 1 < possibilities[selectingFrom].size()) {
		allCombinedStrings(possibilities, result, curr, selectingFrom, counter + 1);
	}

}

int euclidAlgor(int a, int b) {
	if (b < a) {  //swapping to make sure a >= b
		int c = a;
		a = b;
		b = c;
	}
	int r = a % b;
	while (r != 0) {
		a = b;
		b = r;
		r = a % b;
	}
	return b;
}

std::pair<int, std::pair<int, int>> extendedEuclidAlgor(int a, int b) { 
	bool swapped{ false };
	if (b < a) {  //swapping to make sure a >= b
		int c = a;
		a = b;
		b = c;
		swapped = true;
	}
	int s1{ 1 }, s2{ 0 }, t1{ 0 }, t2{ 1 }, q{ 0 };
	int r = a % b;
	q = int(a / b);
	while (r != 0) {
		a = b;
		b = r;
		r = a % b;
		q = int(a / b);
		int s = s1 - (q * s2);
		int t = t1 - (q * t2);
		t1 = t2;
		s1 = s2;
		t2 = t;
		s2 = s;
	}
	if (swapped) {
		return std::pair<int, std::pair<int, int>> {b, std::pair<int, int> {s1, t1}};
	}
	return std::pair<int, std::pair<int, int>> {b, std::pair<int, int> {t1, s1}};

}

std::string intToString( int i ){
	if (i == 0) {
		return "0";
	}

	std::string returnString{""};
	while (i != 0) {
		int k = i % 10;
		returnString.insert(returnString.begin(), char(k) + 48);
		i = int(i / 10);
	}
	return returnString;
}

std::string llongToString(long long i) {
	if (i == 0) {
		return "0";
	}

	std::string returnString{ "" };
	while (i != 0) {
		int k = i % 10;
		returnString.insert(returnString.begin(), char(k) + 48);
		i = long long(i / 10);
	}
	return returnString;
}


#include <cstdio>

std::vector<std::vector<int>> makeMatrix(std::string fileURL) {
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	int y{ 0 };
	std::vector<std::vector<int>> returnMat{ std::vector<int> {} };
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			if (c != 10) {
				returnMat[y].push_back(c);
			}
			else {
				returnMat.push_back(std::vector<int> {});
				y++;
			}
		}
	}
	std::fclose(dataFile);
	return returnMat;
}

std::string fileToString(std::string fileURL) {
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	std::string output{ "" };
	while (true) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			break;
		}
		output += c;
	}
	return output;
}