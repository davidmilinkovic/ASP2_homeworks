//
// Created by md200037d on 26-Dec-21.
//

#include "HashTable.h"


Student* HashTable::findKey(int k) const {
    int initialAddress = hashFunction(k);
    int address = initialAddress;    
    int i = 0;
    Student* ptr = nullptr;
    do {
        ptr = table[address]->findKey(k);
        if (ptr) return ptr;
        if (table[address]->hasNext()) {
            int oldKey = table[address]->getFirstKey();
            address = addressFunction->getAddress(k, initialAddress, ++i, size, oldKey);
            if (address == initialAddress) return nullptr;
        }
        else return nullptr;
    } while (ptr == nullptr);
}


std::ostream& operator<<(std::ostream& os, const HashTable& table) {
    for (int i = 0; i < table.size; i++) {
        os << "ULAZ " << i << ": " << std::endl;
        os << *table.table[i];
    }
    return os;
}

double HashTable::fillRatio() const {
    return (double)count / (size * bucketSize);
}

int HashTable::tableSize() const {
    return size * bucketSize;
}

int HashTable::keyCount() const {
    return count;
}

void HashTable::clear() {
    count = 0;
    for (int i = 0; i < size; i++) {
        table[i]->clear();
    }        
}

bool HashTable::deleteKey(int k) {
    int initialAddress = hashFunction(k);
    int address = initialAddress;
    int i = 0;
    Student* ptr = nullptr;
    do {
        ptr = table[address]->findKey(k);
        if (ptr) {
            table[address]->deleteKey(k);
            count--;
            return true;
        }
        if (table[address]->hasNext()) {
            int oldKey = table[address]->getFirstKey();
            address = addressFunction->getAddress(k, initialAddress, ++i, size, oldKey);
            if (address == initialAddress) return false;
        }
        else return false;
    } while (address != initialAddress);
    return false;
}

bool HashTable::insertKey(int k, Student* student) {
    if (findKey(k)) return false;
    int initialAddress = hashFunction(k);
    int address = initialAddress;
    int i = 0;
    do {
        if (table[address]->isFull()) {
            int oldKey = table[address]->getFirstKey();
            address = addressFunction->getAddress(k, initialAddress, ++i, size, oldKey);
        }
        else {
            count++;
            table[address]->addKey(k, student);
            return true;
        }
    } while (address != initialAddress);
    return false;
}

HashTable::HashTable(int bucketSize, int p, AddressFunction* addressFunction) {
    this->bucketSize = bucketSize;
    this->p = p;
    this->size = 1 << p;
    this->count = 0;
    this->table = new Bucket * [size];
    for (int i = 0; i < size; i++) {
        this->table[i] = new Bucket(bucketSize);
    }
    this->addressFunction = addressFunction;
}

HashTable::~HashTable() {
    for (int i = 0; i < size; i++) {
        delete this->table[i];
    }
    delete table;
}

int HashTable::hashFunction(int key) const {
    return key % size;
}
