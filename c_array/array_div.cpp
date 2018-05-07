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
#include "../to_string.h"

#include <assert.h>
#include <iostream>

void big_integer_array_div_naive(unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int q[NUM_BIG_INTEGER_ARRAY_LIMBS], 
        unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    if (big_integer_array_verbosity > VERBOSITY_LEVEL_DIV)
    {
        std::cout << "BigInteger.div x = " << big_integer_array_toHexString(x) << std::endl;
        std::cout << "BigInteger.div /   " << big_integer_array_toHexString(y) << std::endl;
    }

    
    //q.initSize(x->m_size);
    //r.initSize(x->m_size);
    big_integer_array_zero(q);

    if (big_integer_array_isLessThan(x, y))
    {
        big_integer_array_copy(r, x);
        //q->copy(&q);
        return;
    }


    // get the length of y
    int yl = big_integer_array_getLength(y);
    int rl = big_integer_array_getLength(x);

    int downBit = rl-yl;

    if (big_integer_array_extraChecks) 
        assert(downBit >= 0);

    // get the yl most significant bits of ref
    //r->copy(&ref);
    //if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div r " <<  r->toHexString() << std::endl;

    big_integer_array_range(r, x, rl-1, downBit--);

    if (big_integer_array_verbosity > VERBOSITY_LEVEL_DIV)
        std::cout << "BigInteger.div r range(" << to_string(rl-1) << "," << to_string(downBit+1) << ") = " << big_integer_array_toHexString(r) << std::endl;

    if (big_integer_array_isLessThan(r, y))
    {
        if (big_integer_array_verbosity > VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div r < divisor" << std::endl;

        // take another bit from ref
        big_integer_array_range(r, x, rl-1, downBit--);

        if (big_integer_array_verbosity > VERBOSITY_LEVEL_DIV)
            std::cout << "BigInteger.div r range(" << to_string(rl-1) << "," << to_string(downBit+1) << ") = " << big_integer_array_toHexString(r) << std::endl;
    }

    if (big_integer_array_verbosity > VERBOSITY_LEVEL_DIV)
    {
        std::cout << "BigInteger.div term = " << big_integer_array_toHexString(r) << std::endl;
        std::cout << "BigInteger.div      - " << big_integer_array_toHexString(y) << std::endl;
    }

    big_integer_array_inc(q);
    big_integer_array_subtract_short(r, y);

    if (big_integer_array_verbosity>VERBOSITY_LEVEL_DIV)
    {
        std::cout << "BigInteger.div      = " << big_integer_array_toHexString(r) << std::endl;
        std::cout << "BigInteger.div    q = " << big_integer_array_toHexString(q) << std::endl;
    }
    
    // take another bit
    while (downBit >= 0)
    {
        big_integer_array_shiftLeft_short(r, 1);
        if (big_integer_array_getBit(x, downBit--))
            big_integer_array_inc(r);

        if (big_integer_array_verbosity>VERBOSITY_LEVEL_DIV)
            std::cout << "BigInteger.div term = " << big_integer_array_toHexString(r) << std::endl;

        if (big_integer_array_isLessThan(r, y))
        {
            big_integer_array_shiftLeft_short(q, 1);    // put a zero in q
            //goto loop;

            if (big_integer_array_verbosity>VERBOSITY_LEVEL_DIV)
                std::cout << "BigInteger.div  0 q = " << big_integer_array_toHexString(q) << std::endl;
        }
        else
        {
            big_integer_array_shiftLeft_short(q, 1);     // put a one in q
            big_integer_array_inc(q);

        if (big_integer_array_verbosity>VERBOSITY_LEVEL_DIV)
        {
            std::cout << "BigInteger.div  1 q = " << big_integer_array_toHexString(q) << std::endl;
            std::cout << "BigInteger.div term = " << big_integer_array_toHexString(r) << std::endl;
            std::cout << "BigInteger.div      - " << big_integer_array_toHexString(y) << std::endl;
        }
            
            big_integer_array_subtract_short(r, y);
            //goto loop;

            if (big_integer_array_verbosity>VERBOSITY_LEVEL_DIV) 
                std::cout << "BigInteger.div      = " << big_integer_array_toHexString(r) << std::endl;

        }
    } 

    //nr->copy(&r);
    //nq->copy(&q);

    if (big_integer_array_verbosity>VERBOSITY_LEVEL_DIV) 
    {
        std::cout << "BigInteger.div     nr = " << big_integer_array_toHexString(r) << std::endl;
        std::cout << "BigInteger.div     nq = " << big_integer_array_toHexString(q) << std::endl;
    }
}