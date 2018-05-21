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

void big_integer_apint_radix_multMontgomeryForm3(limbs_radix_array r, limbs_radix_array x, limbs_radix_array y, 
        limbs_radix_array m, limbs_radix_array mprime)
{
    big_integer_apint_radix_verbosity = VERBOSITY_LEVEL_MULT_MONTGOMERY +1;
    
    int i;

    if (big_integer_apint_radix_extraChecks)
    {
        assert(r != x);
        assert(r != y);
        assert(r != m);
        assert(r != mprime);
        assert(big_integer_apint_radix_isLessThan(x, m));
        assert(big_integer_apint_radix_isLessThan(y, m));
    }

    unsigned int sr = big_integer_apint_radix_getLimbLength(m);
    
    limbs_radix_array2 t_big;
    limbs_radix_array tm;
    limbs_radix_array2 tmm_big;
    
    big_integer_apint_radix_mult_big(t_big, x, y);
    big_integer_apint_radix_multLow(tm, t_big, mprime, sr);  
    big_integer_apint_radix_mult_big(tmm_big, tm, m);

    if (big_integer_apint_radix_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
    {
        std::cout << "big_integer_apint_radix_multMontgomeryForm3 x:" << big_integer_apint_radix_toHexString(x) << std::endl;
        std::cout << "big_integer_apint_radix_multMontgomeryForm3 y:" << big_integer_apint_radix_toHexString(y) << std::endl;
        std::cout << "big_integer_apint_radix_multMontgomeryForm3 t:" << big_integer_apint_radix_toHexString_big(t_big) << std::endl;
        std::cout << "big_integer_apint_radix_multMontgomeryForm3 m:" << big_integer_apint_radix_toHexString(m) << std::endl;
        std::cout << "big_integer_apint_radix_multMontgomeryForm3 mprime:" << big_integer_apint_radix_toHexString(mprime) << std::endl;
        std::cout << "big_integer_apint_radix_multMontgomeryForm3 tm:" << big_integer_apint_radix_toHexString(tm) << std::endl;
        std::cout << "big_integer_apint_radix_multMontgomeryForm3 tmm:" << big_integer_apint_radix_toHexString_big(tmm_big) << std::endl;
    }

    limbs_radix_array2 u_big;
    big_integer_apint_radix_add_big(u_big, t_big, tmm_big);

    if (big_integer_apint_radix_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) 
        std::cout << "big_integer_apint_radix_multMontgomeryForm3 u:" << big_integer_apint_radix_toHexString_big(u_big) << std::endl;
    
    big_integer_apint_radix_shiftRight_limbs(r, u_big, sr);

    if (big_integer_apint_radix_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
    {
        std::cout << "big_integer_apint_radix_multMontgomeryForm3 shifting right >> " << (sr*NUM_BIG_INTEGER_APINT_RADIX) << std::endl;
        std::cout << "big_integer_apint_radix_multMontgomeryForm3 r:" << big_integer_apint_radix_toHexString(r) << std::endl;
    }
    
    if (big_integer_apint_radix_isLessThan(m, r))   // if (r >= m) r = r - m
    {
        big_integer_apint_radix_subtract_short(r, m);

        if (big_integer_apint_radix_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
            std::cout << "big_integer_apint_radix_multMontgomeryForm3 r:" << big_integer_apint_radix_toHexString(r) << std::endl;

    }
}