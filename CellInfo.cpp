#include "CellInfo.hpp"


CellInfo::CellInfo()
{
    memset(neighborhood, 0, 5);
}

StatusT &CellInfo::operator() (int s)
{
    if (0 <= s && s < STATUS_NUMBER)
        return neighborhood[s];
}


StatusT &CellInfo::get(int s)
{
    return operator ()(s);
}

