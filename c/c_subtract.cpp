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
#include "../big_integer.h"

void big_integer_subtract_short(big_integer* r, big_integer* y)
{
    big_integer_subtract(r, r, y);
}

void big_integer_subtract(big_integer* r, big_integer* x, big_integer* y)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;

    for (int i=0; i < x->m_size; i++)
    {
        unsigned int sum = 0;

        if (i < x->m_size) sum += x->m_data[i];
        if (i < y->m_size) sum -= y->m_data[i];

        if (sum > x->m_data[i])
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