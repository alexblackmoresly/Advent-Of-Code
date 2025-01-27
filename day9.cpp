#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day9.h"
#include <cstdio>
#include<vector>
#include<string>
#include<unordered_map>
#include <chrono>

void diskFragmenter(std::string fileURL) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	int fileLength{ 0 };
	std::string data{ "" };
	long long total{ 0 };
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			data += c;
			fileLength++;
		}
	}
	int leftPtr{ 0 }, rightPtr{ fileLength - 1 };
	int leftCount{ 0 };
	int rightCount = (int(fileLength + 1) / 2) - 1;
	int rData{ data[rightPtr] - 48 };
	int lSpace{ 0 };
	int count{ 0 };
	//std::cout << rightCount << std::endl;
	std::vector<int> finFile;
	while (leftPtr != rightPtr) {
		//std::cout << "leftPtr " << leftPtr << " rightPtr " << rightPtr << std::endl;
		if (lSpace == 0) {

			for (int i = 0; i < data[leftPtr] - 48; i++) {
				finFile.push_back(leftCount);
				total += count * leftCount;
				count += 1;
			}
			leftCount++;
			leftPtr++;
			lSpace = data[leftPtr] - 48 ;
			
			leftPtr++;
		}
		if (rData == 0) {
			rightPtr -= 2;
			rightCount -= 1;

			rData = data[rightPtr] - 48;
		}
		if (lSpace > 0) {  //there are some 0 length gaps that we need to make sure we are aware of
			finFile.push_back(rightCount);
			total += count * rightCount;
			count++;
			lSpace--;
			rData--;
		}

	}
	for (int j= 0; j < rData; j++) {
		total += count * rightCount;
		count++;
	}
	std::cout << "\n" << lSpace << " " << rData << "\n";
	std::fclose(dataFile);
	/*for (int i : finFile) {
		std::cout << i << " ";
	}*/

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
	std::cout << total;

}
void gapMapUpdate(std::unordered_map<int, std::vector<int>>& gapMap, int gapSize, int gapPos) {
	if (!gapMap.count(gapSize)) {
		gapMap[gapSize].push_back(gapPos);
		return;
	}
	bool inserted{ false };
	int i = 0;
	while (true) {
		if (gapPos < gapMap[gapSize][i]) {
			gapMap[gapSize].insert(gapMap[gapSize].begin() + i, gapPos);
			return;
		}
		i++;
		if (i == gapMap[gapSize].size()) {
			gapMap[gapSize].push_back(gapPos);
			return;
		}


	}
}

void diskDefragmenter(std::string fileURL) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	int fileLength{ 0 };
	std::string data{ "" };
	long long total{ 0 };
	std::unordered_map<int, std::vector<int>> gapMap;
	std::vector<std::pair<int, int>> gapVec;
	int count{ 0 };
	int totalSize{ 0 };
	int maxGap{ 0 };
	
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			if (count % 2) {
				if (c - 48) {
					gapMap[c - 48].push_back(totalSize);
					gapVec.push_back(std::pair<int, int>{ c - 48, totalSize });
					if (c - 48 > maxGap) {
						maxGap = c - 48;
					}
				}
			}
			data += c;
			totalSize += c - 48;
			fileLength++;
		}
		count++;

	}
	int maxPosGap = maxGap;



	int rPtr{ fileLength - 1 };
	int rightCount = (int(fileLength + 1) / 2) - 1;
	int rightDataCount = totalSize;


	count = 0;

	while (rPtr > 0) {
		int dataLen = data[rPtr] - 48;
		int i = 0;
		rightDataCount -= dataLen;
		bool foundGap{ false };


		while (i < gapVec.size() and !foundGap) {
			if (gapVec[i].first >= dataLen and gapVec[i].second < rightDataCount) {
				foundGap = true;
				for (int j = 0; j < dataLen; j++) {
					total += rightCount * (gapVec[i].second + j);
				}
				if (gapVec[i].first > dataLen) {
					gapVec[i].first -= dataLen;
					gapVec[i].second += dataLen;
				}
				else {
					gapVec.erase(gapVec.begin() + i);
				}

			}
			if (gapVec[i].second > rightDataCount or dataLen > maxPosGap){
				break;
			}
			i++;
		}
		if (!foundGap) {
			maxPosGap = dataLen - 1;
			for (int k = 0; k < dataLen; k++) {
				total += rightCount * (rightDataCount + k);
			}
		}
		rPtr--;
		rightDataCount -= data[rPtr] - 48;
		rPtr--;
		rightCount--;

	}


	std::vector<int> finFile;
	
	std::fclose(dataFile);

	std::cout <<std::endl<< total << std::endl;

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

}


//faster: set  up a vector of 9 vectors of ints which records the positions of the size of gaps in the gapVec
//then when we look for a gap for our data to move back we can just look at which of the vectors of positions of gaps has the lowest index is gapVec then we know that we 
//need to put the data there, and we can remove the top from that stack. limits us to at most looking at 9 positions each time we want to 

void diskDefragmenter2(std::string fileURL) { //WAY QUICKER
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	int fileLength{ 0 };
	std::string data{ "" };
	long long total{ 0 };
	int count{ 0 };
	int totalSize{ 0 };
	int maxGap{ 0 };
	std::vector<std::vector<int>> posStacks;
	for (int i = 0; i < 9; i++) {
		posStacks.push_back(std::vector<int> {});
	}
	//std::cout << posStacks.size();

	while (!reachedEnd) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			if (count % 2) {
				if (c - 48) {
					posStacks[c - 49].push_back(totalSize);
					if (c - 48 > maxGap) {
						maxGap = c - 48;
					}
				}
			}
			data += c;
			totalSize += c - 48;
			fileLength++;
		}
		count++;

	}
	int maxPosGap = maxGap;



	int rPtr{ fileLength - 1 };
	int rightCount = (int(fileLength + 1) / 2) - 1;
	int rightDataCount = totalSize;


	count = 0;
	for (std::vector<int>& vec : posStacks) {
		vec.push_back(-1);
	}

	while (rPtr > 0) {
	

		int dataLen = data[rPtr] - 48;
		int i = 0;
		rightDataCount -= dataLen;
		int leftMostGapPos{-1};
		int leftMostGapVal{ 0 };



	
		for (i = dataLen - 1; i < 9; i++) {
			if (posStacks[i][0] != -1) {
				if (posStacks[i][0] < rightDataCount) {
					if (leftMostGapPos == -1 or posStacks[i][0] < leftMostGapPos) {
						leftMostGapPos = posStacks[i][0];
						leftMostGapVal = i;
					}
				}
				else {
					posStacks[i] = std::vector<int>{ -1 };
				}
			}
			
		}
		if (leftMostGapPos != -1) {
			for (int k = 0; k < dataLen; k++) {
				total += rightCount * (leftMostGapPos + k);
			}
			if (leftMostGapVal >= dataLen) {
				int newGapSize = leftMostGapVal - dataLen;
				int newGapPos = leftMostGapPos + dataLen;
				if (newGapSize >= dataLen) {
					posStacks[newGapSize].insert(posStacks[newGapSize].begin(), newGapPos);
				}
				else {
					for (int k = 0; k < posStacks[newGapSize].size(); k++) {
						if (posStacks[newGapSize][k] == -1 or posStacks[leftMostGapVal][0] < posStacks[newGapSize][k]) {
							posStacks[newGapSize].insert(posStacks[newGapSize].begin() + k, newGapPos);
							break;

						}
					}
				}
			}
			posStacks[leftMostGapVal].erase(posStacks[leftMostGapVal].begin());
		}


		if (leftMostGapPos == -1) {
			maxPosGap = dataLen - 1;
			for (int k = 0; k < dataLen; k++) {
				total += rightCount * (rightDataCount + k);
			}
		}
		rPtr--;
		rightDataCount -= data[rPtr] - 48;
		rPtr--;
		rightCount--;

	}
	
;

	


	std::vector<int> finFile;

	std::fclose(dataFile);

	std::cout << std::endl << total << std::endl;

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

}