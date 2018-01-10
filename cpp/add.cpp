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

void BigInteger::add(BigInteger* y)
{
    BigInteger::add(this, this, y);
}

void BigInteger::add(unsigned int y)
{
    BigInteger::add(this, this, y);
}


/**
 * 
 * @param r output value, can point to the any input value
 * @param a
 * @param b
 */
void BigInteger::add(BigInteger* r, BigInteger* a, BigInteger* b)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;
            
    unsigned int sum;
    unsigned int av, bv;
    unsigned int asize = a->m_size;
    unsigned int bsize = b->m_size;
    unsigned int rsize = r->m_size;
    
    unsigned int minSize = minVal(minVal(asize, bsize), rsize);
    
    // we should fille the values up to rsize, if asize or bsize are greater
    // ignore them
    
    // Do the common part
    unsigned int i;
    for (i=0; i < minSize; i++)
    {
        sum = a->m_data[i] + b->m_data[i] + carryIn;

        if (sum < a->m_data[i]) carryIn = 1;
        else if (sum < b->m_data[i]) carryIn = 1;
        else carryIn = 0;
            
        r->m_data[i] = sum;
    }
    
    // do the rest with care
    for (; i < rsize; i++)
    {
        sum = 0;

        if (i < asize) sum += a->m_data[i];
        if (i < bsize) sum += b->m_data[i];

        if ((i < asize) && (sum < a->m_data[i]))
        {
            if (carryIn) sum++;
            carryOut = 1;
        }
        else if (carryIn)
        {
            sum++;
            carryOut = (sum == 0)? 1: 0;
        }

        carryIn = carryOut;
        r->m_data[i] = sum;
    }
    
}

void BigInteger::add(BigInteger* r, BigInteger* a, unsigned int b)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;
            
    unsigned int sum;

    for (int i=0; i < r->m_size; i++)
    {
        sum = 0;

        if (i < a->m_size) sum += a->m_data[i];
        if (i == 0) sum += b;

        if ((i < a->m_size) && (sum < a->m_data[i]))
        {
            if (carryIn) sum++;
            carryOut = 1;
        }
        else if (carryIn)
        {
            sum++;
            carryOut = (sum == 0)? 1: 0;
        }

        carryIn = carryOut;
        r->m_data[i] = sum;
    }
}


/**
 * Computes r = a + (b << shift)
 * @param r
 * @param a
 * @param b
 * @param shift left shifts in bits
 */
void BigInteger::addShifted(BigInteger* r, BigInteger* a, BigInteger* b, int shift)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;
            
    if ((shift % 32) != 0)        
        std::cerr << "## ERROR ## shift should be multiple of 32" << std::endl;
    
    int shiftedLimbs = shift / 32;
    
    
    for (int ia=0; ia < shiftedLimbs; ia++)
    {
        unsigned int sum = 0;

        if (ia < a->m_size) sum += a->m_data[ia];
        
        r->m_data[ia] = sum;
    }
    
    for (int ia=shiftedLimbs, ib = 0; ia < r->m_size; ia++, ib++)
    {
        unsigned int sum = 0;

        if (ia < a->m_size) sum += a->m_data[ia];
        if (ib < b->m_size) sum += b->m_data[ib];

        if ((ia < a->m_size) && (sum < a->m_data[ia]))
        {
            if (carryIn) sum++;
            carryOut = 1;
        }
        else if (carryIn)
        {
            sum++;
            carryOut = (sum == 0)? 1: 0;
        }

        carryIn = carryOut;
        r->m_data[ia] = sum;
    }
}

/**
 * Add one word to the number a mlen words into a. Return the resulting
 * carry.
 */
int BigInteger::incLimb(int limb, unsigned int carry) 
{
    assert(limb < m_size);
    unsigned int p = m_data[limb];
    unsigned int t = p  + carry ;
    
    m_data[limb] = t;
    
    return (t<p)? 1 : 0;
}