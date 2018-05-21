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

#include "../big_integer_apint_radix.h"

void big_integer_apint_radix_range(limbs_radix_array r, limbs_radix_array x, int upper, int lower)
{
    if (big_integer_apint_radix_extraChecks)
    {
        assert(upper <= big_integer_apint_radix_getNumBits(x));
        assert(lower <= big_integer_apint_radix_getNumBits(x));
        assert(big_integer_apint_radix_getNumBits(r) >= (upper-lower));
    }
    
 
    // we move 
    if (lower > 0)
        big_integer_apint_radix_shiftRight(r, x, lower);
    else
        big_integer_apint_radix_copy(r, x);
    
    big_integer_apint_radix_zeroHighBits(r, upper-lower+1);
}

void big_integer_apint_radix_zeroHighBits(limbs_radix_array r, int fromBit)
{
    int fromLimb = fromBit / NUM_BIG_INTEGER_APINT_RADIX;
    int fromLimbBit = fromBit % NUM_BIG_INTEGER_APINT_RADIX;
    
    if ((fromLimb == NUM_BIG_INTEGER_APINT_RADIX_LIMBS) && (fromLimbBit == 0))
        return;
    
    if (fromLimb >= NUM_BIG_INTEGER_APINT_RADIX_LIMBS)
        return;    
    
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> one = 1;
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> mask = one;
    mask <<= fromLimbBit;
    mask -= one;
    
    r[fromLimb] = r[fromLimb] & mask;
    fromLimb++;
    
    while (fromLimb < NUM_BIG_INTEGER_APINT_RADIX_LIMBS)
    {
        r[fromLimb] = 0;
        fromLimb++;
    }
}
