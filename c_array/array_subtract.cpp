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

#include "../big_integer_array.h"

void big_integer_array_subtract_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS], unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
        big_integer_array_subtract(r, r, y);
}

void big_integer_array_subtract(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],
	unsigned int x[NUM_BIG_INTEGER_ARRAY_LIMBS],
	unsigned int y[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;

    for (int i=0; i < NUM_BIG_INTEGER_ARRAY_LIMBS; i++)
    {
        unsigned int sum = 0;

//        if (i < x_size)
            sum += x[i];
//        if (i < y_size)
            sum -= y[i];

        if (sum > x[i])
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
        r[i] = sum;
    }
}