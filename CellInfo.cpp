#include "CellInfo.hpp"


CellInfo::CellInfo()
{
    memset(neighborhood, 0, STATUS_NUMBER * sizeof(StatusT));
}

StatusT &CellInfo::operator() (int s)
{
    if (0 <= s && s < STATUS_NUMBER)
        return neighborhood[s];
    return k;
}


StatusT &CellInfo::get(int s)
{
    return operator ()(s);
}


StatusT &CellInfo::me()
{
    return myStat;
}

