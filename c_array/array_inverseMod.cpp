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

void big_integer_array_inverseMod(limbs_array ret, limbs_array a, limbs_array b)
{
    if (big_integer_array_extraChecks)
    {
        assert(big_integer_array_isLessThan(a, b));
//                assert(b->m_size == ret->m_size);
//                assert(a->m_size == ret->m_size);
    }

    int maxSize = NUM_BIG_INTEGER_ARRAY_LIMBS; //ret->m_size;
//    if (a->m_size > maxSize) maxSize = a->m_size;
//    if (b->m_size > maxSize) maxSize = b->m_size;


    // int t, nt, r, nr, q, tmp;
    // if (b < 0) b = -b;   // not necessary, always positive
    // if (a < 0) a = b - (-a % b); // not necessary, always positive

    unsigned int t[NUM_BIG_INTEGER_ARRAY_LIMBS];// t.initSize(maxSize);
    unsigned int nt[NUM_BIG_INTEGER_ARRAY_LIMBS]; //nt.initSize(maxSize);
    unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS];// r.initSize(maxSize);
    unsigned int nr[NUM_BIG_INTEGER_ARRAY_LIMBS];// nr.initSize(maxSize);
    unsigned int q[NUM_BIG_INTEGER_ARRAY_LIMBS];// q.initSize(maxSize);
    unsigned int tmp[NUM_BIG_INTEGER_ARRAY_LIMBS]; // tmp.initSize(maxSize);
    unsigned int p[NUM_BIG_INTEGER_ARRAY_LIMBS]; // p.initSize(maxSize*2);

    big_integer_array_zero(t);                       // t = 0;  
    big_integer_array_setIntValue(nt, 1);              // nt = 1;  
    big_integer_array_copy(r, b);                      // r = b;  
    big_integer_array_div_naive(a, b, tmp, nr);   // nr = a % b;

    while (!big_integer_array_isZero(nr))            // while (nr != 0) 
    {
        if (big_integer_array_verbosity)
        {
            std::cerr << "r: " << big_integer_array_toHexString(r) << std::endl;
            std::cerr << "nr: " << big_integer_array_toHexString(nr) << std::endl;
            std::cerr << "t: " << big_integer_array_toHexString(t) << std::endl;
            std::cerr << "nt: " << big_integer_array_toHexString(nt) << std::endl;
        }

        big_integer_array_div_naive(r, nr, q, tmp);     // q = r/nr;
        big_integer_array_copy(tmp, nt);                // tmp = nt;
        big_integer_array_mult(p, q, nt);               // p = q * nt
        big_integer_array_copy(nt, t);
        big_integer_array_subtract_short(nt, p);        // nt = t - q*nt;
        big_integer_array_copy(t, tmp);                 // t = tmp;

        if (big_integer_array_verbosity)
        {
            std::cerr << "r: " << big_integer_array_toHexString(r) << std::endl;
            std::cerr << "nr: " << big_integer_array_toHexString(nr) << std::endl;
            std::cerr << "t: " << big_integer_array_toHexString(t) << std::endl;
            std::cerr << "nt: " << big_integer_array_toHexString(nt) << std::endl;
        }

        big_integer_array_copy(tmp, nr);                              // tmp = nr; 
        big_integer_array_mult(p, q, nr);
        big_integer_array_copy(nr, r);
        big_integer_array_subtract_short(nr, p);                            // nr = r - q*nr;
        big_integer_array_copy(r, tmp);
    }

    unsigned int one[NUM_BIG_INTEGER_ARRAY_LIMBS];
    big_integer_array_setIntValue(one, 1);

    if (big_integer_array_isLessThan(one, r))     // if (r > 1) return -1;  / * No inverse * /
    {
        if (big_integer_array_verbosity > VERBOSITY_LEVEL_INV_MOD)
        {
            std::cerr << "BigInteger::inverseMod Number not invertible " << std::endl;
            std::cerr << "BigInteger::inverseMod a: " << big_integer_array_toHexString(a) << std::endl;
            std::cerr << "BigInteger::inverseMod b: " << big_integer_array_toHexString(b) << std::endl;
            std::cerr << "BigInteger::inverseMod r: " << big_integer_array_toHexString(r) << std::endl;
        }
        //assert(false);
        big_integer_array_zero(t);
    }
    else if (big_integer_array_isNegative(t)) // if (t < 0)
    {
        big_integer_array_add_short(t, b);       //  t += b;

        if (big_integer_array_verbosity > VERBOSITY_LEVEL_INV_MOD)
            std::cerr << "BigInteger::inverseMod t: " << big_integer_array_toHexString(t) << std::endl;
    }

    big_integer_array_copy(ret, t);
}