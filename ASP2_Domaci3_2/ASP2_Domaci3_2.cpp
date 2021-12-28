#include <iostream>
#include <string>
#include "ExtendibleHashTable.h"
#include <bitset>

void printSeparator();

int inputInt();

void enterToContinue();

bool checkTable(ExtendibleHashTable*& table);

void menuCreateTable(ExtendibleHashTable*& pTable);

void menuSearch(ExtendibleHashTable* pTable);

void menuInsertInput(ExtendibleHashTable* pTable);

void menuInsertFile(ExtendibleHashTable* pTable);

void menuRemove(ExtendibleHashTable* pTable);

void menuPrintTable(ExtendibleHashTable* pTable);

void menuAddCourse(ExtendibleHashTable* pTable);

void menuRemoveCourse(ExtendibleHashTable* pTable);

using namespace std;

int main()
{
    ExtendibleHashTable* table = nullptr;
    while (true) {
        system("CLS");
        cout << "Implementacija PROSIRLJIVE hes tabele" << endl;
        printSeparator();
        cout << endl;

        cout << "1. Kreiranje prazne hes tabele" << endl;
        // cout << "111. Vracanje hasha za key" << endl;
        // cout << "112. Ispis za proveru bucketa" << endl;
        // cout << "113. Expand" << endl;
        cout << "2. Pretraga studenta" << endl;
        cout << "3. Umetanje novih studenata sa standardnog ulaza" << endl;
        cout << "4. Umetanje novih studenata iz datoteke" << endl;
        cout << "5. Brisanje studenta" << endl;
        cout << "6. Praznjenje tabele" << endl;
        cout << "7. Broj kljuceva u tabeli" << endl;
        cout << "8. Velicina tabele" << endl;
        cout << "9. Ispis tabele" << endl;
        cout << "10. Stepen popunjenosti tabele" << endl;
        cout << endl;
        cout << "11. Dodavanje ispita u listu prijavljenih" << endl;
        cout << "12. Brisanje ispita iz liste prijavljenih" << endl;
        cout << endl;
        cout << "0. kraj rada" << endl << endl;

        printSeparator();
        cout << endl << "Unesite zeljenu opciju: ";
        int option = inputInt();

        if (option > 1)
            if (!checkTable(table)) continue;

        switch (option) {
        case 1:
            menuCreateTable(table);
            break;
        case 2:
            menuSearch(table);
            break;
        case 3:
            menuInsertInput(table);
            break;
        case 4:
            menuInsertFile(table);
            break;
        case 5:
            menuRemove(table);
            break;
        case 6:
            table->clear();
            cout << "Tabela uspesno ispraznjena!" << endl;
            enterToContinue();
            break;
        case 7:
            cout << "Br. kljuceva u tabeli: " << table->keyCount() << endl;
            enterToContinue();
            break;
        case 8:
            cout << "Velicina tabele: " << table->tableSize() << endl;
            enterToContinue();
            break;
        case 9:
            menuPrintTable(table);
            break;
        case 10:
            cout << "Stepen popunjenosti tabele: " << table->fillRatio() << endl;
            enterToContinue();
            break;
        case 11:
            menuAddCourse(table);
            break;
        case 12:
            menuRemoveCourse(table);
            break;        
        case 111:
            system("CLS");
            while (1) {
                int key;
                cin >> key;
                bitset<8> xx(key);
                bitset<8> yy(table->hashFunction(key));
                cout << xx << endl;
                cout << yy << endl;
            }
            break;
        case 112:
            system("CLS");
            table->printBuckets();
            enterToContinue();
            break;
        case 113:
            table->expandTable();
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

void menuRemoveCourse(ExtendibleHashTable* pTable) {
    system("CLS");
    cout << "Brisanje prijavljenog ispita" << endl;
    printSeparator();
    cout << "Unesite broj indeksa: ";
    int k = inputInt();
    Student* student = pTable->findKey(k);
    if (student) {
        cout << "Unesite sifru predmeta: ";
        string course;
        cin >> course;
        student->removeCourse(course);
    }
    else {
        cout << "Student sa datim indeksom se ne nalazi u tabeli!" << endl;
    }
    enterToContinue();
}

void menuAddCourse(ExtendibleHashTable* pTable) {
    system("CLS");
    cout << "Dodavanje prijavljenog ispita" << endl;
    printSeparator();
    cout << "Unesite broj indeksa: ";
    int k = inputInt();
    Student* student = pTable->findKey(k);
    if (student) {
        cout << "Unesite sifru predmeta: ";
        string course;
        cin >> course;
        student->addCourse(course);
    }
    else {
        cout << "Student sa datim indeksom se ne nalazi u tabeli!" << endl;
    }
    enterToContinue();
}

void menuPrintTable(ExtendibleHashTable* pTable) {
    system("CLS");
    cout << "Ispis tabele" << endl;
    printSeparator();
    cout << *pTable << endl;
    enterToContinue();
}

void menuRemove(ExtendibleHashTable* pTable) {
    system("CLS");
    cout << "Pretraga studenta" << endl;
    printSeparator();
    cout << "Unesite broj indeksa: ";
    int k = inputInt();
    cout << endl;
    bool success = pTable->deleteKey(k);
    if (success)
        cout << "Student uspesno izbrisan!" << endl;
    else
        cout << "Nije moguce pronaci studenta." << endl;
    enterToContinue();
}

void menuInsertFile(ExtendibleHashTable* pTable) {
    system("CLS");
    cout << "Ucitavanje podataka o studentima" << endl;
    printSeparator();
    cout << "Unesite naziv fajla: ";
    string filePath;
    cin >> filePath;
    vector<Student*> students = Student::readFromCsv(filePath);
    set<int> keys;
    for (auto student : students) {
        keys.insert(student->getIndex());
    }
    cout << "Ima ih " << keys.size() << endl;
    int successCount = 0;
    for (auto s : students) {      
        bool success = pTable->insertKey(s->getIndex(), s);
        if (!success) delete s;        
        else successCount++;
    }
    cout << endl << "Uspesno dodato " << successCount << " od " << students.size() << " studenata!";
    enterToContinue();
}

void menuInsertInput(ExtendibleHashTable* pTable) {
    system("CLS");
    cout << "Unos podataka o studentima" << endl;
    printSeparator();
    string name;
    cout << "Unesite broj indeksa (-1 za kraj): ";
    int key = inputInt();
    while (key != -1) {
        cout << "Unesite ime i prezime studenta: ";
        getline(cin >> ws, name);
        Student* student = new Student(key, name);
        cout << "Unesite broj ispita za prijavu: ";
        int numCourses = inputInt();
        for (int i = 1; i <= numCourses; i++) {
            cout << "Unesite sifru " << i << ". predmeta: ";
            string course;
            cin >> course;
            student->addCourse(course);
        }
        bool success = pTable->insertKey(key, student);
        if (success)
            cout << "Student uspesno dodat u tabelu!" << endl;
        else
            cout << "Nije moguce dodati studenta u tabelu." << endl;
        cout << endl;
        pTable->printBuckets();
        cout << endl;
        cout << "Unesite broj indeksa (-1 za kraj): ";
        key = inputInt();
    }
    enterToContinue();
}

void menuSearch(ExtendibleHashTable* pTable) {
    system("CLS");
    cout << "Pretraga studenta" << endl;
    printSeparator();
    cout << "Unesite broj indeksa: ";
    int k = inputInt();
    cout << endl;
    Student* student = pTable->findKey(k);
    if (student) {
        cout << *student << endl;
    }
    else {
        cout << "Student sa datim indeksom se ne nalazi u tabeli!" << endl;
    }
    enterToContinue();
}

void menuCreateTable(ExtendibleHashTable*& pTable) {
    system("CLS");
    cout << "Kreiranje hes tabele" << endl;
    printSeparator();
    cout << "Unesite velicinu baketa [k]: ";
    int k = inputInt();
    cout << "Unesite parametar p (inicijalna velicina tabele ce biti 2^p): ";
    int p = inputInt();
    pTable = new ExtendibleHashTable(k, p);
    cout << endl << "Tabela uspesno kreirana!" << endl;
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

bool checkTable(ExtendibleHashTable*& table) {
    if (table == nullptr) {
        cout << endl << "GRESKA: Tabela nije kreirana!" << endl;
        enterToContinue();
        return false;
    }
    return true;
}