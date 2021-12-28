#pragma once
class AddressFunction {
public:
    virtual int getAddress(int keyToInsert,int initialAddress, int i, int tableSize, int oldKey=0) = 0;
};

