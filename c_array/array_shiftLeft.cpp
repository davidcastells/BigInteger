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

void big_integer_array_shiftLeft_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], int bits)
{
    big_integer_array_shiftLeft(r, r, bits );
}


/**
 * 
 * @param r
 * @param a
 * @param sv
 */
void big_integer_array_shiftLeft(limbs_array r, limbs_array a, int sv)
{
    int limbsShifted = sv / 32;
    int limbBitsShifted = sv % 32;
    int cs = (limbBitsShifted == 0) ? 0 : 32 - limbBitsShifted;
    
    for (int i= NUM_BIG_INTEGER_ARRAY_LIMBS-1; i >= 0; i--)
    {
        unsigned int v1 = 0;
        unsigned int v2 = 0;
        
        if ((i-limbsShifted) >= 0 && (i-limbsShifted) < NUM_BIG_INTEGER_ARRAY_LIMBS)
            v1 = a[i-limbsShifted] << limbBitsShifted;
        if ((i-limbsShifted-1) >= 0 && (i-limbsShifted-1) < NUM_BIG_INTEGER_ARRAY_LIMBS && cs)
            v2 = a[i-limbsShifted-1] >> cs;
        r[i] = v1 | v2;
    }
}
