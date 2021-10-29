#include <iostream>
#include <list>
#include <fstream>
#include <stack>
#include <queue>
#include <windows.h>
#include <iomanip>

using namespace std;

struct Node {
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
    string key;
    list<string> values;
};

struct BST {
    Node* root;
};

// BST related
BST* BSTInit();
void BSTInsert(BST* bst, string key, string value);
bool BSTRemove(BST* bst, string key);
Node* BSTSearch(BST* bst, string key);
void BSTDeleteTree(BST*& bst);

// tree construction
BST* createTree(list<pair<string, string>> words);
list<pair<string, string>> readData(istream& is, bool shouldOutput);

// menu items
void menuCreateTree(BST*& bst);
void menuSearchNode(BST*& bst);
void menuPrintTree(BST*& bst);
void menuInsertNode(BST*& bst);
void menuRemoveNode(BST*& bst);
void menuDeleteTree(BST*& bst);
void menuPrefix(BST*& bst);

// output
void printNode(Node* n);
void printInorder(Node* root);
void printTree(BST* tree, int width);

// misc
void printSeparator();
int inputInt();
void enterToContinue();
bool checkBst(BST*& bst);
void freeNode(Node* n);

int main()
{    
    BST* bst = nullptr;
    while (true) {
        system("CLS");
        cout << "Implementacija recnika pomocu binarnog stabla pretrage" << endl;
        printSeparator();
        cout << endl;

        cout << "1. Formiranje stabla na osnovu zadatog skupa reci" << endl;
        cout << "2. Pretraga stabla na zadatu rec" << endl;
        cout << "3. Umetanje novog prevoda u stablo" << endl;
        cout << "4. Ispis sadrzaja stabla" << endl;
        cout << "5. Brisanje zadatog kljuca" << endl;
        cout << "6. Brisanje stabla iz memorije" << endl;
        cout << endl;
        cout << "7. Ispis reci sa zadatim prefiksom" << endl;
        cout << endl;
        cout << "0. kraj rada" << endl << endl;

        printSeparator();
        cout << endl << "Unesite zeljenu opciju: ";
        int option = inputInt();
        
        if(option > 1)
            if (!checkBst(bst)) continue;

        switch (option) {
        case 1:
            menuCreateTree(bst);
            break;
        case 2:            
            menuSearchNode(bst);
            break;
        case 3:
            menuInsertNode(bst);
            break;
        case 4:                       
            menuPrintTree(bst);
            break;
        case 5:
            menuRemoveNode(bst);
            break;
        case 6:
            menuDeleteTree(bst);
            break;
        case 7:
            menuPrefix(bst);
            break;
        case 0:
            return 0;
        case -1:
            cout << endl << "Opcija nije validna!" << endl;
            enterToContinue();
            break;
        default:
            continue;
            break;
        }
    }

    return 0;
}

void printSeparator() {
    cout << "====================================================================" << endl;
}

inline int inputInt() {
    string x;
    cin >> x;
    string::const_iterator it = x.begin();
    while (it != x.end() && isdigit(*it)) ++it;
    bool isNumber = !x.empty() && it == x.end();
    if (isNumber) return atoi(x.c_str());
    return -1;
}

void enterToContinue()
{
    cout << endl << "Pritisnite enter za nastavak..." << endl;
    char x = getchar();    
    x = getchar();
}

bool checkBst(BST*& bst){
    if (bst == nullptr) {
        cout << endl << "GRESKA: Stablo nije uneto!" << endl;
        enterToContinue();
        return false;
    }
    return true;
}

void freeNode(Node* n)
{    
    delete n;
}

BST* BSTInit()
{
    BST* bst = new BST();
    bst->root = nullptr;
    return bst;
}

void BSTInsert(BST* bst, string key, string value) {
    Node *parent = nullptr, *n = bst->root;
    while (n != nullptr && n->key != key) {    
        parent = n;
        if (key > n->key) n = n->right;
        else n = n->left;
    }
    if (n != nullptr) {
        n->values.push_back(value);
    }
    else {
        Node* node = new Node();
        node->parent = parent;
        node->values = list<string>();
        node->values.push_back(value);
        node->key = key;
        if (parent == nullptr) bst->root = node;
        else if (key < parent->key) parent->left = node;
        else parent->right = node;
    }
}

bool BSTRemove(BST* bst, string key) {
    Node * n = bst->root;
    while (n != nullptr && n->key != key) {
        if (key < n->key) n = n->left;
        else n = n->right;
    }
    if (!n) return false;
    Node* parent = n->parent, *repl = nullptr;
    if (!n->left) repl = n->right;
    else if (!n->right) repl = n->left;
    else {
        // looking for successor
        repl = n->right;
        while (repl->left)
            repl = repl->left;

        if(repl->parent != n) {
            repl->parent->left = repl->right;
            if(repl->right) repl->right->parent = repl->parent;
            
            repl->right = n->right;
            if (n->right) n->right->parent = repl;
        }
        repl->left = n->left;
        if (n->left) n->left->parent = repl;
    }
    if (repl) repl->parent = parent;
    if (parent) {
        if (n->key > parent->key) parent->right = repl;
        else parent->left = repl;
    }
    else bst->root = repl;
    freeNode(n);
}

Node* BSTSearch(BST* bst, string key) {
    Node* n = bst->root;
    while (n != nullptr && n->key != key) {
        if (key < n->key) n = n->left;
        else n = n->right;
    }
    return n;
}

void BSTDeleteTree(BST*& tree) {
    if (tree == nullptr) return;
    stack<Node*> s;
    s.push(tree->root);
    while (!s.empty()) {
        Node* n = s.top();
        s.pop();
        if (n) {
            if (n->left != nullptr) s.push(n->left);
            if (n->right != nullptr) s.push(n->right);
            freeNode(n);
        }
    }
    delete tree;
    tree = nullptr;
}

BST* createTree(list<pair<string, string>> words) {
    BST* bst = BSTInit();
    for (auto word : words) {
        BSTInsert(bst, word.first, word.second);
    }
    return bst;
}

void menuCreateTree(BST*& bst) {
    BSTDeleteTree(bst);
    while(true) {
        system("CLS");
        cout << "Na koji nacin zelite da ucitate recnik?" << endl;
        printSeparator();
        cout << endl;
        cout << "1. Ucitavanjem iz datoteke" << endl;
        cout << "2. Ucitavanjem sa standardnog ulaza" << endl;
        cout << endl << "0. nazad";
        cout << endl;
        printSeparator();
        cout << endl << "Unesite zeljenu opciju: ";
        int option = inputInt();    
        if (option == 1 || option == 2) {
            list<pair<string, string>> words;
            if (option == 1) {
                cout << endl << "Unesite naziv datoteke: ";
                char filename[255];
                cin >> filename;
                ifstream file(filename);
                if (!file) {
                    cout << endl << "Trazeni fajl ne postoji" << endl;
                    enterToContinue();
                    continue;
                }
                words = readData(file, false);
                file.close();                
            }
            else {
                words = readData(cin, true);
            }
            bst = createTree(words);
            cout << endl << "Stablo je uspesno kreirano sa " << words.size() << " reci!" << endl;
            enterToContinue();
            return;
        }        
        else if (option == 0) {
            return;
        }
    }    
}

void menuSearchNode(BST*& bst)
{
    system("CLS");
    cout << "Rec za pretragu: ";
    string key;
    cin >> key;
    Node* n = BSTSearch(bst, key);
    if (n == nullptr)
        cout << endl << "Trazena rec nije pronadjena" << endl;
    else
        printNode(n);
    enterToContinue();
}

void menuPrintTree(BST*& bst)
{
    while (true) {
        system("CLS");
        cout << "Prikaz stabla" << endl;
        printSeparator();
        cout << endl << "Na koji nacin zelite da prikazete stablo?" << endl << endl;
        cout << "1. Inorder sa prikazom svih prevoda" << endl;
        cout << "2. Struktura stabla (samo kljucevi)" << endl;
        cout << endl << "0. nazad";
        cout << endl;
        printSeparator();
        cout << endl << endl;
        cout << "Unesite zeljenu opciju: ";
        int option = inputInt();
        if (option == 1) {
            system("CLS");
            printInorder(bst->root);
            enterToContinue();
            return;
        }
        else if (option == 2) {
            cout << "Unesite sirinu kljuca pri stampanju: ";
            int width;
            cin >> width;
            system("CLS");            
            printTree(bst, width);
            enterToContinue();
            return;
        }
        else if (option == 0) {
            return;
        }
    }
}

void menuInsertNode(BST*& bst)
{
    system("CLS");
    cout << "Dodavanje novog prevoda" << endl;
    printSeparator();
    string key, value;
    cout << "Unesite rec koju prevodite: ";
    cin >> key;
    cout << "Unesite njen prevod: ";
    cin >> value;
    BSTInsert(bst, key, value);
    cout << endl << "Prevod uspesno dodat!" << endl;
    enterToContinue();
}

void menuRemoveNode(BST*& bst)
{
    system("CLS");
    cout << "Brisanje reci iz recnika" << endl;
    printSeparator();
    string key;
    cout << "Unesite rec koju zelite da izbrisete: ";
    cin >> key;
    if (BSTRemove(bst, key))
        cout << endl << "Rec uspesno izbrisana!" << endl;
    else
        cout << endl << "Data rec ne postoji u recniku!" << endl;
    enterToContinue();
}

void menuDeleteTree(BST*& bst)
{
    system("CLS");
    BSTDeleteTree(bst);
    cout << endl << "Brisanje stabla uspesno!" << endl;
    enterToContinue();
}

void menuPrefix(BST*& bst)
{
    system("CLS");
    cout << "Pretraga reci sa zadatim prefiksom" << endl;
    printSeparator();
    cout << "Unesite prefiks za pretragu: ";
    string prefix;
    cin >> prefix;
    cout << endl;
    int len = prefix.size();
    Node* n = bst->root;
    Node* last = nullptr;
    while (n != nullptr && n->key != prefix) {
        if (n->key.substr(0, len) == prefix) last = n;
        if (prefix < n->key) n = n->left;
        else n = n->right;
    }
    if (n) last = n;
    if (!last) {
        cout << "GRESKA: Ne postoji rec sa datim prefiksom!" << endl;
    }
    else {        
        int cnt = 0;
        while (last && last->key.substr(0, len) == prefix) {
            printNode(last);
            cnt++;
            if (last->right) {
                last = last->right;
                while (last->left) last = last->left;
            }
            else {
                while (last->parent && last->parent->left != last)
                    last = last->parent;
                last = last->parent;
            }
        }
        cout << endl << "Ukupno " << cnt << " reci sa datim prefiksom" << endl;
    }    
    enterToContinue();
}

list<pair<string, string>> readData(istream& is, bool shouldOutput)
{    
    if(shouldOutput) cout << "Unesite broj podataka: ";
    int n;
    is >> n;
    if(shouldOutput) printSeparator();
    list<pair<string, string>> words;
    for (int i = 0; i < n; i++) {
        pair<string, string> p;
        if (shouldOutput) cout << "Unesite " << i + 1 << ". rec i njen prevod: ";
        is >> p.first >> p.second;
        words.push_back(p);
    }
    return words;
}

void printNode(Node* n)
{
    cout << n->key << " -> ";
    for (auto iter = n->values.begin(); iter != n->values.end(); iter++) {
        cout << *iter;
        if (next(iter) != n->values.end()) cout << ", ";
    }
    cout << endl;
}

void printInorder(Node* root){
    if (root == nullptr) return;
    printInorder(root->left);
    printNode(root);
    printInorder(root->right);    
}

void printTree(BST* tree, int width) {

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
 
    if (!tree) return;
    if (!tree->root) return;
    else {
        queue<Node*> q;
        int line_len = columns;
        int first_skip = line_len, in_between_skip;

        q.push(tree->root);
        int i = 0;
        while(true) {
            bool realNode = false;
            int j = 1 << i++;
            in_between_skip = (line_len - width * j) / j;
            first_skip = in_between_skip / 2;
            cout << setw(first_skip) << " ";
            for (int k = 0; k < j; k++) {
                Node* btn = q.front();
                q.pop();
                if (btn) {
                    realNode = true;
                    q.push(btn->left);
                    q.push(btn->right);
                }
                else {
                    q.push(nullptr);
                    q.push(nullptr);
                }
                if (btn) cout << setw(width) << btn->key.substr(0, width);                
                else cout << setw(width) << " ";
                if(k != j-1) cout << setw(in_between_skip) << " ";
            }
            cout << endl;
            cout << endl;
            if (!realNode) break;
        }
    }
}
