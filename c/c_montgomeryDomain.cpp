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

/**
 * Returns the radix that suits the provided modulo (m)
 * Such that x is the smallest value that meets radix = (2^32)^x > m
 * @param radix
 * @param m
 */
void big_integer_radixFromMontgomeryMod(big_integer* radix, big_integer* m)
{
    assert(radix->m_size > big_integer_getLimbLength(m));
    
    int mLen = big_integer_getLimbLength(m)*32;

    big_integer_setIntValue(radix, 1);
    big_integer_shiftLeft_short(radix, mLen);      
}

/**
 * Computes m' in the montgomery domain, this is a value such m * m' mod radix = -1 
 * 
 * To compute it we first compute 
 *      m' = radix - (m ^ -1) mod radix
 * 
 * If number is not invertible, then mprime is zero
 * 
 * @param mprime
 * @param m
 * @param radix
 */
void big_integer_mprimeFromMontgomeryRadix(big_integer* mprime, big_integer* m, big_integer* radix)
{
    big_integer temp;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(temp, radix->m_size);
    big_integer_copy(&temp, m);
    big_integer invTemp;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(invTemp, radix->m_size);
    
    big_integer_inverseMod(&invTemp, &temp, radix);    // invTemp = (m mod radix) ^ (-1) mod radix
 
    if (big_integer_isZero(&invTemp))
    {
        big_integer_zero(mprime);
    }
    else
    {
        if (big_integer_verbosity > VERBOSITY_LEVEL_MONTGOMERY)
        {
            cout << "BigInteger::mprimeFromMontgomeryRadix m = " << big_integer_toHexString(m) << endl;
            cout << "BigInteger::mprimeFromMontgomeryRadix m^-1 = " << big_integer_toHexString(&invTemp) << endl;
            cout << "BigInteger::mprimeFromMontgomeryRadix radix = " << big_integer_toHexString(radix) << endl;
        }

        big_integer_copy(mprime, radix);
        big_integer_subtract_short(mprime, &invTemp);


        if (big_integer_verbosity > VERBOSITY_LEVEL_MONTGOMERY) 
            cout << "BigInteger::mprimeFromMontgomeryRadix mprime = " << big_integer_toHexString(mprime) << endl;
    }
}