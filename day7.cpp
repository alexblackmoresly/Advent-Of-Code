#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day7.h"
#include <cstdio>
#include<vector>
#include<string>
#include<unordered_set>
#include<unordered_map>
#include <fstream>
#include <chrono>
#include<cmath>


std::vector < std::pair<long long, std::vector<long long >> > makeBridgeMat(std::string fileURL) {
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	std::vector<std::pair<long long, std::vector<long long>>> returnVec;


	long long aimedVal;
	bool foundAimedVal{ false };
	std::vector<long long> currVec;
	while (!reachedEnd) {
		//std::cout << 'a';
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			std::string aimedValString;
			while (c != ':') {
				//std::cout << 'b';
				aimedValString += char(c);
				c = std::fgetc(dataFile);
			}
			aimedVal = std::stoll(aimedValString);
			foundAimedVal = true;
			std::fgetc(dataFile); c = std::fgetc(dataFile);
			std::string currValString;
			while (c != 10 and c != EOF) {
				if (c != ' ') {
					currValString += char(c);
					c = std::fgetc(dataFile);
				}
				else {
					currVec.push_back(std::stoi(currValString));
					c = std::fgetc(dataFile);
					currValString.clear();
				}
			}
			currVec.push_back(std::stoi(currValString));
			std::pair<long long, std::vector<long long>> tempPair;
			tempPair.first = aimedVal; tempPair.second = currVec;
			returnVec.push_back(tempPair);
			currVec.clear();



		}

	}
	std::fclose(dataFile);
	for (std::pair<long long, std::vector<long long>> pr : returnVec) {
		if (pr.first < 0) {
			std::cout << "OVERFLOW BE SCARED";
		}
	}
	

	return returnVec;
}

long long concatenate(long long sum, int curr) {
	int count{ 0 };
	int curr2 = curr;
	while (curr2 != 0) {
		curr2 = int(curr2 /10 );
		count++;
	}
	return sum * pow(10, count) + curr;

}

bool dfs(std::pair<long long, std::vector<long long>>& dataPair, long long carried, int iter, bool part) {
		if (iter == dataPair.second.size()) {
			if (dataPair.first == carried) {
				return true;
			}
		}
		else  {
			long long tempVal = carried + dataPair.second[iter];
			if (tempVal <= dataPair.first) {
				if (dfs(dataPair, tempVal, iter + 1, part)) {
					return true;
				}
			}

			
			tempVal = carried * dataPair.second[iter];
			if (tempVal <= dataPair.first) {
				if (dfs(dataPair, tempVal, iter + 1, part)) {
					return true;
				}
			}
			if (part) {
				tempVal = concatenate(carried, dataPair.second[iter]);
					if (tempVal <= dataPair.first) {
						if (dfs(dataPair, tempVal, iter + 1, part)) {
							return true;
						}

					}
				
			}
		}

	return false;
}


void bridgeRepair(std::string fileURL) {
	//***C++11 Style:***


	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	

	
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	long long total1{ 0 };
	long long total2{ 0 };

	long long aimedVal;
	bool foundAimedVal{ false };
	std::vector<long long> currVec;
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			std::string aimedValString;
			while (c != ':') {
				aimedValString += char(c);
				c = std::fgetc(dataFile);
			}
			aimedVal = std::stoll(aimedValString);
			foundAimedVal = true;
			std::fgetc(dataFile); c = std::fgetc(dataFile);
			std::string currValString;
			while (c != 10 and c != EOF) {
				if (c != ' ') {
					currValString += char(c);
					c = std::fgetc(dataFile);
				}
				else {
					currVec.push_back(std::stoi(currValString));
					c = std::fgetc(dataFile);
					currValString.clear();
				}
			}
			currVec.push_back(std::stoi(currValString));
			std::pair<long long, std::vector<long long>> tempPair;
			tempPair.first = aimedVal; tempPair.second = currVec;
			if (dfs(tempPair, currVec[0], 1, false)) {
				total1 += aimedVal;
			}
			if (dfs(tempPair, currVec[0], 1, true)) {
				total2 += aimedVal;
			}
			currVec.clear();



		}

	}
	std::fclose(dataFile);

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[µs]" << std::endl;
	


	

	
	std::cout << total1 << std::endl << total2;

}










