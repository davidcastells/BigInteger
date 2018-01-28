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
#include "../big_integer_base.h"


/**
 * Returns true if this < v
 * Returns false if this >= v
 * 
 * @param v
 * @return 
 */
int big_integer_base_isLessThan(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size, 
	unsigned int* v_data, const unsigned int v_base, const unsigned int v_size)
{
    // if there is any 1 in a bigger size v before the range of this
    // then return true
    for (int i=v_size-1; i >= m_size; i--)
        if (v_data[v_base+i])
            return true;

    for (int i=m_size-1; i >= 0; i--)
        if (i >= v_size)
        {
            if (m_data[m_base+i] != 0)
                return false;
        }
        else if (m_data[m_base+i] < v_data[v_base+i])
            return true;
        else if (m_data[m_base+i] > v_data[v_base+i])
            return false;

    return false;
}


int big_integer_base_isEqual(unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, 
	unsigned int* b_data, const unsigned int b_base, const unsigned int b_size)
{
    int minSize = (a_size > b_size)? b_size : a_size;

    // check if there is any different limb
    for (int i=minSize-1; i >= 0; i--)
        if (a_data[a_base+i] != b_data[b_base+i])
            return false;

    // if sizes are different, ensure the bigger part is zero
    for (int i=minSize; i < a_size; i++)
        if (a_data[a_base + i] != 0)
            return false;

    for (int i=minSize; i < b_size; i++)
        if (b_data[b_base+i] != 0)
            return false;

    return true;
}

int big_integer_base_isLessThanEqual(unsigned int* m_data, unsigned int m_base, unsigned int m_size,
        unsigned int* v_data, unsigned int v_base, unsigned int v_size)
{
    return !big_integer_base_isBiggerThan(m_data, m_base, m_size, v_data, v_base, v_size);
}

int big_integer_base_isBiggerThan(unsigned int* m_data, unsigned int m_base, unsigned int m_size,
        unsigned int* v_data, unsigned int v_base, unsigned int v_size)
{
    // if there is any 1 in a bigger size v before the range of this
    // then return true
    for (int i=v_size-1; i >= m_size; i--)
        if (v_data[v_base+i])
            return false;

    for (int i=m_size-1; i >= 0; i--)
        if (i >= v_size)
        {
            if (m_data[m_base+i] != 0)
                return true;
        }
        else if (m_data[m_base+i] > v_data[v_base+i])
            return true;
        else if (m_data[m_base+i] < v_data[v_base+i])
            return false;

    return false;
}

int big_integer_base_isZero(unsigned int * data, const unsigned int base, const unsigned int size)
{
    for (int i=0; i<size; i++)
    {
        if (data[base+i] != 0)
            return 0;   // exit if it is not zero
    }

    return 1;
}

int big_integer_base_isNegative(unsigned int * m_data, const unsigned int m_base, const unsigned int m_size)
{
    return m_data[m_base+m_size-1] & 0x80000000;
}