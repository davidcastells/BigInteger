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

#include "big_integer_array.h"

#include <assert.h>

int big_integer_array_extraChecks = 1;
int big_integer_array_verbosity = 0;

void big_integer_array_zero(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    for (int i=0; i < NUM_BIG_INTEGER_ARRAY_LIMBS; i++)
        r[i] = 0;
}

void big_integer_array_init(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int* data, unsigned int size)
{
    assert(size <= NUM_BIG_INTEGER_ARRAY_LIMBS);
    
    big_integer_array_zero(r);
    
    for (int i=0; i < size; i++)
    {
        r[i] = data[i];
    }
}

void big_integer_array_copy(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], 
	unsigned int orig[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
//    int minCopySize = (m_size < orig_size)? m_size : orig_size;

    for (int i=0; i < NUM_BIG_INTEGER_ARRAY_LIMBS; i++)
        m[i] = orig[i];
    
    // zero the rest of the data 
//    for (; i < m_size; i++)
//        m_data[m_base+i] = 0;
}

int big_integer_array_getNumBits(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    unsigned int ret = NUM_BIG_INTEGER_ARRAY_LIMBS * 32;
    return ret;
}

void big_integer_array_setIntValue(unsigned int data[NUM_BIG_INTEGER_ARRAY_LIMBS],  unsigned int v)
{
    big_integer_array_zero(data);
    
    data[0] = v;    
}


int big_integer_array_getLimbLength(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
//    int len = 0;

    // find the highest limb with an active bit
    for (int i=NUM_BIG_INTEGER_ARRAY_LIMBS-1; (i > 0); i--)
    {
        if (m[i] > 0)
            return i+1;
    }
    
    return 1;
}

int big_integer_array_getLength(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    int len = 0;
    int greaterActiveLimb = 0;

    // find the highest limb with an active bit
    for (int i=NUM_BIG_INTEGER_ARRAY_LIMBS-1; (i > 0) && (greaterActiveLimb == 0); i--)
    {
        if (m[i] > 0)
            greaterActiveLimb = i;
    }


    // find the highest bit 
    unsigned int test = m[greaterActiveLimb];
    int numActiveBits = 0;


    while (test > 0)
    {
        test = test >> 1;
        numActiveBits++;
    }

    len = (numActiveBits + greaterActiveLimb * 32);

    return len;
}

void big_integer_array_range(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],
	unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS],
        int upper, int lower)
{
    //if (extraChecks)
    {
        assert(upper <= big_integer_array_getNumBits(x));
        assert(lower <= big_integer_array_getNumBits(x));
        assert(big_integer_array_getNumBits(r) >= (upper-lower));
    }
    
 
    // we move 
    if (lower > 0)
        big_integer_array_shiftRight(r, x, lower);
    else
        big_integer_array_copy(r, x);
    
    big_integer_array_zeroHighBits(r, upper-lower+1);
}

int big_integer_array_getBit(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], int bitnum)
{
   int limbIndex = bitnum / 32;
   int bitIndex = bitnum % 32;

   return (m[limbIndex] >> bitIndex) & 1;
}

void big_integer_array_zeroHighBits(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], int fromBit)
{
    int fromLimb = fromBit / 32;
    int fromLimbBit = fromBit % 32;
    
    if ((fromLimb == NUM_BIG_INTEGER_ARRAY_LIMBS) && (fromLimbBit == 0))
        return;
    
    if (fromLimb >= NUM_BIG_INTEGER_ARRAY_LIMBS)
        return;    
    
    int mask = ((1 << fromLimbBit)-1);
    
    r[fromLimb] = r[fromLimb] & mask;
    fromLimb++;
    
    while (fromLimb < NUM_BIG_INTEGER_ARRAY_LIMBS)
    {
        r[fromLimb] = 0;
        fromLimb++;
    }
}