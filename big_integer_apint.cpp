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

#include "big_integer_apint.h"

void big_integer_apint_zeroHighBits(ap_uint<NUM_BIG_INTEGER_APINT_BITS> *ap_r, int fromBit)
{
    int sv = NUM_BIG_INTEGER_APINT_BITS - fromBit;
    
    ap_uint<NUM_BIG_INTEGER_APINT_BITS> v = *ap_r << sv;
    *ap_r = v >> sv;
}

void big_integer_apint_range(ap_uint<NUM_BIG_INTEGER_APINT_BITS> *ap_r, ap_uint<NUM_BIG_INTEGER_APINT_BITS> *ap_x, int upper, int lower)
{
    // we move 
    if (lower > 0)
        *ap_r = (*ap_x) >> lower;
    else
        *ap_r = *ap_x;
    
    big_integer_apint_zeroHighBits(ap_r, upper-lower+1);
}


   