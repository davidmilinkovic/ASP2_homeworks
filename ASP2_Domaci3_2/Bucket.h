#pragma once

#include "Student.h"

class Bucket {
    static int bucketCount;
    struct BucketItem {
        int key;
        Student* student;
        explicit BucketItem(int _key = 0, Student* _student = nullptr) : key(_key), student(_student){};
    };
    BucketItem* arr;
    int itemCount = 0;
    int size;
    int depth;
    void deleteAt(int index);
    void addAt(int index, int key, Student* student);
    int indexOfKey(int k) const;
public:
    int id = bucketCount++;
    explicit Bucket(int bucketSize, int depth) {
        this->size = bucketSize;
        this->arr = new BucketItem[size];
        this->depth = depth;
    }
    ~Bucket() {
        delete arr;
    }
    Student* findKey(int k) const;
    void deleteKey(int key);
    void addKey(int key, Student* student);
    bool isFull() const;
    bool isFree(int index) const;
    int getItemCount() const;
    int getDepth() const;
    void clear();
    friend std::ostream& operator<<(std::ostream& os, const Bucket& b);
    Bucket** split();
    void printKeys();
    friend Bucket* mergeBuckets(Bucket* b1, Bucket* b2);
};