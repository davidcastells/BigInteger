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
#include "big_integer_base.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

int base_verbosity = 0;

int big_integer_base_maxVal( int x,  int y)
{
    return (x > y) ? x:y;
}

int big_integer_base_minVal( int x,  int y)
{
    return (x < y) ? x:y;
}

/**
 * Returns the value of the ith bit of the integer
 * @param bitnum
 * @return 
 */
int big_integer_base_getBit(unsigned int* m_data, 
	const unsigned int base, 
	const unsigned int size, int bitnum)
{
   int limbIndex = bitnum / 32;
   int bitIndex = bitnum % 32;

   return (m_data[base+limbIndex] >> bitIndex) & 1;
}






void big_integer_base_error(unsigned int* r_data, unsigned int r_base, unsigned int r_size)
{
    for (int i=0; i < r_size; i++)
        r_data[r_base+i] = -1;
}


void big_integer_base_zero(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    for (int i=0; i < m_size; i++)
        m_data[m_base+i] = 0;    
}

void big_integer_base_setIntValue(unsigned int* data, 
	const unsigned int base, 
	const unsigned int size, unsigned int v)
{
    big_integer_base_zero(data, base, size);
    
    data[base + 0] = v;    
}

/**
 * 
 * @param m_data destination
 * @param m_base
 * @param m_size
 * @param orig_data source
 * @param orig_base
 * @param orig_size
 */
void big_integer_base_copy(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size, 
	unsigned int* orig_data, const unsigned int orig_base, const unsigned int orig_size)
{
    int minCopySize = (m_size < orig_size)? m_size : orig_size;
    int i;
    for (i=0; i < minCopySize; i++)
        m_data[m_base+i] = orig_data[orig_base+i];
    
    // zero the rest of the data 
    for (; i < m_size; i++)
        m_data[m_base+i] = 0;
}

/**
 * 
 * @return the number of the most significant active bit
 */
int big_integer_base_getLength(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    int len = 0;
    int greaterActiveLimb = 0;

    // find the highest limb with an active bit
    for (int i=m_size-1; (i > 0) && (greaterActiveLimb == 0); i--)
    {
        if (m_data[m_base+i] > 0)
            greaterActiveLimb = i;
    }


    // find the highest bit 
    unsigned int test = m_data[m_base+greaterActiveLimb];
    int numActiveBits = 0;


    while (test > 0)
    {
        test = test >> 1;
        numActiveBits++;
    }

    len = (numActiveBits + greaterActiveLimb * 32);

    return len;
}

int big_integer_base_getLimbLength(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    int len = 0;

    // find the highest limb with an active bit
    for (int i=m_size-1; (i > 0); i--)
    {
        if (m_data[m_base+i] > 0)
            return i+1;
    }
    
    return 1;
}

/**
 * 
 * @param up
 * @param down
 */
void big_integer_base_range_short(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size, int up, int down)
{
   unsigned int ref_data[m_size];
   const unsigned int ref_base = 0;
   const unsigned int ref_size = m_size;
   big_integer_base_copy(ref_data, ref_base, ref_size, m_data, m_base, m_size);

   big_integer_base_range(m_data, m_base, m_size, ref_data, ref_base, ref_size, up, down);
}

/**
  * @return the number of bits that the number can store
  */
int big_integer_base_getNumBits(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    unsigned int ret = m_size * 32;
    return ret;
}

void big_integer_base_zeroHighBits(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, int fromBit)
{
    int fromLimb = fromBit / 32;
    int fromLimbBit = fromBit % 32;
    
    if ((fromLimb == r_size) && (fromLimbBit == 0))
        return;
    
    if (fromLimb >= r_size)
        return;    
    
    int mask = ((1 << fromLimbBit)-1);
    
    r_data[r_base+fromLimb] = r_data[r_base+fromLimb] & mask;
    fromLimb++;
    
    while (fromLimb < r_size)
    {
        r_data[r_base+fromLimb] = 0;
        fromLimb++;
    }
}

/**
* 
* @param r
* @param x
* @param upper
* @param lower
*/
void big_integer_base_range(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, int upper, int lower)
{
    //if (extraChecks)
    {
        assert(upper <= big_integer_base_getNumBits(x_data, x_base, x_size));
        assert(lower <= big_integer_base_getNumBits(x_data, x_base, x_size));
        assert(big_integer_base_getNumBits(r_data, r_base, r_size) >= (upper-lower));
    }
    
 
    // we move 
    if (lower > 0)
        big_integer_base_shiftRight(r_data, r_base, r_size, x_data, x_base, x_size, lower);
    else
        big_integer_base_copy(r_data, r_base, r_size, x_data, x_base, x_size);
    
    big_integer_base_zeroHighBits(r_data, r_base, r_size, upper-lower+1);
}



void big_integer_base_shiftRight_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, int bits)
{
    big_integer_base_shiftRight(r_data, r_base, r_size, r_data, r_base, r_size, bits );
}







int big_integer_base_isOdd(unsigned int * data, const unsigned int base, const unsigned int size)
{
    return data[base+0] & 0x1;
}

/**
 * 
 * @param r_data
 * @param r_base
 * @param r_size
 * @param v_data
 * @param v_base
 * @param v_size
 * @param power_data
 * @param power_base
 * @param power_size
 * @param mod_data
 * @param mod_base
 * @param mod_size
 */
void big_integer_base_powerMod(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size,
        unsigned int * v_data, const unsigned int v_base, const unsigned int v_size,
        unsigned int * power_data, const unsigned int power_base, const unsigned int power_size,
        unsigned int * mod_data, const unsigned int mod_base, const unsigned int mod_size)
{
    unsigned int x_data[v_size];
    unsigned int x_size = v_size;
    unsigned int x_base = 0;
    
    big_integer_base_copy(x_data, x_base, x_size, v_data, v_base, v_size);

    unsigned int y_data[power_size];
    unsigned int y_size = power_size;
    unsigned int y_base = 0;
    
    big_integer_base_copy(y_data, y_base, y_size, power_data, power_base, power_size);

    // we use a simple square-and-multiply algorithm,
    big_integer_base_setIntValue(r_data, r_base, r_size, 1);
    
    
    while(!big_integer_base_isZero(y_data, y_base, y_size))
    {
            if (big_integer_base_isOdd(y_data, y_base, y_size))
                big_integer_base_multMod_interleaved_short(r_data, r_base, r_size, x_data, x_base, x_size, mod_data, mod_base, mod_size);

            big_integer_base_shiftRight_short(y_data, y_base, y_size, 1);     // / 2
            big_integer_base_squareMod_short(x_data, x_base, x_size, mod_data, mod_base, mod_size);
            
    }

}



void big_integer_multMod_short(unsigned int * r_data, unsigned int r_base, unsigned int r_size,
        unsigned int * b_data, unsigned int b_base, unsigned int b_size,
        unsigned int * m_data, unsigned int m_base, unsigned int m_size)
{
    unsigned int ref_data[r_size];
    const unsigned int ref_base = 0;
    const unsigned int ref_size = r_size;
    big_integer_base_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);

    
    big_integer_base_multMod(r_data, r_base, r_size, ref_data, ref_base, ref_size,
            b_data, b_base, b_size, m_data, m_base, m_size);
}





void big_integer_base_initSize(unsigned int * data, unsigned int base, unsigned int size, int s)
{
    assert(size >= s);
}

void big_integer_base_initFromHexString(unsigned int * data, unsigned int base, unsigned int size, const char* str)
{
    big_integer_base_initSize(data, base, size, (int) (strlen(str)+7)/8);
    big_integer_base_parseHexString(data, base, size, str);
}
    
int big_integer_base_base_isEqual(unsigned int * a_data, unsigned int a_base, unsigned int a_size,
        unsigned int * b_data, unsigned int b_base, unsigned int b_size)
{
    unsigned int* pa = &a_data[a_base];
    unsigned int* pb = &b_data[b_base];

    int minSize = (a_size > b_size)? b_size : a_size;

    // check if there is any different limb
    for (int i=minSize-1; i >= 0; i--)
        if (pa[i] != pb[i])
            return false;

    // if sizes are different, ensure the bigger part is zero
    for (int i=minSize; i < a_size; i++)
        if (pa[i] != 0)
            return false;

    for (int i=minSize; i < b_size; i++)
        if (pb[i] != 0)
            return false;

    return true;
}

void big_integer_base_parseString(unsigned int * data, unsigned int base, unsigned int size, const char* str)
{
    // multiply by x*10 is x*(8+2) -> 8*x + 2*x
    unsigned int l = (unsigned int) strlen(str);

    big_integer_base_zero(data, base, size);

    unsigned int digit_data[1];
    unsigned int digit_base = 0;
    unsigned int digit_size = 1;
    
    
    // V = V*10+s[i];
    for (int i=0; i < l; i++)
    {
        //  x=x*10
        unsigned int ref_data[size];
        unsigned int ref_base = 0;
        unsigned int ref_size = size;
    
        big_integer_base_copy(ref_data, ref_base, ref_size, data, base, size);
        big_integer_base_shiftLeft_short(ref_data, ref_base, ref_size, 1);
        
        big_integer_base_shiftLeft_short(data, base, size, 3);
        
        big_integer_base_add_short(data, base, size, ref_data, ref_base, ref_size);

//                std::cout << toString() << " + " << str[i] << " = "; 

        big_integer_base_setIntValue(digit_data, digit_base, digit_size, str[i]-'0');
        big_integer_base_add_short(data, base, size, digit_data, digit_base, digit_size);

//                std::cout << toString() << std::endl;
    }
}

void big_integer_base_parseHexString(unsigned int* data, unsigned int base, unsigned int size, const char* str)
{
    // multiply by x*10 is x*(8+2) -> 8*x + 2*x
    int l = (int) strlen(str);

    big_integer_base_zero(data, base, size);

    unsigned int digit_data[1];
    unsigned int digit_base = 0;
    unsigned int digit_size = 1;

    // V = V*10+s[i];
    for (int i=0; i < l; i++)
    {
        //  x=x*10
        big_integer_base_shiftLeft_short(data, base, size, 4); // *16


        if ((str[i] >= '0') && (str[i] <= '9'))
            big_integer_base_setIntValue(digit_data, digit_base, digit_size, str[i]-'0');
        else if ((str[i] >= 'A') && (str[i] <= 'F'))
            big_integer_base_setIntValue(digit_data, digit_base, digit_size, 10 + (str[i]-'A'));
        else
            big_integer_base_setIntValue(digit_data, digit_base, digit_size, 10 + (str[i]-'a'));


        if (base_verbosity > 10)
            std::cout << big_integer_base_toHexString(data, base, size) << " + " << big_integer_base_toHexString(digit_data, digit_base, digit_size) << " = "; 


        big_integer_base_add_short(data, base, size, digit_data, digit_base, digit_size);


        if (base_verbosity > 10)
            std::cout << big_integer_base_toHexString(data, base, size) << std::endl;

    }
}


void big_integer_base_random(unsigned int* data, unsigned int base, unsigned int size)
{
    unsigned int lc = (unsigned int) clock();
    while (lc == clock());
    
    srand((unsigned int) clock());
    
    for (int i=0; i < size; i++)
    {
        data[base+i] = rand();
        
        // Mingw random seems to return 16 bit numbers
        #ifdef __MINGW32__
            data[base+i] <<= 16;
            data[base+i] |= rand();
        #endif

        // Windows random seems to return 16 bit numbers also
        #ifdef WIN32
            data[base+i] <<= 16;
            data[base+i] |= rand();
        #endif
    }
}

void big_integer_base_random_bits(unsigned int* data, unsigned int base, unsigned int size, int n)
{
    assert(n < big_integer_base_getNumBits(data, base, size));
    
    big_integer_base_random(data, base, size);
    int shift = big_integer_base_getNumBits(data, base, size) - n;
    big_integer_base_shiftRight_short(data, base, size, shift);
    
}






void big_integer_base_squareMod_interleaved_short(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size,
        unsigned int * m_data, const unsigned int m_base, const unsigned int m_size)
{
    assert(r_data != m_data);
    assert(big_integer_base_isLessThan(r_data, r_base, r_size, m_data, m_base, m_size));
    assert(big_integer_base_getNumBits(r_data, r_base, r_size) > big_integer_base_getLength(m_data, m_base, m_size));
            
    
    unsigned int ref_data[r_size];
    unsigned int ref_base = 0;
    unsigned int ref_size = r_size;
    big_integer_base_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);

    //ref.mod(m);
    big_integer_base_squareMod_interleaved(r_data, r_base, r_size, ref_data, ref_base, ref_size, m_data, m_base, m_size);
}