#ifndef CELLINFO_HPP
#define CELLINFO_HPP

#include <cstring>

typedef unsigned char StatusT;



const int STATUS_NUMBER = 32;



class CellInfo {
private:
    StatusT neighborhood[STATUS_NUMBER];
    StatusT myStat;
public:
    CellInfo();

    StatusT &operator ()(int s);
    StatusT &get(int s);
};

#endif // CELLINFO_HPP
