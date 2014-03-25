#include "Exceptions.hpp"



const char *NullPointerException::what() const noexcept
{
    return "NullPointerexception";
}


const char *IndexOutOfBoundException::what() const noexcept
{
    return "IndexOutOfBoundException";
}


const char *InvalidArgumentException::what() const noexcept
{
    return "InvalidArgumentException";
}

