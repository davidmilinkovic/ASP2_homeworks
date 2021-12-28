#pragma once

#include "Student.h"
#include "Bucket.h"

class ExtendibleHashTable {
    Bucket** table;
    int bucketSize;
    int size;
    int p;
    int initialP;
    int count;
public:
    int hashFunction(int key) const;
    ExtendibleHashTable(int bucketSize, int p);
    ExtendibleHashTable(const ExtendibleHashTable&) = delete;
    ExtendibleHashTable(ExtendibleHashTable&&) = delete;
    ~ExtendibleHashTable();

    Student* findKey(int k) const;
    bool insertKey(int k, Student* student);
    bool deleteKey(int k);
    void clear();
    int keyCount() const;
    int tableSize() const;
    double fillRatio() const;
    void expandTable();
    void shrinkTable();
    friend std::ostream& operator<<(std::ostream& os, const ExtendibleHashTable& table);
    void printBuckets() const;
};


