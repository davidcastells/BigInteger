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
#include "../big_integer_array.h"

int big_integer_array_isLessThan(limbs_array m, limbs_array v)
{
    // if there is any 1 in a bigger size v before the range of this
    // then return true
//    for (int i=NUM_BIG_INTEGER_ARRAY_LIMBS-1; i >= NUM_BIG_INTEGER_ARRAY_LIMBS; i--)
//        if (v_data[v_base+i])
//            return true;

    for (int i=NUM_BIG_INTEGER_ARRAY_LIMBS-1; i >= 0; i--)
//        if (i >= NUM_BIG_INTEGER_ARRAY_LIMBS)
//        {
//            if (m_data[i] != 0)
//                return false;
//        }
//        else 
        if (m[i] < v[i])
            return true;
        else if (m[i] > v[i])
            return false;

    return false;
}


int big_integer_array_isZero(limbs_array data)
{
    for (int i=0; i<NUM_BIG_INTEGER_ARRAY_LIMBS; i++)
    {
        if (data[i] != 0)
            return 0;   // exit if it is not zero
    }

    return 1;
}

int big_integer_array_isLessThanEqual(limbs_array m, limbs_array v)
{
    return !big_integer_array_isBiggerThan(m, v);
}

int big_integer_array_isBiggerThan(limbs_array m, limbs_array v)
{
    // if there is any 1 in a bigger size v before the range of this
    // then return true
//    for (int i=v_size-1; i >= m_size; i--)
//        if (v_data[v_base+i])
//            return false;

    for (int i=NUM_BIG_INTEGER_ARRAY_LIMBS-1; i >= 0; i--)
//        if (i >= v_size)
//        {
//            if (m_data[m_base+i] != 0)
//                return true;
//        }
//        else 
        if (m[i] > v[i])
            return true;
        else if (m[i] < v[i])
            return false;

    return false;
}

int big_integer_array_isOdd(limbs_array m)
{
    return m[0] & 0x1;
}
      

int big_integer_array_isNegative(limbs_array v)
{
    return v[NUM_BIG_INTEGER_ARRAY_LIMBS-1] & 0x80000000;
}