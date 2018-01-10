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
#include "../big_integer_base.h"

/**
 * 
 * @param r_data
 * @param r_base
 * @param r_size
 * @param y_data
 * @param y_base
 * @param y_size
 */
void big_integer_base_subtract_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, 
	unsigned int* y_data, const unsigned int y_base, const unsigned int y_size)
{
        big_integer_base_subtract(r_data, r_base, r_size, r_data, r_base, r_size, y_data, y_base, y_size);
}


/**
 * 
 * @param r_data
 * @param r_base
 * @param r_size
 * @param x_data
 * @param x_base
 * @param x_size
 * @param y_data
 * @param y_base
 * @param y_size
 */
void big_integer_base_subtract(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* x_data, const unsigned int x_base, const unsigned int x_size,
	unsigned int* y_data, const unsigned int y_base, const unsigned int y_size)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;

    for (int i=0; i < x_size; i++)
    {
        unsigned int sum = 0;

        if (i < x_size) sum += x_data[x_base+i];
        if (i < y_size) sum -= y_data[y_base+i];

        if (sum > x_data[x_base+i])
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
        r_data[r_base+i] = sum;
    }
}