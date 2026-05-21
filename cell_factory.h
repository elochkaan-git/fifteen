#pragma once
#include "cell.h"

class CellFactory
{
public:
    static Cell* create(int n) {
        if (n)
            return new NumberCell(n);
        return new NullCell();
    }
};
