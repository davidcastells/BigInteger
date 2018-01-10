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


void big_integer_mod_short(big_integer* x, big_integer* m)
{
    big_integer ref;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(ref, x->m_size);
    big_integer_copy(&ref, x);
    
    big_integer_mod_naive(&ref, m, x);
}

void big_integer_mod(big_integer* r, big_integer* x, big_integer* y)
{
    big_integer_mod_naive(x, y, r);
}

void big_integer_mod_naive(big_integer* x, big_integer* y, big_integer* r)
{
    if (big_integer_verbosity > VERBOSITY_LEVEL_DIV)
        cout << "big_integer_mod_naive x = " << big_integer_toHexString(x) << endl;
    
    if (big_integer_verbosity > VERBOSITY_LEVEL_DIV) 
        cout << "BigInteger.div /   " << big_integer_toHexString(y) << endl;

    //q.initSize(x->m_size);
    //r.initSize(x->m_size);

    if (big_integer_isLessThan(x, y))
    {
        big_integer_copy(r, x);
        //q->copy(&q);
        return;
    }


    // get the length of y
    int yl = big_integer_getLength(y);
    int rl = big_integer_getLength(x);

    int downBit = rl-yl;

//    if (extraChecks) 
        assert(downBit >= 0);

    // get the yl most significant bits of ref
    //r->copy(&ref);
    //if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div r " <<  r->toHexString() << std::endl;

    big_integer_range(r, x, rl-1, downBit--);

//    if (verbosity) // >VERBOSITY_LEVEL_DIV) 
//        cout << "BigInteger.div r range(" << to_string(rl-1) << "," << to_string(downBit+1) << ") = " << big_integer_toHexString(r) << endl;

    if (big_integer_isLessThan(r, y))
    {
        if (big_integer_verbosity) // >VERBOSITY_LEVEL_DIV)
            cout << "BigInteger.div r < divisor" << endl;

        // take another bit from ref
        big_integer_range(r, x, rl-1, downBit--);

//        if (verbosity) // >VERBOSITY_LEVEL_DIV) 
//            cout << "BigInteger.div r range(" << to_string(rl-1) << "," << to_string(downBit+1) << ") = " << big_integer_toHexString(r) << endl;
    }

    if (big_integer_verbosity >VERBOSITY_LEVEL_DIV) 
        cout << "big_integer_div term = " << big_integer_toHexString(r) << endl;
    if (big_integer_verbosity >VERBOSITY_LEVEL_DIV) 
        cout << "big_integer_div      - " << big_integer_toHexString(y) << endl;


    big_integer_subtract_short(r, y);

    if (big_integer_verbosity  >VERBOSITY_LEVEL_DIV)
        cout << "big_integer_div      = " << big_integer_toHexString(r) << endl;


    // take another bit
    while (downBit >= 0)
    {
        big_integer_shiftLeft_short(r, 1);
        if (big_integer_getBit(x, downBit--))
            big_integer_inc(r);

        if (big_integer_verbosity  >VERBOSITY_LEVEL_DIV)
            cout << "big_integer_div term = " << big_integer_toHexString(r) << endl;

        if (big_integer_isLessThan(r, y))
        {
            //goto loop;
        }
        else
        {
            if (big_integer_verbosity  >VERBOSITY_LEVEL_DIV)
                cout << "big_integer_div term = " << big_integer_toHexString(r) << endl;
            if (big_integer_verbosity  >VERBOSITY_LEVEL_DIV) 
                cout << "big_integer_div      - " << big_integer_toHexString(y) << endl;

            big_integer_subtract_short(r, y);
            //goto loop;

            if (big_integer_verbosity  >VERBOSITY_LEVEL_DIV) 
                cout << "big_integer_div      = " << big_integer_toHexString(r) << endl;
        }
    } 


    //nr->copy(&r);
    //nq->copy(&q);

    if (big_integer_verbosity  >VERBOSITY_LEVEL_DIV) 
        cout << "big_integer_div     nr = " << big_integer_toHexString(r) << endl;

}