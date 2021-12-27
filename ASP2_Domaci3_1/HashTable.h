#pragma once

#include "Student.h"
#include "AddressFunction.h"

class HashTable {
    struct BucketItem {
        int key;
        Student* student;
        bool deleted;
        explicit BucketItem(int _key = 0, Student* _student = nullptr, bool _deleted = false) : key(_key), student(_student), deleted(_deleted) {};
    };
    class Bucket {
        BucketItem* arr;
        int itemCount = 0; // includes deleted
        int realItemCount = 0; // doesn't include deleted
        int size;
    public:
        explicit Bucket(int bucketSize) {
            this->size = bucketSize;
            this->arr = new BucketItem[size];
        }
        ~Bucket() {
            delete arr;
        }

        bool isFull() const;
        int indexOfKey(int k) const;
        Student* findKey(int k) const;
        Student* itemAt(int index);
        void deleteAt(int index);
        void addItemAt(int index, int key, Student* student);
        bool hasNext() const;
        bool isFree(int index) const;

        int getItemCount() const;

        int getRealItemCount() const;
    };
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


