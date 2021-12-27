#include "BStarNode.h"

BStarNode::BStarNode(BStarTree* _tree, BStarNode* _parent, bool _leaf)
{
    tree = _tree;
    numKeys = 0;
    for (int i = 0; i < tree->maxChildRoot + 1; i++)
        children[i] = nullptr;
    parent = _parent;
    leaf = _leaf;
}

int BStarNode::insertKey(string key)
{
    int i = upperBound(key);
    for (int j = numKeys - 1; j >= i; j--)
        keys[j + 1] = keys[j];
    keys[i] = key;
    numKeys++;
    return i;
}

int BStarNode::indexInParent() {
    int ind = -1;
    for (int i = 0; i <= parent->numKeys; i++) {
        if (parent->children[i] == this) {
            ind = i;
            break;
        }
    }
    return ind;
}

void BStarNode::shiftLeft(int start, int places) {
    for (int i = start + 1; i < numKeys; i++) {
        keys[i - places] = keys[i];
        children[i - places] = children[i];
    }
    children[numKeys - places] = children[numKeys];
    for (int j = 0; j < places; j++)
        children[numKeys - j] = nullptr;
    numKeys -= places;
}

void BStarNode::shiftRight(int start, int places) {
    children[numKeys + places] = children[numKeys];
    for (int i = numKeys - 1; i >= start; i--) {
        keys[i + places] = keys[i];
        children[i + places] = children[i];
    }
    for (int j = 0; j < places; j++)
        children[start + j] = nullptr;
    numKeys += places;
}

bool BStarNode::hasKey(string key) const
{
    int i = upperBound(key);
    return i != numKeys && keys[i] == key;
}

int BStarNode::upperBound(string key) const
{
    for (int i = 0; i < numKeys; i++)
        if (keys[i] >= key) return i;
    return numKeys;
}

bool BStarNode::isFull() const
{
    if (isRoot()) return numKeys == tree->maxChildRoot - 1;
    return numKeys == tree->m - 1;
}

bool BStarNode::hasEnough() const {
    if (isRoot()) return numKeys >= 1;
    return numKeys >= tree->minChild - 1;
}

bool BStarNode::canTakeOne() const
{
    return numKeys - 1 >= tree->minChild - 1;
}

bool BStarNode::isRoot() const
{
    return parent == nullptr;
}

bool BStarNode::isLeaf() const
{
    return leaf;
}

int BStarNode::getNumKeys() const
{
    return numKeys;
}

BStarNode* BStarNode::getChild(int i) const
{
    return children[i];
}

bool BStarNode::overflow() const
{
    if (isRoot()) return numKeys > tree->maxChildRoot - 1;
    return numKeys > tree->m - 1;
}

BStarNode* BStarNode::rightBrother() const
{
    if (!parent) return nullptr;
    for (int i = 0; i < parent->numKeys + 1; i++)
        if (parent->children[i] == this)
            return i == parent->numKeys ? nullptr : parent->children[i + 1];
    return nullptr;
}

BStarNode* BStarNode::leftBrother() const
{
    if (!parent) return nullptr;
    for (int i = 0; i < parent->numKeys + 1; i++)
        if (parent->children[i] == this)
            return i == 0 ? nullptr : parent->children[i - 1];
    return nullptr;
}

BStarNode* BStarNode::secondRightBrother() const
{
    if (!parent) return nullptr;
    for (int i = 0; i < parent->numKeys + 1; i++)
        if (parent->children[i] == this)
            return i >= parent->numKeys - 1 ? nullptr : parent->children[i + 2];
    return nullptr;
}

BStarNode* BStarNode::secondLeftBrother() const
{
    if (!parent) return nullptr;
    for (int i = 0; i < parent->numKeys + 1; i++)
        if (parent->children[i] == this)
            return i <= 1 ? nullptr : parent->children[i - 2];
    return nullptr;
}

ostream& operator<<(ostream& os, const BStarNode& node)
{
    int keys = max(node.numKeys, node.tree->m - 1);
    string str;
    for (int i = 0; i < keys; i++) {
        str = "";
        if (i < node.numKeys) str = node.keys[i];
        os << "|" << str;
    }
    os << "|" << "  ";
    return os;
}

void BStarNode::addChildrenToVector(std::vector<BStarNode*>& v) const {
    for (int i = 0; i <= numKeys; i++)
        v.push_back(children[i]);
}

void BStarNode::addKeysToVector(vector<string>& v) const {
    for (int i = 0; i < numKeys; i++)
        v.push_back(keys[i]);
}
