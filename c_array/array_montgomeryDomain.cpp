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

#include <assert.h>

void big_integer_array_radixFromMontgomeryModBase2(limbs_array radix, limbs_array m)
{
    if (big_integer_array_extraChecks)
        assert(big_integer_array_getNumBits(radix) > big_integer_array_getLimbLength(m));
    
    int mLen = big_integer_array_getLength(m);

    big_integer_array_setIntValue(radix, 1);
    big_integer_array_shiftLeft_short(radix, mLen);      
}

void big_integer_array_radixFromMontgomeryMod(limbs_array radix, limbs_array m)
{
    if (big_integer_array_extraChecks)
        assert(big_integer_array_getNumBits(radix) > big_integer_array_getLimbLength(m));
    
    int mLen = big_integer_array_getLimbLength(m)*32;

    big_integer_array_setIntValue(radix, 1);
    big_integer_array_shiftLeft_short(radix, mLen);    
}

void big_integer_array_mprimeFromMontgomeryRadix(limbs_array mprime, limbs_array m, limbs_array radix)
{
    limbs_array temp;
    big_integer_array_copy(temp, m);
    
    limbs_array invTemp;
    big_integer_array_inverseMod(invTemp, temp, radix);    // invTemp = (m mod radix) ^ (-1) mod radix
 
    if (big_integer_array_isZero(invTemp))
    {
        big_integer_array_zero(mprime);
    }
    else
    {
        if (big_integer_array_verbosity > VERBOSITY_LEVEL_MONTGOMERY)
        {
            std::cout << "BigInteger::mprimeFromMontgomeryRadix m = " << big_integer_array_toHexString(m) << std::endl;
            std::cout << "BigInteger::mprimeFromMontgomeryRadix m^-1 = " << big_integer_array_toHexString(invTemp) << std::endl;
            std::cout << "BigInteger::mprimeFromMontgomeryRadix radix = " << big_integer_array_toHexString(radix) << std::endl;
        }
        
        big_integer_array_copy(mprime, radix);
        big_integer_array_subtract_short(mprime, invTemp);


        if (big_integer_array_verbosity > VERBOSITY_LEVEL_MONTGOMERY) 
            std::cout << "BigInteger::mprimeFromMontgomeryRadix mprime = " << big_integer_array_toHexString(mprime) << std::endl;
    }
}