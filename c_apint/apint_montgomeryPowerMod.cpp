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

ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_powerModMontgomeryBase2_noradix(ap_uint<NUM_BIG_INTEGER_APINT_BITS> x,
        ap_uint<NUM_BIG_INTEGER_APINT_BITS> e, ap_uint<NUM_BIG_INTEGER_APINT_BITS> m)
{
    if (big_integer_apint_extraChecks)
    {
        assertf(NUM_BIG_INTEGER_APINT_BITS > big_integer_apint_getLength(m),
                "r size = %d > m len = %d not true!", 
                NUM_BIG_INTEGER_APINT_BITS , big_integer_apint_getLength(m));
    }
    
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> radix;
        
    radix = big_integer_apint_radixFromMontgomeryModBase2(m);        
    //BigInteger::mprimeFromMontgomeryRadix(&mprime, m, &radix);
    
    return big_integer_apint_powerModMontgomeryBase2(x, e, m, radix);
}

ap_uint<NUM_BIG_INTEGER_APINT_BITS> big_integer_apint_powerModMontgomeryBase2(ap_uint<NUM_BIG_INTEGER_APINT_BITS> x,
        ap_uint<NUM_BIG_INTEGER_APINT_BITS> e,
        ap_uint<NUM_BIG_INTEGER_APINT_BITS> m, 
        ap_uint<NUM_BIG_INTEGER_APINT_BITS> radix)
{
    if (big_integer_apint_extraChecks)
    {
        assert(m != 0);
        assertf(NUM_BIG_INTEGER_APINT_BITS > big_integer_apint_getLength(m), 
                "r size = %d > m limbs = %d not true!", NUM_BIG_INTEGER_APINT_BITS, big_integer_apint_getLength(m));
    }
    
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> radix2 = big_integer_apint_squareMod(radix, m);

    if (big_integer_apint_verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        std::cout << "big_integer_array_powerModMontgomeryBase2 x: " << x.toHexString() << std::endl;
        std::cout << "big_integer_array_powerModMontgomeryBase2 e: " << e.toHexString() << std::endl;
        std::cout << "big_integer_array_powerModMontgomeryBase2 radix: " << radix.toHexString() << std::endl;
        std::cout << "big_integer_array_powerModMontgomeryBase2 m: " << m.toHexString() << std::endl;
//        std::cout << "BigInteger::powerModMontgomery m': " << mprime->toHexString() << std::endl;
        std::cout << "big_integer_array_powerModMontgomeryBase2 radix ^2 mod m: " << radix2.toHexString() << std::endl;
    }

    ap_uint<NUM_BIG_INTEGER_APINT_BITS> xprime;
    

    // make sure x < m
    x = big_integer_apint_mod(x, m);

    // we compute xprime = x * (radix^2 mod m) * radix ^-1 mod m 
    //multMontgomeryForm 3(&xprime, x, &radix2, m, mprime);
    xprime = big_integer_apint_montgomeryMultBase2(x, radix2, m);

    if (big_integer_apint_verbosity > VERBOSITY_LEVEL_POWER_MOD)
        std::cout << "big_integer_array_powerModMontgomeryBase2 xprime: " << xprime.toHexString() << std::endl;

//            BigInteger a(*radix);
//            a.mod(m);
//            a.reduceWorkingSize(m->m_size);

    ap_uint<NUM_BIG_INTEGER_APINT_BITS> a = big_integer_apint_mod(radix, m);

    if (big_integer_apint_verbosity > VERBOSITY_LEVEL_POWER_MOD)
        std::cout << "big_integer_array_powerModMontgomeryBase2 a: " << a.toHexString() << std::endl;
    
    int t = big_integer_apint_getLength(e);

    ap_uint<NUM_BIG_INTEGER_APINT_BITS> temp;
    
    for (int i=t-1; i >= 0; i--)
    {
        int ei = big_integer_apint_getBit(e, i);

        temp = a;
        //multMontgomeryForm3(&a, &temp, &temp, m, mprime);
        a = big_integer_apint_montgomeryMultBase2(temp, temp, m);
        
        if (big_integer_apint_verbosity > VERBOSITY_LEVEL_POWER_MOD)
        {
            std::cout << "a = a^2 * R^-1 mod m =  " << a.toHexString() << std::endl;
        }
        
        if (ei)
        {
            temp = a;
            //multMontgomeryForm3(&a, &temp, &xprime, m, mprime);
            a = big_integer_apint_montgomeryMultBase2(temp, xprime, m);
            
            if (big_integer_apint_verbosity > VERBOSITY_LEVEL_POWER_MOD)
            {
                std::cout << "a = x' * a * R^-1 mod m =  " << a.toHexString() << std::endl;
            }
        }   
    }

    ap_uint<NUM_BIG_INTEGER_APINT_BITS> one = 1;

    //multMontgomeryForm3(r, &a, &one, m, mprime);
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> r = big_integer_apint_montgomeryMultBase2(a, one, m);
    
    if (big_integer_apint_verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        std::cout << "r = a * 1 * R^-1 mod m =  " << r.toHexString() << std::endl;
    }
    
    return r;
}