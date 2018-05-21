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

#include "../big_integer_apint_radix.h"

int big_integer_apint_radix_isEqual(limbs_radix_array a, limbs_radix_array b)
{
    // check if there is any different limb
    for (int i=NUM_BIG_INTEGER_APINT_RADIX_LIMBS-1; i >= 0; i--)
        if (a[i] != b[i])
            return false;

    return true;
}

int big_integer_apint_radix_isNegative(limbs_radix_array v)
{
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> one = 1;
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> negBit = (one << (NUM_BIG_INTEGER_APINT_RADIX-1));
    return (v[NUM_BIG_INTEGER_APINT_RADIX_LIMBS-1] & negBit)? 1 : 0;
}

int big_integer_apint_radix_isLessThan(limbs_radix_array m, limbs_radix_array v)
{
    for (int i=NUM_BIG_INTEGER_APINT_RADIX_LIMBS-1; i >= 0; i--)
        if (m[i] < v[i])
            return true;
        else if (m[i] > v[i])
            return false;

    return false;
}

int big_integer_apint_radix_isLessThanEqual(limbs_radix_array m, limbs_radix_array v)
{
    return !big_integer_apint_radix_isBiggerThan(m, v);
}

int big_integer_apint_radix_isBiggerThan(limbs_radix_array m, limbs_radix_array v)
{
    for (int i=NUM_BIG_INTEGER_APINT_RADIX_LIMBS-1; i >= 0; i--)
        if (m[i] > v[i])
            return true;
        else if (m[i] < v[i])
            return false;

    return false;
}



int big_integer_apint_radix_isZero(limbs_radix_array data)
{
    for (int i=0; i<NUM_BIG_INTEGER_APINT_RADIX_LIMBS; i++)
    {
        if (data[i] != 0)
            return 0;   // exit if it is not zero
    }

    return 1;
}