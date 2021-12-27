#include <iostream>
#include <string>
#include "BStarTree.h"
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

void printSeparator();
int inputInt();
void enterToContinue();
bool checkBst(BStarTree*& tree);

void menuCreateTree(BStarTree*& pTree);
void menuSearchNode(BStarTree* pTree);
void menuInsertNode(BStarTree* pTree);
void menuRemoveNode(BStarTree* pTree);
void menuPrintTree(BStarTree* pTree);
void menuNumSmaller(BStarTree* pTree);
void menuDeleteTree(BStarTree*& pTree);

int main()
{
    BStarTree* tree = nullptr;
    while (true) {
        system("CLS");
        cout << "Implementacija B* stabla" << endl;
        printSeparator();
        cout << endl;

        cout << "1. Formiranje stabla sa zadatim redom" << endl;
        cout << "2. Pretraga stabla na zadai kljuc" << endl;
        cout << "3. Umetanje novog kljuca u stablo" << endl;
        cout << "4. Brisanje zadatog kljuca" << endl;
        cout << "5. Ispis sadrzaja stabla" << endl;
        cout << "6. Broj leksikografski manjih kljuceva od zadatog" << endl;
        cout << endl;
        cout << "7. Brisanje stabla" << endl;
        cout << endl;
        cout << "0. kraj rada" << endl << endl;

        printSeparator();
        cout << endl << "Unesite zeljenu opciju: ";
        int option = inputInt();

        if (option > 1)
            if (!checkBst(tree)) continue;

        switch (option) {
        case 1:
            menuCreateTree(tree);
            break;
        case 2:
            menuSearchNode(tree);
            break;
        case 3:
            menuInsertNode(tree);
            break;
        case 4:
            menuRemoveNode(tree);
            break;
        case 5:
            menuPrintTree(tree);
            break;
        case 6:
            menuNumSmaller(tree);
            break;
        case 7:
            menuDeleteTree(tree);
            break;
        case 0:
            return 0;
        case -1:
            cout << endl << "Opcija nije validna!" << endl;
            enterToContinue();
            break;
        default:
            continue;
        }
    }
}

void menuDeleteTree(BStarTree*& pTree) {
    system("CLS");
    delete pTree;
    pTree = nullptr;
    cout << endl << "Stablo je uspesno izbrisano";
    enterToContinue();
}

void menuNumSmaller(BStarTree* pTree) {
    system("CLS");
    cout << "Unesite kljuc za pretragu broja manjih kljuceva: ";
    string s;
    cin >> s;
    int smaller = pTree->numberOfSmaller(s);
    cout << endl << "Broj kljuceva manjih od " << s << " je: " << smaller << endl;
    enterToContinue();
}

void menuPrintTree(BStarTree* pTree) {
    system("CLS");
    cout << *pTree;
    cout << endl;
    enterToContinue();
}

void menuRemoveNode(BStarTree* pTree) {
    system("CLS");
    while (true) {
        cout << "Na koji nacin zelite da ucitate kljuceve [0 - standardni ulaz, 1 - datoteka]: ";
        int mode;
        cin >> mode;
        if (mode == 0) {
            int cnt;
            cout << "Unesite broj kljuceva za brisanje: ";
            cin >> cnt;
            for (int i = 1; i <= cnt; i++) {
                cout << i << ". kljuc: ";
                string s;
                cin >> s;
                bool deleted = pTree->removeKey(s);
                if (deleted) cout << endl << *pTree << endl << endl;
                else cout << endl << "Kljuc se ne nalazi u stablu." << endl;
            }
            break;
        }
        else if (mode == 1) {
            string fileName;
            while (true) {
                cout << "Unesite naziv datoteke: ";
                cin >> fileName;
                ifstream is;
                is.open(fileName);
                if (!is.is_open()) continue;
                string key;
                int cnt = 0;
                while (!is.eof()) {
                    is >> key;
                    cnt += pTree->removeKey(key);
                }
                cout << endl << cnt << " kljuceva uspesno izbrisano!" << endl;
                break;
            }
            break;
        }
    }
    enterToContinue();
}

void menuInsertNode(BStarTree* pTree) {
    system("CLS");
    while (true) {
        cout << "Na koji nacin zelite da ucitate kljuceve [0 - standardni ulaz, 1 - datoteka]: ";
        int mode;
        cin >> mode;
        if (mode == 0) {
            int cnt;
            cout << "Unesite broj kljuceva za ubacivanje: ";
            cin >> cnt;
            for (int i = 1; i <= cnt; i++) {
                cout << i << ". kljuc: ";
                string s;
                cin >> s;
                bool inserted = pTree->addKey(s);
                if (inserted) cout << endl << *pTree << endl << endl;
                else cout << endl << "Kljuc se vec nalazi u stablu." << endl;
            }
            break;
        }
        else if (mode == 1) {
            string fileName;
            while (true) {
                cout << "Unesite naziv datoteke: ";
                cin >> fileName;
                ifstream is;
                is.open(fileName);
                if (!is.is_open()) continue;
                int cnt = 0;
                while (!is.eof()) {
                    string key;
                    is >> key;
                    cnt += pTree->addKey(key);
                }
                is.close();
                cout << endl << cnt << " kljuceva uspesno dodato!" << endl;
                break;
            }
            break;
        }
    }
    enterToContinue();
}

void menuSearchNode(BStarTree* pTree) {
    system("CLS");
    cout << "Unesite kljuc za pretragu: ";
    string s;
    cin >> s;
    bool found = pTree->hasKey(s);
    if (found) cout << endl << "Kljuc se nalazi u stablu" << endl;
    else cout << endl << "Kljuc se ne nalazi u stablu" << endl;
    enterToContinue();
}

void menuCreateTree(BStarTree*& pTree) {
    system("CLS");
    cout << "Unesite red stabla (m): ";
    int m;
    cin >> m;
    if (m < 3 || m > 10) {
        cout << endl << "Red nije validan!" << endl;
    }
    else {
        pTree = new BStarTree(m);
        cout << endl << "Stablo uspesno kreirano!" << endl;
    }
    enterToContinue();
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
    if (isNumber) return stoi(x);
    return -1;
}

void enterToContinue()
{
    cout << endl << "Pritisnite enter za nastavak..." << endl;
    getchar();
    getchar();
}

bool checkBst(BStarTree*& tree) {
    if (tree == nullptr) {
        cout << endl << "GRESKA: Stablo nije uneto!" << endl;
        enterToContinue();
        return false;
    }
    return true;
}
