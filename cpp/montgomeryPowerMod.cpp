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
 * As implemented in "Handbook of Applied Cryptography" algorithm 14.94
 * we compute r = x ^ e mod m
 * @param r
 * @param v
 * @param mod
 */
void BigInteger::powerModMontgomery(BigInteger* r, BigInteger* x, BigInteger* e, BigInteger* m, BigInteger* mprime, BigInteger* radix)
{
    if (extraChecks)
    {
        assert(!m->isZero());
        assert(!mprime->isZero());
        assert(r != x);
        assert(r != e);
        assert(r != m);
        assert(r != mprime);
        assert(r != radix);
    }
    BigInteger radix2;
    radix2.initSize(m->m_size);
    squareMod(&radix2, radix, m);

    if (verbosity > VERBOSITY_LEVEL_POWER_MOD)
    {
        std::cout << "BigInteger::powerModMontgomery x: " << x->toHexString() << std::endl;
        std::cout << "BigInteger::powerModMontgomery e: " << e->toHexString() << std::endl;
        std::cout << "BigInteger::powerModMontgomery radix: " << radix->toHexString() << std::endl;
        std::cout << "BigInteger::powerModMontgomery m: " << m->toHexString() << std::endl;
        std::cout << "BigInteger::powerModMontgomery m': " << mprime->toHexString() << std::endl;
        std::cout << "BigInteger::powerModMontgomery radix ^2 mod m: " << radix2.toHexString() << std::endl;
    }

    BigInteger xprime;
    xprime.initSize(m->m_size);

    // make sure x < m
    x->mod(m);

    // we compute xprime = x * (radix^2 mod m) * radix ^-1 mod m 
    multMontgomeryForm3(&xprime, x, &radix2, m, mprime);

    if (verbosity > VERBOSITY_LEVEL_POWER_MOD)
        std::cout << "BigInteger::powerModMontgomery xprime: " << xprime.toHexString() << std::endl;

//            BigInteger a(*radix);
//            a.mod(m);
//            a.reduceWorkingSize(m->m_size);

    BigInteger a;
    a.initSize(m->m_size);
    BigInteger::mod_naive( radix, m, &a);

    int t = e->getLength();

    BigInteger temp(a);

    for (int i=t-1; i >= 0; i--)
    {
        int ei = e->getBit(i);

        temp.copy(&a);
        multMontgomeryForm3(&a, &temp, &temp, m, mprime);

        if (ei)
        {
            temp.copy(&a);
            multMontgomeryForm3(&a, &temp, &xprime, m, mprime);
        }   
    }

    BigInteger one;
    one.initSize(1);
    one.setIntValue(1);

    multMontgomeryForm3(r, &a, &one, m, mprime);
}