#pragma once

#include "Student.h"
#include "AddressFunction.h"
#include "Bucket.h"

class HashTable {
    Bucket** table;
    int bucketSize;
    int size;
    int p;
    int count;
    AddressFunction* addressFunction;
    int hashFunction(int key) const;
public:
    HashTable(int bucketSize, int p, AddressFunction* addressFunction);
    HashTable(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;
    ~HashTable();

    Student* findKey(int k) const;
    bool insertKey(int k, Student* student);
    bool deleteKey(int k);
    void clear();
    int keyCount() const;
    int tableSize() const;
    double fillRatio() const;
    friend std::ostream& operator<<(std::ostream& os, const HashTable& table);
};


