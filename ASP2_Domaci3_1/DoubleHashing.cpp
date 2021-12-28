#include "DoubleHashing.h"

int DoubleHashing::getAddress(int keyToInsert, int initialAddress, int i, int tableSize, int oldKey=0)
{
    return (initialAddress + i * (q + (keyToInsert % p))) % tableSize;
}
