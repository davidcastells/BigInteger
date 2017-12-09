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

void BigInteger::subtract(BigInteger* y)
{
    BigInteger::subtract(this, this, y);
}

/**
 * Static method. The output result object can be the same of any of the input parameters
 * @param r
 * @param x
 * @param y
 */
void BigInteger::subtract(BigInteger* r, BigInteger* a, BigInteger* b)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;

    unsigned int sum;
    unsigned int av, bv;
    unsigned int asize = a->m_size;
    unsigned int bsize = b->m_size;
    unsigned int rsize = r->m_size;

    if ((rsize == asize) && (rsize == bsize))
    {
        // Optimized version when all sizes are the same
        for (int i=0; i < rsize; i++)
        {
            sum = a->m_data[i] - b->m_data[i] - carryIn;

            carryIn = (sum > a->m_data[i]); 
            r->m_data[i] = sum;
        }
    }
    else
        for (int i=0; i < rsize; i++)
        {
            sum = 0;

            if (i < asize) sum += a->m_data[i];
            if (i < bsize) sum -= b->m_data[i];

            if (sum > a->m_data[i])
            {
                if (carryIn) sum--;
                carryOut = 1;
            }
            else if (carryIn)
            {
                carryOut = (sum == 0)? 1: 0;
                sum--;
            }

            carryIn = carryOut;
            r->m_data[i] = sum;
        }
}

/**
 * Compute r = (r - x ) mod m
 * If r < x we add the m until it is bigger
 * @param x
 * @param mod
 */
void BigInteger::subtractMod(BigInteger* x, BigInteger* m)
{
    assert(m_size >= m->m_size);
    assert(m_size >= x->m_size);

    while (isLessThan(x))
    {
//                std::cout << " submod: r = r - x, and r < x!!" << std::endl;
//                std::cout << " r: " << this->toHexString() << std::endl;
//                std::cout << " x: " << x->toHexString() << std::endl;
//                std::cout << " m: " << m->toHexString() << std::endl;
        add(m);
//                std::cout << " r+m: " << this->toHexString() << std::endl;
    }

    subtract(x);
}