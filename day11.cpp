#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day11.h"
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
#pragma once

void plutonianPebbles(std::string fileURL) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	std::string curr{ "" };
	
	std::deque<std::string> pebblesVec;
	while (!reachedEnd) {
		int c = std::getc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
			pebblesVec.push_back(curr);
		}
		else {
			if (c == ' ') {
				pebblesVec.push_back(curr);
				curr.clear();
			}
			else {
				curr += char(c);
			}
		}

	}
	//for (std::string pebble : pebblesVec) {
	//	std::cout << pebble << " ";
	//}

	for (int i = 0; i < 25; i++) {
		//std::cout << "\n i is " << i << "\n";
		std::deque < std::string> newPebbles;
		/*for (std::string pebble : pebblesVec) {
			std::cout << pebble << " ";
		}
		std::cout << "\n";*/
		int pebbleCount = pebblesVec.size();
		for (int j = 0; j < pebbleCount; j ++) {
			std::string pebble = pebblesVec.front();
			
			//std::cout << ' ' << pebble;
			if (pebble == "0") {
				//pebblesVec[counter] = 1;
				newPebbles.push_back("1");
				
			}
			else if (pebble.length() % 2 == 0) {
				//std::cout << 'a';
				
				std::string newPebble1 = pebble.substr(0, pebble.length() / 2);
				std::string newPebble2 = pebble.substr(pebble.length() / 2);
				if (!newPebble2.empty()) {
					newPebble2 = llongToString(std::stoll(newPebble2));
				}
				/*while (newPebble2[0] == '0') {
					newPebble2.erase(newPebble2.begin());
				}*/
				if (newPebble2.empty()) {
					newPebble2 = "0";
				}
				newPebbles.push_back(newPebble1);
				newPebbles.push_back(newPebble2);

				//pebblesVec[counter] = newPebble1;
				//pebblesVec.insert(pebblesVec.begin() + counter, newPebble2);
			}
			else {
				//pebblesVec[counter] = llongToString(std::stoll(pebble) * 2024);
				newPebbles.push_back(llongToString(std::stoll(pebble) * 2024));
			}
			
			pebblesVec.pop_front();
		}
		pebblesVec = newPebbles;
	}
	std::cout << pebblesVec.size() << "\n";
	/*for (std::string pebble : pebblesVec) {
		std::cout << pebble << " ";
	}*/
	//std::cout << "\n";
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	
}


void inLayerN(int stackLayer, std::array<std::unordered_map<long long ,Stack>, 75>& layerMap, long long numToPutIntoStack) {
	if (stackLayer == 0) {
		Stack newStack;
		newStack.push(numToPutIntoStack);
		layerMap[0][numToPutIntoStack].push(numToPutIntoStack);
		return;
	}
	else {
		if (!layerMap[stackLayer - 1].count(numToPutIntoStack)) {
			inLayerN(stackLayer - 1, layerMap, numToPutIntoStack);
		}
		Stack prevLayerStack = layerMap[stackLayer - 1][numToPutIntoStack];
		long long stackL = prevLayerStack.size;
		Stack newLayerStack;
		for (int i = 0; i < stackL; i++) {
			long long pebble = prevLayerStack.peek();
			prevLayerStack.pop();
			if (pebble == 0) {
				newLayerStack.push(1);
			}
			else {
				std::string pebbleString = llongToString(pebble);
					if (pebbleString.length() % 2 == 0) {

						std::string newPebble1 = pebbleString.substr(0, pebbleString.length() / 2);
						std::string newPebble2 = pebbleString.substr(pebbleString.length() / 2);
						if (!newPebble2.empty()) {
							newPebble2 = llongToString(std::stoll(newPebble2));
						}

						if (newPebble2.empty()) {
							newPebble2 = "0";
						}
						newLayerStack.push(std::stoll(newPebble1));
						newLayerStack.push(std::stoll(newPebble2));
					}
					else {
						newLayerStack.push(pebble * 2024);
					}
			}
		}
		layerMap[stackLayer][numToPutIntoStack] = newLayerStack;
		layerMap[stackLayer - 1].erase(numToPutIntoStack);
	}

}


void inLayerNTemp(int stackLayer, std::array<std::unordered_map<long long, Stack>, 75>& layerMap, long long numToPutIntoStack, std::unordered_map<long long, Stack>& layer1Lookup) {
	if (stackLayer == 0) {
		Stack newStack;
		newStack.push(numToPutIntoStack);
		layerMap[0][numToPutIntoStack].push(numToPutIntoStack);
		return;
	}
	else {
		if (!layerMap[stackLayer - 1].count(numToPutIntoStack)) {
			inLayerNTemp(stackLayer - 1, layerMap, numToPutIntoStack, layer1Lookup);
		}
		Stack prevLayerStack = layerMap[stackLayer - 1][numToPutIntoStack];
		long long stackL = prevLayerStack.size;
		Stack newLayerStack;
		for (int i = 0; i < stackL; i++) {
			long long pebble = prevLayerStack.peek();
			prevLayerStack.pop();
			if (!layer1Lookup.count(pebble)) {
				Stack newLayer1Stack;
				std::string pebbleString = llongToString(pebble);
				if (pebble == 0) {
					newLayer1Stack.push(1);
				}
				else {
					if (pebbleString.length() % 2 == 0) {

						std::string newPebble1 = pebbleString.substr(0, pebbleString.length() / 2);
						std::string newPebble2 = pebbleString.substr(pebbleString.length() / 2);
						if (!newPebble2.empty()) {
							newPebble2 = llongToString(std::stoll(newPebble2));
						}

						if (newPebble2.empty()) {
							newPebble2 = "0";
						}
						newLayer1Stack.push(std::stoll(newPebble1));
						newLayer1Stack.push(std::stoll(newPebble2));
					}
					else {
						newLayer1Stack.push(pebble * 2024);
					}
				}
				
				layer1Lookup[pebble] = newLayer1Stack;
				//std::cout << pebble << " " <<layer1Lookup[pebble].size << " ";
			
			}

			if (layer1Lookup[pebble].size == 1) {
				newLayerStack.push(layer1Lookup[pebble].peek());
			}
			else {
				long long temp = layer1Lookup[pebble].peek();
				layer1Lookup[pebble].pop();
				newLayerStack.push(temp);
				newLayerStack.push(layer1Lookup[pebble].peek());
				layer1Lookup[pebble].push(temp);
			}
		}
		layerMap[stackLayer][numToPutIntoStack] = newLayerStack;
		layerMap[stackLayer - 1].erase(numToPutIntoStack);
	}

}


void plutonianPebbles2(std::string fileURL) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	std::string curr{ "" };

	std::vector<int> pebbleStack;
	while (!reachedEnd) {
		int c = std::getc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
			pebbleStack.push_back(std::stoi(curr));
		}
		else {
			if (c == ' ') {
				pebbleStack.push_back(std::stoi(curr));
				curr.clear();
			}
			else {
				curr += char(c);
			}
		}

	}
	std::array<std::unordered_map<long long, Stack>, 75> layerMap;
	int total{ 0 };
	int howDeep = 75;

	std::unordered_map<long long, Stack> layer1Lookup;
	layer1Lookup[0].push(1);
	for (long long pebble : pebbleStack) {
		inLayerNTemp(howDeep, layerMap, pebble, layer1Lookup);
		//inLayerN(howDeep, layerMap, pebble);
		total += layerMap[howDeep][pebble].size;
	}

	std::cout << "\n" << total << "\n";
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}


void inLayerN3(int stackLayer, std::array<std::unordered_map<long long, std::unordered_map<long long, long long>>,
		2000>& layerMap, long long numToPutIntoStack, std::unordered_map<long long, Stack>& layer1Lookup) {
	if (stackLayer == 0) {
		layerMap[0][numToPutIntoStack][numToPutIntoStack] = 1;
		return;
	}
	else {
		if (!layerMap[stackLayer - 1].count(numToPutIntoStack)) {
			inLayerN3(stackLayer - 1, layerMap, numToPutIntoStack, layer1Lookup);
		}
		std::unordered_map<long long, long long> prevLayerMap = layerMap[stackLayer - 1][numToPutIntoStack];
		std::unordered_map<long long, long long>  newLayerMap;
		for (std::pair<long long, long long> pebblePr : prevLayerMap) {
			long long pebble = pebblePr.first;
			long long pebbleCount = pebblePr.second;
			if (!layer1Lookup.count(pebble)) {
				Stack newLayer1Stack;
				std::string pebbleString = llongToString(pebble);
				if (pebble == 0) {
					newLayer1Stack.push(1);
				}
				else {
					if (pebbleString.length() % 2 == 0) {

						std::string newPebble1 = pebbleString.substr(0, pebbleString.length() / 2);
						std::string newPebble2 = pebbleString.substr(pebbleString.length() / 2);
						if (!newPebble2.empty()) {
							newPebble2 = llongToString(std::stoll(newPebble2));
						}

						if (newPebble2.empty()) {
							newPebble2 = "0";
						}
						newLayer1Stack.push(std::stoll(newPebble1));
						newLayer1Stack.push(std::stoll(newPebble2));
					}
					else {
						newLayer1Stack.push(pebble * 2024);
					}
				}

				layer1Lookup[pebble] = newLayer1Stack;
				//std::cout << pebble << " " <<layer1Lookup[pebble].size << " ";

			}

			if (layer1Lookup[pebble].size == 1) {
				layerMap[stackLayer][numToPutIntoStack][layer1Lookup[pebble].peek()] += pebbleCount;
			}
			else {
				layerMap[stackLayer][numToPutIntoStack][layer1Lookup[pebble].peek()] += pebbleCount;
				long long temp = layer1Lookup[pebble].peek();
				layer1Lookup[pebble].pop();
				layerMap[stackLayer][numToPutIntoStack][layer1Lookup[pebble].peek()] += pebbleCount;
				layer1Lookup[pebble].push(temp);
			}
		}
		//layerMap[stackLayer][numToPutIntoStack] = newLayerStack;
		//layerMap[stackLayer - 1].erase(numToPutIntoStack);
	}

}


void plutonianPebbles3(std::string fileURL, int depth) {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	std::string curr{ "" };

	std::vector<int> pebbleStack;
	while (!reachedEnd) {
		int c = std::getc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
			pebbleStack.push_back(std::stoi(curr));
		}
		else {
			if (c == ' ') {
				pebbleStack.push_back(std::stoi(curr));
				curr.clear();
			}
			else {
				curr += char(c);
			}
		}

	}

	std::array<std::unordered_map<long long, std::unordered_map<long long, long long>>, 2000> layerMap;
	long long total{ 0 };
	int howDeep = depth;

	std::unordered_map<long long, Stack> layer1Lookup;
	layer1Lookup[0].push(1);
	for (long long pebble : pebbleStack) {
		inLayerN3(howDeep, layerMap, pebble, layer1Lookup);
		//inLayerN(howDeep, layerMap, pebble);
		
	}
	for (std::pair<long long, std::unordered_map<long long, long long>> pebbleMap: layerMap[howDeep]) {
		long long pebble = pebbleMap.first;
		for (std::pair<long long, long long> pebblePr : layerMap[howDeep][pebble]) {
			total += pebblePr.second;
		}
	}
	std::cout << "\n" << total << "\n";
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}
