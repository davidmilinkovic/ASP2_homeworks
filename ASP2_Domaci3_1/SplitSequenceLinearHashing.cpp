#include "SplitSequenceLinearHashing.h"

int SplitSequenceLinearHashing::getAddress(int keyToInsert, int initialAddress, int i, int tableSize, int oldKey)
{
	if (keyToInsert > oldKey) 
		return (initialAddress + s1 * i) % tableSize;
	else
		return (initialAddress + s2 * i) % tableSize;
}
