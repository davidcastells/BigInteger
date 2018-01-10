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

#include "big_integer.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

int big_integer_verbosity = 0;

int big_integer_maxVal( int x,  int y)
{
    return (x > y) ? x:y;
}

int big_integer_minVal( int x,  int y)
{
    return (x < y) ? x:y;
}


void big_integer_init(big_integer* x, unsigned int* data, unsigned int size)
{
    x->m_data = data;
    x->m_size = size;
}


/**
 * Returns the value of the ith bit of the integer
 * @param bitnum
 * @return 
 */
int big_integer_base_getBit(big_integer* x, int bitnum)
{
   int limbIndex = bitnum / 32;
   int bitIndex = bitnum % 32;

   return (x->m_data[limbIndex] >> bitIndex) & 1;
}



int big_integer_getLimbLength(big_integer* x)
{
    int len = 0;

    // find the highest limb with an active bit
    for (int i=x->m_size-1; (i > 0); i--)
    {
        if (x->m_data[i] > 0)
            return i+1;
    }
    
    return 1;
}

void big_integer_error(big_integer* x)
{
    for (int i=0; i < x->m_size; i++)
        x->m_data[i] = -1;
}

void big_integer_zero(big_integer* x)
{
    for (int i=0; i < x->m_size; i++)
        x->m_data[i] = 0;    
}

void big_integer_copy(big_integer* x, big_integer* orig)
{
    int minCopySize = big_integer_minVal(x->m_size, orig->m_size);
    int i;
    for (i=0; i < minCopySize; i++)
        x->m_data[i] = orig->m_data[i];
    
    // zero the rest of the data 
    for (; i < x->m_size; i++)
        x->m_data[i] = 0;
}

int big_integer_getNumBits(big_integer* x)
{
    unsigned int ret = x->m_size * 32;
    return ret;
}

/**
 * 
 * @return the number of the most significant active bit
 */
int big_integer_getLength(big_integer* x)
{
    int len = 0;
    int greaterActiveLimb = 0;

    // find the highest limb with an active bit
    for (int i=x->m_size-1; (i > 0) && (greaterActiveLimb == 0); i--)
    {
        if (x->m_data[i] > 0)
            greaterActiveLimb = i;
    }


    // find the highest bit 
    unsigned int test = x->m_data[greaterActiveLimb];
    int numActiveBits = 0;


    while (test > 0)
    {
        test = test >> 1;
        numActiveBits++;
    }

    len = (numActiveBits + greaterActiveLimb * 32);

    return len;
}

/**
 * 
 * @param r
 * @param x
 * @param upper
 * @param lower
 */
void big_integer_range(big_integer* r, big_integer* x, int upper, int lower)
{
//    if (extraChecks)
    {
        assert(upper <= big_integer_getNumBits(x));
        assert(lower <= big_integer_getNumBits(x));
        assert(big_integer_getNumBits(r) >= (upper-lower));
    }
 
    // we move 
    if (lower > 0)
        big_integer_shiftRight(r, x, lower);
    else
        big_integer_copy(r, x);
    
    big_integer_zeroHighBits(r, upper-lower+1);
}


/**
 * Returns the value of the ith bit of the integer
 * @param bitnum
 * @return 
 */
int big_integer_getBit(big_integer* x, int bitnum)
{
   int limbIndex = bitnum / 32;
   int bitIndex = bitnum % 32;

   return (x->m_data[limbIndex] >> bitIndex) & 1;
}



void big_integer_zeroHighBits(big_integer* r, int fromBit)
{
    int fromLimb = fromBit / 32;
    int fromLimbBit = fromBit % 32;
    
    if ((fromLimb == r->m_size) && (fromLimbBit == 0))
        return;
    
    if (fromLimb >= r->m_size)
        return;    
    
    int mask = ((1 << fromLimbBit)-1);
    
    r->m_data[fromLimb] = r->m_data[fromLimb] & mask;
    fromLimb++;
    
    while (fromLimb < r->m_size)
    {
        r->m_data[fromLimb] = 0;
        fromLimb++;
    }
}

void big_integer_setIntValue(big_integer* x, unsigned int v)
{
    big_integer_zero(x);
    x->m_data[0] = v;    
}