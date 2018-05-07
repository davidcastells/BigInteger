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

void big_integer_mult(big_integer* r, big_integer* a, big_integer* b)
{
    big_integer_zero(r); 

    unsigned long long carry;
    unsigned long long accum;
    
    int idx;
    unsigned int lo;
    unsigned int hi;

    int a_top = big_integer_getLimbLength(a);
    int b_top = big_integer_getLimbLength(b);
    
    for (int i=0; i < a_top; i++)
    {
        carry = 0;
        
        for (int j=0; j< b_top; j++)
        {
            idx = i + j;
            
            if (idx > r->m_size)
            {
                big_integer_error(r);
                return;
            }

            //BigInteger::mult(adata, b->m_data[j], &hi, &lo);
            unsigned long long ap = a->m_data[i];
            unsigned long long bp = b->m_data[j];
            unsigned long long res = ap * bp;
            lo = (unsigned int) res;
            hi = (unsigned int) (res >> 32);

            accum = r->m_data[idx] + carry + lo;
                        
            carry = (accum >> 32) + hi;
            
            r->m_data[idx] = (unsigned int) accum;      
        }
        
        if ((i + b_top)< r->m_size)
            r->m_data[i + b_top] = (unsigned int) carry;
    }
}        

void big_integer_mult32(unsigned int x, unsigned int y, unsigned int *rHight, unsigned int *rLow)
{
    unsigned long long res = (unsigned long long) x * (unsigned long long) y;
    *rLow = (unsigned int) res;
    *rHight = (unsigned int) (res >> 32);
}

/**
 * from https://github.com/adamwalker/mmul/blob/master/crypto.c (big_mul_low)
 */        
void big_integer_multLow(big_integer* r, big_integer* a, big_integer* b)
{
    //if (extraChecks) 
    {
        assert(r != a);
        assert(r != b);
    }

    int i, j;

    big_integer_zero(r);

    int atop = big_integer_getLimbLength(a);
    int btop = big_integer_getLimbLength(b);
    int rsize = r->m_size;
    
    uint32_t lo, hi;
    uint64_t accum;
    
    for(i=0; i<atop; i++)
    {
        uint64_t carry = 0;
        for(j=0; j<btop; j++)
        {
            int idx = i + j;
            if (idx < rsize)
            {
                big_integer_mult32(a->m_data[i], b->m_data[j], &hi, &lo);

                accum  = r->m_data[idx] + carry + lo;
                carry  = (accum >> 32) + hi;
                r->m_data[idx] = (unsigned int) accum;
            }
        }
    }
}