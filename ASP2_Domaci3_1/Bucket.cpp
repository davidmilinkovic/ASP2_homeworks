#include "Bucket.h"


int Bucket::indexOfKey(int k) const {
    for (int i = 0; i < size; i++)
        if (arr[i].key == k && !arr[i].deleted)
            return i;
    return -1;
}

Student* Bucket::findKey(int k) const {
    int i = indexOfKey(k);
    return i == -1 ? nullptr : arr[i].student;
}

void Bucket::deleteAt(int index) {
    if (!arr[index].deleted && arr[index].student) {
        realItemCount--;
        arr[index].deleted = true;
        arr[index].student = nullptr;
        arr[index].key = 0;
    }
}

void Bucket::deleteKey(int key)
{
    int i = indexOfKey(key);
    if (i != -1) deleteAt(i);
}

void Bucket::addAt(int index, int key, Student* student) {
    if (arr[index].student) deleteAt(index);
    realItemCount++;
    itemCount++;
    arr[index].deleted = false;
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
    return realItemCount == size;
}

bool Bucket::hasNext() const {
    return itemCount == size;
}

bool Bucket::isFree(int index) const {
    return arr[index].deleted || !arr[index].student;
}

bool Bucket::isDeleted(int index) const
{
    return arr[index].deleted;
}

int Bucket::getItemCount() const {
    return itemCount;
}

int Bucket::getRealItemCount() const {
    return realItemCount;
}

void Bucket::clear()
{
    realItemCount = itemCount = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i].student) {
            delete arr[i].student;            
            arr[i].student = nullptr;
            arr[i].key = 0;
            arr[i].deleted = false;
        }
    }
}

int Bucket::getFirstKey() const
{
    for (int i = 0; i < size; i++) {
        if (!arr[i].deleted && arr[i].student)
            return arr[i].key;
    }
    return 0;
}

std::ostream& operator<<(std::ostream& os, const Bucket& b)
{
    for (int i = 0; i < b.size; i++) {
        os << "\t";
        Student* s = b.arr[i].student;
        if (s && !b.isDeleted(i)) os << *s;
        else if (b.isDeleted(i)) os << "[DELETED]\n";
        else os << "[EMPTY]\n";
    }
    return os;
}