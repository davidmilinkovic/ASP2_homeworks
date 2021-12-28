#pragma once

#include "Student.h"

class Bucket {    
    struct BucketItem {
        int key;
        Student* student;
        bool deleted;
        explicit BucketItem(int _key = 0, Student* _student = nullptr, bool _deleted = false) : key(_key), student(_student), deleted(_deleted) {};
    };
    BucketItem* arr;
    int itemCount = 0; // includes deleted
    int realItemCount = 0; // doesn't include deleted
    int size;
    void deleteAt(int index);
    void addAt(int index, int key, Student* student);
    int indexOfKey(int k) const;
public:
    explicit Bucket(int bucketSize) {
        this->size = bucketSize;
        this->arr = new BucketItem[size];
    }
    ~Bucket() {
        delete arr;
    }
    Student* findKey(int k) const;
    void deleteKey(int key);
    void addKey(int key, Student* student);
    bool isFull() const;
    bool hasNext() const;
    bool isFree(int index) const;
    bool isDeleted(int index) const;
    int getItemCount() const;
    int getRealItemCount() const;
    void clear();
    int getFirstKey() const;
    friend std::ostream& operator<<(std::ostream& os, const Bucket& b);
};