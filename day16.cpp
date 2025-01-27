#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include "day13.h"
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
#include<regex>
#pragma once



std::vector<std::vector<int>> makeReindeerMatrix(std::string fileURL) {
	FILE* dataFile = std::fopen(fileURL.c_str(), "r+");
	bool reachedEnd{ false };
	int y{ 0 },x{ 0 };
	std::vector<std::vector<int>> returnMat{ std::vector<int> {} };
	while (!reachedEnd) {
		int c = std::fgetc(dataFile);
		if (c == EOF) {
			reachedEnd = true;
		}
		else {
			if (c != 10) {
				returnMat[y].push_back(c);
				x++;
			}
			else {
				returnMat.push_back(std::vector<int> {});
				y++;
				x = 0;
			}
		}
	}
	std::fclose(dataFile);
	return returnMat;
}


void reindeerMaze(std::string fileURL) {
	std::vector<std::vector<int>> matrix = makeReindeerMatrix(fileURL);
	int x{ 0 }, y{ 0 };
	std::vector<int> startPos, endPos;
	minBinTree tentativeTree;
	std::unordered_map<std::vector<int>, minTreeNode*, vec_hash> allNodes;          //if a point is in our min tree then this map gives us the pointer to it
	for (std::vector<int> row : matrix) {
		for (int i : row) {
			if (i == 'S') {
				startPos = { y, x , 0 };
				//std::cout << "start is " << x << " " << y << "\n";
				minTreeNode* startNode = new minTreeNode(0, startPos);
				tentativeTree.push(startNode);
				allNodes.insert({ { y,x,0 }, startNode });
				for (int i = 1; i < 4; i++) {
					minTreeNode* node = new minTreeNode(INT_MAX, { y, x, i });
					allNodes.insert({ { y,x,i }, node });
					tentativeTree.push(node);

				}
			}
			if (i == 'E') {
				//std::cout << "end is " << x << " " << y << "\n";
				for (int i = 0; i < 4; i++) {
					minTreeNode* node = new minTreeNode(INT_MAX, { y, x, i });
					allNodes.insert({ { y,x,i }, node });
					tentativeTree.push(node);
				}
				endPos = { y,x };											//end point can be facing any direction??
			}
			if (i == '.') {
				for (int i = 0; i < 4; i++) {
					minTreeNode* node = new minTreeNode(INT_MAX, { y, x, i });
					allNodes.insert({ { y,x,i }, node});
					tentativeTree.push(node);

				}
			}
			x++;
		}
		y++; x = 0;
	}
	long shortestFoundPath = -1;
	while (!tentativeTree.empty()) {
		//std::cout << "\n";
		//std::cout << tentative.empty();
		//for (std::pair<std::vector<int>, minTreeNode*> pr : tentative) {
		//	//std::cout << " " << pr.first[0] << "," << pr.first[1] << "," << pr.first[2] << "," << pr.second->value << "\n";

		//}

		std::vector<int> coords = tentativeTree.peek().data; long dist = tentativeTree.peek().value;
		//std::cout << "a";
		std::cout << "looking at " << coords[0] << " " << coords[1] << " " << coords[2] << " which has dist " << dist << "\n" << "tree has size " << tentativeTree.size << "\n";

		tentativeTree.pop();
		allNodes[coords]->visited = true;
		std::cout << "b";




		if (coords[0] == endPos[0] and coords[1] == endPos[1]) {
			//std::cout << "a DIST IS " << dist << "\n";
			if (dist < shortestFoundPath or shortestFoundPath == -1) {
				shortestFoundPath = dist;
			}
			
		}
		
		std::vector<int> turnNeighbour = { coords[0],coords[1],(coords[2] + 1) % 4 };
		int turn2Neighbourfuckoff = coords[2]- 1;
		if (turn2Neighbourfuckoff < 0) {
			turn2Neighbourfuckoff += 4;

		}
		//std::cout << "c";

		std::vector<int> turnNeighbour2 = { coords[0],coords[1],turn2Neighbourfuckoff };
		std::vector<int> moveNeighbour = coords;
		if (coords[2] == 0) {
			moveNeighbour[1]++;
		}
		if (coords[2] == 1) {
			moveNeighbour[0]++;
		}
		if (coords[2] == 2) {
			moveNeighbour[1]--;
		}
		if (coords[2] == 3) {
			moveNeighbour[0]--;
		}
		

		if (allNodes.count(turnNeighbour) and !allNodes[turnNeighbour]->visited) {
			minTreeNode* node = allNodes[turnNeighbour];
			std::cout << " updating " << node->data[0] << "," << node->data[1] << "," << node->data[2] << " to " << dist + 1000 << "\n";			
			tentativeTree.decreasePriority(node, dist + 1000);
		}

		if (allNodes.count(turnNeighbour2) and !allNodes[turnNeighbour2]->visited) {
			minTreeNode* node = allNodes[turnNeighbour2];
			std::cout << " updating " << node->data[0] << "," << node->data[1] << "," <<node->data[2] << " to " << dist + 1000 << "\n";
			tentativeTree.decreasePriority(node, dist + 1000);
			
		}

		if (allNodes.count(moveNeighbour) and !allNodes[moveNeighbour]->visited) {
			minTreeNode* node = allNodes[moveNeighbour];
			std::cout << " updating " << node->data[0] << "," << node->data[1] << "," << node->data[2] << " to " << dist + 1 << "\n";
			tentativeTree.decreasePriority(node, dist + 1);
			
		}
		//std::cout << "g";


	 }


	std::cout << "final score is " << shortestFoundPath;

}


int pointsOnShortestPath(std::vector<int> startPos, std::unordered_map<std::vector<int>, std::vector<minTreeNode*>, vec_hash>& closestLastNode, std::unordered_set < std::vector<int>, vec_hash>& onFinalPath, std::vector<int> coords) {
	onFinalPath.insert({ coords[0], coords[1] });
	for (minTreeNode* node : closestLastNode[coords]) {
		std::vector<int> newCoords = node->data;
		pointsOnShortestPath(startPos, closestLastNode, onFinalPath, newCoords);
	}
	return onFinalPath.size();

	
}


void reindeerMazePartTwo(std::string fileURL) {
	std::vector<std::vector<int>> matrix = makeReindeerMatrix(fileURL);
	int x{ 0 }, y{ 0 };
	std::vector<int> startPos, endPos;
	minBinTree tentativeTree;
	std::unordered_map<std::vector<int>, minTreeNode*, vec_hash> allNodes;          //if a point is in our min tree then this map gives us the pointer to it
	std::unordered_map<std::vector<int>, std::vector<minTreeNode*>, vec_hash> closestLastNode;	
	for (std::vector<int> row : matrix) {
		for (int i : row) {
			if (i == 'S') {
				startPos = { y, x , 0 };
				//std::cout << "start is " << x << " " << y << "\n";
				minTreeNode* startNode = new minTreeNode(0, startPos);
				tentativeTree.push(startNode);
				allNodes.insert({ { y,x,0 }, startNode });
				for (int i = 1; i < 4; i++) {
					minTreeNode* node = new minTreeNode(INT_MAX, { y, x, i });
					allNodes.insert({ { y,x,i }, node });
					tentativeTree.push(node);

				}
			}
			if (i == 'E') {
				//std::cout << "end is " << x << " " << y << "\n";
				for (int i = 0; i < 4; i++) {
					minTreeNode* node = new minTreeNode(INT_MAX, { y, x, i });
					allNodes.insert({ { y,x,i }, node });
					tentativeTree.push(node);
				}
				endPos = { y,x };											//end point can be facing any direction??
			}
			if (i == '.') {
				for (int i = 0; i < 4; i++) {
					minTreeNode* node = new minTreeNode(INT_MAX, { y, x, i });
					allNodes.insert({ { y,x,i }, node });
					tentativeTree.push(node);

				}
			}
			x++;
		}
		y++; x = 0;
	}
	long shortestFoundPath = -1;
	minTreeNode* finalNode = new minTreeNode(0, { -1,-1,-1 });
	while (!tentativeTree.empty()) {


		std::vector<int> coords = tentativeTree.peek().data; long dist = tentativeTree.peek().value;

		tentativeTree.pop();
		allNodes[coords]->visited = true;




		if (coords[0] == endPos[0] and coords[1] == endPos[1]) {
			if (dist < shortestFoundPath or shortestFoundPath == -1) {
				shortestFoundPath = dist;

				finalNode = allNodes[coords];

			}

		}

		std::vector<int> turnNeighbour = { coords[0],coords[1],(coords[2] + 1) % 4 };
		int turn2Neighbourfuckoff = coords[2] - 1;
		if (turn2Neighbourfuckoff < 0) {
			turn2Neighbourfuckoff += 4;

		}

		std::vector<int> turnNeighbour2 = { coords[0],coords[1],turn2Neighbourfuckoff };
		std::vector<int> moveNeighbour = coords;
		if (coords[2] == 0) {
			moveNeighbour[1]++;
		}
		if (coords[2] == 1) {
			moveNeighbour[0]++;
		}
		if (coords[2] == 2) {
			moveNeighbour[1]--;
		}
		if (coords[2] == 3) {
			moveNeighbour[0]--;
		}


		if (allNodes.count(turnNeighbour) and !allNodes[turnNeighbour]->visited) {
			minTreeNode* node = allNodes[turnNeighbour];
			int newDist = dist + 1000;
			if (newDist < node->value) {
				closestLastNode[turnNeighbour] = {allNodes[coords]};
			}
			else if (newDist == node->value) {
				closestLastNode[turnNeighbour].push_back(allNodes[coords]);
			}
			tentativeTree.decreasePriority(node, newDist);
		}

		if (allNodes.count(turnNeighbour2) and !allNodes[turnNeighbour2]->visited) {
			minTreeNode* node = allNodes[turnNeighbour2];
			int newDist = dist + 1000;
			if (newDist < node->value) {
				closestLastNode[turnNeighbour2] = { allNodes[coords] };
			}
			else if (newDist == node->value) {
				closestLastNode[turnNeighbour2].push_back(allNodes[coords]);
			}

			tentativeTree.decreasePriority(node, newDist);

		}

		if (allNodes.count(moveNeighbour) and !allNodes[moveNeighbour]->visited) {
			minTreeNode* node = allNodes[moveNeighbour];
			int newDist = dist + 1;
			if (newDist < node->value) {
				closestLastNode[moveNeighbour] = { allNodes[coords] };
			}
			else if (newDist == node->value) {
				closestLastNode[moveNeighbour].push_back(allNodes[coords]);
			}

			tentativeTree.decreasePriority(node, newDist);

		}


	}

	std::unordered_set < std::vector<int>, vec_hash> onFinalPath;
	std::vector<int> finalCoords = finalNode->data;
	std::cout << pointsOnShortestPath(startPos, closestLastNode, onFinalPath, finalCoords);


}




void minTreeTest(std::string fileURL) {
	//std::vector<std::vector<int>> matrix = makeReindeerMatrix(fileURL);
	//int x{ 0 }, y{ 0 };
	//std::vector<int> startPos, endPos;
	//minBinTree tentativeTree;
	//std::unordered_map<std::vector<int>, minTreeNode*, vec_hash> allNodes;          //if a point is in our min tree then this map gives us the pointer to it
	//for (std::vector<int> row : matrix) {
	//	for (int i : row) {
	//		if (i == 'S') {
	//			startPos = { y, x , 0 };
	//			//std::cout << "start is " << x << " " << y << "\n";
	//			minTreeNode* startNode = new minTreeNode(0, startPos);
	//			tentativeTree.push(startNode);
	//			allNodes.insert({ { y,x,0 }, startNode });
	//			for (int i = 1; i < 4; i++) {
	//				minTreeNode* node = new minTreeNode(INT_MAX, { y, x, i });
	//				allNodes.insert({ { y,x,i }, node });
	//				tentativeTree.push(node);
	//			}
	//		}
	//		if (i == 'E') {
	//			//std::cout << "end is " << x << " " << y << "\n";
	//			for (int i = 0; i < 4; i++) {
	//				minTreeNode* node = new minTreeNode(INT_MAX, { y, x, i });
	//				allNodes.insert({ { y,x,i }, node });
	//				tentativeTree.push(node);
	//			}
	//			endPos = { y,x };											//end point can be facing any direction??
	//		}
	//		if (i == '.') {
	//			for (int i = 0; i < 4; i++) {
	//				minTreeNode* node = new minTreeNode(INT_MAX, { y, x, i });
	//				allNodes.insert({ { y,x,i }, node });
	//				tentativeTree.push(node);
	//			}
	//		}
	//		x++;
	//	}
	//	y++; x = 0;
	//}


	//tentativeTree.decreasePriority(allNodes[{2, 1, 1}], 1000);

	//tentativeTree.decreasePriority(allNodes[{2, 1, 2}], 1000);

	//tentativeTree.decreasePriority(allNodes[{2, 2, 0}], 1);



	minBinTree tree;
	minTreeNode n1(1, { 1,1,1 });
	minTreeNode n2(2, { 2,1,1 });
	minTreeNode n3(3, { 3,1,1 });
	minTreeNode n4(4, { 4,1,1 });
	minTreeNode n0(0, { 0,1,1 });
	
	tree.push(&n1); tree.push(&n2);  
	tree.push(&n3);
	tree.push(&n4);
	tree.push(&n0);
	std::cout << n2.parent->value;


	
	
	






 



}
	

