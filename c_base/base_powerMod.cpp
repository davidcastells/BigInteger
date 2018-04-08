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

#include "../big_integer_base.h"

#include <assert.h>

void big_integer_base_powerMod_interleaved(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size,
        unsigned int * v_data, const unsigned int v_base, const unsigned int v_size,
        unsigned int * power_data, const unsigned int power_base, const unsigned int power_size,
        unsigned int * mod_data, const unsigned int mod_base, const unsigned int mod_size)
{
        //if (extraChecks)
        {
            assert(r_data != v_data);
            assert(r_data != power_data);
            assert(r_data != mod_data);
            assert(big_integer_base_getNumBits(r_data, r_base, r_size) > big_integer_base_getLength(v_data, v_base, v_size));
            assert(big_integer_base_getNumBits(r_data, r_base, r_size) > big_integer_base_getLength(power_data, power_base, power_size));
            assert(big_integer_base_getNumBits(r_data, r_base, r_size) > big_integer_base_getLength(mod_data, mod_base, mod_size));
        }

        unsigned int x_data[mod_size+1];
        unsigned int x_base = 0;
        unsigned int x_size = mod_size+1;
        big_integer_base_mod_naive(v_data, v_base, v_size, mod_data, mod_base, mod_size, x_data, x_base, x_size);

        
        int maxBit = big_integer_base_getLength(power_data, power_base, power_size);

        
        // we use a simple square-and-multiply algorithm,
//        if (verbosity) std::cout << "BigInteger::powerMod v " << x.toHexString() << std::endl;
//        if (verbosity) std::cout << "BigInteger::powerMod power " << y.toHexString() << std::endl;
//        if (verbosity) std::cout << "BigInteger::powerMod mod " << mod->toHexString() << std::endl;

        big_integer_base_setIntValue(r_data, r_base, r_size, 1);

//        while(!y.isZero())
        for (int i=0; i < maxBit; i++)
        {
                //if (y.isOdd())
            if (big_integer_base_getBit(power_data, power_base, power_size, i))
            {
//                    std::cout << " r " << r->toHexString() << std::endl;
//                    std::cout << "  * x " << x.toHexString() << std::endl;
//                    std::cout << " mod mod " << mod->toHexString() << std::endl;

                    big_integer_base_multMod_interleaved_short(r_data, r_base, r_size,
                            x_data, x_base, x_size,
                            mod_data, mod_base, mod_size);
                    
//                    std::cout << " = r " << r->toHexString() << std::endl;
                }

                // y.shiftRight(1);    // / 2
                big_integer_base_squareMod_interleaved_short(x_data, x_base, x_size, mod_data, mod_base, mod_size); 
        }

//        if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod r " << r->toHexString() << std::endl;
}
