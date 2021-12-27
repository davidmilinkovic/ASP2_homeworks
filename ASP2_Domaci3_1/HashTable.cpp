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
            address = addressFunction->getAddress(k, initialAddress, ++i, size);
            if (address == initialAddress) return nullptr;
        }
        else return nullptr;
    } while (ptr == nullptr);
}

std::ostream& operator<<(std::ostream& os, const HashTable& table) {
    for (int i = 0; i < table.size; i++) {
        os << "KLJUC " << i << ": " << std::endl;
        for (int j = 0; j < table.bucketSize; j++) {
            Student* s = table.table[i]->itemAt(j);
            if (s) os << "\t" << *s;
        }
        os << std::endl;
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
    // TODO
}

bool HashTable::deleteKey(int k) {
    return false;
}

bool HashTable::insertKey(int k, Student* student) {
    int hash = hashFunction(k);
    Bucket* bucket = table[hash];
    if (bucket->findKey(k) != nullptr || bucket->isFull()) return false;
    for (int i = 0; i < bucketSize; i++) {
        if (bucket->isFree(i)) {
            bucket->addItemAt(i, k, student);
            count++;
            return true;
        }
    }
    return false;
}

HashTable::HashTable(int bucketSize, int p, AddressFunction* addressFunction) {
    this->bucketSize = bucketSize;
    this->p = p;
    this->size = 1 << p;
    this->table = new Bucket * [size];
    for (int i = 0; i < size; i++) {
        this->table[i] = new Bucket(bucketSize);
    }
    this->addressFunction = addressFunction;
}

HashTable::~HashTable() {

}

int HashTable::hashFunction(int key) const {
    return key % size;
}

int HashTable::Bucket::indexOfKey(int k) const {
    for (int i = 0; i < itemCount; i++)
        if (arr[i].key == k && !arr[i].deleted)
            return i;
    return -1;
}

Student* HashTable::Bucket::findKey(int k) const {
    int i = indexOfKey(k);
    return i == -1 ? nullptr : arr[i].student;
}

void HashTable::Bucket::deleteAt(int index) {
    if (!arr[index].deleted && arr[index].student) {
        realItemCount--;
        arr[index].deleted = true;
        arr[index].student = nullptr;
        arr[index].key = 0;
    }
}

void HashTable::Bucket::addItemAt(int index, int key, Student* student) {
    if (arr[index].student) deleteAt(index);
    realItemCount++;
    itemCount++;
    arr[index] = *new BucketItem(key, student);
}

bool HashTable::Bucket::isFull() const {
    return realItemCount == size;
}

bool HashTable::Bucket::hasNext() const {
    return itemCount == size;
}

bool HashTable::Bucket::isFree(int index) const {
    return arr[index].deleted || !arr[index].student;
}

int HashTable::Bucket::getItemCount() const {
    return itemCount;
}

int HashTable::Bucket::getRealItemCount() const {
    return realItemCount;
}

Student* HashTable::Bucket::itemAt(int index) {
    return arr[index].student;
}
