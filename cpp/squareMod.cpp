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

/**
 * Computes r = v^2 mod m
 * 
 * @param r
 * @param v
 * @param mod
 */
void BigInteger::squareMod(BigInteger* r, BigInteger* v, BigInteger* m)
{
    if (extraChecks) 
    {
        assert(r != v);
        assert(r != m);
    }

    multMod(r, v, v, m);
}

/**
 * 
 * @param r the size of r must be bigger that the length of mod, because it 
 *          should be able to accommodate values bigger than 2*mod
 * @param v
 * @param m
 */
void BigInteger::squareMod_interleaved(BigInteger* r, BigInteger* v, BigInteger* m)
{
    if (extraChecks) 
    {
        assert(r != v);
        assert(r != m);
        assert(v->isLessThan(m));
        assert(r->getNumBits() > m->getLength() + 1);
    }

    multMod_interleaved(r, v, v, m);
}

/**
 * Assume that the value is already smaller than m
 * @param m the modulo
 */
void BigInteger::squareMod_interleaved(BigInteger* m)
{
    assert(isLessThan(m));
    BigInteger ref(*this);
    //ref.mod(m);
    squareMod_interleaved(this, &ref, m);
}

void BigInteger::squareMod(BigInteger* m)
{
    BigInteger ref(*this);
    //ref.mod(m);
    squareMod(this, &ref, m);
}