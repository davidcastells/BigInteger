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


void big_integer_inverseMod(big_integer* ret, big_integer* a, big_integer* b)
{
//    if (extraChecks)
    {
        assert(big_integer_isLessThan(a, b));
//                assert(b->m_size == ret->m_size);
//                assert(a->m_size == ret->m_size);
    }

    int maxSize = ret->m_size;
    if (a->m_size > maxSize) maxSize = a->m_size;
    if (b->m_size > maxSize) maxSize = b->m_size;


    // int t, nt, r, nr, q, tmp;
    // if (b < 0) b = -b;   // not necessary, always positive
    // if (a < 0) a = b - (-a % b); // not necessary, always positive

    big_integer t; 
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(t, maxSize);
    big_integer nt; 
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(nt, maxSize);
    big_integer r;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(r, maxSize);
    big_integer nr; 
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(nr, maxSize);
    big_integer q;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(q, maxSize);
    big_integer tmp; 
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(tmp, maxSize);
    big_integer p; 
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(p, maxSize*2);

    big_integer_zero(&t);                        // t = 0;  
    big_integer_setIntValue(&nt, 1);             // nt = 1;  
    big_integer_copy(&r, b);                     // r = b;  
    big_integer_div_naive(a, b, &tmp, &nr);   // nr = a % b;

    while (!big_integer_isZero(&nr))            // while (nr != 0) 
    {
        if (big_integer_verbosity)
        {
            cerr << "r: " << big_integer_toHexString(&r) << std::endl;
            cerr << "nr: " << big_integer_toHexString(&nr) << std::endl;
            cerr << "t: " << big_integer_toHexString(&t) << std::endl;
            cerr << "nt: " << big_integer_toHexString(&nt) << std::endl;
        }

        big_integer_div_naive(&r, &nr, &q, &tmp);   // q = r/nr;
        big_integer_copy(&tmp, &nt);                              // tmp = nt;
        big_integer_mult(&p, &q, &nt);
        big_integer_copy(&nt, &t);
        big_integer_subtract_short(&nt, &p);                            // nt = t - q*nt;
        big_integer_copy(&t, &tmp);                               // t = tmp;

        if (big_integer_verbosity)
        {
            cerr << "r: " << big_integer_toHexString(&r) << endl;
            cerr << "nr: " << big_integer_toHexString(&nr) << endl;
            cerr << "t: " << big_integer_toHexString(&t) << endl;
            cerr << "nt: " << big_integer_toHexString(&nt) << endl;
        }

        big_integer_copy(&tmp, &nr);                              // tmp = nr; 
        big_integer_mult(&p, &q, &nr);
        big_integer_copy(&nr, &r);
        big_integer_subtract_short(&nr, &p);                            // nr = r - q*nr;
        big_integer_copy(&r, &tmp);
    }

    big_integer one;
    STATIC_ALLOCATE_AND_INIT_BIG_INTEGER_DATA(one, 1);
    big_integer_setIntValue(&one, 1);

    if (big_integer_isLessThan(&one, &r))     // if (r > 1) return -1;  / * No inverse * /
    {
        if (big_integer_verbosity > VERBOSITY_LEVEL_INV_MOD)
        {
            cerr << "BigInteger::inverseMod Number not invertible " << std::endl;
            cerr << "BigInteger::inverseMod a: " << big_integer_toHexString(a) << std::endl;
            cerr << "BigInteger::inverseMod b: " << big_integer_toHexString(b) << std::endl;
            cerr << "BigInteger::inverseMod r: " << big_integer_toHexString(&r) << std::endl;
        }
        //assert(false);
        big_integer_zero(&t);
    }
    else if (big_integer_isNegative(&t)) // if (t < 0)
    {
        big_integer_add_short(&t, b);       //  t += b;

        if (big_integer_verbosity > VERBOSITY_LEVEL_INV_MOD)
            cerr << "BigInteger::inverseMod t: " << big_integer_toHexString(&t) << endl;
    }

    big_integer_copy(ret, &t);
}
