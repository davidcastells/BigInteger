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

#include <assert.h>
#include <iostream>

void big_integer_base_add(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, 
	unsigned int* b_data, const unsigned int b_base, const unsigned int b_size)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;
            
    unsigned int sum;
    unsigned int i;
    
    unsigned int minSize = big_integer_base_minVal(big_integer_base_minVal(a_size, b_size), r_size);
        
    for (i=0; i < minSize; i++)
    {
        sum = a_data[a_base+i] + b_data[b_base+i] + carryIn;

        if (sum < a_data[a_base+i]) carryIn = 1;
        else if (sum < b_data[b_base+i]) carryIn = 1;
        else carryIn = 0;
            
        r_data[r_base+i] = sum;
    }
    
    for (; i < r_size; i++)
    {
        sum = 0;

        if (i < a_size) sum += a_data[a_base+i];
        if (i < b_size) sum += b_data[b_base+i];

        if ((i < a_size) && (sum < a_data[a_base+i]))
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
        r_data[r_base+i] = sum;
    }
}


    

void big_integer_base_add_short(unsigned int * r_data, unsigned int r_base, unsigned int r_size,
        unsigned int* b_data, const unsigned int b_base, const unsigned int b_size)
{
//    unsigned int ref_data[r_size];
//    const unsigned int ref_base = 0;
//    const unsigned int ref_size = r_size;
//    big_integer_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);
//    
//    big_integer_add(r_data, r_base, r_size, ref_data, ref_base, ref_size, b_data, b_base, b_size);
//    
    big_integer_base_add(r_data, r_base, r_size, r_data, r_base, r_size, b_data, b_base, b_size);
}



/**
 * Increment a big number
 * @todo check m_size
 */
void big_integer_base_inc(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    int i = 0;
    int doRun = 1;
    while (doRun)
    {                
        m_data[m_base+i]++;

        doRun = (m_data[m_base+i] == 0);

        i++;
    }
}
