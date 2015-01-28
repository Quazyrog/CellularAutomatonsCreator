/** @file
 *  @brief Header for basic exceptions classes.
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

#include <cstdio>

namespace Exceptions {


#define DEBUG_MESSAGE(format, args...) fprintf(stderr, "[%s:%i]> " format, __FILE__, __LINE__, ##args)

/**
 * The Exception class is used to handle exceptions. All other exception classes inherits from this one. It contains informations about the point where the exception was thrown -- file and line.
 * It may also contain a message with detailed description. All other exceptions classes are subclasses of this one.
 *
 * Exceptions should be thrown using RAISE_EXCEPTION macro (or RAISE_EXCEPTION_NM when they are thrown without message).
 */
class Exception
{
protected:
    /** File where the exception was thrown. */
    const char *_file;
    /** Line where exception was thrown. */
    int _line;
    /** Message from place where the exception was thrown. */
    QString _message;

public:
    /**
     * @brief Exception's class constructor.
     * @param file file where exception was thrown
     * @param line number of line where exception was thrown
     * @param message message stored inm exception's instance
     */
    Exception(const char *file, const int line, QString message);

    /**
     * @brief Returns name of file where exception was thrown.
     * @return path to source file where exception was thrown (as <code>const char *</code>).
     */
    const char *getFile() const;

    /**
     * @brief Returns number of line in source file, where exception wsa thrown.
     * @return number of line in source file, where exception wsa thrown.
     */
    int getLine() const;

    /**
     * @brief Returns exception's message.
     * @return message saved in this exception
     */
    QString getMessage() const;

    /**
     * @brief Returns all informations stored in this class in one string.
     * @return <code>QString</code> in following format: ExceptionClassName [PathToSourceFile:LineInSourceFile]-> Message
     */
    virtual QString getAsString() const;
};

/**
 * @brief The IllegalArgumentException class is thrown when value of argument passed to a function is invalid.
 *
 * If the argument is array's index, you should throw <code>IndexOutOfBoundsException</code>. If the argument is pointer and it's value is null, you should throw <code>NullPointerException</code>.
 */
class IllegalArgumentException : public Exception
{
public:
    IllegalArgumentException(const char *file, const int line, QString message);

    virtual QString getAsString() const;
};


/**
 * @brief The IndexOutOfBoundsException class is thrown when trying to acces array's index witch is out of bound.
 */
class IndexOutOfBoundsException : public Exception
{
public:
    IndexOutOfBoundsException(const char *file, const int line, QString message);

    virtual QString getAsString() const;
};



/**
 * @brief The NullPointerException class is thrown when pointer witch shouldn't be null, is null.
 */
class NullPointerException : public Exception
{
public:
    NullPointerException(const char *file, const int line, QString message);

    virtual QString getAsString() const;
};


/**
 * @brief The IOException class is thrown after occurrence of error when reading ro writting to file.
 */
class IOException : public Exception
{
public:
    IOException(const char *file, const int line, QString message);

    virtual QString getAsString() const;
};

}

#endif // EXCEPTION_HPP
