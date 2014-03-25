#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <iostream>
#include <exception>



class NullPointerException :
        public std::exception
{
    virtual const char *what() const noexcept;
};


class IndexOutOfBoundException :
        public std::exception
{
    virtual const char *what() const noexcept;
};


class InvalidArgumentException :
        public std::exception
{
    virtual const char *what() const noexcept;
};

#endif // EXCEPTIONS_HPP
