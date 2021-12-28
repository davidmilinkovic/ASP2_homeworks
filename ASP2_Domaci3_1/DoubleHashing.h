#pragma once

#include "AddressFunction.h"

class DoubleHashing : public AddressFunction
{
	int p, q;
public:
	DoubleHashing(int _p, int _q) : p(_p), q(_q) {};
	int getAddress(int keyToInsert,  int initialAddress, int i, int tableSize, int oldKey) override;
};

