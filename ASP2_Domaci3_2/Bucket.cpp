#include "Bucket.h"

int Bucket::bucketCount = 0;

int Bucket::indexOfKey(int k) const {
    for (int i = 0; i < size; i++)
        if (arr[i].key == k)
            return i;
    return -1;
}

Student* Bucket::findKey(int k) const {
    int i = indexOfKey(k);
    return i == -1 ? nullptr : arr[i].student;
}

void Bucket::deleteAt(int index) {
    if (arr[index].student) {
        arr[index].student = nullptr;
        arr[index].key = 0;
        itemCount--;
    }
}

void Bucket::deleteKey(int key)
{
    int i = indexOfKey(key);
    if (i != -1) deleteAt(i);
}

void Bucket::addAt(int index, int key, Student* student) {
    if (arr[index].student) deleteAt(index);
    itemCount++;
    arr[index].key = key;
    arr[index].student = student;
}

void Bucket::addKey(int key, Student* student)
{
    if (isFull()) return;
    for (int i = 0; i < size; i++) {
        if (isFree(i)) {
            addAt(i, key, student);
            break;
        }
    }
}

bool Bucket::isFull() const {
    return itemCount == size;
}

bool Bucket::isFree(int index) const {
    return !arr[index].student;
}

int Bucket::getItemCount() const {
    return itemCount;
}

int Bucket::getDepth() const
{
    return depth;
}

std::ostream& operator<<(std::ostream& os, const Bucket& b)
{
    for (int i = 0; i < b.size; i++) {
        os << "\t";
        Student* s = b.arr[i].student;
        if (s) os << *s;
        else os << "[EMPTY]\n";
    }
    return os;
}

Bucket* mergeBuckets(Bucket* b1, Bucket* b2)
{
    Bucket* newBucket = new Bucket(b1->size, b1->depth - 1);
    for (int i = 0; i < b1->size; i++) {
        if (b1->arr[i].student) {
            newBucket->addKey(b1->arr[i].key, b1->arr[i].student);
            b1->arr[i].student = nullptr;
        }
    }
    for (int i = 0; i < b2->size; i++) {
        if (b2->arr[i].student) {
            newBucket->addKey(b2->arr[i].key, b2->arr[i].student);
            b2->arr[i].student = nullptr;
        }
    }
    return newBucket;
}

void Bucket::clear()
{
    itemCount = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i].student) {
            delete arr[i].student;
            arr[i].student = nullptr;
            arr[i].key = 0;
        }
    }
}

Bucket** Bucket::split()
{
    Bucket* b0 = new Bucket(size, depth + 1);
    Bucket* b1 = new Bucket(size, depth + 1);
    for (int i = 0; i < size; i++) {
        if (arr[i].student) {
            if (arr[i].key & (1 << depth)) b1->addKey(arr[i].key, arr[i].student);
            else b0->addKey(arr[i].key, arr[i].student);
            arr[i].student = nullptr;
        }
    }
    Bucket** ret = new Bucket * [2];
    ret[0] = b0;
    ret[1] = b1;
    std::cout << *b0;
    std::cout << *b1;
    return ret;
}

void Bucket::printKeys()
{
    for (int i = 0; i < size; i++) {
        if (arr[i].student) std::cout << arr[i].key;
        else std::cout << "[]";
        std::cout << " ";
    }
}
