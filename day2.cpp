
#define _CRT_SECURE_NO_DEPRECATE
#include "day2.h"
#include <iostream>
#include <cstdio>
#include<vector>
#include<string>
#include<cmath>
#pragma once

void printVec(std::vector<int> vec) {
	for (int i : vec) {
		std::cout << i << " ";
	}
}


bool checkValid(std::vector<int> report) {
	int prev{ -1 };
	int prevPrev{ -1 };
	bool asc{ true };

	//std::cout << "\n";
	for (size_t k = 0; k < report.size(); k++) {
		int i = report[k]; 
 
		if (prev != -1) {
			if (prev == i or abs(i - prev) > 3) {
				//std::cout << " returned false";
				return false;
			}
		}
		if (prevPrev != -1) {
			if ((i > prev and prev < prevPrev) or ((i < prev) and (prev > prevPrev))) {
				//std::cout << " returned false";
				return false;
			}
		}
		prevPrev = prev;
		prev = i;

	}
	//std::cout << " returned true";
	return true;
}

bool checkValidTwo(std::vector<int> report) {
	int prev{ -1 };
	int prevPrev{ -1 };
	bool errorFound = false;

	for (size_t k = 0; k < report.size(); k++) {
		int i = report[k]; 


		if (prev != -1) {
			if (prev == i or abs(i - prev) > 3) {
				errorFound = true;
			}
		}
		if (prevPrev != -1) {
			if ((i > prev and prev < prevPrev) or ((i < prev) and (prev > prevPrev))) {
				errorFound = true;
			}
		}
		if (errorFound) {
		
				std::vector<int> reportCopy1 = report;
				std::vector<int> reportCopy2 = report;
				reportCopy1.erase(reportCopy1.begin() + k - 1);
				report.erase(report.begin() + k);

				if (k > 1) {
					reportCopy2.erase(reportCopy2.begin() + k - 2);
					if (checkValid(reportCopy2)) {
						return true;
					}
				}


				
			
				if (checkValid(reportCopy1) or checkValid(report)) {
					return true;
				}
				/*std::cout << " skipping1 " << i <<  "\n";
				skipNumber = true;
				problemFound = true;*/
			
			return false;
		}
		
			prevPrev = prev;
			prev = i;
		
		

	}
	return true;
}





int redNosedReports(std::string fileUrl, int solType) {
	FILE* dataFile = std::fopen(fileUrl.c_str(), "r+");
	bool reachedEnd{ false };
	std::vector<int> report;
	int total{ 0 };
	std::string curr{ "" };
	while (not reachedEnd) {
	
		int num = std::fgetc(dataFile);
		//std::cout << num << " ";
		 if (num == 10) {
			 if (!curr.empty()) {
				 //std::cout << curr << " ";
				 report.push_back(std::stoi(curr));
				 curr.clear();

			 }
			 if (solType == 1 and checkValid(report)) {
				 total += 1;
			 }
			 else if (solType == 2 and checkValidTwo(report) ) {
				 total += 1;
			 }
			 

			 report.clear();
		}

		 else if (num == EOF) {
			 report.push_back(std::stoi(curr));
			 if (solType == 1 and checkValid(report)) {
				 total += 1;
			 }
			 else if (solType == 2 and checkValidTwo(report)) {
				 total += 1;
			 }

			
			reachedEnd = true;
		}

		 else if (num == 32) {
			 if (!curr.empty()) {
				 //std::cout << curr << " ";
				 report.push_back(std::stoi(curr));
				 curr.clear();

			 }
		 }
		 else {
			 curr += char(num);
		 }

	}
	std::fclose(dataFile);
	std::cout <<" "<< total;
	return total;
}