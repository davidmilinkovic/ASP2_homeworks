#include "BStarTree.h"

#include <vector>
#include <queue>


BStarTree::BStarTree(int _m)
{
    m = _m;
    maxChildRoot = 2 * ((2 * m - 2) / 3) + 1;
    minChild = ceil((double)(2 * m - 1) / 3);
    root = nullptr;
}

BStarTree::~BStarTree()
{
    queue<BStarNode*> q;
    q.push(root);
    while (!q.empty()) {
        BStarNode* node = q.front();
        q.pop();
        if (node) {
            for (int i = 0; i <= node->numKeys; i++) {
                q.push(node->children[i]);
            }
            delete node;
        }
    }
    root = nullptr;
}

bool BStarTree::addKey(string key)
{
    BStarNode* node = correspondingNode(key);
    if (!node) {
        root = new BStarNode(this, nullptr, true);
        root->insertKey(key);
        return true;
    }
    if (node->hasKey(key)) return false;
    node->insertKey(key);
    while (node && node->overflow()) {
        if (node != root) {
            // nadjemo njegov indeks u roditelju
            int ind = node->indexInParent();

            BStarNode* leftBro = node->leftBrother();
            BStarNode* rightBro = node->rightBrother();

            // prelivanje sa desnim
            if (rightBro && !rightBro->isFull()) {
                int x = node->numKeys + rightBro->numKeys + 1;
                int num = node->numKeys - (x - 1) / 2; // koliko treba prebaciti iz left u right
                spillToRight(node, rightBro, ind, num);
                continue;
            }

            // prelivanje sa levim
            if (leftBro && !leftBro->isFull()) {
                int x = node->numKeys + leftBro->numKeys + 1;
                int y = (x - 1) / 2; // koliko treba da bude u left
                int num = y - leftBro->numKeys; // koliko treba prebaciti iz right u left
                spillToLeft(leftBro, node, ind - 1, num);
                continue;
            }

            // radi se prelom
            node = break2to3(node, ind, leftBro, rightBro);
        }
        else {
            // prelom korena
            breakRoot();
            break;
        }
    }
    return true;
}

bool BStarTree::removeKey(const string& key)
{
    if (!root) return  false;
    BStarNode* node = correspondingNode(key);
    if (!node->hasKey(key)) return false;

    if (!node->leaf) {
        int ind = node->upperBound(key);
        BStarNode* sled = node->children[ind + 1];
        while (!sled->leaf) {
            sled = sled->children[0];
        }
        swap(node->keys[ind], sled->keys[0]);
        node = sled;
    }

    int ind = node->upperBound(key);
    node->shiftLeft(ind);
    if (node == root) {
        if (node->numKeys == 0) {
            delete root;
            root = nullptr;
        }
        return true;
    }

    while (!node->hasEnough()) {
        if (node->isRoot()) {
            root = node->children[0];
            root->parent = nullptr;
            delete node;
            break;
        }
        int indexInParent = node->indexInParent();

        BStarNode* rightBro = node->rightBrother();
        BStarNode* leftBro = node->leftBrother();
        BStarNode* secondRightBro = node->secondRightBrother();
        BStarNode* secondLeftBro = node->secondLeftBrother();

        if (rightBro && rightBro->canTakeOne()) {
            spillToLeft(node, rightBro, indexInParent, 1);
            return true;
        }
        if (leftBro && leftBro->canTakeOne()) {
            spillToRight(leftBro, node, indexInParent - 1, 1);
            return true;
        }
        if (secondRightBro && secondRightBro->canTakeOne()) {
            spillToLeft(node, rightBro, indexInParent, 1);
            spillToLeft(rightBro, secondRightBro, indexInParent + 1, 1);
            return true;
        }
        if (secondLeftBro && secondLeftBro->canTakeOne()) {
            spillToRight(leftBro, node, indexInParent - 1, 1);
            spillToRight(secondLeftBro, leftBro, indexInParent - 2, 1);
            return true;
        }

        // spajanje 3 u 2
        int numBrothers = (rightBro != nullptr) + (leftBro != nullptr) + (secondRightBro != nullptr) + (secondLeftBro !=
            nullptr);
        if (numBrothers == 1) {
            if (rightBro) node = merge2to1(node, rightBro);
            else if (leftBro) node = merge2to1(leftBro, node);
            else if (secondRightBro) node = merge2to1(node, secondRightBro);
            else if (secondLeftBro) node = merge2to1(secondLeftBro, node);
        }
        else if (numBrothers >= 2) {
            if (leftBro && rightBro) node = merge3to2(leftBro, node, rightBro);
            else if (rightBro && secondRightBro) node = merge3to2(node, rightBro, secondRightBro);
            else if (leftBro && secondLeftBro) node = merge3to2(secondLeftBro, leftBro, node);
        }
        else {
            return false;
        }
    }
    return true;
}

bool BStarTree::hasKey(const string& key) const
{
    if (!root) return false;
    BStarNode* node = correspondingNode(key);
    return node->hasKey(key);
}

BStarNode* BStarTree::correspondingNode(string key) const
{
    BStarNode* node = root;
    while (node) {
        int i = node->upperBound(key);
        if (i != node->numKeys && node->keys[i] == key)
            return node;
        if (node->children[i] == nullptr)
            return node;
        node = node->children[i];
    }
    return nullptr;
}

ostream& operator<<(ostream& os, const BStarTree& bst)
{
    if (!bst.root) {
        os << "Stablo je prazno!" << endl;
        return os;
    }

    queue<pair<BStarNode*, int>> q;
    q.push({ bst.root, 0 });
    int level = 0;
    while (!q.empty()) {
        BStarNode* node = q.front().first;
        if (level != q.front().second) {
            os << endl << endl;
            level++;
        }
        q.pop();
        if (node) {
            os << *node;
            if (!node->isLeaf()) {
                for (int i = 0; i <= node->getNumKeys(); i++)
                    q.push({ node->getChild(i), level + 1 });
            }
        }
        else {
            for (int i = 0; i < bst.m - 1; i++) {
                os << "|  ";
            }
            os << "|";
        }
    }
    return os;
}

void BStarTree::spillToRight(BStarNode* left, BStarNode* right, int indLeft, int num) {
    // shiftujemo sve u desnom bratu za num udesno
    right->shiftRight(0, num);

    // prebacimo razdelni kljuci u desnog brata
    // njegovo levo dete postaje najdesnije podstablo u cvoru left
    right->keys[num - 1] = left->parent->keys[indLeft];
    right->children[num - 1] = left->children[left->numKeys];
    if (right->children[num - 1])
        right->children[num - 1]->parent = right;
    left->children[left->numKeys] = nullptr;

    // num-1 kljuceva sa kraja left se zajedno sa svojim levim sinom prebacuju u brata
    for (int i = 0; i < num - 1; i++) {
        int indNode = left->numKeys - num + i + 1;
        right->keys[i] = left->keys[indNode];
        right->children[i] = left->children[indNode];
        if (right->children[i])
            right->children[i]->parent = right;
    }

    // num-ti kljuc sa kraja left prelazi u razdelni
    left->parent->keys[indLeft] = left->keys[left->numKeys - num];

    left->numKeys -= num;
}


void BStarTree::spillToLeft(BStarNode* left, BStarNode* right, int indLeft, int num) {
    // prebacimo razdelni kljuci u levog brata
    // njegovo desno dete postaje najlevlje podstablo u cvoru right
    left->keys[left->numKeys] = right->parent->keys[indLeft];
    left->children[left->numKeys + 1] = right->children[0];
    if (left->children[left->numKeys + 1])
        left->children[left->numKeys + 1]->parent = left;
    right->children[0] = nullptr;

    // num-1 kljuceva sa pocetka right se zajedno sa svojim desnim sinom prebacuju u brata
    for (int i = 0; i < num - 1; i++) {
        int indL = left->numKeys + i + 1;
        left->keys[indL] = right->keys[i];
        left->children[indL + 1] = right->children[i + 1];
        if (left->children[indL + 1])
            left->children[indL + 1]->parent = left;
    }

    // num-ti kljuc sa pocetka right prelazi u razdelni
    right->parent->keys[indLeft] = right->keys[num - 1];

    // shiftujemo sve u nodeu za num ulevo
    right->shiftLeft(num - 1, num);

    left->numKeys += num;
}

BStarNode* BStarTree::break2to3(BStarNode* node, int ind, BStarNode* leftBro, BStarNode* rightBro) {

    BStarNode* brLeft, * brRight;
    if (rightBro) brLeft = node, brRight = rightBro;
    else brLeft = leftBro, brRight = node, ind--;

    BStarNode* parent = brLeft->parent;

    vector<string> keys;
    vector<BStarNode*> children;

    brLeft->addKeysToVector(keys);
    brLeft->addChildrenToVector(children);
    keys.push_back(brLeft->parent->keys[ind]);
    brRight->addKeysToVector(keys);
    brRight->addChildrenToVector(children);

    int num1 = (2 * m - 2) / 3; // br kljuceva u brLeft
    int num2 = (2 * m - 1) / 3; // br kljuceva u brRight
    int num3 = 2 * m / 3; // br kljuceva u novom cvoru

    // shiftujemo u roditelju sve posle indeksa ind u desno za 1
    parent->shiftRight(ind + 1, 1);

    auto new1 = new BStarNode(this, brLeft->parent, brLeft->leaf);
    auto new2 = new BStarNode(this, brLeft->parent, brLeft->leaf);
    auto new3 = new BStarNode(this, brLeft->parent, brLeft->leaf);

    int indVectors = 0;

    for (int i = 0; i < num1; i++) {
        new1->keys[i] = keys[indVectors];
        new1->children[i] = children[indVectors];
        if (children[indVectors])
            children[indVectors]->parent = new1;
        indVectors++;
    }
    new1->children[num1] = children[indVectors];
    if (children[indVectors])
        children[indVectors]->parent = new1;
    new1->numKeys = num1;

    parent->keys[ind] = keys[indVectors++];

    for (int i = 0; i < num2; i++) {
        new2->keys[i] = keys[indVectors];
        new2->children[i] = children[indVectors];
        if (children[indVectors])
            children[indVectors]->parent = new2;
        indVectors++;
    }
    new2->children[num2] = children[indVectors];
    if (children[indVectors])
        children[indVectors]->parent = new2;
    new2->numKeys = num2;

    parent->keys[ind + 1] = keys[indVectors++];

    for (int i = 0; i < num3; i++) {
        new3->keys[i] = keys[indVectors];
        new3->children[i] = children[indVectors];
        if (children[indVectors])
            children[indVectors]->parent = new3;
        indVectors++;
    }
    new3->children[num3] = children[indVectors];
    if (children[indVectors])
        children[indVectors]->parent = new3;
    new3->numKeys = num3;

    parent->children[ind] = new1;
    parent->children[ind + 1] = new2;
    parent->children[ind + 2] = new3;

    BStarNode* next = brLeft->parent;

    delete brLeft;
    delete brRight;

    return next;
}

void BStarTree::breakRoot() {
    auto newRoot = new BStarNode(this, nullptr, false);
    auto new1 = new BStarNode(this, newRoot, root->leaf);
    auto new2 = new BStarNode(this, newRoot, root->leaf);

    int ind = (root->numKeys - 1) / 2;
    int num1 = ind;
    int num2 = root->numKeys - 1 - ind;

    newRoot->keys[0] = root->keys[ind];
    newRoot->children[0] = new1;
    newRoot->children[1] = new2;
    newRoot->numKeys = 1;

    for (int i = 0; i < num1; i++) {
        new1->keys[i] = root->keys[i];
        new1->children[i] = root->children[i];
        if (root->children[i])
            root->children[i]->parent = new1;
    }
    new1->children[num1] = root->children[num1];
    if (root->children[num1])
        root->children[num1]->parent = new1;
    new1->numKeys = num1;

    for (int i = 0; i < num2; i++) {
        new2->keys[i] = root->keys[ind + i + 1];
        new2->children[i] = root->children[ind + i + 1];
        if (root->children[ind + i + 1])
            root->children[ind + i + 1]->parent = new2;
    }
    new2->children[num2] = root->children[ind + num2 + 1];
    if (root->children[ind + num2 + 1]) {
        root->children[ind + num2 + 1]->parent = new2;
    }
    new2->numKeys = num2;

    delete root;
    root = newRoot;
}

int BStarTree::numberOfSmaller(const string& key) const {
    int res = 0;
    queue<BStarNode*> q;
    BStarNode* node = root;
    while (node) {
        int ind = node->upperBound(key);
        res += ind;
        for (int i = 0; i < ind; i++) q.push(node->children[i]);
        node = node->children[ind];
    }
    while (!q.empty()) {
        node = q.front();
        q.pop();
        if (node) {
            res += node->numKeys;
            if (!node->isLeaf()) {
                for (int i = 0; i <= node->numKeys; i++)
                    q.push(node->children[i]);
            }
        }
    }
    return res;
}

BStarNode* BStarTree::merge2to1(BStarNode* left, BStarNode* right) {
    BStarNode* parent = left->parent;
    int indLeftParent = left->indexInParent();

    auto newNode = new BStarNode(this, parent, left->leaf);
    int indNewNode = 0;

    for (int i = 0; i < left->numKeys; i++) {
        newNode->keys[indNewNode] = left->keys[i];
        newNode->children[indNewNode] = left->children[i];
        if (newNode->children[indNewNode])
            newNode->children[indNewNode]->parent = newNode;
        indNewNode++;
    }
    newNode->keys[indNewNode] = parent->keys[indLeftParent];
    newNode->children[indNewNode] = left->children[left->numKeys];
    if (newNode->children[indNewNode])
        newNode->children[indNewNode]->parent = newNode;
    indNewNode++;
    for (int i = 0; i < right->numKeys; i++) {
        newNode->keys[indNewNode] = right->keys[i];
        newNode->children[indNewNode] = right->children[i];
        if (newNode->children[indNewNode])
            newNode->children[indNewNode]->parent = newNode;
        indNewNode++;
    }
    newNode->keys[indNewNode] = parent->keys[indLeftParent];
    newNode->children[indNewNode] = right->children[right->numKeys];
    if (newNode->children[indNewNode])
        newNode->children[indNewNode]->parent = newNode;

    newNode->numKeys = left->numKeys + right->numKeys + 1;

    parent->shiftLeft(indLeftParent);
    parent->children[indLeftParent] = newNode;

    delete left;
    delete right;

    return parent;
}

BStarNode* BStarTree::merge3to2(BStarNode* left, BStarNode* center, BStarNode* right) {
    BStarNode* parent = left->parent;
    int indLeftParent = left->indexInParent();
    int indCenterParent = indLeftParent + 1;

    auto new1 = new BStarNode(this, parent, left->leaf);
    auto new2 = new BStarNode(this, parent, left->leaf);

    vector<string> keys;
    vector<BStarNode*> children;

    left->addKeysToVector(keys);
    keys.push_back(parent->keys[indLeftParent]);
    center->addKeysToVector(keys);
    keys.push_back(parent->keys[indCenterParent]);
    right->addKeysToVector(keys);

    left->addChildrenToVector(children);
    center->addChildrenToVector(children);
    right->addChildrenToVector(children);

    int numKeys = (int)keys.size();
    int num1 = (numKeys - 1) / 2;
    int num2 = numKeys - num1 - 1;

    parent->shiftLeft(indLeftParent);

    int indVectors = 0;
    for (int i = 0; i < num1; i++) {
        new1->keys[i] = keys[indVectors];
        new1->children[i] = children[indVectors];
        if (new1->children[i])
            new1->children[i]->parent = new1;
        indVectors++;
    }
    new1->children[num1] = children[indVectors];
    if (new1->children[num1])
        new1->children[num1]->parent = new1;
    parent->keys[indLeftParent] = keys[indVectors];
    indVectors++;

    for (int i = 0; i < num2; i++) {
        new2->keys[i] = keys[indVectors];
        new2->children[i] = children[indVectors];
        if (new2->children[i])
            new2->children[i]->parent = new2;
        indVectors++;
    }
    new2->children[num2] = children[indVectors];
    if (new2->children[num2])
        new2->children[num2]->parent = new2;

    parent->children[indLeftParent] = new1;
    parent->children[indCenterParent] = new2;

    new1->numKeys = num1;
    new2->numKeys = num2;

    delete left;
    delete right;

    return parent;
}