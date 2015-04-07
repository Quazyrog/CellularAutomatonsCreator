/** @file
 *  Header for exceptions classes.
 */
/* Copyright 2014 Wojciech Matusiak
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <QString>
#include <exception>

#include <cstdio>

namespace Exceptions {

/**
 * The IllegalArgumentException class is thrown when value of argument passed to a function is invalid.
 *
 * If the argument is array's index, you should throw <code>IndexOutOfBoundsException</code>. If the argument is pointer and it's value is null, you should throw <code>NullPointerException</code>.
 */
class IllegalArgumentException : public std::exception
{
public:
    virtual const char* what() const noexcept override;
};


/**
 * The IndexOutOfBoundsException class is thrown when trying to acces array's index witch is out of bound.
 */
class IndexOutOfBoundsException : public std::exception
{
public:
    virtual const char* what() const noexcept override;
};


/**
 * The NullPointerException class is thrown when pointer witch shouldn't be null, is null.
 */
class NullPointerException : public std::exception
{
public:
    virtual const char* what() const noexcept override;
};

/**
 * The IOException class is thrown after occurrence of error when reading ro writting to file.
 */
class IOException : public std::exception
{
public:
    virtual const char* what() const noexcept override;
};


/**
 * The RuntimeException may be thrown, when thing that shouldn't happen happens...
 */
class RuntimeException : public std::exception
{
public:
    virtual const char* what() const noexcept override;
};


/**
 * The StateLimitReachedException class is thrown when requesting to add new state to cellukar automaton's register, and the state limit was already reached.
 * @see CellularAutomaton::STATES_LIMIT
 */
class StateLimitReachedException : public std::exception
{
public:
    virtual const char* what() const noexcept override;
};


class SyntaxErrorException : public std::exception
{
private:
    int _where;
    QString _message;

public:
    SyntaxErrorException(int where, QString message);
    virtual const char* what() const noexcept override;

    int where() const;
    QString message() const;
};


class ArithmeticErrorException : public std::exception
{
public:
    enum Error {
        DIVISION_BY_ZERO,
        INVALID_ARGUMENT
    };

private:
    Error _error;

public:
    ArithmeticErrorException(Error error);
    virtual const char* what() const noexcept override;
};

}

#endif // EXCEPTION_HPP
