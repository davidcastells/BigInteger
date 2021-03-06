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

#include "big_integer_apint_radix.h"
#include "to_string.h"

#include <assert.h>

int big_integer_apint_radix_extraChecks = 1;
int big_integer_apint_radix_verbosity = 0;



void big_integer_apint_radix_zero(limbs_radix_array r)
{
    for (int i=0; i < NUM_BIG_INTEGER_APINT_RADIX_LIMBS; i++)
        r[i] = 0;
}

void big_integer_apint_radix_zero_big(limbs_radix_array2 r)
{
    for (int i=0; i < 2*NUM_BIG_INTEGER_APINT_RADIX_LIMBS; i++)
        r[i] = 0;
}


void big_integer_apint_radix_initfromLimbArray(limbs_radix_array r, unsigned int* m_data, unsigned int limbs)
{
    assert(NUM_BIG_INTEGER_APINT_RADIX_BITS >= (limbs*32));
    
    big_integer_apint_radix_zero(r);
    
    for (int i=0; i < limbs; i++)
    {
        unsigned int limb = i/(NUM_BIG_INTEGER_APINT_RADIX/32);
        unsigned int radixIndex = i % (NUM_BIG_INTEGER_APINT_RADIX/32);
        
        ap_uint<NUM_BIG_INTEGER_APINT_RADIX> newV = m_data[i];
        newV <<= (32*radixIndex);
        
        r[limb] |= newV;
        
//        std::cout << "32bits limb ["<< to_string(i)<< "] = " << to_hex_string(m_data[i]) <<" => Radix Limb ["<< limb <<"] =  " << r[limb].toHexString() << std::endl; 
    }

}


int big_integer_apint_radix_getLimbLength(limbs_radix_array m)
{
    int i;
    // find the highest limb with an active bit
    for (i=NUM_BIG_INTEGER_APINT_RADIX_LIMBS-1; (i > 0); i--)
    {
        if (m[i] != 0)
            return i+1;
    }
    
    return 1;
}

int big_integer_apint_radix_getLimbLength_big(limbs_radix_array2 m)
{
    int i;
    // find the highest limb with an active bit
    for (i=(2*NUM_BIG_INTEGER_APINT_RADIX_LIMBS)-1; (i > 0); i--)
    {
        if (m[i] != 0)
            return i+1;
    }
    
    return 1;
}

int big_integer_apint_radix_getNumBits(limbs_radix_array m)
{
    unsigned int ret = NUM_BIG_INTEGER_APINT_RADIX_BITS;
    return ret;
}

int big_integer_apint_radix_getBit(limbs_radix_array m, int bitnum)
{
   int limbIndex = bitnum / NUM_BIG_INTEGER_APINT_RADIX;
   int bitIndex = bitnum % NUM_BIG_INTEGER_APINT_RADIX;

   return (m[limbIndex] >> bitIndex) & 1;
}

void big_integer_apint_radix_copy(limbs_radix_array m, limbs_radix_array orig)
{
    for (int i=0; i < NUM_BIG_INTEGER_APINT_RADIX_LIMBS; i++)
        m[i] = orig[i];    
}


int big_integer_apint_radix_getLength(limbs_radix_array m)
{
    int len = 0;
    int greaterActiveLimb = 0;

    // find the highest limb with an active bit
    for (int i=NUM_BIG_INTEGER_APINT_RADIX_LIMBS-1; (i > 0) && (greaterActiveLimb == 0); i--)
    {
        if (m[i] != 0)
            greaterActiveLimb = i;
    }


    // find the highest bit 
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> test = m[greaterActiveLimb];
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> zero = 0;
    int numActiveBits = 0;


    while (test != zero)
    {
        test = test >> 1;
        numActiveBits++;
    }

    len = (numActiveBits + greaterActiveLimb * NUM_BIG_INTEGER_APINT_RADIX);

    return len;
}

void big_integer_apint_radix_setIntValue(limbs_radix_array data, unsigned int v)
{
    big_integer_apint_radix_zero(data);
    
    data[0] = v;    
}