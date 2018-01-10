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
#include <iostream>


void big_integer_base_multMod(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size,
	unsigned int* b_data, const unsigned int b_base, const unsigned int b_size,
	unsigned int* mod_data, const unsigned int mod_base, const unsigned int mod_size)
{
    unsigned int m_data[a_size+b_size];
    const unsigned int m_base = 0;
    const unsigned int m_size = a_size + b_size;
    
    unsigned int q_data[a_size];
    const unsigned int q_base = 0;
    const unsigned int q_size = a_size; 

    big_integer_base_mult(m_data, m_base, m_size, a_data, a_base, a_size, b_data, b_base, b_size);
    big_integer_base_div_naive(m_data, m_base, m_size, mod_data, mod_base, mod_size, q_data, q_base, q_size, r_data, r_base, r_size);
}

void big_integer_base_multMod_interleaved(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size,
	unsigned int* b_data, const unsigned int b_base, const unsigned int b_size,
	unsigned int* mod_data, const unsigned int mod_base, const unsigned int mod_size)
{
    assert(big_integer_base_isLessThan(a_data, a_base, a_size, mod_data, mod_base, mod_size));
    assert(big_integer_base_isLessThan(b_data, b_base, b_size, mod_data, mod_base, mod_size));
    assert(big_integer_base_getNumBits(r_data, r_base, r_size) > (big_integer_base_getLength(mod_data, mod_base, mod_size) + 1));
    
    big_integer_base_zero(r_data, r_base, r_size);
    
    // @todo apply mod to a and b before proceeding
    // @todo find the smallest number and change order accordingly
    int n = big_integer_base_getLength(a_data, a_base, a_size);
    for (int i=n-1; i>=0; i-- )
    {
        big_integer_base_shiftLeft_short(r_data, r_base, r_size, 1);
        int xi = big_integer_base_getBit(a_data, a_base, a_size, i);
        
        if (xi)
            big_integer_base_add_short(r_data, r_base, r_size, b_data, b_base, b_size);
        
        if (big_integer_base_isLessThanEqual(mod_data, mod_base, mod_size, r_data, r_base, r_size))
            big_integer_base_subtract_short(r_data, r_base, r_size, mod_data, mod_base, mod_size);
        if (big_integer_base_isLessThanEqual(mod_data, mod_base, mod_size, r_data, r_base, r_size))
            big_integer_base_subtract_short(r_data, r_base, r_size, mod_data, mod_base, mod_size);
    }
}   
