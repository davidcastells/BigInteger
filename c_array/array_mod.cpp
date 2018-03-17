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


void big_integer_array_mod_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],
	unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    unsigned int ref[NUM_BIG_INTEGER_ARRAY_LIMBS];
    
    big_integer_array_copy(ref, r);
	
    big_integer_array_mod_naive(ref, m, r);
}

void big_integer_array_mod(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    big_integer_array_mod_naive(x, m, r);    
}

void big_integer_array_mod_naive(unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    assert(r != x);
    assert(r != y);
    
    if (big_integer_array_verbosity> 6) 
    {
        std::cout << "big_integer_div_naive x = " << big_integer_array_toHexString(x) << std::endl;
        std::cout << "big_integer_div_naive /   " << big_integer_array_toHexString(y) << std::endl;
    }

    int ret = big_integer_array_isLessThan(x, y);
    
    if (ret)
    {
    	big_integer_array_copy(r, x);
//        big_integer_copy(nq_data, nq_base, nq_size, q_data, q_base, q_size);
    }
    else
    {
        // get the length of y
        int yl = big_integer_array_getLength(y);
        int rl = big_integer_array_getLength(x);

        int downBit = rl-yl;

        // get the yl most significant bits of ref
        big_integer_array_range(r, x, rl-1, downBit);

        if (big_integer_array_verbosity> 6) 
            std::cout << "big_integer_div_naive x[" << (rl-1) << ","<< downBit <<" ]   " << big_integer_array_toHexString(r) << std::endl;

        downBit--;


        ret = big_integer_array_isLessThan(r, y);
        if (ret)
        {

                // take another bit from ref
                big_integer_array_range(r, x, rl-1, downBit);

                if (big_integer_array_verbosity> 6) 
                    std::cout << "big_integer_div_naive x[" << (rl-1) << ","<< downBit <<" ]   " << big_integer_array_toHexString(r) << std::endl;

                downBit--;	
        }

        big_integer_array_subtract_short(r, y);	

        if (big_integer_array_verbosity> 6)
            std::cout << "big_integer_div_naive r=" << big_integer_array_toHexString(r) << std::endl;

        // take another bit
        while (downBit >= 0)
        {
                big_integer_array_shiftLeft_short(r, 1);

                if (big_integer_array_getBit(x, downBit))
                {
                    big_integer_array_inc(r);
                }

                downBit--;

                ret = big_integer_array_isLessThan(r, y);

                if (ret)
                {
//				big_integer_shiftLeft_short(q_data, q_base, q_size, 1);	// put a zero in q
                }
                else
                {
//				big_integer_shiftLeft_short(q_data, q_base, q_size, 1);	// put a one in q
//				big_integer_inc(q_data, q_base, q_size);

                        big_integer_array_subtract_short(r, y);	
                }

//                        if (verbosity> 6) std::cout << "big_integer_div_naive q=" << big_integer_toHexString(q_data, q_base, q_size) << std::endl;
                if (big_integer_array_verbosity> 6) 
                    std::cout << "big_integer_div_naive r=" << big_integer_array_toHexString(r) << std::endl;
        } 

//		big_integer_copy(nr_data, nr_base, nr_size, r_data, r_base, r_size);
//		big_integer_copy(nq_data, nq_base, nq_size, q_data, q_base, q_size);
    }

}
