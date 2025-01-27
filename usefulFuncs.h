#include <iostream>
#include <vector>
#include <functional>
#include <regex>
#pragma once


struct pair_hash {

	size_t operator()(const std::pair<int, int>& p) const {
		// A simple hash combining both integers
		return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
	}
};

struct vec_hash {
    size_t operator()(const std::vector<int>& v) const {
        size_t hash = 0;
        size_t prime = 31; // A small prime number for hashing
        for (int num : v) {
            hash ^= std::hash<int>{}(num)+0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};


std::string llongToString(long long i);
std::string intToString(int i);
std::vector<std::vector<int>> makeMatrix(std::string fileURL);
int euclidAlgor(int a, int b);
std::pair<int, std::pair<int, int>> extendedEuclidAlgor(int a, int b);
std::string fileToString(std::string fileURL);
std::vector<std::string> regexRepeatedData(std::string, std::regex);
void allCombinedStrings(std::vector<std::vector<std::string>>& possibilities, std::vector<std::string>& result, std::string curr = "", int selectingFrom = 0, int counter = 0); 	//return a vector of all possible combined strings ie from {{"a", "b}, {"c", "d"}} we need {"ac", "ad", "ac", "ad"}}
std::string stringMult(std::string s, int mult); //returns string s multiplied mult times
std::vector <std::string> permutations(std::string base); //returns a vector of all permuations of the string



struct Node {
    long long data;      // Data field
    Node* next;    // Pointer to the next node
    Node(long long val) : data(val), next(nullptr) {} // Constructor
};

class Stack {
private:
    Node* top;     // Pointer to the top of the stack
public:
    long long size;
    Stack() : top(nullptr), size(0) {} // Constructor initializes an empty stack

    // Push operation: Add a value to the stack
    void push(long long val) {
        Node* newNode = new Node(val); // Create a new node
        newNode->next = top;          // Link the new node to the current top
        top = newNode;                // Update the top pointer
        size++;
    }

    // Pop operation: Remove the top value from the stack
    void pop() {
        if (isEmpty()) {
            std::cout << "Stack underflow!" << std::endl;
            return;
        }
        Node* temp = top;   // Store the current top
        top = top->next;    // Update the top to the next node
        delete temp;        // Delete the old top
        size--;
    }

    // Peek operation: Get the value of the top element
    long long peek() const {
        if (isEmpty()) {
            std::cout << "Stack is empty!" << std::endl;
            return -1; // Return an invalid value
        }
        return top->data;
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return top == nullptr;
    }

    // Merge another stack into this stack in O(1) time
    void merge(Stack& other) {
        if (other.isEmpty()) {
            return; // Nothing to merge if the other stack is empty
        }
        Node* otherTop = other.top;
        other.top = nullptr; // Clear the other stack

        // Attach the other stack's list to this stack
        Node* temp = top;
        top = otherTop;

        // Traverse to the end of the other stack's list and attach this stack's list
        while (otherTop->next) {
            otherTop = otherTop->next;
        }
        otherTop->next = temp;
        size += other.size;
        other.size = 0;
    }

    // Display the stack contents
    void display() const {
        Node* current = top;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

struct minTreeNode {
    std::pair<minTreeNode*, minTreeNode*> children;  
    minTreeNode* parent;
    bool isFirstChild, isSecondChild;
    bool hasFirstChild, hasSecondChild, isTop, visited;
    int value;
    std::vector<int> data;
    minTreeNode(long val, std::vector<int> data) : value(val), data(data), children({ nullptr,nullptr }), hasFirstChild(false), hasSecondChild(false), isTop(false), parent(nullptr),
        isFirstChild(false), isSecondChild(false), visited(false)
    {} // Constructor


    void addFirstChild(minTreeNode* child) {
        if (child != nullptr) {
            children.first = child;
            child->addParent(this, true);
            hasFirstChild = true;
        }
        else { 
            hasFirstChild = false;
            children.first = nullptr;
        }
       
    }

    void addSecondChild(minTreeNode* child) {
        if (child != nullptr) {
            children.second = child;
            child->addParent(this, false);
            hasSecondChild = true;
        }
        else {
            hasSecondChild = false;
            children.second = nullptr;
        }

        
    }

    void removeFirstChild() {
        children.first->removeParent();
        children.first = nullptr;
        hasFirstChild = false;
    }

    void removeSecondChild() {
        children.second->removeParent();

        children.second = nullptr;
        hasSecondChild = false;
    }

    void addParent(minTreeNode* newParent, bool left) {
        parent = newParent;
        if (left) {
            isFirstChild = true;
            isSecondChild = false;
        }
        else {
            isFirstChild = false;
            isSecondChild = true;
        }
    }
    void removeParent() {
        parent = nullptr;
        isFirstChild = false;
        isSecondChild = false;
    }
};



class minBinTree {
private:
public:
    minTreeNode top;
    long size;
    minBinTree() : top(minTreeNode(-1, { -1, -1 , -1 })), size(0) {}

    bool empty() {
        if (size == 0) {
            return true;
        }
        return false;
    }

    minTreeNode& peek() {
        if (empty()) {
            std::cout << "ERROR PEEKING EMPTY LIST";
        }
        return *(top.children.first);
    }

    void swapNodes(minTreeNode& node1, minTreeNode& node2) {
        if (&node1 == node2.parent) {
            swapParentChild(node1, node2);
            return;
        }
        if (&node2 == node1.parent) {
            swapParentChild(node2, node1);
            return;
        }

        std::pair<minTreeNode*, minTreeNode*> node1Children = node1.children;
        minTreeNode* node1Parent = node1.parent; bool node1IsFirstChild = node1.isFirstChild;
        if (node2.parent != nullptr) {
            if (node2.isFirstChild) {
                node2.parent->addFirstChild(&node1);
            }
            else {
                node2.parent->addSecondChild(&node1);
            }
        }
        else {
            node1.removeParent();
        }
        if (node1Parent != nullptr) {
            if (node1IsFirstChild) {
                node1Parent->addFirstChild(&node2);
            }
            else {
                node1Parent->addSecondChild(&node2);
            }
        }
        else {
            node2.removeParent();
        }


        node1.addFirstChild(node2.children.first); node1.addSecondChild(node2.children.second);
        node2.addFirstChild(node1Children.first); node2.addSecondChild(node1Children.second);
        return;
    }

    void swapParentChild(minTreeNode& parent, minTreeNode& child) {
        //std::cout << "swapping parent " << parent.value << " with " << child.value << "\n";
        minTreeNode* childFirst = child.children.first; minTreeNode* childSecond = child.children.second;
        minTreeNode* parentParent = parent.parent; bool parentIsFirstChild = parent.isFirstChild; bool parentIsSecondChild = parent.isSecondChild;
        if (child.isFirstChild) {
            child.addSecondChild(parent.children.second);
            child.addFirstChild(&parent);
        }
        else if (child.isSecondChild) {
            child.addFirstChild(parent.children.first);
            child.addSecondChild(&parent);
        }
        if (parentIsFirstChild) {
            parentParent->addFirstChild(&child);
        }
        else if (parentIsSecondChild){
            parentParent->addSecondChild(&child);
        }
        else {
            child.parent = nullptr;
            child.isFirstChild = false;
            child.isSecondChild = false;
        }
        parent.addFirstChild(childFirst); parent.addSecondChild(childSecond);
    }
            
    void pop(minTreeNode* startPoint = nullptr) {

        if (size == 0) {
            std::cout << "CANT POP FROM EMPTY LIST";
            return;
        }
        if (size == 1) {
            top.removeFirstChild();
            size = 0;
            return;
        }
        size--;
        minTreeNode* current;
        minTreeNode* prev;
        bool prevLeft;
        if (startPoint == nullptr) {
            current = &peek();
            prev = &top;
            prevLeft =  true ;
        }
        else {
            current = startPoint;
            prev = startPoint->parent;
            prevLeft = current->isFirstChild;
        }
  
        while (true) {
            //std::cout << "current is " << current->data[0] << current->data[1] << current->data[2] << "," << current->value << " with parent " << current->parent->data[0] << current->parent->data[1] << current->parent->data[2] << "," << current->parent->value << "\n";
            if (prev != &top) {
               //std::cout << "prev is " << prev->value << " with parent " << prev->parent->value << "\n";
                //std::cout << "peek is " << peek().value << " with parent " << peek().parent->value << "\n";
            }
            if (!current->hasFirstChild and !current->hasSecondChild) {
                //std::cout << "current has no children \n";
                if (prevLeft) {
                   // std::cout << "current was first child \n";
                    prev->removeFirstChild();
                    
                }
                else {
                    //std::cout << "current was second child \n";
                    prev->removeSecondChild();
                }
                return;
            }
            else if (!current->hasFirstChild) {
               // std::cout << "Z";
                if (current == &peek()) {
                   // std::cout << "current is top! \n";
                    top.removeFirstChild();
                    top.addFirstChild(current->children.second);
                }
                else {
                    if (prevLeft) {
                        prev->removeFirstChild();
                        prev->addFirstChild(current->children.second);
                    }
                    else {
                        prev->removeSecondChild();
                        prev->addSecondChild(current->children.second);
                    }
                }
                return;
            }
            else if (!current->hasSecondChild) {
                if (current == &peek()) {
                    //std::cout << "current is top! \n";
                    top.removeFirstChild();
                    top.addFirstChild(current->children.first);
                }
                else {
                    if (prevLeft) {
                        prev->removeFirstChild();
                        prev->addFirstChild(current->children.first);

                    }
                    else {
                        prev->removeSecondChild();
                        prev->addSecondChild(current->children.first);
                    }
                }
                return;
            }
            else {
                if (current->children.first->value <= current->children.second->value) {
                    minTreeNode* child = current->children.first;

                    //std::cout << "pop swapped with first, swapping " << current->data[0] << current->data[1] << current->data[2] << current->value << " with " << child->data[0] << child->data[1] << child->data[2] << "," << child->value << "\n";
                    swapNodes(*current, *child);
                    prev = child;
                    prevLeft = true;
                }
                else {
                    minTreeNode* child = current->children.second;
                    //std::cout << "pop swapped with second, swapping " << current->data[0] << current->data[1] << current->data[2] << current->value << " with " << child->data[0] << child->data[1] << child->data[2] << "," << child->value << "\n";
                    swapNodes(*current, *child);
                    prev = child;
                    prevLeft = false;
                }
            }

        }
    }

    void push(minTreeNode* current) {

        if (size == 0) {
            top.addFirstChild(current);
            size = 1;
            return;
        }
        size++;
        minTreeNode* prev = &top;
        minTreeNode* lookingAt = &peek();

        bool prevLeft{ true };
        while (true) {
            //std::cout << "current is " << current->data[0] << current->data[1] << current->data[2] << "," << current->value << " with prev " << prev->data[0] << prev->data[1] << prev->data[2] << "," << prev->value << "\n";

            if (current->value < lookingAt->value) {
                //std::cout << "push swapping " << lookingAt->value << " with " << current->value << "\n";
                //std::cout << "push swapping " << lookingAt->value << " with " << current->value << "\n";
                /*if (prevLeft) {
                    (*prev).addFirstChild(current);
                }
                else {
                    (*prev).addSecondChild(current);
                }*/


                swapNodes(*current, *lookingAt);
                std::swap(lookingAt, current);
                //pushInTree(lookingAt, current, prev, prevLeft);
                //return;
            }
            else if (!lookingAt->hasFirstChild) {
                //std::cout << "adding " << current->value << " as first child to " << lookingAt->value << "\n";
                lookingAt->addFirstChild(current);
                return;
            }
            else if (!lookingAt->hasSecondChild) {
                //std::cout << "adding " << current->value << " as second child to " << lookingAt->value << "\n";

                lookingAt->addSecondChild(current);
                return;
            }
            else if (lookingAt->children.first->value >= lookingAt->children.second->value) {
                prev = lookingAt;
                lookingAt = prev->children.first;
                prevLeft = true;
            }
            else {
                prev = lookingAt;
                lookingAt = prev->children.second;
                prevLeft = false;
            }
        }


    }

   /* void pushInTree(minTreeNode* current, minTreeNode* lookingAt, minTreeNode* prev, bool prevLeft) {
        while (true) {
            if (current->value < lookingAt->value) {
                if (prevLeft) {
                    (*prev).addFirstChild(current);
                }
                else {
                    (*prev).addSecondChild(current);
                }
                swapNodes(*current, *lookingAt);
                pushInTree(lookingAt, current, prev, prevLeft);
                return;
            }
            else if (!lookingAt.hasFirstChild) {
                std::cout << "adding " << current.value << " as first child to " << lookingAt.value << "\n";

                lookingAt.addFirstChild(&current);
                return;
            }
            else if (!lookingAt.hasSecondChild) {
                std::cout << "adding " << current.value << " as second child to " << lookingAt.value << "\n";

                lookingAt.addSecondChild(&current);
                return;
            }
            else if (lookingAt.children.first->value > lookingAt.children.second->value) {
                prev = &lookingAt;
                lookingAt = *lookingAt.children.first;
                prevLeft = true;
            }
            else {
                prev = &lookingAt;
                lookingAt = *lookingAt.children.second;
                prevLeft = false;
            }
        }


    }*/

    void decreasePriority(minTreeNode* node, int val) {

        if (val >= node->value) {
            //std::cout << "CANT DECREASE TO A HIGHER PRIORITY" << "\n";
            return;
        }
        else {
            /*pop(node);
            node = new minTreeNode(val, node->data);
            push(node);*/
            node->value = val;
            while (node->parent->value > node->value and node->parent->value != -1) {
                swapNodes(*node, *node->parent);
            }
            
        }


    }

};