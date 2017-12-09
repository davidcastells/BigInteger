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
#include "../BigInteger.h"

#include <assert.h>
#include <iostream>

/**
 * Compute the inverse modulo fr = x^(-1) mod m
 * so that fr * x mod m = 1
 * 
 * 
 * from https://rosettacode.org/wiki/Modular_inverse
 * 
 * If the number is non invertible returns zero
 * 
 * @param ret    result = a ^ -1 mod b
 * @param a     number we are computing the inverse
 * @param b     modulo   
 */
void BigInteger::inverseMod(BigInteger* ret, BigInteger* a, BigInteger* b)
{
    if (extraChecks)
    {
        assert(a->isLessThan(b));
//                assert(b->m_size == ret->m_size);
//                assert(a->m_size == ret->m_size);
    }



    int maxSize = ret->m_size;
    if (a->m_size > maxSize) maxSize = a->m_size;
    if (b->m_size > maxSize) maxSize = b->m_size;


    // int t, nt, r, nr, q, tmp;
    // if (b < 0) b = -b;   // not necessary, always positive
    // if (a < 0) a = b - (-a % b); // not necessary, always positive

    BigInteger t; t.initSize(maxSize);
    BigInteger nt; nt.initSize(maxSize);
    BigInteger r; r.initSize(maxSize);
    BigInteger nr; nr.initSize(maxSize);
    BigInteger q; q.initSize(maxSize);
    BigInteger tmp; tmp.initSize(maxSize);
    BigInteger p; p.initSize(maxSize*2);

    t.zero();                       // t = 0;  
    nt.setIntValue(1);              // nt = 1;  
    r.copy(b);                      // r = b;  
    BigInteger::div_naive(a, b, &tmp, &nr);   // nr = a % b;

    while (!nr.isZero())            // while (nr != 0) 
    {
        if (verbosity)
        {
            std::cerr << "r: " << r.toHexString() << std::endl;
            std::cerr << "nr: " << nr.toHexString() << std::endl;
            std::cerr << "t: " << t.toHexString() << std::endl;
            std::cerr << "nt: " << nt.toHexString() << std::endl;
        }

        BigInteger::div_naive(&r, &nr, &q, &tmp);   // q = r/nr;
        tmp.copy(&nt);                              // tmp = nt;
        BigInteger::mult(&p, &q, &nt);
        nt.copy(&t);
        nt.subtract(&p);                            // nt = t - q*nt;
        t.copy(&tmp);                               // t = tmp;

        if (verbosity)
        {
            std::cerr << "r: " << r.toHexString() << std::endl;
            std::cerr << "nr: " << nr.toHexString() << std::endl;
            std::cerr << "t: " << t.toHexString() << std::endl;
            std::cerr << "nt: " << nt.toHexString() << std::endl;
        }

        tmp.copy(&nr);                              // tmp = nr; 
        BigInteger::mult(&p, &q, &nr);
        nr.copy(&r);
        nr.subtract(&p);                            // nr = r - q*nr;
        r.copy(&tmp);
    }

    BigInteger one;
    one.initSize(1);
    one.setIntValue(1);

    if (one.isLessThan(&r))     // if (r > 1) return -1;  / * No inverse * /
    {
        if (verbosity > VERBOSITY_LEVEL_INV_MOD)
        {
            std::cerr << "BigInteger::inverseMod Number not invertible " << std::endl;
            std::cerr << "BigInteger::inverseMod a: " << a->toHexString() << std::endl;
            std::cerr << "BigInteger::inverseMod b: " << b->toHexString() << std::endl;
            std::cerr << "BigInteger::inverseMod r: " << r.toHexString() << std::endl;
        }
        //assert(false);
        t.zero();
    }
    else if (t.isNegative()) // if (t < 0)
    {
        t.add(b);       //  t += b;

        if (verbosity > VERBOSITY_LEVEL_INV_MOD) std::cerr << "BigInteger::inverseMod t: " << t.toHexString() << std::endl;
    }

    ret->copy(&t);
}