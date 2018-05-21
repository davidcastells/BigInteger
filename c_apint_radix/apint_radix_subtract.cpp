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

void big_integer_apint_radix_subtract_short(limbs_radix_array r, limbs_radix_array y)
{
    big_integer_apint_radix_subtract(r, r, y);
}

void big_integer_apint_radix_subtract(limbs_radix_array r, limbs_radix_array x, limbs_radix_array y)
{
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> carryIn = 0;
    ap_uint<NUM_BIG_INTEGER_APINT_RADIX> carryOut = 0;

    for (int i=0; i < NUM_BIG_INTEGER_APINT_RADIX_LIMBS; i++)
    {
        ap_uint<NUM_BIG_INTEGER_APINT_RADIX> sum = 0;

//        if (i < x_size)
            sum += x[i];
//        if (i < y_size)
            sum -= y[i];

        if (sum > x[i])
        {
            if (carryIn) sum-=1;
            carryOut = 1;
        }
        else if (carryIn)
        {
            carryOut = (sum == 0)? 1: 0;
            sum-=1;
        }

        carryIn = carryOut;
        r[i] = sum;
    }
}