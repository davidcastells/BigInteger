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

void big_integer_apint_radix_multMod_interleaved(limbs_radix_array r,
	limbs_radix_array a,
	limbs_radix_array b,
	limbs_radix_array mod)
{
    // big_integer_apint_radix_verbosity = VERBOSITY_LEVEL_MULT_MOD+1;
    
    assert(big_integer_apint_radix_isLessThan(a, mod));
    assert(big_integer_apint_radix_isLessThan(b, mod));
    assert(big_integer_apint_radix_getNumBits(r) > (big_integer_apint_radix_getLength(mod) + 1));
    
    if (big_integer_apint_radix_verbosity > VERBOSITY_LEVEL_MULT_MOD)
    {
        std::cout << "a = " << big_integer_apint_radix_toHexString(a) << std::endl; 
        std::cout << "b = " << big_integer_apint_radix_toHexString(b) << std::endl; 
        std::cout << "mod = " << big_integer_apint_radix_toHexString(mod) << std::endl; 
    }
    
    big_integer_apint_radix_zero(r);
    
    // @todo apply mod to a and b before proceeding
    // @todo find the smallest number and change order accordingly
    int n = big_integer_apint_radix_getLength(a);
    for (int i=n-1; i>=0; i-- )
    {
        big_integer_apint_radix_shiftLeft_short(r, 1);
        int xi = big_integer_apint_radix_getBit(a, i);

        if (big_integer_apint_radix_verbosity > VERBOSITY_LEVEL_MULT_MOD)
        {
            std::cout << "r (<<1) = " << big_integer_apint_radix_toHexString(r) << std::endl; 
            std::cout << "x["<< to_string(i) <<"] = " << to_string(xi) << std::endl; 
        }
        
        if (xi)
        {
            big_integer_apint_radix_add_short(r, b);
            
            if (big_integer_apint_radix_verbosity > VERBOSITY_LEVEL_MULT_MOD)
            {
                std::cout << "r += b = " << big_integer_apint_radix_toHexString(r) << std::endl; 
            }

        }
        
        
        if (big_integer_apint_radix_isLessThanEqual(mod, r)) // if (mod <= r) r -= mod;
            big_integer_apint_radix_subtract_short(r, mod);
        if (big_integer_apint_radix_isLessThanEqual(mod, r))
            big_integer_apint_radix_subtract_short(r, mod);
        
        if (big_integer_apint_radix_verbosity > VERBOSITY_LEVEL_MULT_MOD)
        {
            std::cout << "r -= mod " << big_integer_apint_radix_toHexString(r) << std::endl; 
        }
    }
}