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
#include <iostream>

/**
 * Computes r = v^2 mod m
 * 
 * @param r
 * @param v
 * @param mod
 */
void big_integer_array_squareMod(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int v[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    if (big_integer_array_extraChecks) 
    {
        assert(r != v);
        assert(r != m);
    }

    unsigned int vmod[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    big_integer_array_mod(vmod, v, m);
    
    //big_integer_array_multMod(r, v, v, m);
    big_integer_array_multMod_interleaved(r, vmod, vmod, m);
    
//    if (big_integer_array_verbosity)
//    {
//        std::cout << " vmod = " << big_integer_array_toHexString(vmod) << std::endl;
//        std::cout << " m = " << big_integer_array_toHexString(m) << std::endl;
//        std::cout << " r = " << big_integer_array_toHexString(r) << std::endl;
//    }
}

