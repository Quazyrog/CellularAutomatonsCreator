/* Copyright 2014 Wojciech Matusiak
 *
 * This file is part of CellularAutomatonCreator.
 *
 * CellularAutomatonCreator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CellularAutomatonCreator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CellularAutomatonCreator.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "CellInfo.hpp"


CellInfo::CellInfo()
{
    memset(neighborhood, 0, STATUS_NUMBER * sizeof(StatusT));
}


StatusT CellInfo::get(int s) const
{
    return neighborhood[s];
}


StatusT &CellInfo::getRef(int s)
{
    return neighborhood[s];
}


StatusT CellInfo::me() const
{
    return myStat;
}


StatusT &CellInfo::meRef()
{
    return myStat;
}


std::ostream &operator<< (std::ostream &out, const CellInfo obj)
{
    out << "[" << (int) obj.me() << "][";
    for (int i = 0; i < STATUS_NUMBER; i++) {
        out << (int) obj.get(i) << (i != STATUS_NUMBER - 1 ? ", " : "]\n");
    }
    return out;
}

