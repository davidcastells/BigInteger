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


void big_integer_base_mult(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, 
	unsigned int* b_data, const unsigned int b_base, const unsigned int b_size)
{
    big_integer_base_zero(r_data, r_base, r_size); 

    unsigned long long carry;
    unsigned long long accum;
    
    int idx;
    unsigned int lo;
    unsigned int hi;

    int a_top = big_integer_base_getLimbLength(a_data, a_base, a_size);
    int b_top = big_integer_base_getLimbLength(b_data, b_base, b_size);
    
    for (int i=0; i < a_top; i++)
    {
        carry = 0;
        
        for (int j=0; j< b_top; j++)
        {
            idx = i + j;
            
            if (idx > r_size)
            {
                big_integer_base_error(r_data, r_base, r_size);
                return;
            }

            //BigInteger::mult(adata, b->m_data[j], &hi, &lo);
            unsigned long long ap = a_data[a_base+i];
            unsigned long long bp = b_data[b_base+j];
            unsigned long long res = ap * bp;
            lo = (unsigned int) res;
            hi = (unsigned int) (res >> 32);

            accum = r_data[r_base + idx] + carry + lo;
                        
            carry = (accum >> 32) + hi;
            
            r_data[r_base + idx] = (unsigned int) accum;
            
        }
        
        if ((i + b_top)< r_size)
            r_data[r_base + i + b_top] = (unsigned int) carry;
    }
}        


/**
 * from https://github.com/adamwalker/mmul/blob/master/crypto.c (big_mul_low)
 */        
void big_integer_base_multLow(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, 
	unsigned int* b_data, const unsigned int b_base, const unsigned int b_size)
{
    int i;
    int j;

    big_integer_base_zero(r_data, r_base, r_size);
    
    unsigned int lo;
    unsigned int hi;
    
    int a_top = (big_integer_base_getLength(a_data, a_base, a_size) + 31) / 32;
    int b_top = (big_integer_base_getLength(b_data, b_base, b_size) + 31) / 32;
    
    for(i=0; i < a_top; i++)
    {
        unsigned long long carry = 0;
        
        for(j=0; j < b_top; j++)
        {
            int idx = i + j;
            if (idx < r_size)
            {
                //mult(a_data[i], b_data[j], &hi, &lo);
                unsigned long long ap = a_data[a_base+i];
                unsigned long long bp = b_data[b_base+j];
                unsigned long long res = ap * bp;
                lo = (unsigned int) res;
                hi = (unsigned int) (res >> 32);

                unsigned long long accum  = r_data[r_base+idx] + carry + lo;
                carry = (accum >> 32) + hi;
                r_data[r_base+idx] = (unsigned int) accum;
            }
        }
    }
}
