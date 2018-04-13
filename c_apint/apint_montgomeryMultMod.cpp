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
#include "../big_integer_apint.h"
#include "../assertf.h"

#include <iostream>

ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_montgomeryMultBase2(ap_uint<NUM_BIG_INTEGER_APINT_BITS> x,
        ap_uint<NUM_BIG_INTEGER_APINT_BITS> y,
        ap_uint<NUM_BIG_INTEGER_APINT_BITS> m)
{
    if (big_integer_apint_extraChecks)
    {
        assert(x < m);
        assert(y < m);
        assertf(NUM_BIG_INTEGER_APINT_BITS > big_integer_apint_getLength(m),
                "r size = %d > m limbs = %d not true!", NUM_BIG_INTEGER_APINT_BITS, big_integer_apint_getLength(m));
    }
    
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> r = 0;
    
    //int n = m->getLimbLength();
    //int n = big_integer_apint_getLength(m);
    
    ap_uint<NUM_BIG_INTEGER_APINT_BITS*2> m2 = m << 1;
    ap_uint<NUM_BIG_INTEGER_APINT_BITS*2> m3 = m2 >> NUM_BIG_INTEGER_APINT_BITS;
    
    // initialize variables used in the loop
//    unsigned int u[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> t2;    
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> t3;
 
    int startLoop = 0;

    ap_uint<NUM_BIG_INTEGER_APINT_BITS> x2 = x;
    
    // i are bits now!
//    for (int i=0; i < n; i++)
    for (int i = 0; i < NUM_BIG_INTEGER_APINT_BITS; i++)
    {
        int mi = m3 & 0x1;

        if (!startLoop && mi)
        {
            startLoop = 1;
        }
        
        if (startLoop)
        {
            // std::cout << "["<< i << "]";

            // ui = (a0 + xi*y0 ) * m' mod b
            // since m' = 1 and b is 2...
            // ui = (a0 + xi*y0 ) % 2

    //        unsigned int ui = (big_integer_array_getBit(r, 0) + 
    //            big_integer_array_getBit(x, i) * big_integer_array_getBit(y, 0)) % 2;

            // A = (A  + xi * y + ui * m) / b

            // second term
            //if (big_integer_apint_getBit(x, i))
            if (x2 & 0x1)
                t2 = y;
            else
                t2 = 0;

            r = r + t2;
    
            // std::cout << " r = " << r.toHexString() << std::endl;

            // third term
            if (big_integer_apint_isOdd(r))
                t3 = m;
            else
                t3 = 0;

            r = r + t3;
    
            // std::cout << " r = " << r.toHexString() << std::endl;

            r = r >> 1;            

            x2 >>= 1;
        }

        m2 <<= 1;
        m3 = m2 >> NUM_BIG_INTEGER_APINT_BITS;
   
    }
    
    // instead of checking if (r >= m) always subtract and then check if the result is negative
//    if (r >= m)
//    {
//        r = r - m;
//        // std::cout << " r = " << r.toHexString() << std::endl;
//    }
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> r2 = r - m;
    if (((r >> (NUM_BIG_INTEGER_APINT_BITS-1)) & 0x1)) && ! big_integer_apint_isZero(r2))
    {
        // if negative, then r was already smaller than m, no change
    }
    else
    {
        std::cout << " r = " << r.toHexString() << std::endl;
        std::cout << " m = " << m.toHexString() << std::endl;
        std::cout << " r2 = " << r2.toHexString() << std::endl;
        r = r2;
        std::cout << " r = " << r.toHexString() << std::endl;
    }
    
    return r;
}