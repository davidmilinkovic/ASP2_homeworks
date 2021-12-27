//
// Created by md200037d on 29-Nov-21.
//

#ifndef ASP2_Domaci2_BSTARTREE_H
#define ASP2_Domaci2_BSTARTREE_H

#include "BStarNode.h"

class BStarNode;

class BStarTree
{
    BStarNode* root;
    int minChild;
    int maxChildRoot;
    static void spillToLeft(BStarNode* left, BStarNode* right, int indLeft, int num);
    static void spillToRight(BStarNode* left, BStarNode* right, int indLeft, int num);
    BStarNode* break2to3(BStarNode* node, int ind, BStarNode* leftBro, BStarNode* rightBro);
    void breakRoot();
    BStarNode* merge2to1(BStarNode* left, BStarNode* right);
    BStarNode* merge3to2(BStarNode* left, BStarNode* center, BStarNode* right);
    BStarNode* correspondingNode(string key) const;
public:
    int m;
    BStarTree(int _m);
    ~BStarTree();

    bool addKey(string key);
    bool removeKey(const string& key);
    bool hasKey(const string& key) const;
    int numberOfSmaller(const string& key) const;

    friend ostream& operator<<(ostream& os, const BStarTree& bst);

    friend class BStarNode;
};

#endif //ASP2_Domaci2_BSTARTREE_H
