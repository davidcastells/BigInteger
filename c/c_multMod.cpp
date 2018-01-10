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

void big_integer_multMod_interleaved_short(big_integer* x, big_integer* b, big_integer* m)
{
    assert(big_integer_isLessThan(x, m));
    assert(big_integer_isLessThan(b, m));
    
    big_integer ref;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(ref, x->m_size);
    big_integer_copy(&ref, x);

    big_integer_multMod_interleaved(x, &ref, b, m);
}

void big_integer_multMod_interleaved(big_integer* r, big_integer* a, big_integer* b, big_integer* mod)
{
    assert(big_integer_isLessThan(a, mod));
    assert(big_integer_isLessThan(b, mod));
    assert(big_integer_getNumBits(r) > (big_integer_getLength(mod) + 1));
    
    big_integer_zero(r);
    
    // @todo apply mod to a and b before proceeding
    // @todo find the smallest number and change order accordingly
    int n = big_integer_getLength(a);
    for (int i=n-1; i>=0; i-- )
    {
        big_integer_shiftLeft_short(r, 1);
        int xi = big_integer_getBit(a, i);
        
        if (xi)
            big_integer_add_short(r, b);
        
        if (big_integer_isLessThanEqual(mod, r))
            big_integer_subtract_short(r, mod);
        if (big_integer_isLessThanEqual(mod, r))
            big_integer_subtract_short(r, mod);
    }
}   
