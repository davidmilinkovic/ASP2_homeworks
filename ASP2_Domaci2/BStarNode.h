//
// Created by md200037d on 29-Nov-21.
//

#ifndef ASP2_Domaci2_BSTARNODE_H
#define ASP2_Domaci2_BSTARNODE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "BStarTree.h"

class BStarTree;

class BStarNode
{
    string keys[15] = {};
    BStarNode* children[15] = {};
    BStarNode* parent;
    const BStarTree* tree;
    int numKeys;
    bool leaf;
    explicit BStarNode(BStarTree* _tree, BStarNode* _parent = nullptr, bool _leaf = false);

    int insertKey(string key); //
    int upperBound(string key) const; // index of first >= key, returns (numKeys+1) if all keys are < key
    int indexInParent();

    bool hasKey(string key) const;
    bool isFull() const;
    bool canTakeOne() const;
    bool isRoot() const;
    bool hasEnough() const;
    bool overflow() const;

    BStarNode* rightBrother() const;
    BStarNode* leftBrother() const;
    BStarNode* secondRightBrother() const;
    BStarNode* secondLeftBrother() const;

    void shiftLeft(int start = 0, int places = 1);
    void shiftRight(int start = 0, int places = 1);
    void addKeysToVector(vector<string>& v) const;
    void addChildrenToVector(std::vector<BStarNode*>& v) const;

    friend ostream& operator<<(ostream& os, const BStarNode& node);

    friend class BStarTree;
public:
    bool isLeaf() const;
    int getNumKeys() const;
    BStarNode* getChild(int i) const;
};



#endif //ASP2_Domaci2_BSTARNODE_H
