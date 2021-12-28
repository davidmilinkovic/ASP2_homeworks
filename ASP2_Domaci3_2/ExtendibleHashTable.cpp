//
// Created by md200037d on 26-Dec-21.
//

#include "ExtendibleHashTable.h"


Student* ExtendibleHashTable::findKey(int k) const {
    int address = hashFunction(k);
    return table[address]->findKey(k);
}


std::ostream& operator<<(std::ostream& os, const ExtendibleHashTable& table) {
    for (int i = 0; i < table.size; i++) {
        os << "KLJUC " << i << ": " << std::endl;
        os << *table.table[i];
    }
    return os;
}

double ExtendibleHashTable::fillRatio() const {
    return (double)count / (size * bucketSize);
}

void ExtendibleHashTable::expandTable()
{
    p++;
    size *= 2;
    Bucket** newTable = new Bucket * [size];
    for (int i = 0; i < size; i++) {
        newTable[i] = table[i / 2];
    }
    delete table;
    table = newTable;    
}


void ExtendibleHashTable::shrinkTable()
{
    if (p > initialP) {
        p--;
        size /= 2;
        Bucket** newTable = new Bucket * [size];
        for (int i = 0; i < size; i++) {
            newTable[i] = table[2*i];
        }
        delete table;
        table = newTable;        
    }
}


void ExtendibleHashTable::printBuckets() const
{
    for (int i = 0; i < size; i++) {
        std::cout << i << ": " << table[i]->id << "(";
        table[i]->printKeys();
        std::cout << ")\n";
    }
}

int ExtendibleHashTable::tableSize() const {
    return size * bucketSize;
}

int ExtendibleHashTable::keyCount() const {
    return count;
}

void ExtendibleHashTable::clear() {
    count = 0;
    for (int i = 0; i < size; i++) {
        table[i]->clear();
    }
}


bool ExtendibleHashTable::deleteKey(int key) {
    if (!findKey(key)) return false;
    int address = hashFunction(key);
    Bucket* b = table[address];
    b->deleteKey(key);
    int buddyAddress = address ^ 1;
    Bucket* buddy = table[buddyAddress];
    if (buddy->getDepth() == b->getDepth() && buddy != b) {
        if (buddy->getItemCount() + b->getItemCount() <= bucketSize) {
            Bucket* mergedBucket = mergeBuckets(b, buddy);
            delete b;
            delete buddy;
            for (int i = 0; i < size; i++) {
                if (table[i] == b || table[i] == buddy) {
                    table[i] = mergedBucket;
                }
            }            
            bool shoudShrink = true;
            for (int i = 0; i < size; i++) {
                if (table[i]->getDepth() == p) {
                    shoudShrink = false;
                    break;
                }
            }
            if (shoudShrink) shrinkTable();
        }
    }
    count--;
    return true;
}

bool ExtendibleHashTable::insertKey(int k, Student* student) {
    if (findKey(k)) return false;
    while (table[hashFunction(k)]->isFull()) {
        int address = hashFunction(k);
        Bucket* oldBucket = table[address];
        Bucket** newBuckets = table[address]->split();
        Bucket * b0 = newBuckets[0];
        Bucket* b1 = newBuckets[1];
        delete newBuckets;
        if (table[address]->getDepth() == p) {
            expandTable();
        }
        int l = -1, r = -1;
        for (int i = 0; i < size; i++) {
            if (table[i] == oldBucket) {
                if (l == -1) l = i;
                r = i;
            }
        }
        int firstRight = l + (r - l + 1) / 2;
        for (int i = l; i <= r; i++) {
            if (i < firstRight) table[i] = b0;
            else table[i] = b1;
        }
        delete oldBucket;
    }
    table[hashFunction(k)]->addKey(k, student);
    count++;
    return true;
}

ExtendibleHashTable::ExtendibleHashTable(int bucketSize, int p) {
    this->bucketSize = bucketSize;
    this->p = p;
    this->initialP = p;
    this->size = 1 << p;
    this->count = 0;
    this->table = new Bucket * [size];
    for (int i = 0; i < size; i++) {
        this->table[i] = new Bucket(bucketSize, p);
    }
}

ExtendibleHashTable::~ExtendibleHashTable() {
    for (int i = 0; i < size; i++) {
        delete this->table[i];
    }
    delete table;
}

int ExtendibleHashTable::hashFunction(int key) const {
    unsigned int flipped = key % size;
    unsigned int hash = 0;    
    for (int i = 0; i < p; i++) {
        if (flipped & (1 << i))
            hash |= (1 << (p - i - 1));
    }
    return hash;
}
