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
#include "../BigInteger.h"

#include <assert.h>
#include <iostream>

using namespace std;
                
/**
 *  This is the Additive Chaining algorithm which can be found on
 * p. 244 of "Applied Cryptography, Second Edition" by Bruce Schneier.
 * @param r 
 * @param x
 * @param power
 * @param mod
 */
void BigInteger::powerMod(BigInteger* r, BigInteger* v, BigInteger* power, BigInteger* mod)
{
    if (extraChecks)
    {
        assert(r != v);
        assert(r != power);
        assert(r != mod);
    }

    BigInteger x;
    x.initSize(mod->m_size+1); // for interleaved operations
    x.copy(v);
    BigInteger y(*power);


    // we use a simple square-and-multiply algorithm,
    if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod v " << v->toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod power " << power->toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod mod " << mod->toHexString() << std::endl;

    r->setIntValue(1);

    while(!y.isZero())
    {
            if (y.isOdd())
                r->multMod(&x, mod);

            y.shiftRight(1);    // / 2
            x.squareMod(mod); 
    }

    if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod r " << r->toHexString() << std::endl;
}

/**
 * 
 * @param r must be bigger than mod
 * @param v
 * @param power
 * @param mod
 */
void BigInteger::powerMod_interleaved(BigInteger* r, BigInteger* v, BigInteger* power, BigInteger* mod)
{
    if (extraChecks)
    {
        assert(r != v);
        assert(r != power);
        assert(r != mod);
        assert(r->getNumBits() > v->getLength());
        assert(r->getNumBits() > power->getLength());
        assert(r->getNumBits() > mod->getLength());
    }

    BigInteger x;
    x.initSize(mod->m_size+1); // for interleaved operations
    x.copy(v);
    BigInteger y(*power);   // @todo remove y

    int maxBit = y.getLength();

    // When using interleaved we need to apply mod before
    x.mod(mod);
    // y.mod(mod);

    // we use a simple square-and-multiply algorithm,
    if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod v " << x.toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod power " << y.toHexString() << std::endl;
    if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod mod " << mod->toHexString() << std::endl;

    r->setIntValue(1);

//        while(!y.isZero())
    for (int i=0; i < maxBit; i++)
    {
            //if (y.isOdd())
        if (y.getBit(i))
            {
//                    std::cout << " r " << r->toHexString() << std::endl;
//                    std::cout << "  * x " << x.toHexString() << std::endl;
//                    std::cout << " mod mod " << mod->toHexString() << std::endl;

                r->multMod_interleaved(&x, mod);

//                    std::cout << " = r " << r->toHexString() << std::endl;
            }

            // y.shiftRight(1);    // / 2
            x.squareMod_interleaved(mod); 
    }

    if (verbosity > VERBOSITY_LEVEL_POWER_MOD) std::cout << "BigInteger::powerMod r " << r->toHexString() << std::endl;
}


#define SLIDING_WINDOW_SIZE 5

/**
 *  Sliding Window Exponentiation 
 * @from https://github.com/an4/Applied-Security-Mod/blob/master/modmul.c
 */
int BigInteger::binary_to_decimal(int start, int end) 
{
    int i;
    int result = 0;
    int g = 1;
    for(i = end; i<=start; i++) 
    {
        int bit = getBit(i);; // mpz_tstbit(input, i);
        if(bit) 
        {
            result += g * bit;
        }
        g <<= 1;
    }
    return result;
}

/**
 * Computes the modular exponentiation using the sliding window algorithm, which
 * precomputes a table of products to reuse them
 * 
 * Inspired in https://github.com/an4/Applied-Security-Mod/blob/master/modmul.c
 *
 * @param r return value
 * @param v
 * @param exp
 * @param m
 */
void BigInteger::powerModSlidingWindow(BigInteger* r, BigInteger* v, BigInteger* exp, BigInteger* m)
{
    const int table_length = 1 << (SLIDING_WINDOW_SIZE - 1);
    BigInteger table[table_length];
    BigInteger square_base;
    BigInteger temp;
    int i;
    int exp_length;
    int l;
    // value in window
    int u;

    temp.initSize(r->m_size);
    
    // T[0] = x
    table[0].initSize(v->m_size);
    table[0].copy(v);
    
    // x^2 mod N
    square_base.initSize(m->m_size);
    square_base.copy(v);
    square_base.squareMod(m);
    
    
    // Compute lookup table.
    for(i = 1; i < table_length; i++) 
    {
        // table[i] = table[i-1] * v ^ 2 mod m
        table[i].initSize(m->m_size);
        BigInteger::multMod(&table[i], &table[i-1], &square_base, m);
    }
    
    // t = 1
    r->setIntValue(1);
    
    // |y|
    exp_length = exp->getNumBits();
    
    // |y| - 1
    i = exp_length - 1;
    while(i >= 0) 
    {
        if(! exp->getBit(i))    
        {
            l = i;
            u = 0;
        }
        else
        {
            l = ((i - SLIDING_WINDOW_SIZE + 1) > 0) ? (i - SLIDING_WINDOW_SIZE + 1) : 0;

            while(! exp->getBit(l)) 
            {
                l++;
            }
            // Set u = exp bits between i and l
            u = exp->binary_to_decimal(i, l);
        }

        // t' = t
        temp.copy(r);
        
        // t^p
        int p = 1 << (i - l + 1);
        if(p>0) 
        {
            // t = t'^p (mod N)
            BigInteger bip;
            bip.initSize(1);
            bip.setIntValue(p);
            
            BigInteger::powerMod(r, &temp, &bip, m);
        }
        
        if(u != 0) 
        {
            // t = t * T[(u-1)/2] mod N
            r->multMod(&table[(u-1)/2], m);
        }
        i = l - 1;
    }
}

