#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day1.h"
#include <cstdio>
#include<vector>
#include<string>
#include<algorithm>
#include<cmath>
#include<unordered_map>
#pragma once

int historianHysteria(std::string fileUrl) {
	FILE* dataFile = std::fopen(fileUrl.c_str(), "r+");
	bool reachedEnd{ false };
	bool addFirst{ true };
	std::vector<int> unsortedDataFirst;
	std::vector<int> unsortedDataSecond;
	std::string unaddedString{ "" };
	while (not reachedEnd){
		int num = std::fgetc(dataFile);
		if (num == EOF) {
			if (!unaddedString.empty()) {
				if (addFirst) {
					unsortedDataFirst.push_back(std::stoi(unaddedString));
				}
				else {
					unsortedDataSecond.push_back(std::stoi(unaddedString));
				}
			}
			reachedEnd = true;
		}
		else if (!std::isspace(num)) {												
			unaddedString += static_cast<char> (num);
		}
		else {
			if (!unaddedString.empty()) {

				if (addFirst) {
					unsortedDataFirst.push_back(std::stoi(unaddedString));
					addFirst = false;
				}
				else {
					unsortedDataSecond.push_back(std::stoi(unaddedString));
					addFirst = true;
				}

				unaddedString.erase();
			}
		}

	 
	}



	
	std::fclose(dataFile);
	
	std::sort(unsortedDataFirst.begin(), unsortedDataFirst.end());
	std::sort(unsortedDataSecond.begin(), unsortedDataSecond.end());
	int dist{ 0 };
	std::size_t dataSize = unsortedDataFirst.size();
	for (int i = 0; i < static_cast<int>(dataSize); i++) {
		
		dist += std::abs(unsortedDataFirst[i] - unsortedDataSecond[i]);
	}
	std::cout << "\n" << dist;

	return 0;
}


int historianHysteriaPartTwo(std::string fileUrl) {
	FILE* dataFile = std::fopen(fileUrl.c_str(), "r+");
	bool reachedEnd{ false };
	bool addFirst{ true };
	std::size_t pos{};
	std::unordered_map<int, int> unsortedDataFirst;
	std::unordered_map<int, int> unsortedDataSecond;
	std::string unaddedString{ "" };
	while (not reachedEnd) {
		int num = std::fgetc(dataFile);
		if (num == EOF) {
			if (!unaddedString.empty()) {
				if (addFirst) {
					unsortedDataFirst[std::stoi(unaddedString)] +=1;
				}
				else {
					unsortedDataSecond[std::stoi(unaddedString)] += 1;
				}
			}
			reachedEnd = true;
		}
		else if (!std::isspace(num)) {												//9 is ascii for white space, for some reason " " breaks it..
			unaddedString += static_cast<char> (num);
		}
		else {
			if (!unaddedString.empty()) {

				if (addFirst) {
					unsortedDataFirst[std::stoi(unaddedString)] += 1;
					addFirst = false;
				}
				else {
					unsortedDataSecond[std::stoi(unaddedString)] += 1;
					addFirst = true;
				}

				unaddedString.erase();
			}
		}


	}
	std::fclose(dataFile);
	int simScore = 0;
	int key;

	for (std::pair<int, int> entry : unsortedDataFirst) {
		key = entry.first;

		if ( !(unsortedDataSecond.find(key) == unsortedDataSecond.end())) {
			//std::cout << "\n" <<entry.first << " " << entry.second << " " << unsortedDataSecond[key];
			simScore += key * entry.second * unsortedDataSecond[key];
		}

	}
	std::cout << " " << simScore ;
	return simScore;



	
}