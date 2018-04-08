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
    int n = big_integer_apint_getLength(m);
    
    // initialize variables used in the loop
//    unsigned int u[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> t2;    
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> t3;
    
    // i are bits now!
    for (int i=0; i < n; i++)
    {
        // ui = (a0 + xi*y0 ) * m' mod b
        // since m' = 1 and b is 2...
        // ui = (a0 + xi*y0 ) % 2
        
//        unsigned int ui = (big_integer_array_getBit(r, 0) + 
//            big_integer_array_getBit(x, i) * big_integer_array_getBit(y, 0)) % 2;
        
        // A = (A  + xi * y + ui * m) / b
        
        // second term
        if (big_integer_apint_getBit(x, i))
            t2 = y;
        else
            t2 = 0;
        
        r = r + t2;
//        cout << " r = " << r->toHexString() << endl;
        
        // third term
        if (big_integer_apint_isOdd(r))
            t3 = m;
        else
            t3 = 0;
        
        r = r + t3;
//        cout << " r = " << r->toHexString() << endl;

        r = r >> 1;

    }
    
    if (r >= m)
    {
        r = r - m;
//        cout << " r = " << r->toHexString() << endl;
    }
    
    return r;
}