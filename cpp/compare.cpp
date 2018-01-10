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
#include "../BigInteger.h"

#include <assert.h>
#include <iostream>

 
/**
 * 
 * @param v
 * @return 1 if the two numbers are equal, ignoring their size
 */
int BigInteger::isEqual(BigInteger* a, BigInteger* b)
{
    unsigned int* pa = a->m_data;
    unsigned int* pb = b->m_data;

    int minSize = (a->m_size > b->m_size)? b->m_size : a->m_size;

    // check if there is any different limb
    for (int i=minSize-1; i >= 0; i--)
        if (pa[i] != pb[i])
            return false;

    // if sizes are different, ensure the bigger part is zero
    for (int i=minSize; i < a->m_size; i++)
        if (pa[i] != 0)
            return false;

    for (int i=minSize; i < b->m_size; i++)
        if (pb[i] != 0)
            return false;

    return true;
}
        
/**
 * Returns true if this < v
 * Returns false if this >= v
 * 
 * @param v
 * @return 
 */
int BigInteger::isLessThan(BigInteger* v)
{
    unsigned int* pv = v->m_data;

    // if there is any 1 in a bigger size v before the range of this
    // then return true
    for (int i=v->m_size-1; i >= m_size; i--)
        if (pv[i])
            return true;

    for (int i=m_size-1; i >= 0; i--)
        if (i >= v->m_size)
        {
            if (m_data[i] != 0)
                return false;
        }
        else if (m_data[i] < pv[i])
            return true;
        else if (m_data[i] > pv[i])
            return false;

    return false;
}

int BigInteger::isBiggerThan(BigInteger* v)
{
    unsigned int* pv = v->m_data;

    // if there is any 1 in a bigger size v before the range of this
    // then return true
    for (int i=v->m_size-1; i >= m_size; i--)
        if (pv[i])
            return false;

    for (int i=m_size-1; i >= 0; i--)
        if (i >= v->m_size)
        {
            if (m_data[i] != 0)
                return true;
        }
        else if (m_data[i] > pv[i])
            return true;
        else if (m_data[i] < pv[i])
            return false;

    return false;
}

int BigInteger::isLessThanEqual(BigInteger* v)
{
    return !isBiggerThan(v);
}

/**
 * 
 * @return true is the number is odd
 */
int BigInteger::isOdd()
{
    return m_data[0] & 0x1;
}
        
/**
 * 
 * @return true is the number is zero
 */
int BigInteger::isZero()
{
    unsigned int* p = m_data;

    for (int i=0; i<m_size; i++)
    {
        if (p[i] != 0)
            return 0;   // exit if it is not zero
    }

    return 1;
}
        
int BigInteger::isOne()
{
    for (int i=1; i<m_size; i++)
        if (m_data[i] != 0)
            return 0;   // exit if it is not zero

    return (m_data[0] == 1);
}

int BigInteger::isNegative()
{
    return m_data[m_size-1] & 0x80000000;
}