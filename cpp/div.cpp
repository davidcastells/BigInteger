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

void BigInteger::div(BigInteger* m)
{
    BigInteger ref(*this);
    BigInteger r(*this);

    BigInteger::div_naive(&ref, m, this, &r);
}
 
        
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
void BigInteger::div_naive(BigInteger* x,
        BigInteger* y, 
        BigInteger* q, 
        BigInteger* r)
{
    if (verbosity> VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div x = " << x->toHexString() << std::endl;
    if (verbosity> VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div /   " << y->toHexString() << std::endl;

    
    //q.initSize(x->m_size);
    //r.initSize(x->m_size);
    q->zero();

    if (x->isLessThan(y))
    {
        r->copy(x);
        //q->copy(&q);
        return;
    }


    // get the length of y
    int yl = y->getLength();
    int rl = x->getLength();

    int downBit = rl-yl;

    if (extraChecks) assert(downBit >= 0);

    // get the yl most significant bits of ref
    //r->copy(&ref);
    //if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div r " <<  r->toHexString() << std::endl;

    BigInteger::range(r, x, rl-1, downBit--);

    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div r range(" << to_string(rl-1) << "," << to_string(downBit+1) << ") = " << r->toHexString() << std::endl;

    if (r->isLessThan(y))
    {
        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div r < divisor" << std::endl;

        // take another bit from ref
        BigInteger::range(r, x, rl-1, downBit--);

        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div r range(" << to_string(rl-1) << "," << to_string(downBit+1) << ") = " << r->toHexString() << std::endl;
    }

    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div term = " << r->toHexString() << std::endl;
    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div      - " << y->toHexString() << std::endl;

    q->inc();
    r->subtract(y);

    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div      = " << r->toHexString() << std::endl;
    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div    q = " << q->toHexString() << std::endl;

    // take another bit
    while (downBit >= 0)
    {
        r->shiftLeft(1);
        if (x->getBit(downBit--))
            r->inc();

        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div term = " << r->toHexString() << std::endl;

        if (r->isLessThan(y))
        {
            q->shiftLeft(1);    // put a zero in q
            //goto loop;

            if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div  0 q = " << q->toHexString() << std::endl;
        }
        else
        {
            q->shiftLeft(1);     // put a one in q
            q->inc();

        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div  1 q = " << q->toHexString() << std::endl;

        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div term = " << r->toHexString() << std::endl;
        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div      - " << y->toHexString() << std::endl;

            r->subtract(y);
            //goto loop;

        if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div      = " << r->toHexString() << std::endl;

        }
    } 

    //nr->copy(&r);
    //nq->copy(&q);

    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div     nr = " << r->toHexString() << std::endl;
    if (verbosity>VERBOSITY_LEVEL_DIV) std::cout << "BigInteger.div     nq = " << q->toHexString() << std::endl;
}