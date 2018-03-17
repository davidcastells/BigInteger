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

void big_integer_array_add_short(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],
        unsigned int b[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
//    unsigned int ref_data[r_size];
//    const unsigned int ref_base = 0;
//    const unsigned int ref_size = r_size;
//    big_integer_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);
//    
//    big_integer_add(r_data, r_base, r_size, ref_data, ref_base, ref_size, b_data, b_base, b_size);
//    
    big_integer_array_add(r, r, b);
}

void big_integer_array_inc(unsigned int m[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    int i = 0;
    int doRun = 1;
    while (doRun)
    {                
        m[i]++;

        doRun = (m[i] == 0);

        i++;
    }
}

void big_integer_array_add(unsigned int r[NUM_BIG_INTEGER_ARRAY_LIMBS],
	unsigned int a[NUM_BIG_INTEGER_ARRAY_LIMBS], 
	unsigned int b[NUM_BIG_INTEGER_ARRAY_LIMBS])
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;
            
    unsigned int sum;
    unsigned int i;
    
//    unsigned int minSize = big_integer_base_minVal(big_integer_base_minVal(a_size, b_size), r_size);
        
    for (i=0; i < NUM_BIG_INTEGER_ARRAY_LIMBS; i++)
    {
        sum = a[i] + b[i] + carryIn;

        if (sum < a[i]) carryIn = 1;
        else if (sum < b[i]) carryIn = 1;
        else carryIn = 0;
            
        r[i] = sum;
    }
}