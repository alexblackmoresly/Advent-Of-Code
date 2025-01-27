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

bool checkTowelPattern(std::string towel, std::unordered_set<std::string>& fullTowels) {
	if (towel.size() == 0) {
		return true;
	}
	std::string lookingAt = "";	
	while (towel.size() > 0) {
		lookingAt += towel[0];
		towel.erase(towel.begin());
		if (fullTowels.count(lookingAt)) {
			if (checkTowelPattern(towel, fullTowels)) {
				return true;
			}
		}


	}
	return false;
}

void linenLayout(std::string fileURL) {
	std::string inputString = fileToString(fileURL);
	std::unordered_set<std::string> fullTowels, partTowels;
	std::regex towelPattern(R"(([a-z]+),)");
	std::vector<std::string> usuableTowelVec = regexRepeatedData(inputString, towelPattern);
	std::regex removeTowels(R"(([a-z]+,))");
	std::smatch match;
	auto start = inputString.cbegin();
	while (std::regex_search(start, inputString.cend(), match, removeTowels)) {
		start = match.suffix().first;
	}
	std::string neededTowels(match.suffix().first, match.suffix().second);
	std::regex needPattern(R"(([a-z]+))");
	std::vector<std::string> neededTowelVec = regexRepeatedData(neededTowels, needPattern);
	for (std::string towel : usuableTowelVec) {
		fullTowels.insert(towel);
		for (int i = 1; i <= towel.size(); i++) {
			partTowels.insert(towel.substr(0, i));
		}
	}
	int total{ 0 };
	for (std::string towel : usuableTowelVec) {							//this prunes our list of usuable towels to include only ones which are not composites to improve speed of checking all the needed towels
		std::unordered_set<std::string> fullTowelsCopy = fullTowels;
		fullTowelsCopy.erase(towel);
		if (checkTowelPattern(towel, fullTowelsCopy)) {
			fullTowels.erase(towel);
		}
	}


	for (std::string towel : neededTowelVec) {
		if (checkTowelPattern(towel, fullTowels)) {
			total++;
		}

	}


	std::cout << total;
}




void linenLayoutPartTwo(std::string fileURL) {
	std::string inputString = fileToString(fileURL);
	std::unordered_set<std::string> fullTowels, partTowels;
	std::regex towelPattern(R"(([a-z]+),)");
	std::vector<std::string> usuableTowelVec = regexRepeatedData(inputString, towelPattern);
	std::regex removeTowels(R"(([a-z]+,))");
	std::smatch match;
	auto start = inputString.cbegin();
	while (std::regex_search(start, inputString.cend(), match, removeTowels)) {
		start = match.suffix().first;
	}
	std::string neededTowels(match.suffix().first, match.suffix().second);
	std::regex needPattern(R"(([a-z]+))");
	std::vector<std::string> neededTowelVec = regexRepeatedData(neededTowels, needPattern);
	std::unordered_map<std::string, long long> currentLayer;
	std::unordered_map<std::string, long long> nextLayer;

	currentLayer.insert({ "" ,1 });
	std::unordered_set<std::string> partials, fulls;
	for (std::string towel : neededTowelVec) {
		for (int i = 1; i < towel.size(); i++) {
			partials.insert(towel.substr(0, i));
		}
		fulls.insert(towel);
	}
	long long total{ 0 };
	while (!currentLayer.empty()) {
		for (std::pair<std::string, long long> baseTowelPair : currentLayer) {
			std::string baseTowel = baseTowelPair.first; 
			long long towelCount = baseTowelPair.second;
			for (std::string addingTowel : usuableTowelVec) {
				std::string newTowel = baseTowel + addingTowel;
				if (fulls.count(newTowel)) {
					total += towelCount;
				}
				if (partials.count(newTowel)) {
					nextLayer[newTowel] += towelCount;
				}
			}
		}
		currentLayer = nextLayer;
		nextLayer.clear();

	}
	std::cout << total;


}
