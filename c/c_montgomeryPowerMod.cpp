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
#include "../big_integer.h"

#include <assert.h>
#include <iostream>

using namespace std;

void big_integer_powerModMont(big_integer* r, big_integer* x, big_integer* e, big_integer* m)
{
    big_integer radix;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(radix, m->m_size+1);
    big_integer mprime;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(mprime, radix.m_size);
        
    big_integer_radixFromMontgomeryMod(&radix, m);        
    big_integer_mprimeFromMontgomeryRadix(&mprime, m, &radix);
    
    big_integer_powerModMontgomery(r, x, e, m, &mprime, &radix);
}

/**
 * 
 * @param r
 * @param x
 * @param e
 * @param m
 * @param mprime
 * @param radix
 */
void big_integer_powerModMontgomery(big_integer* r, big_integer* x, big_integer* e, big_integer* m, big_integer* mprime, big_integer* radix)
{
    //if (extraChecks)
    {
        assert(!big_integer_isZero(m));
        assert(!big_integer_isZero(mprime));
        assert(r != x);
        assert(r != e);
        assert(r != m);
        assert(r != mprime);
        assert(r != radix);
    }

    big_integer radix2;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(radix2, m->m_size);

    big_integer a;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(a, m->m_size);

    // a = radix mod m
    big_integer_mod(&a, radix, m);
    
    big_integer_squareMod(&radix2, &a, m);

    if (big_integer_verbosity  > VERBOSITY_LEVEL_POWER_MOD)
    {
        cout << "big_integer_powerModMontgomery x: " << big_integer_toHexString(x) << endl;
        cout << "big_integer_powerModMontgomery e: " << big_integer_toHexString(e) << endl;
        cout << "big_integer_powerModMontgomery radix: " << big_integer_toHexString(radix) << endl;
        cout << "big_integer_powerModMontgomery m: " << big_integer_toHexString(m) << endl;
        cout << "big_integer_powerModMontgomery m': " << big_integer_toHexString(mprime) << endl;
        cout << "big_integer_powerModMontgomery radix ^2 mod m: " << big_integer_toHexString(&radix2) << endl;
    }

    big_integer xprime;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(xprime, m->m_size);

    // make sure x < m
    big_integer_mod_short(x, m);

    // we compute xprime = x * (radix^2 mod m) * radix ^-1 mod m 
    big_integer_multMontgomeryForm3(&xprime, x, &radix2, m, mprime);

    if (big_integer_verbosity > VERBOSITY_LEVEL_POWER_MOD)
        cout << "big_integer_powerModMontgomery xprime: " << big_integer_toHexString(&xprime) << endl;


    int t = big_integer_getLength(e);

    big_integer temp;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(temp, m->m_size);
    big_integer_copy(&temp, &a);
    
    for (int i=t-1; i >= 0; i--)
    {
        int ei = big_integer_getBit(e, i);

        big_integer_copy(&temp, &a);
        big_integer_multMontgomeryForm3(&a, &temp, &temp, m, mprime);

        if (big_integer_verbosity > VERBOSITY_LEVEL_POWER_MOD)
        {
            cout << "a = a^2 * R^-1 mod m =  " << big_integer_toHexString(&a) << endl;
        }

        if (ei)
        {
            big_integer_copy(&temp, &a);
            big_integer_multMontgomeryForm3(&a, &temp, &xprime, m, mprime);
            
            if (big_integer_verbosity > VERBOSITY_LEVEL_POWER_MOD)
            {
                cout << "a = x' * a * R^-1 mod m =  " << big_integer_toHexString(&a) << endl;
            }
        }   
    }

    unsigned int one_data[]={1, 0};
    unsigned int one_size =2;
    big_integer one;
    big_integer_init(&one, one_data, one_size);
    

    big_integer_multMontgomeryForm3(r, &a, &one, m, mprime);
}