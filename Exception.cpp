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

#include "Exception.hpp"

namespace Exceptions {

Exception::Exception(const char *file, const int line, QString message)
{
    _file = file;
    _line = line;
    _message = message;
}


const char *Exception::getFile() const
{
    return _file;
}

int Exception::getLine() const
{
    return _line;
}


QString Exception::getMessage() const
{
    return _message;
}


QString Exception::getAsString() const
{
    return QString().sprintf("Exception [%s:%i]-> ", _file, _line) + _message;
}


IllegalArgumentException::IllegalArgumentException(const char *file, const int line, QString message) :
    Exception(file, line, message)
{

}


QString IllegalArgumentException::getAsString() const
{
    return QString().sprintf("IllegalArgumentException [%s:%i]-> ", _file, _line) + _message;
}


IndexOutOfBoundsException::IndexOutOfBoundsException(const char *file, const int line, QString message) :
    Exception(file, line, message)
{

}


QString IndexOutOfBoundsException::getAsString() const
{
    return QString().sprintf("IndexOutOfBoundsException [%s:%i]-> ", _file, _line) + _message;
}


NullPointerException::NullPointerException(const char *file, const int line, QString message) :
    Exception(file, line, message)
{

}


QString NullPointerException::getAsString() const
{
    return QString().sprintf("NullPointerException [%s:%i]-> ", _file, _line) + _message;
}


IOException::IOException(const char *file, const int line, QString message) :
    Exception(file, line, message)
{

}


QString IOException::getAsString() const
{
    return QString().sprintf("IOException [%s:%i]-> ", _file, _line) + _message;
}

}
