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

#include "../assertf.h"

#include <stdint.h>


void big_integer_array_mult_hi_lo(unsigned int x, unsigned int y, unsigned int *rHight, unsigned int *rLow)
{
    unsigned long long res = (unsigned long long) x * (unsigned long long) y;
    *rLow = (unsigned int) res;
    *rHight = (unsigned int) (res >> 32);
}

void big_integer_array_mult(limbs_array r, limbs_array a, limbs_array b)
{
    if (big_integer_array_extraChecks) 
    {
        if (!big_integer_array_isNegative(a) && !big_integer_array_isNegative(b))
            // for non negative a and b, check that size is enough
            assertf((NUM_BIG_INTEGER_ARRAY_BITS >= (big_integer_array_getLength(a) + big_integer_array_getLength(b))),
                "%d < (a len =%d) + (b len =%d)", NUM_BIG_INTEGER_ARRAY_BITS,
                big_integer_array_getLength(a),
                big_integer_array_getLength(b));
    }
    
    big_integer_array_zero(r);

    int asize = big_integer_array_getLimbLength(a);
    int bsize = big_integer_array_getLimbLength(b);
    int rsize = NUM_BIG_INTEGER_ARRAY_LIMBS;
    uint64_t carry;
    uint64_t accum;
    unsigned int adata;
    int idx;
    uint32_t lo, hi;
    
    for (int i=0; i< asize; i++)
    {
        carry = 0;
        adata = a[i];
        
        for (int j=0; j<bsize; j++)
        {
            idx = i + j;

            big_integer_array_mult_hi_lo(adata, b[j], &hi, &lo);

            accum = r[idx] + carry + lo;
            
            carry = (accum >> 32);
            carry += hi;
            
            r[idx] = (unsigned int) accum;
        }
        
        if ((i + bsize)< rsize)
            r[i + bsize] = (unsigned int) carry;
    }
}

/**
 * multiply two arrays and store the result.
 * The resulting array has twice the size of the input operands
 * @param r result r = a * b
 * @param a
 * @param b
 */
void big_integer_array_mult_big(limbs_array2 r_big, limbs_array a, limbs_array b)
{
    if (big_integer_array_extraChecks) 
    {
        assert((NUM_BIG_INTEGER_ARRAY_BITS*2) >= (big_integer_array_getLength(a) + big_integer_array_getLength(b)));
    }
    
    big_integer_array_zero_big(r_big);

    int asize = big_integer_array_getLimbLength(a);
    int bsize = big_integer_array_getLimbLength(b);
    int rsize = NUM_BIG_INTEGER_ARRAY_LIMBS*2;
    uint64_t carry;
    uint64_t accum;
    unsigned int adata;
    int idx;
    uint32_t lo, hi;
    
    for (int i=0; i< asize; i++)
    {
        carry = 0;
        adata = a[i];
        
        for (int j=0; j<bsize; j++)
        {
            idx = i + j;

            big_integer_array_mult_hi_lo(adata, b[j], &hi, &lo);

            accum = r_big[idx] + carry + lo;
            
            carry = (accum >> 32);
            carry += hi;
            
            r_big[idx] = (unsigned int) accum;
        }
        
        if ((i + bsize)< rsize)
            r_big[i + bsize] = (unsigned int) carry;
    }
}

/**
 * r = (a * b ) truncated to rsize limbs
 * 
 * @param r
 * @param a_big
 * @param b
 * @param rsize truncate result to this number of limbs
 */
void big_integer_array_multLow(limbs_array r, limbs_array2 a_big, limbs_array b, unsigned int rsize)
{
    if (big_integer_array_extraChecks) 
    {
        assert(r != a_big);
        assert(r != b);
    }

    int i, j;

    big_integer_array_zero(r);

    int atop = big_integer_array_getLimbLength_big(a_big);      // !! This reduces the actual size
    int btop = big_integer_array_getLimbLength(b);
    //int rsize = NUM_BIG_INTEGER_ARRAY_LIMBS;
    
    uint32_t lo, hi;
    uint64_t accum;
    
    for(i=0; i<atop; i++)
    {
        uint64_t carry = 0;
        for(j=0; j<btop; j++)
        {
            int idx = i + j;
            if (idx < rsize)
            {
                big_integer_array_mult_hi_lo(a_big[i], b[j], &hi, &lo);

                accum  = r[idx] + carry + lo;
                carry  = (accum >> 32) + hi;
                r[idx] = (unsigned int) accum;
            }
        }
    }
}