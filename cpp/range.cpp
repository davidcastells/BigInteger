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
 * 
 * @param up
 * @param down
 */
void BigInteger::range(int up, int down)
{
   BigInteger ref(*this);
   BigInteger::range(this, &ref, up, down);
}

/**
 * copy [upper, lower] bits from x to r
 * 
 * @param r return value
 * @param x original value
 * @param upper the index of the upper bit
 * @param lower the index of the lower bit (zero is the minimum value)
 */
void BigInteger::range(BigInteger* r, BigInteger* x, int upper, int lower)
{
    if (extraChecks)
    {
        assert(upper <= x->getNumBits());
        assert(lower <= x->getNumBits());
        assert(r->getNumBits() >= (upper-lower));
    }
 
    // we move 
    if (lower > 0)
        shiftRight(r, x, lower);
    else
        r->copy(x);
    
    zeroHighBits(r, upper-lower+1);
}
