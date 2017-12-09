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

void BigInteger::shiftRight(int bits)
{
//    BigInteger ref(*this);
//    BigInteger::shiftRight(this, &ref, bits );
    
    BigInteger::shiftRight(this, this, bits );
}

/**
 * Shift right a number of bits, zeros are inserted at the left
 * First we move slots (limb multiples)
 * 
 *  For instance if slots is 2
 * 
 *  ... a[6] a[5] a[4] a[3] a[2]
 *  ... r[4] r[3] r[2] r[1] r[0]
 * 
 * @param r retsult r = a >> sv
 * @param a
 * @param sv    number of bits to shift to the right
 */
void BigInteger::shiftRight(BigInteger* r, BigInteger* a, int sv)
{
    unsigned int carry = 0;
    int limbsShifted = sv / 32;
    int limbBitsShifted = sv % 32;
    int cs = (limbBitsShifted == 0) ? 0 : 32 - limbBitsShifted;
    
    for (int i=0; i < r->m_size; i++)
    {
        unsigned int v1 = 0;
        unsigned int v2 = 0;
        
        if ((i+limbsShifted) < a->m_size)
            v1 = a->m_data[i+limbsShifted] >> limbBitsShifted;
        if ((i+limbsShifted+1) < a->m_size && cs)
            v2 = a->m_data[i+limbsShifted+1] << cs;
        r->m_data[i] = v1 | v2;
    }
}