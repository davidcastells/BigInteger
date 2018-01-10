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

using namespace std;

void big_integer_base_multMod_interleaved_short(unsigned int * r_data, unsigned int r_base, unsigned int r_size,
        unsigned int * b_data, unsigned int b_base, unsigned int b_size,
        unsigned int * m_data, unsigned int m_base, unsigned int m_size)
{
    unsigned int ref_data[r_size];
    const unsigned int ref_base = 0;
    const unsigned int ref_size = r_size;
    big_integer_base_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);

    
    big_integer_base_multMod_interleaved(r_data, r_base, r_size, ref_data, ref_base, ref_size,
            b_data, b_base, b_size, m_data, m_base, m_size);
}

/**
 * Ensure that r and v are < than m
 * @param r_data
 * @param r_base
 * @param r_size
 * @param v_data
 * @param v_base
 * @param v_size
 * @param m_data
 * @param m_base
 * @param m_size
 */
void big_integer_base_squareMod_interleaved(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* v_data, const unsigned int v_base, const unsigned int v_size,
	unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    assert(r_data != v_data);
    assert(r_data != m_data);
    assert(big_integer_base_isLessThan(v_data, v_base, v_size, m_data, m_base, m_size));
        
    big_integer_base_multMod_interleaved(r_data, r_base, r_size,
        v_data, v_base, v_size,
    	v_data, v_base, v_size,
    	m_data, m_base, m_size);
}



/**
 * Computes r = v^2 mod m
 * 
 * @param r
 * @param v
 * @param mod
 */
void big_integer_base_squareMod(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* v_data, const unsigned int v_base, const unsigned int v_size,
	unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    big_integer_base_multMod(r_data, r_base, r_size,
    	v_data, v_base, v_size,
    	v_data, v_base, v_size,
    	m_data, m_base, m_size);
}





void big_integer_base_squareMod_short(unsigned int * r_data, unsigned int r_base, unsigned int r_size,
        unsigned int * m_data, unsigned int m_base, unsigned int m_size)
{
    unsigned int ref_data[r_size];
    const unsigned int ref_base = 0;
    const unsigned int ref_size = r_size;
    big_integer_base_mod_naive(r_data, r_base, r_size, m_data, m_base, m_size, ref_data, ref_base, ref_size);
    
    big_integer_base_squareMod_interleaved(r_data, r_base, r_size,
            ref_data, ref_base, ref_size,
            m_data, m_base, m_size);
}