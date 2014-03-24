#ifndef CELLINFO_HPP
#define CELLINFO_HPP

#include <cstring>

typedef unsigned char StatusT;



class CellInfo {
private:
    StatusT neighborhood[32];
    StatusT myStat;
public:
    CellInfo();

    StatusT &operator ()(int s);
    StatusT &get(int s);
};

#endif // CELLINFO_HPP
