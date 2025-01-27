#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day5.h"
#include <cstdio>
#include<vector>
#include<string>
#include<unordered_set>
#include<unordered_map>

#pragma once
struct lineData {
	bool lineCorrect{ false };
	int posToChange{ -1 };
	int suggestedChange{ -1 };
};

std::unordered_map<int, std::unordered_set<int>> orderMap(std::string fileURL) {
	std::unordered_map<int, std::unordered_set<int>> map;
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	bool onFirstDigit{ true };
	std::string tempNumStr{};
	int savedNum = 0;
	int c = std::fgetc(dataFile);
	while (!reachedEnd) {
		

		if (c == EOF) {
			reachedEnd = true;
		}
		else if (c == 10) {
			savedNum = 0;
		}
		else if (c != '|') {
			int d = std::fgetc(dataFile);
			tempNumStr = char(c);
			tempNumStr += char(d);
			if (savedNum == 0){
				savedNum = std::stoi(tempNumStr);
			}
			else {

				if (map.count(savedNum)) {
					map[savedNum].insert(std::stoi(tempNumStr));
				}
				else {
					std::unordered_set<int> tempVec;
					tempVec.insert(std::stoi(tempNumStr));
					std::pair<int, std::unordered_set<int>> tempPair;
					tempPair.first = savedNum;
					tempPair.second = tempVec;
					//std::cout << "\n" << tempPair.first;
					map.insert(tempPair);
				
				}
			}
			
		}


	}
	
	std::fclose(dataFile);
	return map;
} 

std::vector <std::vector<int>> genLineVecs(std::string fileURL) {
	std::vector<std::vector<int>> lineVecs;
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	std::vector<int> curr;
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);

		if (c == EOF) {
			lineVecs.push_back(curr);
			reachedEnd = true;
		}
		else if (c == 10) {
			lineVecs.push_back(curr);
			curr.clear();
		}
		else if (c != ',') {
			int d = std::fgetc(dataFile);
			std::string tempNumStr;
			tempNumStr = char(c);
			tempNumStr += char(d);
			curr.push_back(std::stoi(tempNumStr));
		}
		
	}
	

	std::fclose(dataFile);
	return lineVecs;
}

 lineData getLineData(std::unordered_map<int, std::unordered_set<int>>& orderReqs,std::vector<int> line) {
	 bool validLine{ true };
	 int wrongPos{ -1 };
	 int suggestedPos{ -1 };
	 for (int i = 0; i < line.size(); i++) {

		 for (int j = i - 1; j >= 0; j--) {

			 if (orderReqs[line[i]].count(line[j])) {
				 validLine = false;
				 wrongPos = i;
				 suggestedPos = j;
			 }

		 }
	 }
	 lineData newLineData;
	 newLineData.lineCorrect = validLine; newLineData.posToChange = wrongPos; newLineData.suggestedChange = suggestedPos;
	 return newLineData;
	 
	

}


void printQueue(std::string fileURL, std::string dataFileURL) {
	std::unordered_map<int, std::unordered_set<int>> orderReqs = orderMap(dataFileURL);
	std::vector <std::vector<int>> lineVecs = genLineVecs(fileURL);
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	int total{ 0 };
	for (std::vector<int> line : lineVecs) {
		lineData lineDetails = getLineData(orderReqs, line);
		std::cout << "\n " << lineDetails.posToChange << " " << lineDetails.suggestedChange;
		
		if (lineDetails.lineCorrect) {
			int midPoint = int((line.size() + 1) / 2);
			total += line[midPoint - 1];
		}

	}
	std::cout << total;

}

void printQueuePartTwo(std::string fileURL, std::string dataFileURL) {
	std::unordered_map<int, std::unordered_set<int>> orderReqs = orderMap(dataFileURL);
	std::vector <std::vector<int>> lineVecs = genLineVecs(fileURL);
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	int total{ 0 };
	for (std::vector<int> line : lineVecs) {
		lineData lineDetails = getLineData(orderReqs, line);

		if (!lineDetails.lineCorrect) {
			bool lineCorrect{ false };
			while (!lineCorrect) {
				int posToChange = lineDetails.posToChange; int suggestedPos = lineDetails.suggestedChange;
				int takenOut = line[posToChange];
				line.erase(line.begin() + posToChange);
				line.insert(line.begin() + suggestedPos, takenOut);
				lineDetails = getLineData(orderReqs, line);
				lineCorrect = lineDetails.lineCorrect;
			}
			int midPoint = int((line.size() + 1) / 2);
			total += line[midPoint - 1];

		}

	}
	std::cout << total;

}