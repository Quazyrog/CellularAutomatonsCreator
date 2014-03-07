#include "CellInfo.hpp"


CellInfo::CellInfo()
{
    memset(neighborhood, 0, 5);
}

char &CellInfo::operator() (int s) {
    if (0 <= s && s <= 4)
        return neighborhood[s];
}


