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
 * 
 * @param r
 * @param a
 * @param b
 * @param mod
 */        
void BigInteger::multMod(BigInteger* r, BigInteger* a, BigInteger* b, BigInteger* mod)
{
    BigInteger m;
    m.initSize(a->m_size + b->m_size);

    mult(&m, a, b);
    mod_naive(&m, mod, r);
}

 
/**
 * Computes this = this * b mod m
 * @param b
 * @param m
 */
void BigInteger::multMod(BigInteger* b, BigInteger* m)
{
    BigInteger ref(*this);
    BigInteger::multMod(this, &ref, b, m);
}

void BigInteger::multMod_interleaved(BigInteger* b, BigInteger* m)
{
    assert(isLessThan(m));
    assert(b->isLessThan(m));
    
    BigInteger ref(*this);
    BigInteger::multMod_interleaved(this, &ref, b, m);
}

/**
 * 
 * @param r the size of r must be bigger that the length of mod, because it 
 *          should be able to accommodate values bigger than 2*mod
 * @param a
 * @param b
 * @param mod
 */
void BigInteger::multMod_interleaved(BigInteger* r, BigInteger* a, BigInteger* b, BigInteger* mod)
{
    if (extraChecks)
    {
        assert(a->isLessThan(mod));
        assert(b->isLessThan(mod));
        assert(r->getNumBits() > mod->getLength() + 1);
    }
    
    if (verbosity)
    {
        std::cout << "a: " << a->toHexString() << std::endl;
        std::cout << "b: " << b->toHexString() << std::endl;
        std::cout << "mod: " << mod->toHexString() << std::endl;
    }
    
    r->zero();
    // @todo find the smallest number (a or b) and change order accordingly
    int n = a->getLength();
    for (int i=n-1; i>=0; i-- )
    {
        r->shiftLeft(1);
        if (verbosity) std::cout << "r: " << r->toHexString() << std::endl;
        
        int xi = a->getBit(i);
        
        if (xi)
        {
            r->add(b);
            if (verbosity) std::cout << "r: " << r->toHexString() << std::endl;
        }
        if (mod->isLessThanEqual(r))
        {
            r->subtract(mod);
            if (verbosity) std::cout << "r: " << r->toHexString() << std::endl;
        }
        if (mod->isLessThanEqual(r))
        {
            r->subtract(mod);
            if (verbosity) std::cout << "r: " << r->toHexString() << std::endl;
        }
    }
}   