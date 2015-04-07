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

const char *IllegalArgumentException::what()  const noexcept
{
    return "IllegalArgumentException";
}


const char *IndexOutOfBoundsException::what()  const noexcept
{
    return "IndexOutOfBoundsException";
}


const char *NullPointerException::what()  const noexcept
{
    return "NullPointerException";
}


const char *IOException::what()  const noexcept
{
    return "IOException";
}


const char *RuntimeException::what()  const noexcept
{
    return "RuntimeException";
}


const char *StateLimitReachedException::what()  const noexcept
{
    return "StateLimitReachedException";
}


SyntaxErrorException::SyntaxErrorException(int where, QString message)
{
    _message = message;
    _where = where;
}


const char *SyntaxErrorException::what() const noexcept
{
    return "SyntaxErrorException";
}


int SyntaxErrorException::where() const
{
    return _where;
}


QString SyntaxErrorException::message() const
{
    return _message;
}


ArithmeticErrorException::ArithmeticErrorException(Error what)
{
    _error = what;
}


const char *ArithmeticErrorException::what() const noexcept
{
    switch (_error) {
    case DIVISION_BY_ZERO:
        return "ArithmeticErrorException – division by 0";
    case INVALID_ARGUMENT:
        return "ArithmeticErrorException – division by 0";
    }
    return "ArithmeticErrorException";
}

}
