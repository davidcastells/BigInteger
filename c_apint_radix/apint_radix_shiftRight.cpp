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


void big_integer_apint_radix_shiftRight(limbs_radix_array r, limbs_radix_array a, int sv)
{
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> carry = 0;
    int limbsShifted = sv / NUM_BIG_INTEGER_APINT_RADIX;
    int limbBitsShifted = sv % NUM_BIG_INTEGER_APINT_RADIX;
    int cs = (limbBitsShifted == 0) ? 0 : NUM_BIG_INTEGER_APINT_RADIX - limbBitsShifted;
    
    for (int i=0; i < NUM_BIG_INTEGER_APINT_RADIX_LIMBS; i++)
    {
        ap_uint<NUM_BIG_INTEGER_APINT_RADIX> v1 = 0;
        ap_uint<NUM_BIG_INTEGER_APINT_RADIX> v2 = 0;
        
        if ((i+limbsShifted) < NUM_BIG_INTEGER_APINT_RADIX_LIMBS)
            v1 = a[i+limbsShifted] >> limbBitsShifted;
        if ((i+limbsShifted+1) < NUM_BIG_INTEGER_APINT_RADIX_LIMBS && cs)
            v2 = a[i+limbsShifted+1] << cs;
        r[i] = v1 | v2;
    }
}


void big_integer_apint_radix_shiftRight_limbs(limbs_radix_array r, limbs_radix_array a, int limbsShifted)
{    
    for (int i=0; i < NUM_BIG_INTEGER_APINT_RADIX_LIMBS; i++)
    {
        ap_uint<NUM_BIG_INTEGER_APINT_RADIX> v1 = 0;
        
        if ((i+limbsShifted) < NUM_BIG_INTEGER_APINT_RADIX_LIMBS)
            v1 = a[i+limbsShifted];

        r[i] = v1;
    }
}