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

/**
 * Computes this = this * R^-1 mod m
 * @param m
 * @param radix
 * @param mprime
 */
void BigInteger::montgomeryReduction(BigInteger* m, BigInteger* radix, unsigned int mprime)
{
    BigInteger ref(*this);
    BigInteger::montgomeryReduction(this, &ref, m, radix, mprime);
}

/**
 * Algorithm 14.32 from Handbook from applied cryptography
 * Compute montgomery reduction
 * base b= 2^32
 * 
 * @param r the result r = x * R^-1 mod m
 * @param x input value, such that < m*radix
 * @param m modulo
 * @param radix a value (2^32)^n such that it is bigger than m
 * @param mprime is a value such that m' = -m ^-1 mod b
 *      so m'*m mod b = should be -1
 *      notice that m' is < 2^32
 */
void BigInteger::montgomeryReduction(BigInteger* r, BigInteger* x, BigInteger* m, BigInteger* radix, unsigned int mprime)
{
    assert(r != x);
    assert(r != m);
    assert(r != radix);
    //assert(r != mprime);
    assert(m->isLessThan(radix));
    assert(m->isOdd());
    
    BigInteger a;
    a.initSize(m->m_size+radix->m_size);
    a.copy(x);
    
    int verbose = 0;
    
    int n = m->getLimbLength();
    for (int i=0; i < n; i++)
    {
        unsigned long long ui = (unsigned long long) a.m_data[i] * (unsigned long long) mprime;
        ui = ui & 0xFFFFFFFF;   // mod b
        
        BigInteger mc;
        mc.initSize(m->m_size+1);
        BigInteger::mult(&mc, m, (unsigned int) ui);
        
        
        if (verbose)
        {
            std::cout << "m = " << m->toHexString() << std::endl;
            std::cout << "* ui = " << to_hex_string(ui) << std::endl;
            std::cout << "= mc = " << mc.toHexString() << std::endl;
            
            std::cout << "a = " << a.toHexString() << std::endl;
        }
        
        BigInteger::addShifted(&a, &a, &mc, 32*i);
        
        if (verbose)
            std::cout << "a += ui*m << 32* " << i << " = " << a.toHexString() << std::endl;
        //a = a + ui * m * b^i;
    }

    
    a.shiftRight(32*n);
    //a = a >> 32 * n; // / b ^ n
    
    if (verbose)
            std::cout << "a >>= 32* " << n << " = " << a.toHexString() << std::endl;
    
    if (m->isLessThan(&a))
        a.subtract(m);
    
    if (verbose)
            std::cout << "a =  " << a.toHexString() << std::endl;
    
    r->copy(&a);
}

/**
 *  from http://grepcode.com/file_/repository.grepcode.com/java/root/jdk/openjdk/6-b14/java/math/BigInteger.java/?v=source
 */
//void BigInteger::montgomeryReduction_ColinPlumb(BigInteger* n, BigInteger* mod, int mlen, int inv) 
//{
//        int c=0;
//        int len = mlen;
//        int offset=0;
//
//        do 
//        {
//            int nEnd = n->m_data[n->m_size-1-offset];
//            int carry = mulAdd(n, mod, offset, mlen, inv * nEnd);
//            c += incLimb(n, offset, mlen, carry);
//            offset++;
//        } while(--len > 0);
//
//        while(c>0)
//            c += subN(n, mod, mlen);
//
//        while (intArrayCmpToLen(n, mod, mlen) >= 0)
//            subN(n, mod, mlen);
//
//        //return n;
//    }


