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

#include "../to_string.h"

void big_integer_apint_radix_radixFromMontgomeryMod(limbs_radix_array radix, limbs_radix_array m)
{
    if (big_integer_apint_radix_extraChecks)
        assert(big_integer_apint_radix_getNumBits(radix) > big_integer_apint_radix_getLimbLength(m));
    
    int mLen = big_integer_apint_radix_getLimbLength(m)*NUM_BIG_INTEGER_APINT_RADIX;

    big_integer_apint_radix_setIntValue(radix, 1);
    big_integer_apint_radix_shiftLeft_short(radix, mLen);    
}

void big_integer_apint_radix_mprimeFromMontgomeryRadix(limbs_radix_array mprime, limbs_radix_array m, limbs_radix_array radix)
{
    limbs_radix_array temp;
    big_integer_apint_radix_copy(temp, m);
    
    limbs_radix_array invTemp;
    big_integer_apint_radix_inverseMod(invTemp, temp, radix);    // invTemp = (m mod radix) ^ (-1) mod radix
 
    if (big_integer_apint_radix_isZero(invTemp))
    {
        big_integer_apint_radix_zero(mprime);
    }
    else
    {
        if (big_integer_apint_radix_verbosity > VERBOSITY_LEVEL_MONTGOMERY)
        {
            std::cout << "big_integer_apint_radix_mprimeFromMontgomeryRadix m = " << big_integer_apint_radix_toHexString(m) << std::endl;
            std::cout << "big_integer_apint_radix_mprimeFromMontgomeryRadix m^-1 = " << big_integer_apint_radix_toHexString(invTemp) << std::endl;
            std::cout << "big_integer_apint_radix_mprimeFromMontgomeryRadix radix = " << big_integer_apint_radix_toHexString(radix) << std::endl;
        }
        
        big_integer_apint_radix_copy(mprime, radix);
        big_integer_apint_radix_subtract_short(mprime, invTemp);


        if (big_integer_apint_radix_verbosity > VERBOSITY_LEVEL_MONTGOMERY) 
            std::cout << "big_integer_apint_radix_mprimeFromMontgomeryRadix mprime = " << big_integer_apint_radix_toHexString(mprime) << std::endl;
    }
}