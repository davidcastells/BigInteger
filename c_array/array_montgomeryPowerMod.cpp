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


#include <iostream>

void big_integer_array_powerModMontgomeryBase2_noradix(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    if (big_integer_array_extraChecks)
    {
        assertf(big_integer_array_getNumBits(r) > big_integer_array_getLength(m),
                "r size = %d > m len = %d not true!", 
                big_integer_array_getNumBits(r) , big_integer_array_getLength(m));
    }
    
    unsigned int radix[NUM_BIG_INTEGER_ARRAY_LIMBS];
        
    big_integer_array_radixFromMontgomeryModBase2(radix, m);        
    //BigInteger::mprimeFromMontgomeryRadix(&mprime, m, &radix);
    
    big_integer_array_powerModMontgomeryBase2(r, x, e, m, radix);
}

/**
 * Computes powerMod using montgomery multiplication base2
 * @param r
 * @param x
 * @param e
 * @param m
 * @param mprime
 * @param radix
 */
void big_integer_array_powerModMontgomeryBase2(limbs_array r,
        limbs_array x,
        limbs_array e,
        limbs_array m, 
        limbs_array radix)
{
    if (big_integer_array_extraChecks)
    {
        assert(!big_integer_array_isZero(m));
        assert(r != x);
        assert(r != e);
        assert(r != m);
        assert(r != radix);
        assertf(NUM_BIG_INTEGER_ARRAY_LIMBS > big_integer_array_getLimbLength(m), 
                "r size = %d > m limbs = %d not true!", NUM_BIG_INTEGER_ARRAY_LIMBS, big_integer_array_getLimbLength(m));
    }
    
    unsigned int radix2[NUM_BIG_INTEGER_ARRAY_LIMBS];
    //radix2.initSize(m->m_size);
    big_integer_array_squareMod(radix2, radix, m);

    if (big_integer_array_verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        std::cout << "BigInteger::powerModMontgomery x: " << big_integer_array_toHexString(x) << std::endl;
        std::cout << "BigInteger::powerModMontgomery e: " << big_integer_array_toHexString(e) << std::endl;
        std::cout << "BigInteger::powerModMontgomery radix: " << big_integer_array_toHexString(radix) << std::endl;
        std::cout << "BigInteger::powerModMontgomery m: " << big_integer_array_toHexString(m) << std::endl;
//        std::cout << "BigInteger::powerModMontgomery m': " << mprime->toHexString() << std::endl;
        std::cout << "BigInteger::powerModMontgomery radix ^2 mod m: " << big_integer_array_toHexString(radix2) << std::endl;
    }

    unsigned int xprime[NUM_BIG_INTEGER_ARRAY_LIMBS];
    

    // make sure x < m
    big_integer_array_mod_short(x, m);

    // we compute xprime = x * (radix^2 mod m) * radix ^-1 mod m 
    //multMontgomeryForm 3(&xprime, x, &radix2, m, mprime);
    big_integer_array_montgomeryMultBase2(xprime, x, radix2, m);

    if (big_integer_array_verbosity > VERBOSITY_LEVEL_POWER_MOD)
        std::cout << "BigInteger::powerModMontgomery xprime: " << big_integer_array_toHexString(xprime) << std::endl;

//            BigInteger a(*radix);
//            a.mod(m);
//            a.reduceWorkingSize(m->m_size);

    unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    big_integer_array_mod(a, radix, m);

    if (big_integer_array_verbosity > VERBOSITY_LEVEL_POWER_MOD)
        std::cout << "BigInteger::powerModMontgomery a: " << big_integer_array_toHexString(a) << std::endl;
    
    big_integer_array_powerModMontgomeryBase2_xprime(r, a, e, m, xprime);
}

void big_integer_array_powerModMontgomeryBase2_xprime(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int xprime[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    int t = big_integer_array_getLength(e);

    unsigned int temp[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    for (int i=t-1; i >= 0; i--)
    {
        int ei = big_integer_array_getBit(e, i);

        big_integer_array_copy(temp, a);
        //multMontgomeryForm3(&a, &temp, &temp, m, mprime);
        big_integer_array_montgomeryMultBase2(a, temp, temp, m);
        
        if (big_integer_array_verbosity > VERBOSITY_LEVEL_POWER_MOD)
        {
            std::cout << "a = a^2 * R^-1 mod m =  " << big_integer_array_toHexString(a) << std::endl;
        }
        
        if (ei)
        {
            big_integer_array_copy(temp, a);
            //multMontgomeryForm3(&a, &temp, &xprime, m, mprime);
            big_integer_array_montgomeryMultBase2(a, temp, xprime, m);
            
            if (big_integer_array_verbosity > VERBOSITY_LEVEL_POWER_MOD)
            {
                std::cout << "a = x' * a * R^-1 mod m =  " << big_integer_array_toHexString(a) << std::endl;
            }
        }   
    }

    unsigned int one[NUM_BIG_INTEGER_ARRAY_LIMBS];
//    one.bit_integer_array_initSize(1);
    big_integer_array_setIntValue(one, 1);

    //multMontgomeryForm3(r, &a, &one, m, mprime);
    big_integer_array_montgomeryMultBase2(r, a, one, m);
    
    if (big_integer_array_verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        std::cout << "r = a * 1 * R^-1 mod m =  " << big_integer_array_toHexString(r) << std::endl;
    }
}

void big_integer_array_powerModMontgomery(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    unsigned int radix[NUM_BIG_INTEGER_ARRAY_LIMBS];
    unsigned int mprime[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    big_integer_array_radixFromMontgomeryMod(radix, m);        
    big_integer_array_mprimeFromMontgomeryRadix(mprime, m, radix);
    
    big_integer_array_powerModMontgomery_mprime(r, x, e, m, mprime, radix);
}

void big_integer_array_powerModMontgomery_mprime(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int e[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int mprime[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int radix[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    if (big_integer_array_extraChecks)
    {
        assert(!big_integer_array_isZero(m));
        assert(!big_integer_array_isZero(mprime));
        assert(r != x);
        assert(r != e);
        assert(r != m);
        assert(r != mprime);
        assert(r != radix);
    }

    unsigned int radix2[NUM_BIG_INTEGER_ARRAY_LIMBS];
    big_integer_array_squareMod(radix2, radix, m);

    if (big_integer_array_verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        std::cout << "BigInteger::powerModMontgomery x: " << big_integer_array_toHexString(x) << std::endl;
        std::cout << "BigInteger::powerModMontgomery e: " << big_integer_array_toHexString(e) << std::endl;
        std::cout << "BigInteger::powerModMontgomery radix: " << big_integer_array_toHexString(radix) << std::endl;
        std::cout << "BigInteger::powerModMontgomery m: " << big_integer_array_toHexString(m) << std::endl;
        std::cout << "BigInteger::powerModMontgomery m': " << big_integer_array_toHexString(mprime) << std::endl;
        std::cout << "BigInteger::powerModMontgomery radix ^2 mod m: " << big_integer_array_toHexString(radix2) << std::endl;
    }

    unsigned int xprime[NUM_BIG_INTEGER_ARRAY_LIMBS];

    // make sure x < m
    big_integer_array_mod_short(x, m);

    // we compute xprime = x * (radix^2 mod m) * radix ^-1 mod m 
    big_integer_array_multMontgomeryForm3(xprime, x, radix2, m, mprime);

    if (big_integer_array_verbosity > VERBOSITY_LEVEL_POWER_MOD)
        std::cout << "BigInteger::powerModMontgomery xprime: " << big_integer_array_toHexString(xprime) << std::endl;

//            BigInteger a(*radix);
//            a.mod(m);
//            a.reduceWorkingSize(m->m_size);

    unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS];
    big_integer_array_mod_naive( radix, m, a);

    int t = big_integer_array_getLength(e);

    unsigned int temp[NUM_BIG_INTEGER_ARRAY_LIMBS];
    big_integer_array_copy(temp, a);

    for (int i=t-1; i >= 0; i--)
    {
        int ei = big_integer_array_getBit(e, i);

        big_integer_array_copy(temp, a);
        big_integer_array_multMontgomeryForm3(a, temp, temp, m, mprime);

        if (big_integer_array_verbosity > VERBOSITY_LEVEL_POWER_MOD)
        {
            std::cout << "a = a^2 * R^-1 mod m =  " << big_integer_array_toHexString(a) << std::endl;
        }
        
        if (ei)
        {
            big_integer_array_copy(temp, a);
            big_integer_array_multMontgomeryForm3(a, temp, xprime, m, mprime);
            
            if (big_integer_array_verbosity > VERBOSITY_LEVEL_POWER_MOD)
            {
                std::cout << "a = x' * a * R^-1 mod m =  " << big_integer_array_toHexString(a) << std::endl;
            }
        }   
    }

    unsigned int one[NUM_BIG_INTEGER_ARRAY_LIMBS];
    big_integer_array_setIntValue(one, 1);

    big_integer_array_multMontgomeryForm3(r, a, one, m, mprime);
    
    if (big_integer_array_verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        std::cout << "r = a * 1 * R^-1 mod m =  " << big_integer_array_toHexString(r) << std::endl;
    }
}
