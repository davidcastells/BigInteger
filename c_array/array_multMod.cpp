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

void big_integer_array_multMod_interleaved(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],
	unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS],
	unsigned int b[NUM_BIG_INTEGER_ARRAY_LIMBS],
	unsigned int mod[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    assert(big_integer_array_isLessThan(a, mod));
    assert(big_integer_array_isLessThan(b, mod));
    assert(big_integer_array_getNumBits(r) > (big_integer_array_getLength(mod) + 1));
    
    big_integer_array_zero(r);
    
    // @todo apply mod to a and b before proceeding
    // @todo find the smallest number and change order accordingly
    int n = big_integer_array_getLength(a);
    for (int i=n-1; i>=0; i-- )
    {
        big_integer_array_shiftLeft_short(r, 1);
        int xi = big_integer_array_getBit(a, i);
        
        if (xi)
            big_integer_array_add_short(r, b);
        
        if (big_integer_array_isLessThanEqual(mod, r))
            big_integer_array_subtract_short(r, mod);
        if (big_integer_array_isLessThanEqual(mod, r))
            big_integer_array_subtract_short(r, mod);
    }
}