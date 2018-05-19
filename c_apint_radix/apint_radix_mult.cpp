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

#include "../assertf.h"

#include "../to_string.h"

void big_integer_apint_radix_mult_hi_lo(ap_uint<NUM_BIG_INTEGER_APINT_RADIX> x, 
        ap_uint<NUM_BIG_INTEGER_APINT_RADIX> y, 
        ap_uint<NUM_BIG_INTEGER_APINT_RADIX>* rHight,
        ap_uint<NUM_BIG_INTEGER_APINT_RADIX>* rLow)
{
    ap_uint<2*NUM_BIG_INTEGER_APINT_RADIX> x2 = x;
    ap_uint<2*NUM_BIG_INTEGER_APINT_RADIX> y2 = y;
    ap_uint<2*NUM_BIG_INTEGER_APINT_RADIX> res = x2*y2;
    
    *rLow = res;
    *rHight = (res >> NUM_BIG_INTEGER_APINT_RADIX);
}

void big_integer_apint_radix_mult(limbs_radix_array r, limbs_radix_array a, limbs_radix_array b)
{
    std::cout << "a =" << big_integer_apint_radix_toHexString(a) << std::endl; 
    std::cout << "b =" << big_integer_apint_radix_toHexString(b) << std::endl; 

    if (big_integer_apint_radix_extraChecks) 
    {
        if (!big_integer_apint_radix_isNegative(a) && !big_integer_apint_radix_isNegative(b))
            // for non negative a and b, check that size is enough
            assertf((NUM_BIG_INTEGER_APINT_RADIX_BITS >= (big_integer_apint_radix_getLength(a) + big_integer_apint_radix_getLength(b))),
                "%d < (a len =%d) + (b len =%d)", NUM_BIG_INTEGER_APINT_RADIX_BITS,
                big_integer_apint_radix_getLength(a),
                big_integer_apint_radix_getLength(b));
    }
    
    big_integer_apint_radix_zero(r);

    int asize = big_integer_apint_radix_getLimbLength(a);
    int bsize = big_integer_apint_radix_getLimbLength(b);
    int rsize = NUM_BIG_INTEGER_APINT_RADIX_LIMBS;
    ap_uint<2*NUM_BIG_INTEGER_APINT_RADIX> carry;
    ap_uint<2*NUM_BIG_INTEGER_APINT_RADIX> accum;
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> adata;
    int idx;
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> lo;
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> hi;

    std::cout << "a size " << to_string(asize) << std::endl; 
    std::cout << "b size " << to_string(bsize) << std::endl; 
    
    for (int i=0; i< asize; i++)
    {
        carry = 0;
        adata = a[i];
        
        for (int j=0; j<bsize; j++)
        {
            idx = i + j;

            big_integer_apint_radix_mult_hi_lo(adata, b[j], &hi, &lo);

            ap_uint<2*NUM_BIG_INTEGER_APINT_RADIX> r2 = r[idx];
            ap_uint<2*NUM_BIG_INTEGER_APINT_RADIX> lo2 = lo;
            
            accum = (r2 + carry + lo2);
            
            carry = (accum >> NUM_BIG_INTEGER_APINT_RADIX);
            ap_uint<2*NUM_BIG_INTEGER_APINT_RADIX> hi2 = hi;
            carry += hi2;
            
            std::cout << "a[" << to_string(i) << "] = " << adata.toHexString() << std::endl; 
            std::cout << "b[" << to_string(j) << "] = " << b[j].toHexString() << std::endl; 
            std::cout << "hi = " << hi.toHexString() << std::endl; 
            std::cout << "lo= " << lo.toHexString() << std::endl; 

            std::cout << "carry = " << carry.toHexString() << std::endl; 

            
            r[idx] = (ap_uint<NUM_BIG_INTEGER_APINT_RADIX>) accum;
            
            std::cout << "r[" << to_string(idx) << "]  = " << r[idx].toHexString() << std::endl; 

        }
        
        if ((i + bsize)< rsize)
        {
            r[i + bsize] = carry;

            std::cout << "r[" << to_string(i+bsize) << "]  = " << r[i+bsize].toHexString() << std::endl; 

        }
    }
}