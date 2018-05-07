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

void big_integer_array_random(limbs_array v)
{
    unsigned int lc = (unsigned int) clock();
    while (lc == (unsigned int) clock());
    
    srand((unsigned int) clock());

    
    for (int i=0; i < NUM_BIG_INTEGER_ARRAY_LIMBS; i++)
    {
        v[i] = rand();
        
        // Mingw random seems to return 16 bit numbers
        #ifdef __MINGW32__
            v[i] <<= 16;
            v[i] |= rand();
        #endif

        // Windows random seems to return 16 bit numbers also
        #ifdef WIN32
            v[i] <<= 16;
            v[i] |= rand();
        #endif
    }
}


/**
 * Creates a random number having n bits
 */
void big_integer_array_random_bits(limbs_array v, int n)
{
    assert(n < big_integer_array_getNumBits(v));
    
    big_integer_array_random(v);
    int shift = big_integer_array_getNumBits(v) - n;
    big_integer_array_shiftRight_short(v, shift);    
}
