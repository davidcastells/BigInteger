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

#include <assert.h>
#include <iostream>



void big_integer_add_short(big_integer* r, big_integer* b)
{
    big_integer_add(r, r, b);
}


void big_integer_add(big_integer* r, big_integer* a, big_integer* b)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;
            
    unsigned int sum;
    unsigned int i;

    unsigned int minSize = big_integer_minVal(big_integer_minVal(a->m_size, b->m_size), r->m_size);
        
    for (i=0; i < minSize; i++)
    {
        sum = a->m_data[i] + b->m_data[i] + carryIn;

        if (sum < a->m_data[i]) carryIn = 1;
        else if (sum < b->m_data[i]) carryIn = 1;
        else carryIn = 0;
            
        r->m_data[i] = sum;
    }
    
    for (; i < r->m_size; i++)
    {
        sum = 0;

        if (i < a->m_size) sum += a->m_data[i];
        if (i < b->m_size) sum += b->m_data[i];

        if ((i < a->m_size) && (sum < a->m_data[i]))
        {
            if (carryIn) sum++;
            carryOut = 1;
        }
        else if (carryIn)
        {
            sum++;
            carryOut = (sum == 0)? 1: 0;
        }

        carryIn = carryOut;
        r->m_data[i] = sum;
    }
}


/**
 * Increment a big number
 * @todo check m_size
 */
void big_integer_inc(big_integer* x)
{
    int i = 0;
    int doRun = 1;
    while (doRun)
    {                
        x->m_data[i]++;

        doRun = (x->m_data[i] == 0);

        i++;
    }
}
