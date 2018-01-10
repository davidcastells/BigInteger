/*
 * Copyright (C) 2017 Universitat Autonoma de Barcelona - David Castells-Rufas <david.castells@uab.cat>
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

#include "../big_integer.h"

/**
 * Returns true if this < v
 * Returns false if this >= v
 * 
 * @param v
 * @return 
 */
int big_integer_isLessThan(big_integer* x, big_integer* y)
{
    // if there is any 1 in a bigger size v before the range of this
    // then return true
    for (int i=y->m_size-1; i >= x->m_size; i--)
        if (y->m_data[i])
            return true;

    for (int i=x->m_size-1; i >= 0; i--)
        if (i >= y->m_size)
        {
            if (x->m_data[i] != 0)
                return false;
        }
        else if (x->m_data[i] < y->m_data[i])
            return true;
        else if (x->m_data[i] > y->m_data[i])
            return false;

    return false;
}


int big_integer_isLessThanEqual(big_integer* x, big_integer* y)
{
    return !big_integer_isBiggerThan(x, y);
}


int big_integer_isBiggerThan(big_integer* x, big_integer* y)
{
    // if there is any 1 in a bigger size v before the range of this
    // then return true
    for (int i=y->m_size-1; i >= x->m_size; i--)
        if (y->m_data[i])
            return false;

    for (int i=x->m_size-1; i >= 0; i--)
        if (i >= y->m_size)
        {
            if (x->m_data[i] != 0)
                return true;
        }
        else if (x->m_data[i] > y->m_data[i])
            return true;
        else if (x->m_data[i] < y->m_data[i])
            return false;

    return false;
}

int big_integer_isZero(big_integer* x)
{
    for (int i=0; i< x->m_size; i++)
    {
        if (x->m_data[i] != 0)
            return 0;   // exit if it is not zero
    }

    return 1;
}

int big_integer_isOdd(big_integer* x)
{
    return x->m_data[0] & 0x1;
}