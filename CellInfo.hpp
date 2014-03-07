#ifndef CELLINFO_HPP
#define CELLINFO_HPP

#include <cstring>


class CellInfo {
private:
    char neighborhood[5];
    char myStat;
public:
    CellInfo();
    char &operator() (int s);
};

#endif // CELLINFO_HPP
