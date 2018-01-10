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

#include <iostream>

#include <string>

#include "BigInteger.h"
//#include "BigNum.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <time.h>

int BigInteger::verbosity = 0;
int BigInteger::extraChecks = 1;

/**
 * Default constructor 
 */
BigInteger::BigInteger()
{
    m_data = NULL;
    m_size = 0;
}

/**
 * Copy constructor
 * @param orig
 */
BigInteger::BigInteger(const BigInteger& orig)
{
    m_data = NULL;
    m_size = 0;
    
    initSize(orig.m_size);
        
    copy(&orig);
}

BigInteger::BigInteger(int size)
{
    m_data = NULL;
    m_size = 0;
    
    initSize(size);
}

void BigInteger::setIntValue(unsigned int v)
{
    zero();
    m_data[0] = v;    
}

/**
 * Copies the orig number into this object.
 * If the destination object is bigger than the source, it sets all other limbs
 * to zero
 * @param orig
 */
void BigInteger::copy(const BigInteger* orig)
{
    int minCopySize = (m_size < orig->m_size)? m_size : orig->m_size;
    int i;
    for (i=0; i < minCopySize; i++)
        m_data[i] = orig->m_data[i];
    
    // zero the rest of the data 
    for (; i < m_size; i++)
        m_data[i] = 0;
}

void BigInteger::copy(unsigned int* limbs)
{
    for (int i=0; i < m_size; i++)
        m_data[i] = limbs[i];    
}

BigInteger::~BigInteger()
{
    if (m_data != NULL)
        delete [] m_data;
    
    m_data = NULL;
}

/**
 * Returns the value of the ith bit of the integer
 * @param bitnum
 * @return 
 */
int BigInteger::getBit(int bitnum)
{
   int limbIndex = bitnum / 32;
   int bitIndex = bitnum % 32;

   return (m_data[limbIndex] >> bitIndex) & 1;
}

 /**
  * @return the number of bits that the number can store
  */
int BigInteger::getNumBits()
{
    return m_size * 32;
}

/**
 * @return the number of the most significant active limb
 *  the minimum value is 1 and the maximum value is m_size
 */
int BigInteger::getLimbLength()
{
    int len = 0;

    // find the highest limb with an active bit
    for (int i=m_size-1; (i > 0); i--)
    {
        if (m_data[i] > 0)
            return i+1;
    }
    
    return 1;
}

/**
 * 
 * @return the number of the most significant active bit
 */
int BigInteger::getLength()
{
    int len = 0;
    int greaterActiveLimb = 0;

    // find the highest limb with an active bit
    for (int i=m_size-1; (i > 0) && (greaterActiveLimb == 0); i--)
    {
        if (m_data[i] > 0)
            greaterActiveLimb = i;
    }

    if (verbosity > VERBOSITY_LEVEL_GET_LENGTH) std::cout << "BigInteger::getLength greaterActiveLimb = " << greaterActiveLimb << std::endl;

    // find the highest bit 
    unsigned int test = m_data[greaterActiveLimb];
    int numActiveBits = 0;

    if (verbosity > VERBOSITY_LEVEL_GET_LENGTH) std::cout << "BigInteger::getLength test = " << test << std::endl;

    while (test > 0)
    {
        test = test >> 1;
        numActiveBits++;
    }

    len = (numActiveBits + greaterActiveLimb * 32);

    return len;
}
        

       
        
/**
 * Initializes size and parses hex string
 * @param str
 */
void BigInteger::initFromHexString(const char* str)
{
    initSize((strlen(str)+7)/8);
    parseHexString(str);
}
        
        
/**
 * Initialize the size of the limbs array which are 32 bits numbers
 * @param s number of limbs 
 */
void BigInteger::initSize(int s)
{
    if  (m_size > 0)
        delete m_data;
        
    m_size = s;
    m_data = new unsigned int [m_size];
    
    assert(m_data);
}

        

        
/**
 * Increment a big number
 */
void BigInteger::inc()
{
    int i = 0;
    int doRun;
    do
    {                
        m_data[i]++;

        doRun = (m_data[i] == 0);

        i++;


    } while(doRun);
}
        


/**
 * Sets to zero the higher bits
 * if the number has less bits, ignore it 
 */
void BigInteger::zeroHighBits(BigInteger* r, int fromBit)
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


int BigInteger::maxVal( int x,  int y)
{
    return (x > y) ? x:y;
}

int BigInteger::minVal( int x,  int y)
{
    return (x < y) ? x:y;
}

/**
 * Reduce the working size of the integer (without reallocating memory)
 * @param s new size
 */
void BigInteger::reduceWorkingSize(int s)
{
    assert(m_size >= s);
    for (int i=s; i < m_size; i++)
        assert(m_data[i] == 0);

    m_size = s;
}

void BigInteger::zero()
{
    for (int i=0; i < m_size; i++)
        m_data[i] = 0;    
}

/**
 * Returns the index of the rightmost (lowest-order) one bit in this
 * BigInteger (the number of zero bits to the right of the rightmost
 * one bit).  Returns -1 if this BigInteger contains no one bits.
 * (Computes {@code (this==0? -1 : log2(this & -this))}.)
 *
 * @return index of the rightmost one bit in this BigInteger.
 */
int BigInteger::getLowestSetBit() 
{
    assert(!isNegative());

    for (int i=0; i < getLength(); i++)
        if (getBit(i))
            return i;
    return -1;        
}