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


/**
 * Computes 
 * Inspired in https://github.com/adamwalker/mmul/blob/master/crypto.c
 * 
 * @param r
 * @param x
 * @param y
 * @param m
 * @param mprime
 */
void big_integer_base_multMontgomeryForm3(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* x_data, const unsigned int x_base, const unsigned int x_size,
	unsigned int* y_data, const unsigned int y_base, const unsigned int y_size,
	unsigned int* m_data, const unsigned int m_base, const unsigned int m_size,
	unsigned int* mprime_data, const unsigned int mprime_base, const unsigned int mprime_size)
{
    int i;
    
    unsigned int t_data[x_size+y_size];
    const unsigned int t_base = 0;
    const unsigned int t_size = x_size + y_size;
    
    unsigned int pretm_data[t_size];
    const unsigned int pretm_base = 0;
    const unsigned int pretm_size = t_size;

    int sr = big_integer_base_getLimbLength(m_data, m_base, m_size);

    unsigned int tm_data[sr];
    const unsigned int tm_base = 0;
    const unsigned int tm_size = sr;

    unsigned int tmm_data[tm_size + sr];
    const unsigned int tmm_base = 0;
    const unsigned int tmm_size = tm_size + sr;
    
    big_integer_base_mult(t_data, t_base, t_size,
    	x_data, x_base, x_size,
    	y_data, y_base, y_size);
    
    big_integer_base_multLow(pretm_data, pretm_base, pretm_size,
    	t_data, t_base, t_size,
    	mprime_data, mprime_base, mprime_size);  // was mult low
    
    big_integer_base_copy(tm_data, tm_base, tm_size, pretm_data, pretm_base, pretm_size);
    big_integer_base_mult(tmm_data, tmm_base, tmm_size, tm_data, tm_base, tm_size, m_data, m_base, m_size);

    
    if (base_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
    {
        std::cout << "big_integer_base_multMontgomeryForm3 x:" << big_integer_base_toHexString(x_data, x_base, x_size) << std::endl;
        std::cout << "big_integer_base_multMontgomeryForm3 y:" << big_integer_base_toHexString(y_data, y_base, y_size) << std::endl;
        std::cout << "big_integer_base_multMontgomeryForm3 t:" << big_integer_base_toHexString(t_data, t_base, t_size) << std::endl;
        std::cout << "big_integer_base_multMontgomeryForm3 m:" << big_integer_base_toHexString(m_data, m_base, m_size) << std::endl;
        std::cout << "big_integer_base_multMontgomeryForm3 mprime:" << big_integer_base_toHexString(mprime_data, mprime_base, mprime_size) << std::endl;
        std::cout << "big_integer_base_multMontgomeryForm3 tm:" << big_integer_base_toHexString(tm_data, tm_base, tm_size) << std::endl;
        std::cout << "big_integer_base_multMontgomeryForm3 tmm:" << big_integer_base_toHexString(tmm_data, tmm_base, tmm_size) << std::endl;
    }

    unsigned int u_data[tmm_size];
    const unsigned int u_base = 0;
    const unsigned int u_size = tmm_size; 
    
    big_integer_base_add(u_data, u_base, u_size,
    	t_data, t_base, t_size,
    	tmm_data, tmm_base, tmm_size);

    if (base_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY) 
        cout << "big_integer_base:multMontgomeryForm3 u:" << big_integer_base_toHexString(u_data, u_base, u_size) << endl;

    
    big_integer_base_shiftRight(r_data, r_base, r_size, u_data, u_base, u_size, sr*32);

    if (base_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
    {
        cout << "big_integer_base_multMontgomeryForm3 shifting right >> " << (sr*32) << endl;
        cout << "big_integer_base_multMontgomeryForm3 r:" << big_integer_base_toHexString(r_data, r_base, r_size) << endl;
    }
    
    int ret;
    ret = big_integer_base_isLessThan(m_data, m_base, m_size, r_data, r_base, r_size);
    
    // (m->isLessThan(r))   // if (r >= m) r = r - m
    if (ret) 
    {
        big_integer_base_subtract_short(r_data, r_base, r_size, m_data, m_base, m_size);
        
        if (base_verbosity > VERBOSITY_LEVEL_MULT_MONTGOMERY)
            cout << "big_integer_base_multMontgomeryForm3 r:" << big_integer_base_toHexString(r_data, r_base, r_size) << endl;
    }
}