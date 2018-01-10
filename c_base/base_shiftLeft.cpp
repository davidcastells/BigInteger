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

void big_integer_base_shiftLeft_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, int bits)
{
    big_integer_base_shiftLeft(r_data, r_base, r_size, r_data, r_base, r_size, bits );
}


/**
 * 
 * @param r
 * @param a
 * @param sv
 */
void big_integer_base_shiftLeft(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, int sv)
{
    int limbsShifted = sv / 32;
    int limbBitsShifted = sv % 32;
    int cs = (limbBitsShifted == 0) ? 0 : 32 - limbBitsShifted;
    
    for (int i= r_size-1; i >= 0; i--)
    {
        unsigned int v1 = 0;
        unsigned int v2 = 0;
        
        if ((i-limbsShifted) >= 0 && (i-limbsShifted) < a_size)
            v1 = a_data[a_base+i-limbsShifted] << limbBitsShifted;
        if ((i-limbsShifted-1) >= 0 && (i-limbsShifted-1) < a_size && cs)
            v2 = a_data[a_base+i-limbsShifted-1] >> cs;
        r_data[r_base+i] = v1 | v2;
    }
}
