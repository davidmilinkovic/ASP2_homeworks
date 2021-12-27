#pragma once
#include "AddressFunction.h"

class SplitSequenceLinearHashing : public AddressFunction {
    int s1, s2;
public:
    SplitSequenceLinearHashing(int _s1, int _s2) : s1(_s1), s2(_s2) {};
    int getAddress(int k, int a, int i, int n) override;
};
