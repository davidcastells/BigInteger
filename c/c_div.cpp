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


using namespace std;

/**
 * Compute division, so that
 *  nq = x / y
 *  nr = x % y
 * 
 * @param x
 * @param y
 * @param nq
 * @param nr
 */
void big_integer_div_naive(big_integer* x, big_integer* y, big_integer* q, big_integer* r)
{
    if (big_integer_verbosity> 6) cout << "big_integer_div_naive x = " << big_integer_toHexString(x) << endl;
    if (big_integer_verbosity> 6) cout << "big_integer_div_naive /   " << big_integer_toHexString(y) << endl;
    
    big_integer_zero(q);

    int ret = big_integer_isLessThan(x, y);
    
    if (ret)
    {
    	big_integer_copy(r, x);
//        big_integer_copy(nq_data, nq_base, nq_size, q_data, q_base, q_size);
    }
    else
    {
        // get the length of y
        int yl = big_integer_getLength(y);
        int rl = big_integer_getLength(x);

        assert(rl);
                
        int downBit = rl-yl;
	
        // get the yl most significant bits of ref
        big_integer_range(r, x, rl-1, downBit);
		
        if (big_integer_verbosity> 6) 
            cout << "big_integer_div_naive x[" << (rl-1) << ","<< downBit <<" ]   " << big_integer_toHexString(r) << endl;

        downBit--;
	
                
        ret = big_integer_isLessThan(r, y);
        
        if (ret)
        {	
                // take another bit from ref
                big_integer_range(r, x, rl-1, downBit);

                if (big_integer_verbosity> 6) cout << "big_integer_div_naive x[" << (rl-1) << ","<< downBit <<" ]   " << big_integer_toHexString(r) << endl;

                downBit--;	
        }

        big_integer_inc(q);
        big_integer_subtract_short(r, y);	

        if (big_integer_verbosity> 6) cout << "big_integer_div_naive q=" << big_integer_toHexString(q) << endl;
        if (big_integer_verbosity> 6) cout << "big_integer_div_naive r=" << big_integer_toHexString(r) << endl;

        // take another bit
        while (downBit >= 0)
        {
                big_integer_shiftLeft_short(r, 1);

                if (big_integer_getBit(x, downBit))
                {
                        big_integer_inc(r);
                }

                downBit--;

                ret = big_integer_isLessThan(r, y);

                if (ret)
                {
                        big_integer_shiftLeft_short(q, 1);	// put a zero in q
                }
                else
                {
                        big_integer_shiftLeft_short(q, 1);	// put a one in q
                        big_integer_inc(q);

                        big_integer_subtract_short(r, y);	
                }

                if (big_integer_verbosity> 6) cout << "big_integer_div_naive q=" << big_integer_toHexString(q) << std::endl;
                if (big_integer_verbosity> 6) cout << "big_integer_div_naive r=" << big_integer_toHexString(r) << std::endl;
        } 
//		big_integer_copy(nr_data, nr_base, nr_size, r_data, r_base, r_size);
//		big_integer_copy(nq_data, nq_base, nq_size, q_data, q_base, q_size);
    }
}