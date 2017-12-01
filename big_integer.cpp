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
#include "big_integer.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>

static int verbosity = 0;

/**
 * Returns the value of the ith bit of the integer
 * @param bitnum
 * @return 
 */
int big_integer_getBit(unsigned int* m_data, 
	const unsigned int base, 
	const unsigned int size, int bitnum)
{
   int limbIndex = bitnum / 32;
   int bitIndex = bitnum % 32;

   return (m_data[base+limbIndex] >> bitIndex) & 1;
}



/**
 * Ensure that r and v are < than m
 * @param r_data
 * @param r_base
 * @param r_size
 * @param v_data
 * @param v_base
 * @param v_size
 * @param m_data
 * @param m_base
 * @param m_size
 */
void big_integer_squareMod_interleaved(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* v_data, const unsigned int v_base, const unsigned int v_size,
	unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    assert(r_data != v_data);
    assert(r_data != m_data);
    assert(big_integer_isLessThan(v_data, v_base, v_size, m_data, m_base, m_size));
        
    big_integer_multMod_interleaved(r_data, r_base, r_size,
        v_data, v_base, v_size,
    	v_data, v_base, v_size,
    	m_data, m_base, m_size);
}



/**
 * Computes r = v^2 mod m
 * 
 * @param r
 * @param v
 * @param mod
 */
void big_integer_squareMod(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* v_data, const unsigned int v_base, const unsigned int v_size,
	unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    big_integer_multMod(r_data, r_base, r_size,
    	v_data, v_base, v_size,
    	v_data, v_base, v_size,
    	m_data, m_base, m_size);
}

/**
 * Inspired in https://github.com/adamwalker/mmul/blob/master/crypto.c
 * 
 * @param r
 * @param x
 * @param y
 * @param m
 * @param mprime
 */
void big_integer_multMontgomeryForm2(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* x_data, const unsigned int x_base, const unsigned int x_size,
	unsigned int* y_data, const unsigned int y_base, const unsigned int y_size,
	unsigned int* m_data, const unsigned int m_base, const unsigned int m_size,
	unsigned int* mprime_data, const unsigned int mprime_base, const unsigned int mprime_size)
{
    int i;
    
    unsigned int t_data[x_size+y_size];
    const unsigned int t_base = 0;
    const unsigned int t_size = x_size + y_size;
    
    unsigned int pretm_data[t_size];
    const unsigned int pretm_base = 0;
    const unsigned int pretm_size = t_size;

    unsigned int tm_data[m_size];
    const unsigned int tm_base = 0;
    const unsigned int tm_size = m_size;

    unsigned int tmm_data[tm_size + m_size];
    const unsigned int tmm_base = 0;
    const unsigned int tmm_size = tm_size + m_size;
    
    big_integer_mult(t_data, t_base, t_size,
    	x_data, x_base, x_size,
    	y_data, y_base, y_size);
    
    big_integer_multLow(pretm_data, pretm_base, pretm_size,
    	t_data, t_base, t_size,
    	mprime_data, mprime_base, mprime_size);  // was mult low
    
    big_integer_copy(tm_data, tm_base, tm_size, pretm_data, pretm_base, pretm_size);
    big_integer_mult(tmm_data, tmm_base, tmm_size, tm_data, tm_base, tm_size, m_data, m_base, m_size);

    unsigned int u_data[tmm_size];
    const unsigned int u_base = 0;
    const unsigned int u_size = tmm_size; 
    
    big_integer_add(u_data, u_base, u_size,
    	t_data, t_base, t_size,
    	tmm_data, tmm_base, tmm_size);

    big_integer_shiftRight(r_data, r_base, r_size, u_data, u_base, u_size, x_size*32);

    int ret;
    ret = big_integer_isLessThan(m_data, m_base, m_size, r_data, r_base, r_size);
    
    // (m->isLessThan(r))   // if (r >= m) r = r - m
    if (ret) 
    {
        big_integer_subtract_short(r_data, r_base, r_size, m_data, m_base, m_size);
    }
}

void big_integer_error(unsigned int* r_data, unsigned int r_base, unsigned int r_size)
{
    for (int i=0; i < r_size; i++)
        r_data[r_base+i] = -1;
}

void big_integer_mult(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, 
	unsigned int* b_data, const unsigned int b_base, const unsigned int b_size)
{
    big_integer_zero(r_data, r_base, r_size); 

    unsigned long long carry;
    unsigned long long accum;
    
    int idx;
    unsigned int lo;
    unsigned int hi;

    int a_top = (big_integer_getLength(a_data, a_base, a_size) + 31) / 32;
    int b_top = (big_integer_getLength(b_data, b_base, b_size) + 31) / 32;
    
    for (int i=0; i < a_top; i++)
    {
        carry = 0;
        
        for (int j=0; j< b_top; j++)
        {
            idx = i + j;
            
            if (idx > r_size)
            {
                big_integer_error(r_data, r_base, r_size);
                return;
            }

            //BigInteger::mult(adata, b->m_data[j], &hi, &lo);
            unsigned long long ap = a_data[a_base+i];
            unsigned long long bp = b_data[b_base+j];
            unsigned long long res = ap * bp;
            lo = res;
            hi = res >> 32;

            accum = r_data[r_base + idx] + carry + lo;
                        
            carry = (accum >> 32) + hi;
            
            r_data[r_base + idx] = accum;
            
        }
        
        if ((i + b_top)< r_size)
            r_data[r_base + i + b_top] = carry;
    }
}        

/**
 * from https://github.com/adamwalker/mmul/blob/master/crypto.c (big_mul_low)
 */        
void big_integer_multLow(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, 
	unsigned int* b_data, const unsigned int b_base, const unsigned int b_size)
{
    int i;
    int j;

    big_integer_zero(r_data, r_base, r_size);
    
    unsigned int lo;
    unsigned int hi;
    
    int a_top = (big_integer_getLength(a_data, a_base, a_size) + 31) / 32;
    int b_top = (big_integer_getLength(b_data, b_base, b_size) + 31) / 32;
    
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
                lo = res;
                hi = res >> 32;

                unsigned long long accum  = r_data[r_base+idx] + carry + lo;
                carry = (accum >> 32) + hi;
                r_data[r_base+idx] = accum;
            }
        }
    }
}


void big_integer_multMod(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size,
	unsigned int* b_data, const unsigned int b_base, const unsigned int b_size,
	unsigned int* mod_data, const unsigned int mod_base, const unsigned int mod_size)
{
    unsigned int m_data[a_size+b_size];
    const unsigned int m_base = 0;
    const unsigned int m_size = a_size + b_size;
    
    unsigned int q_data[a_size];
    const unsigned int q_base = 0;
    const unsigned int q_size = a_size; 

    big_integer_mult(m_data, m_base, m_size, a_data, a_base, a_size, b_data, b_base, b_size);
    big_integer_div_naive(m_data, m_base, m_size, mod_data, mod_base, mod_size, q_data, q_base, q_size, r_data, r_base, r_size);
}

void big_integer_multMod_interleaved(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size,
	unsigned int* b_data, const unsigned int b_base, const unsigned int b_size,
	unsigned int* mod_data, const unsigned int mod_base, const unsigned int mod_size)
{
    assert(big_integer_isLessThan(a_data, a_base, a_size, mod_data, mod_base, mod_size));
    assert(big_integer_isLessThan(b_data, b_base, b_size, mod_data, mod_base, mod_size));
    assert(big_integer_getNumBits(r_data, r_base, r_size) > (big_integer_getLength(mod_data, mod_base, mod_size) + 1));
    
    big_integer_zero(r_data, r_base, r_size);
    
    // @todo apply mod to a and b before proceeding
    // @todo find the smallest number and change order accordingly
    int n = big_integer_getLength(a_data, a_base, a_size);
    for (int i=n-1; i>=0; i-- )
    {
        big_integer_shiftLeft_short(r_data, r_base, r_size, 1);
        int xi = big_integer_getBit(a_data, a_base, a_size, i);
        
        if (xi)
            big_integer_add_short(r_data, r_base, r_size, b_data, b_base, b_size);
        
        if (big_integer_isLessThanEqual(mod_data, mod_base, mod_size, r_data, r_base, r_size))
            big_integer_subtract_short(r_data, r_base, r_size, mod_data, mod_base, mod_size);
        if (big_integer_isLessThanEqual(mod_data, mod_base, mod_size, r_data, r_base, r_size))
            big_integer_subtract_short(r_data, r_base, r_size, mod_data, mod_base, mod_size);
    }
}   


void big_integer_add(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, 
	unsigned int* b_data, const unsigned int b_base, const unsigned int b_size)
{
    unsigned int carryIn = 0;
    unsigned int carryOut = 0;
            
    unsigned int sum;

    for (int i=0; i < r_size; i++)
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

void big_integer_add_short(unsigned int * r_data, unsigned int r_base, unsigned int r_size,
        unsigned int* b_data, const unsigned int b_base, const unsigned int b_size)
{
//    unsigned int ref_data[r_size];
//    const unsigned int ref_base = 0;
//    const unsigned int ref_size = r_size;
//    big_integer_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);
//    
//    big_integer_add(r_data, r_base, r_size, ref_data, ref_base, ref_size, b_data, b_base, b_size);
//    
    big_integer_add(r_data, r_base, r_size, r_data, r_base, r_size, b_data, b_base, b_size);
}

void big_integer_mod_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, 
	unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    unsigned int ref_data[r_size];
	const unsigned int ref_base = 0;
	const unsigned int ref_size = r_size;
	big_integer_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);
	
	unsigned int q_data[r_size];
	const unsigned int q_base = 0;
	const unsigned int q_size = r_size;
	
    big_integer_div_naive(ref_data, ref_base, ref_size, m_data, m_base, m_size,
    	q_data, q_base, q_size, r_data, r_base, r_size);
}
      

/**
 * Compute division, so that
 *  nq = x / y
 *  nr = x % y
 * 
 * @param x
 * @param y
 * @param nq
 * @param nr
 */
void big_integer_div_naive(unsigned int* x_data, const unsigned int x_base, const unsigned int x_size,
        unsigned int* y_data, const unsigned int y_base, const unsigned int y_size, 
        unsigned int* q_data, const unsigned int q_base, const unsigned int q_size,
        unsigned int* r_data, const unsigned int r_base, const unsigned int r_size)
{
    if (verbosity> 6) std::cout << "big_integer_div_naive x = " << big_integer_toHexString(x_data, x_base, x_size) << std::endl;
    if (verbosity> 6) std::cout << "big_integer_div_naive /   " << big_integer_toHexString(y_data, y_base, y_size) << std::endl;
    
    big_integer_zero(q_data, q_base, q_size);

    int ret = big_integer_isLessThan(x_data, x_base, x_size, y_data, y_base, y_size);
    
    if (ret)
    {
    	big_integer_copy(r_data, r_base, r_size, x_data, x_base, x_size);
//        big_integer_copy(nq_data, nq_base, nq_size, q_data, q_base, q_size);
    }
    else
    {
		// get the length of y
		int yl = big_integer_getLength(y_data, y_base, y_size);
		int rl = big_integer_getLength(x_data, x_base, x_size);
	
                assert(rl);
                
		int downBit = rl-yl;
	
		// get the yl most significant bits of ref
		big_integer_range(r_data, r_base, r_size, x_data, x_base, x_size, rl-1, downBit);
		
                if (verbosity> 6) std::cout << "big_integer_div_naive x[" << (rl-1) << ","<< downBit <<" ]   " << big_integer_toHexString(r_data, r_base, r_size) << std::endl;
                
                downBit--;
	
                
		ret = big_integer_isLessThan(r_data, r_base, r_size, y_data, y_base, y_size);
		if (ret)
		{
	
			// take another bit from ref
			big_integer_range(r_data, r_base, r_size, x_data, x_base, x_size, rl-1, downBit);
                        
                        if (verbosity> 6) std::cout << "big_integer_div_naive x[" << (rl-1) << ","<< downBit <<" ]   " << big_integer_toHexString(r_data, r_base, r_size) << std::endl;
                        
			downBit--;	
		}
	
		big_integer_inc(q_data, q_base, q_size);
		big_integer_subtract_short(r_data, r_base, r_size, y_data, y_base, y_size);	
	
                if (verbosity> 6) std::cout << "big_integer_div_naive q=" << big_integer_toHexString(q_data, q_base, q_size) << std::endl;
                if (verbosity> 6) std::cout << "big_integer_div_naive r=" << big_integer_toHexString(r_data, r_base, r_size) << std::endl;
                
		// take another bit
		while (downBit >= 0)
		{
			big_integer_shiftLeft_short(r_data, r_base, r_size, 1);

			if (big_integer_getBit(x_data, x_base, x_size, downBit))
			{
				big_integer_inc(r_data, r_base, r_size);
			}
			
			downBit--;

			ret = big_integer_isLessThan(r_data, r_base, r_size, y_data, y_base, y_size);

			if (ret)
			{
				big_integer_shiftLeft_short(q_data, q_base, q_size, 1);	// put a zero in q
			}
			else
			{
				big_integer_shiftLeft_short(q_data, q_base, q_size, 1);	// put a one in q
				big_integer_inc(q_data, q_base, q_size);
	
				big_integer_subtract_short(r_data, r_base, r_size, y_data, y_base, y_size);	
			}
                        
                        if (verbosity> 6) std::cout << "big_integer_div_naive q=" << big_integer_toHexString(q_data, q_base, q_size) << std::endl;
                        if (verbosity> 6) std::cout << "big_integer_div_naive r=" << big_integer_toHexString(r_data, r_base, r_size) << std::endl;
		} 
	
//		big_integer_copy(nr_data, nr_base, nr_size, r_data, r_base, r_size);
//		big_integer_copy(nq_data, nq_base, nq_size, q_data, q_base, q_size);
	}
    
}

void big_integer_mod_naive(unsigned int* x_data, const unsigned int x_base, const unsigned int x_size,
        unsigned int* y_data, const unsigned int y_base, const unsigned int y_size, 
        unsigned int* r_data, const unsigned int r_base, const unsigned int r_size)
{
    if (verbosity> 6) std::cout << "big_integer_div_naive x = " << big_integer_toHexString(x_data, x_base, x_size) << std::endl;
    if (verbosity> 6) std::cout << "big_integer_div_naive /   " << big_integer_toHexString(y_data, y_base, y_size) << std::endl;
    

    int ret = big_integer_isLessThan(x_data, x_base, x_size, y_data, y_base, y_size);
    
    if (ret)
    {
    	big_integer_copy(r_data, r_base, r_size, x_data, x_base, x_size);
//        big_integer_copy(nq_data, nq_base, nq_size, q_data, q_base, q_size);
    }
    else
    {
		// get the length of y
		int yl = big_integer_getLength(y_data, y_base, y_size);
		int rl = big_integer_getLength(x_data, x_base, x_size);
	
		int downBit = rl-yl;
	
		// get the yl most significant bits of ref
		big_integer_range(r_data, r_base, r_size, x_data, x_base, x_size, rl-1, downBit);
		
                if (verbosity> 6) std::cout << "big_integer_div_naive x[" << (rl-1) << ","<< downBit <<" ]   " << big_integer_toHexString(r_data, r_base, r_size) << std::endl;
                
                downBit--;
	
                
		ret = big_integer_isLessThan(r_data, r_base, r_size, y_data, y_base, y_size);
		if (ret)
		{
	
			// take another bit from ref
			big_integer_range(r_data, r_base, r_size, x_data, x_base, x_size, rl-1, downBit);
                        
                        if (verbosity> 6) std::cout << "big_integer_div_naive x[" << (rl-1) << ","<< downBit <<" ]   " << big_integer_toHexString(r_data, r_base, r_size) << std::endl;
                        
			downBit--;	
		}
	
		big_integer_subtract_short(r_data, r_base, r_size, y_data, y_base, y_size);	
	
                if (verbosity> 6) std::cout << "big_integer_div_naive r=" << big_integer_toHexString(r_data, r_base, r_size) << std::endl;
                
		// take another bit
		while (downBit >= 0)
		{
			big_integer_shiftLeft_short(r_data, r_base, r_size, 1);

			if (big_integer_getBit(x_data, x_base, x_size, downBit))
			{
				big_integer_inc(r_data, r_base, r_size);
			}
			
			downBit--;

			ret = big_integer_isLessThan(r_data, r_base, r_size, y_data, y_base, y_size);

			if (ret)
			{
//				big_integer_shiftLeft_short(q_data, q_base, q_size, 1);	// put a zero in q
			}
			else
			{
//				big_integer_shiftLeft_short(q_data, q_base, q_size, 1);	// put a one in q
//				big_integer_inc(q_data, q_base, q_size);
	
				big_integer_subtract_short(r_data, r_base, r_size, y_data, y_base, y_size);	
			}
                        
//                        if (verbosity> 6) std::cout << "big_integer_div_naive q=" << big_integer_toHexString(q_data, q_base, q_size) << std::endl;
                        if (verbosity> 6) std::cout << "big_integer_div_naive r=" << big_integer_toHexString(r_data, r_base, r_size) << std::endl;
		} 
	
//		big_integer_copy(nr_data, nr_base, nr_size, r_data, r_base, r_size);
//		big_integer_copy(nq_data, nq_base, nq_size, q_data, q_base, q_size);
	}
    
}

void big_integer_zero(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    for (int i=0; i < m_size; i++)
        m_data[m_base+i] = 0;    
}

void big_integer_setIntValue(unsigned int* data, 
	const unsigned int base, 
	const unsigned int size, unsigned int v)
{
    big_integer_zero(data, base, size);
    
    data[base + 0] = v;    
}

/**
 * 
 * @param m_data destination
 * @param m_base
 * @param m_size
 * @param orig_data source
 * @param orig_base
 * @param orig_size
 */
void big_integer_copy(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size, 
	unsigned int* orig_data, const unsigned int orig_base, const unsigned int orig_size)
{
    int minCopySize = (m_size < orig_size)? m_size : orig_size;
    int i;
    for (i=0; i < minCopySize; i++)
        m_data[m_base+i] = orig_data[orig_base+i];
    
    // zero the rest of the data 
    for (; i < m_size; i++)
        m_data[m_base+i] = 0;
}

/**
 * Returns true if this < v
 * Returns false if this >= v
 * 
 * @param v
 * @return 
 */
int big_integer_isLessThan(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size, 
	unsigned int* v_data, const unsigned int v_base, const unsigned int v_size)
{
    // if there is any 1 in a bigger size v before the range of this
    // then return true
    for (int i=v_size-1; i >= m_size; i--)
        if (v_data[v_base+i])
            return true;

    for (int i=m_size-1; i >= 0; i--)
        if (i >= v_size)
        {
            if (m_data[m_base+i] != 0)
                return false;
        }
        else if (m_data[m_base+i] < v_data[v_base+i])
            return true;
        else if (m_data[m_base+i] > v_data[v_base+i])
            return false;

    return false;
}

/**
 * 
 * @return the number of the most significant active bit
 */
int big_integer_getLength(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    int len = 0;
    int greaterActiveLimb = 0;

    // find the highest limb with an active bit
    for (int i=m_size-1; (i > 0) && (greaterActiveLimb == 0); i--)
    {
        if (m_data[m_base+i] > 0)
            greaterActiveLimb = i;
    }


    // find the highest bit 
    unsigned int test = m_data[m_base+greaterActiveLimb];
    int numActiveBits = 0;


    while (test > 0)
    {
        test = test >> 1;
        numActiveBits++;
    }

    len = (numActiveBits + greaterActiveLimb * 32);

    return len;
}


/**
 * 
 * @param up
 * @param down
 */
void big_integer_range_short(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size, int up, int down)
{
   unsigned int ref_data[m_size];
   const unsigned int ref_base = 0;
   const unsigned int ref_size = m_size;
   big_integer_copy(ref_data, ref_base, ref_size, m_data, m_base, m_size);

   big_integer_range(m_data, m_base, m_size, ref_data, ref_base, ref_size, up, down);
}

/**
  * @return the number of bits that the number can store
  */
int big_integer_getNumBits(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
{
    unsigned int ret = m_size * 32;
    return ret;
}

void big_integer_zeroHighBits(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, int fromBit)
{
    int fromLimb = fromBit / 32;
    int fromLimbBit = fromBit % 32;
    
    if ((fromLimb == r_size) && (fromLimbBit == 0))
        return;
    
    if (fromLimb >= r_size)
        return;    
    
    int mask = ((1 << fromLimbBit)-1);
    
    r_data[r_base+fromLimb] = r_data[r_base+fromLimb] & mask;
    fromLimb++;
    
    while (fromLimb < r_size)
    {
        r_data[r_base+fromLimb] = 0;
        fromLimb++;
    }
}

/**
* 
* @param r
* @param x
* @param upper
* @param lower
*/
void big_integer_range(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, int upper, int lower)
{
//    if (extraChecks)
//    {
//        assert(upper <= big_integer_getNumBits(x_data, x_base, x_size));
//        assert(lower <= big_integer_getNumBits(x_data, x_base, x_size));
//        assert(big_integer_getNumBits(r_data, r_base, r_size) >= (upper-lower));
//    }
 
    // we move 
    if (lower > 0)
        big_integer_shiftRight(r_data, r_base, r_size, x_data, x_base, x_size, lower);
    else
        big_integer_copy(r_data, r_base, r_size, x_data, x_base, x_size);
    
    big_integer_zeroHighBits(r_data, r_base, r_size, upper-lower+1);
}


/**
 * Increment a big number
 * @todo check m_size
 */
void big_integer_inc(unsigned int* m_data, const unsigned int m_base, const unsigned int m_size)
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

void big_integer_shiftLeft_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, int bits)
{
//    unsigned int ref_data[r_size];
//    const unsigned int ref_base = 0;
//    const unsigned int ref_size = r_size;
//    big_integer_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);
//	
//    big_integer_shiftLeft(r_data, r_base, r_size, ref_data, ref_base, ref_size, bits );
    
    big_integer_shiftLeft(r_data, r_base, r_size, r_data, r_base, r_size, bits );
}

void big_integer_shiftRight_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, int bits)
{
//    unsigned int ref_data[r_size];
//    const unsigned int ref_base = 0;
//    const unsigned int ref_size = r_size;
//    big_integer_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);
//    
//    big_integer_shiftRight(r_data, r_base, r_size, ref_data, ref_base, ref_size, bits );
    big_integer_shiftRight(r_data, r_base, r_size, r_data, r_base, r_size, bits );
}

void big_integer_subtract_short(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size, 
	unsigned int* y_data, const unsigned int y_base, const unsigned int y_size)
{
//	unsigned int ref_data[r_size];
//	const unsigned int ref_base = 0;
//	const unsigned int ref_size = r_size;
//	big_integer_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);
//   
//	big_integer_subtract(r_data, r_base, r_size, ref_data, ref_base, ref_size, y_data, y_base, y_size);
        
        big_integer_subtract(r_data, r_base, r_size, r_data, r_base, r_size, y_data, y_base, y_size);
}


void big_integer_subtract(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
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


/**
 * Shift right a number of bits, zeros are inserted at the left
 * First we move slots (limb multiples)
 * 
 *  For instance if slots is 2
 * 
 *  ... a[6] a[5] a[4] a[3] a[2]
 *  ... r[4] r[3] r[2] r[1] r[0]
 * 
 * @param r retsult r = a >> sv
 * @param a
 * @param sv    number of bits to shift to the right
 */
void big_integer_shiftRight(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, int sv)
{
    unsigned int carry = 0;
    int limbsShifted = sv / 32;
    int limbBitsShifted = sv % 32;
    int cs = (limbBitsShifted == 0) ? 0 : 32 - limbBitsShifted;
    
    for (int i=0; i < r_size; i++)
    {
        unsigned int v1 = 0;
        unsigned int v2 = 0;
        
        if ((i+limbsShifted) < a_size)
            v1 = a_data[a_base+i+limbsShifted] >> limbBitsShifted;
        if ((i+limbsShifted+1) < a_size && cs)
            v2 = a_data[a_base+i+limbsShifted+1] << cs;
        r_data[r_base+i] = v1 | v2;
    }
}

/**
 * 
 * @param r
 * @param a
 * @param sv
 */
void big_integer_shiftLeft(unsigned int* r_data, const unsigned int r_base, const unsigned int r_size,
	unsigned int* a_data, const unsigned int a_base, const unsigned int a_size, int sv)
{
    int limbsShifted = sv / 32;
    int limbBitsShifted = sv % 32;
    int cs = (limbBitsShifted == 0) ? 0 : 32 - limbBitsShifted;
    
    for (int i= r_size-1; i >= 0; i--)
    {
        unsigned int v1 = 0;
        unsigned int v2 = 0;
        
        if ((i-limbsShifted) >= 0 && (i-limbsShifted) < a_size)
            v1 = a_data[a_base+i-limbsShifted] << limbBitsShifted;
        if ((i-limbsShifted-1) >= 0 && (i-limbsShifted-1) < a_size && cs)
            v2 = a_data[a_base+i-limbsShifted-1] >> cs;
        r_data[r_base+i] = v1 | v2;
    }
}

int big_integer_isZero(unsigned int * data, const unsigned int base, const unsigned int size)
{
    for (int i=0; i<size; i++)
    {
        if (data[base+i] != 0)
            return 0;   // exit if it is not zero
    }

    return 1;
}

int big_integer_isOdd(unsigned int * data, const unsigned int base, const unsigned int size)
{
    return data[base+0] & 0x1;
}

void big_integer_powerMod(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size,
        unsigned int * v_data, const unsigned int v_base, const unsigned int v_size,
        unsigned int * power_data, const unsigned int power_base, const unsigned int power_size,
        unsigned int * mod_data, const unsigned int mod_base, const unsigned int mod_size)
{
    unsigned int x_data[v_size];
    unsigned int x_size = v_size;
    unsigned int x_base = 0;
    
    big_integer_copy(x_data, x_base, x_size, v_data, v_base, v_size);

    unsigned int y_data[power_size];
    unsigned int y_size = power_size;
    unsigned int y_base = 0;
    
    big_integer_copy(y_data, y_base, y_size, power_data, power_base, power_size);

    // we use a simple square-and-multiply algorithm,
    big_integer_setIntValue(r_data, r_base, r_size, 1);
    
    
    while(!big_integer_isZero(y_data, y_base, y_size))
    {
            if (big_integer_isOdd(y_data, y_base, y_size))
                big_integer_multMod_short(r_data, r_base, r_size, x_data, x_base, x_size, mod_data, mod_base, mod_size);

            big_integer_shiftRight_short(y_data, y_base, y_size, 1);     // / 2
            big_integer_squareMod_short(x_data, x_base, x_size, mod_data, mod_base, mod_size);
            
    }

}



void big_integer_multMod_short(unsigned int * r_data, unsigned int r_base, unsigned int r_size,
        unsigned int * b_data, unsigned int b_base, unsigned int b_size,
        unsigned int * m_data, unsigned int m_base, unsigned int m_size)
{
    unsigned int ref_data[r_size];
    const unsigned int ref_base = 0;
    const unsigned int ref_size = r_size;
    big_integer_copy(ref_data, ref_base, ref_size, r_data, r_base, r_size);

    
    big_integer_multMod(r_data, r_base, r_size, ref_data, ref_base, ref_size,
            b_data, b_base, b_size, m_data, m_base, m_size);
}

void big_integer_squareMod_short(unsigned int * r_data, unsigned int r_base, unsigned int r_size,
        unsigned int * m_data, unsigned int m_base, unsigned int m_size)
{
    unsigned int ref_data[r_size];
    const unsigned int ref_base = 0;
    const unsigned int ref_size = r_size;
    big_integer_mod_naive(r_data, r_base, r_size, m_data, m_base, m_size, ref_data, ref_base, ref_size);
    
    big_integer_squareMod_interleaved(r_data, r_base, r_size,
            ref_data, ref_base, ref_size,
            m_data, m_base, m_size);
}

/**
 * As implemented in "Handbook of Applied Cryptography" algorithm 14.94
 * we compute r = x ^ e mod m
 * @param r
 * @param v
 * @param mod
 */
void big_integer_powerModMontgomery(unsigned int * r_data, const unsigned int r_base, const unsigned int r_size, 
	unsigned int* x_data, const unsigned int x_base, const unsigned int x_size, 
	unsigned int* e_data, const unsigned int e_base, const unsigned int e_size,
	unsigned int* m_data, const unsigned int m_base, const unsigned int m_size,
	unsigned int* mprime_data, const unsigned int mprime_base, const unsigned int mprime_size, 
	unsigned int* radix_data, const unsigned int radix_base, const unsigned int radix_size)
{
	unsigned int radix2_data[m_size];
	const unsigned int radix2_size = m_size;
	const unsigned int radix2_base = 0;
	
	big_integer_squareMod(radix2_data, radix2_base, radix2_size,
		radix_data, radix_base, radix_size, 
		m_data, m_base, m_size);

	unsigned int xprime_data[m_size];
	const unsigned int xprime_size = m_size;
	const unsigned int xprime_base = 0;
	
	big_integer_multMontgomeryForm2(xprime_data, xprime_base, xprime_size,
		x_data, x_base, x_size,
		radix2_data, radix2_base, radix2_size,
		m_data, m_base, m_size,
		mprime_data, mprime_base, mprime_size);
	
	unsigned int a_data[radix_size];
	unsigned int a_size = radix_size;
	const unsigned int a_base = 0;
	
	big_integer_copy(a_data, a_base, a_size, radix_data, radix_base, radix_size);

	big_integer_mod_short(a_data, a_base, a_size, m_data, m_base, m_size);
	
	// a.reduceWorkingSize(m->m_size);
	a_size = m_size;
	
	int t = big_integer_getLength(e_data, e_base, e_size);
	
	for (int i=t-1; i >= 0; i--)
	{
		int ei = big_integer_getBit(e_data, e_base, e_size, i);
		
		unsigned int temp_data[a_size];
		const unsigned int temp_size = a_size;
		const unsigned int temp_base = 0;
		
		big_integer_multMontgomeryForm2(a_data, a_base, a_size,
			temp_data, temp_base, temp_size,
			temp_data, temp_base, temp_size,
			m_data, m_base, m_size,
			mprime_data, mprime_base, mprime_size);
		
		if (ei)
		{
			big_integer_copy(temp_data, temp_base, temp_size, a_data, a_base, a_size);
			big_integer_multMontgomeryForm2(a_data, a_base, a_size,
				temp_data, temp_base, temp_size,
				xprime_data, xprime_base, xprime_size,
				m_data, m_base, m_size,
				mprime_data, mprime_base, mprime_size);
		}   
	}
	
	unsigned int one_data[1];
	const unsigned int one_size = 1;
	const unsigned int one_base = 0;
	one_data[0] = 1;
	
	big_integer_multMontgomeryForm2(r_data, r_base, r_size,
		a_data, a_base, a_size,
		one_data, one_base, one_size,
		m_data, m_base, m_size,
		mprime_data, mprime_base, mprime_size);
}

void big_integer_initSize(unsigned int * data, unsigned int base, unsigned int size, int s)
{
    assert(size > s);
}

void big_integer_initFromHexString(unsigned int * data, unsigned int base, unsigned int size, const char* str)
{
    big_integer_initSize(data, base, size, (strlen(str)+7)/8);
    big_integer_parseHexString(data, base, size, str);
}
    
int big_integer_isEqual(unsigned int * a_data, unsigned int a_base, unsigned int a_size,
        unsigned int * b_data, unsigned int b_base, unsigned int b_size)
{
    unsigned int* pa = &a_data[a_base];
    unsigned int* pb = &b_data[b_base];

    int minSize = (a_size > b_size)? b_size : a_size;

    // check if there is any different limb
    for (int i=minSize-1; i >= 0; i--)
        if (pa[i] != pb[i])
            return false;

    // if sizes are different, ensure the bigger part is zero
    for (int i=minSize; i < a_size; i++)
        if (pa[i] != 0)
            return false;

    for (int i=minSize; i < b_size; i++)
        if (pb[i] != 0)
            return false;

    return true;
}

void big_integer_parseString(unsigned int * data, unsigned int base, unsigned int size, const char* str)
{
    // multiply by x*10 is x*(8+2) -> 8*x + 2*x
    int l = strlen(str);

    big_integer_zero(data, base, size);

    unsigned int digit_data[1];
    unsigned int digit_base = 0;
    unsigned int digit_size = 1;
    
    
    // V = V*10+s[i];
    for (int i=0; i < l; i++)
    {
        //  x=x*10
        unsigned int ref_data[size];
        unsigned int ref_base = 0;
        unsigned int ref_size = size;
    
        big_integer_copy(ref_data, ref_base, ref_size, data, base, size);
        big_integer_shiftLeft_short(ref_data, ref_base, ref_size, 1);
        
        big_integer_shiftLeft_short(data, base, size, 3);
        
        big_integer_add_short(data, base, size, ref_data, ref_base, ref_size);

//                std::cout << toString() << " + " << str[i] << " = "; 

        big_integer_setIntValue(digit_data, digit_base, digit_size, str[i]-'0');
        big_integer_add_short(data, base, size, digit_data, digit_base, digit_size);

//                std::cout << toString() << std::endl;
    }
}

void big_integer_parseHexString(unsigned int* data, unsigned int base, unsigned int size, const char* str)
{
    // multiply by x*10 is x*(8+2) -> 8*x + 2*x
    int l = strlen(str);

    big_integer_zero(data, base, size);

    unsigned int digit_data[1];
    unsigned int digit_base = 0;
    unsigned int digit_size = 1;

    // V = V*10+s[i];
    for (int i=0; i < l; i++)
    {
        //  x=x*10
        big_integer_shiftLeft_short(data, base, size, 4); // *16


        if ((str[i] >= '0') && (str[i] <= '9'))
            big_integer_setIntValue(digit_data, digit_base, digit_size, str[i]-'0');
        else if ((str[i] >= 'A') && (str[i] <= 'F'))
            big_integer_setIntValue(digit_data, digit_base, digit_size, 10 + (str[i]-'A'));
        else
            big_integer_setIntValue(digit_data, digit_base, digit_size, 10 + (str[i]-'a'));


        if (verbosity > 10)
            std::cout << big_integer_toHexString(data, base, size) << " + " << big_integer_toHexString(digit_data, digit_base, digit_size) << " = "; 


        big_integer_add_short(data, base, size, digit_data, digit_base, digit_size);


        if (verbosity > 10)
            std::cout << big_integer_toHexString(data, base, size) << std::endl;

    }
}

const char* big_integer_toHexString(unsigned int* data, unsigned int base, unsigned int size)
{
    static char s[200];

    int c = 0;
    for (int i=0; i < size; i++)
    {
        char buf[10];
        sprintf(buf, "%08X ", data[base+i]);
        //s = s + std::string(buf);
        
        strcpy(&s[c], buf);
        c += strlen(buf);
    }

    return s;
}

void big_integer_random(unsigned int* data, unsigned int base, unsigned int size)
{
    int lc = clock();
    while (lc == clock());
    
    srand(clock());
    
    for (int i=0; i < size; i++)
    {
        data[base+i] = rand();
        
        // Mingw random seems to return 16 bit numbers
        #ifdef __MINGW32__
            data[base+i] <<= 16;
            data[base+i] |= rand();
        #endif

        // Windows random seems to return 16 bit numbers also
        #ifdef WIN32
            data[base+i] <<= 16;
            data[base+i] |= rand();
        #endif
    }
}

void big_integer_random_bits(unsigned int* data, unsigned int base, unsigned int size, int n)
{
    assert(n < big_integer_getNumBits(data, base, size));
    
    big_integer_random(data, base, size);
    int shift = big_integer_getNumBits(data, base, size) - n;
    big_integer_shiftRight_short(data, base, size, shift);
    
}

int big_integer_isLessThanEqual(unsigned int* m_data, unsigned int m_base, unsigned int m_size,
        unsigned int* v_data, unsigned int v_base, unsigned int v_size)
{
    return !big_integer_isBiggerThan(m_data, m_base, m_size, v_data, v_base, v_size);
}

int big_integer_isBiggerThan(unsigned int* m_data, unsigned int m_base, unsigned int m_size,
        unsigned int* v_data, unsigned int v_base, unsigned int v_size)
{
    // if there is any 1 in a bigger size v before the range of this
    // then return true
    for (int i=v_size-1; i >= m_size; i--)
        if (v_data[v_base+i])
            return false;

    for (int i=m_size-1; i >= 0; i--)
        if (i >= v_size)
        {
            if (m_data[m_base+i] != 0)
                return true;
        }
        else if (m_data[m_base+i] > v_data[v_base+i])
            return true;
        else if (m_data[m_base+i] < v_data[v_base+i])
            return false;

    return false;
}