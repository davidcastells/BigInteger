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

void big_integer_base_mod_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, 
	unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    unsigned int ref_data[r_size];
    const unsigned int ref_base = 0;
    const unsigned int ref_size = r_size;
    big_integer_base_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);
	
    unsigned int q_data[r_size];
    const unsigned int q_base = 0;
    const unsigned int q_size = r_size;
	
    big_integer_base_div_naive(ref_data, ref_base, ref_size, m_data, m_base, m_size,
    	q_data, q_base, q_size, r_data, r_base, r_size);
}

void big_integer_base_mod(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
        unsigned int* x_data, const unsigned int x_base, const unsigned int x_size,
        unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    big_integer_base_mod_naive(x_data, x_base, x_size, m_data, m_base, m_size, r_data, r_base, r_size);    
}

void big_integer_base_mod_naive(unsigned int* x_data, const unsigned int x_base, const unsigned int x_size,
        unsigned int* y_data, const unsigned int y_base, const unsigned int y_size, 
        unsigned int* r_data, const unsigned int r_base, const unsigned int r_size)
{
    assert(r_data != x_data);
    assert(r_data != y_data);
    
    if (base_verbosity> 6) std::cout << "big_integer_div_naive x = " << big_integer_base_toHexString(x_data, x_base, x_size) << std::endl;
    if (base_verbosity> 6) std::cout << "big_integer_div_naive /   " << big_integer_base_toHexString(y_data, y_base, y_size) << std::endl;
    

    int ret = big_integer_base_isLessThan(x_data, x_base, x_size, y_data, y_base, y_size);
    
    if (ret)
    {
    	big_integer_base_copy(r_data, r_base, r_size, x_data, x_base, x_size);
//        big_integer_copy(nq_data, nq_base, nq_size, q_data, q_base, q_size);
    }
    else
    {
		// get the length of y
		int yl = big_integer_base_getLength(y_data, y_base, y_size);
		int rl = big_integer_base_getLength(x_data, x_base, x_size);
	
		int downBit = rl-yl;
	
		// get the yl most significant bits of ref
		big_integer_base_range(r_data, r_base, r_size, x_data, x_base, x_size, rl-1, downBit);
		
                if (base_verbosity> 6) std::cout << "big_integer_div_naive x[" << (rl-1) << ","<< downBit <<" ]   " << big_integer_base_toHexString(r_data, r_base, r_size) << std::endl;
                
                downBit--;
	
                
		ret = big_integer_base_isLessThan(r_data, r_base, r_size, y_data, y_base, y_size);
		if (ret)
		{
	
			// take another bit from ref
			big_integer_base_range(r_data, r_base, r_size, x_data, x_base, x_size, rl-1, downBit);
                        
                        if (base_verbosity> 6) std::cout << "big_integer_div_naive x[" << (rl-1) << ","<< downBit <<" ]   " << big_integer_base_toHexString(r_data, r_base, r_size) << std::endl;
                        
			downBit--;	
		}
	
		big_integer_base_subtract_short(r_data, r_base, r_size, y_data, y_base, y_size);	
	
                if (base_verbosity> 6) std::cout << "big_integer_div_naive r=" << big_integer_base_toHexString(r_data, r_base, r_size) << std::endl;
                
		// take another bit
		while (downBit >= 0)
		{
			big_integer_base_shiftLeft_short(r_data, r_base, r_size, 1);

			if (big_integer_base_getBit(x_data, x_base, x_size, downBit))
			{
				big_integer_base_inc(r_data, r_base, r_size);
			}
			
			downBit--;

			ret = big_integer_base_isLessThan(r_data, r_base, r_size, y_data, y_base, y_size);

			if (ret)
			{
//				big_integer_shiftLeft_short(q_data, q_base, q_size, 1);	// put a zero in q
			}
			else
			{
//				big_integer_shiftLeft_short(q_data, q_base, q_size, 1);	// put a one in q
//				big_integer_inc(q_data, q_base, q_size);
	
				big_integer_base_subtract_short(r_data, r_base, r_size, y_data, y_base, y_size);	
			}
                        
//                        if (verbosity> 6) std::cout << "big_integer_div_naive q=" << big_integer_toHexString(q_data, q_base, q_size) << std::endl;
                        if (base_verbosity> 6) std::cout << "big_integer_div_naive r=" << big_integer_base_toHexString(r_data, r_base, r_size) << std::endl;
		} 
	
//		big_integer_copy(nr_data, nr_base, nr_size, r_data, r_base, r_size);
//		big_integer_copy(nq_data, nq_base, nq_size, q_data, q_base, q_size);
	}
    
}
