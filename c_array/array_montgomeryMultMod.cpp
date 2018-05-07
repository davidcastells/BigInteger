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

#include <assert.h>

void big_integer_array_montgomeryMultBase2(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    if (big_integer_array_extraChecks)
    {
        assert(r != x);
        assert(r != y);
        assert(r != m);
        assert(big_integer_array_isLessThan(x, m));
        assert(big_integer_array_isLessThan(y, m));
        assertf(NUM_BIG_INTEGER_ARRAY_LIMBS > big_integer_array_getLimbLength(m),
                "r size = %d > m limbs = %d not true!", NUM_BIG_INTEGER_ARRAY_LIMBS, big_integer_array_getLimbLength(m));
    }
    
    big_integer_array_zero(r);
    
    //int n = m->getLimbLength();
    int n = big_integer_array_getLength(m);
    
    // initialize variables used in the loop
//    unsigned int u[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    unsigned int t2[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    unsigned int t3[NUM_BIG_INTEGER_ARRAY_LIMBS];
        
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
        if (big_integer_array_getBit(x, i))
            big_integer_array_copy(t2, y);
        else
            big_integer_array_zero(t2);
        
        big_integer_array_add_short(r, t2);
//        cout << " r = " << r->toHexString() << endl;
        
        // third term
        if (big_integer_array_isOdd(r))
            big_integer_array_copy(t3, m);
        else
            big_integer_array_zero(t3);
        
        big_integer_array_add_short(r, t3);
//        cout << " r = " << r->toHexString() << endl;

        big_integer_array_shiftRight_short(r, 1);   // /b
        
//        cout << " r = " << r->toHexString() << endl;

    }
    
    if (!big_integer_array_isLessThan(r, m))
    {
        big_integer_array_subtract_short(r, m);
//        cout << " r = " << r->toHexString() << endl;
    }
}

void big_integer_array_multMontgomeryForm3(limbs_array r, limbs_array x, limbs_array y, 
        limbs_array m, limbs_array mprime)
{
    int i;

    if (big_integer_array_extraChecks)
    {
        assert(r != x);
        assert(r != y);
        assert(r != m);
        assert(r != mprime);
        assert(big_integer_array_isLessThan(x, m));
        assert(big_integer_array_isLessThan(y, m));
    }

    unsigned int sr = big_integer_array_getLimbLength(m);
    
    limbs_array2 t_big;
    limbs_array tm;
    limbs_array2 tmm_big;
    
    big_integer_array_mult_big(t_big, x, y);
    big_integer_array_multLow(tm, t_big, mprime, sr);  
    big_integer_array_mult_big(tmm_big, tm, m);

    if (big_integer_array_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
    {
        std::cout << "big_integer_array_multMontgomeryForm3 x:" << big_integer_array_toHexString(x) << std::endl;
        std::cout << "big_integer_array_multMontgomeryForm3 y:" << big_integer_array_toHexString(y) << std::endl;
        std::cout << "big_integer_array_multMontgomeryForm3 t:" << big_integer_array_toHexString_big(t_big) << std::endl;
        std::cout << "big_integer_array_multMontgomeryForm3 m:" << big_integer_array_toHexString(m) << std::endl;
        std::cout << "big_integer_array_multMontgomeryForm3 mprime:" << big_integer_array_toHexString(mprime) << std::endl;
        std::cout << "big_integer_array_multMontgomeryForm3 tm:" << big_integer_array_toHexString(tm) << std::endl;
        std::cout << "big_integer_array_multMontgomeryForm3 tmm:" << big_integer_array_toHexString_big(tmm_big) << std::endl;
    }

    limbs_array2 u_big;
    big_integer_array_add_big(u_big, t_big, tmm_big);


    if (big_integer_array_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) 
        std::cout << "big_integer_array_multMontgomeryForm3 u:" << big_integer_array_toHexString_big(u_big) << std::endl;

    
    big_integer_array_shiftRight_limbs(r, u_big, sr);


    if (big_integer_array_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
    {
        std::cout << "big_integer_array_multMontgomeryForm3 shifting right >> " << (sr*32) << std::endl;
        std::cout << "big_integer_array_multMontgomeryForm3 r:" << big_integer_array_toHexString(r) << std::endl;
    }
    
    if (big_integer_array_isLessThan(m, r))   // if (r >= m) r = r - m
    {
        big_integer_array_subtract_short(r, m);

        if (big_integer_array_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
            std::cout << "big_integer_array_multMontgomeryForm3 r:" << big_integer_array_toHexString(r) << std::endl;

    }
}